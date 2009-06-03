/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#include "hypar/url.hpp"
#include "hypar/debug.hpp"
#include "hypar/textutils.hpp"

#include <iostream>
#include <cassert>
#include <vector>

#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <limits.h>

using std::string;

/* Remove spaces from s */
static const char *txt_trim (string &s)
{
    const char *pString = s.c_str ();
    const char *pStart = 0, *pEnd = 0;
    pStart = pString;
    while (*pStart && isspace (*pStart))
    {
        pStart++;
    }
    if (!*pStart)
    {
        pStart--;
    }
    pEnd = pString + strlen (pString); pEnd--;
    while ((pEnd != pStart) && (isspace (*pEnd)))
    {
        pEnd--;
    }
    pEnd++;
    string new_string (pStart, pEnd - pStart);
    s = new_string;
    return s.c_str ();
}

#ifdef USE_WIDECHAR
static const wchar_t *txt_trim (wstring &s)
{
    const wchar_t *pString = s.c_str ();
    const wchar_t *pStart = 0, *pEnd = 0;
    pStart = pString;
    while (*pStart && iswspace (*pStart))
    {
        pStart++;
    }
    if (!*pStart)
    {
        pStart--;
    }
    pEnd = pString + wcslen (pString); pEnd--;
    while ((pEnd != pStart) && (iswspace (*pEnd)))
    {
        pEnd--;
    }
    pEnd++;
    wstring new_string (pStart, pEnd - pStart);
    s = new_string;
    return s.c_str ();
}
#endif /* USE_WIDECHAR */

USING_NAMESPACE (std);

BEGIN_NAMESPACE (hy);

URL::URL (const string &absolute) :
    rc (URL_BAD_SYNTAX)
{
    clear ();
    parse (absolute, m_protocol, m_host, m_sPort, m_path, m_fragment,
            m_cgiUri, m_bIsCgi);
    if (m_protocol.empty ())
    {
#if DEBUG
        hy_warn (("No protocol supplied!\n"));
#endif
        return;
    }
    if (m_host.empty ())
    {
#if DEBUG
        hy_warn (("No host supplied!\n"));
#endif
        return;
    }
    if (m_sPort == 0)
    {
#if DEBUG
        hy_warn (("No appropriate port found!\n"));
#endif
        return;
    }
    if (m_path.empty ())
    {
#if DEBUG
        hy_warn (("Empty path portion!\n"));
#endif
        return;
    }
    rc = URL_OK;
#if 0
    m_path = escape (m_path);
#endif
    simplifyPath ();
}

    void
URL::clear ()
{
    m_protocol.erase ();
    m_host.erase ();
    m_sPort = 0;
    m_path.erase ();
    m_fragment.erase ();
    m_cgiUri.erase ();
    m_bIsCgi = false;
    m_bIsCgiUriParsed = false;
    m_bNoFollow = false;
    m_CGIAttrList.clear ();
    rc = URL_BAD_SYNTAX;
}


URL::URL (const string &relative, const string &base) :
    rc (URL_BAD_SYNTAX)
{
    clear ();
    if (URL::isAbsolute (relative))
    {
        parse (relative, m_protocol, m_host, m_sPort, m_path,
                m_fragment, m_cgiUri, m_bIsCgi);
    }
    else
    {
        URL ubase (base);
        if (ubase.rc != URL::URL_OK)
        {
#if DEBUG
            hy_warn (("Erroneous base URL '%s'!\n", base.c_str ()));
#endif
            return;
        }
        m_protocol = ubase.getProtocol ();
        m_host = ubase.getHost ();
        m_sPort = ubase.getPort ();
#if 0
        string _relative = escape (relative);
#else
        string _relative = relative;
        txt_trim (_relative);
#endif
        if ('/' == _relative[0])
        {
            m_path = _relative;
        }
        else
        {
            string bpath = ubase.getPath ();
            string::size_type lastSlashPos = bpath.find_last_of ("/");
            assert (lastSlashPos != bpath.npos);
            bpath.erase (lastSlashPos);
            m_path = bpath + "/" + _relative;
        }
    }

    /* Remove fragment/cgiUri from the path */
    string::size_type locFrag, locCgi;
    locCgi = m_path.find_first_of ("?");
    if (locCgi != m_path.npos)
    {
        m_bIsCgi = true;
        m_cgiUri = m_path.substr (locCgi + 1);
        m_path = m_path.substr (0, locCgi);
    }
    locFrag = m_path.find_first_of ("#");
    if (locFrag != m_path.npos)
    {
        m_fragment =  m_path.substr (locFrag);
        m_path = m_path.substr (0, locFrag);
    }
    rc = URL_OK;
    simplifyPath ();
}

    void
URL::parse (const string &_url, string &proto, string &host, short &port,
        string &path, string &fragment, string &cgiUri, bool &iscgi)
{
    static const char _proto[] = "://";
    string url = _url;
    string::size_type loc, loc1;

    if ((loc = url.find (_proto)) == url.npos)
    {
        url = "http://" + url;
    }
    loc = url.find (_proto);
    proto = url.substr (0, loc);
    url = url.substr (loc + strlen(_proto));
    if ((loc = url.find_first_of ('/', 0)) == string::npos)
    {
        url += "/";
        loc = url.find_first_of ('/', 0 );
    }
    if (((loc1 = url.find_first_of (':', 0)) != url.npos) && (loc1 < loc))
    {
        host = url.substr (0, loc1);
        textutils::tolower (host);
        if (loc1 < loc - 1)
        {
            if (1 != sscanf (url.substr (loc1 + 1, (loc - loc1 - 1)).c_str(),
                        "%hd", &port))
            {
                port = 0;
            }
        }
    }
    else
    {
        host = url.substr (0, loc);
        textutils::tolower (host);
        if (proto.size () > 0)
        {
            struct servent *serv = getservbyname (proto.c_str (), "tcp");
            if (serv)
            {
                port = ntohs (serv->s_port);
            }
        }
    }
    path = url.substr (loc); /* Skip to first slash */
    txt_trim (path);
    loc = path.find_first_of ("?");
    if (loc != path.npos)
    {
        cgiUri = path.substr (loc + 1);
        path = path.substr (0, loc);
        iscgi = true;
    }
    loc1 = path.find_first_of ("#");
    if (loc1 != path.npos)
    {
        fragment = path.substr (loc1);
        path = path.substr (0, loc1);
    }
}

void URL::simplifyPath ()
{
    const char *cpath, *c, *d;
    string tmpstr;
    vector<string> dir;
    vector<string>::iterator diriter;
    int endDir = 0;

    dir.clear ();
    cpath = m_path.c_str ();
    c = cpath + 1;
    while (*c && isspace (*c))
    {
        c++;
    }
    assert (m_path[0] == '/');
    while ((d = strchr (c, '/')) != NULL)
    {
        tmpstr = "";
        while (c != d)
        {
            tmpstr += *c;
            c++;
        }
        c++;
        dir.push_back (tmpstr);
    }
    if (strlen (c) > 0)
    {
        dir.push_back (string (c));
    }
    else
    {
        endDir = 1;
    }
    int i = -1;
    diriter = dir.begin ();
    while (diriter != dir.end ())
    {
        if (string ("..") == *diriter)
        {
            if (i >= 0)
            {
                dir.erase (diriter);
                i--;
                diriter--;
            }
            dir.erase (diriter);
        }
        else if (string ( ".") == *diriter)
        {
            dir.erase (diriter);
        }
        else
        {
            i++;
            diriter++;
        }
    }
    diriter = dir.begin ();
    tmpstr = "";
    while (diriter != dir.end ())
    {
        tmpstr += "/" + (*diriter);
        diriter++;
    }
    if (endDir == 1)
    {
        tmpstr += "/";
    }
    m_path = tmpstr;
}

bool URL::isAbsolute (const string &url)
{
    string::size_type loc;
    loc = url.find ("://", 0);
    if (loc == string::npos)
    {
        return false;
    }
    if (getservbyname (url.substr (0, loc).c_str (), "tcp"))
    {
        return true;
    }
    return false;
}

string URL::getCanonical (bool dropFragment) const
{
    string ans;
    if (rc != URL_OK)
        return ans;
    ans.append (m_protocol);
    ans.append ("://");
    ans.append (m_host);
    ans.append (":");

    char cport[sizeof (short) * 2 * CHAR_BIT];
    sprintf (cport, "%hd", m_sPort);
    ans.append (cport);
    ans.append (m_path);
    if (!dropFragment && m_fragment.size ())
        ans.append (m_fragment);
    if (m_bIsCgi)
    {
        ans.append ("?");
        ans.append (m_cgiUri);
    }
    return ans;
}


const char URL::hex[] = "0123456789ABCDEF";
const unsigned char URL::acceptMask[] =
{
    /* 0x0 0x1 0x2 0x3 0x4 0x5 0x6 0x7 0x8 0x9 0xA 0xB 0xC 0xD 0xE 0xF */
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF,0xE,0x0,0xF,0xF,0xC,
    /* 2x  !"#$%&'()*+,-./  */
    0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0x8,0x0,0x0,0x0,0x0,0x0,
    /* 3x 0123456789:&lt;=>?  */
    0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,
    /* 4x @ABCDEFGHIJKLMNO  */
    0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0x0,0x0,0x0,0x0,0xF,
    /* 5X PQRSTUVWXYZ[\]^_  */
    0x0,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,
    /* 6x `abcdefghijklmno  */
    0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0x0,0x0,0x0,0x0,0x0
        /* 7X pqrstuvwxyz{\}~DEL */
};

bool URL::isAcceptable (unsigned char cx)
{
    if ('/' == cx)
        return true;
    return
        (cx >= 32) &&
        (cx < 128) &&
        ((acceptMask[cx - 32]) & XALPHAS);
}

string URL::escape (const string &str)
{
    string result;
    if (!str.empty ())
    {
        for (unsigned int i = 0; i < str.size (); i++)
        {
            unsigned char a = (unsigned char) (str[i]);
            if (!isAcceptable (a))
            {
                result += "%";
                result += hex[a >> 4];
                result += hex[a & 15];
            }
            else
            {
                result += str[i];
            }
        }
    }
    return result;
}


void URL::parseCgiUri ()
{
    if (!m_bIsCgi)
    {
        return;
    }
    if (m_bIsCgiUriParsed)
    {
        return;
    }
    string tmpstr = m_cgiUri;
    string::size_type locamp, loceq;

    for (;;)
    {
        loceq = tmpstr.find_first_of ("=");
        locamp = tmpstr.find_first_of ("&");
        if (loceq == string::npos)
        {
            break;
        }
        string name = tmpstr.substr (0, loceq);
        string value = tmpstr.substr (loceq + 1, locamp - loceq - 1);
        debug (("Name = '%s'. Value = '%s'.\n", name.c_str (), value.c_str ()));
        NVPair p (name, value);
        m_CGIAttrList.push_back (p);
        if (locamp == string::npos)
        {
            break;
        }
        tmpstr = tmpstr.substr (locamp + 1);
    }
    m_bIsCgiUriParsed = true;
    return;
}

bool URL::getCgiValue (const string &name, string &value)
{
    CGIAttrList::iterator cx = m_CGIAttrList.begin ();
    while (cx != m_CGIAttrList.end ())
    {
        if ((*cx).first == name)
        {
            value = (*cx).second;
            return true;
        }
        cx++;
    }
    return false;
}

string URL::getURI ()
{
    string retval;
    retval = m_path;
    if (m_bIsCgi)
    {
        retval += "?";
        retval += m_cgiUri;
    }
    return retval;
}

URL::ReturnCode URL::getParsestatus_t () const { return rc; }
string URL::getProtocol () const { return m_protocol; }
string URL::getHost () const { return m_host; }
short URL::getPort () const { return m_sPort; }
string URL::getPath () const { return m_path; }
string URL::getFragment () const { return m_fragment; }
string URL::getCgiUri () const { return m_cgiUri; }
bool URL::isCGI () const { return m_bIsCgi; }


END_NAMESPACE (hy);
