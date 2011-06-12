#include "url.hpp"
#include "html.hpp"
#include "debug.hpp"
#include <cstdlib>

BEGIN_NAMESPACE (hy);
using namespace std;

/* Definitions of the URLCollector class and functions */
//unsigned int URLCollector::m_iMaxURLLen = 0;
//string URLCollector::m_baseHref;

URLCollector::URLCollector (DOMNode *pNode) :
    m_pEncoding(0),
    m_iMaxURLLen (0),
    m_baseHref ("")
{
    //m_baseHref = "";
    //collect(pNode);
}

URLCollector::URLCollector (_char *pBuffer) :
    m_pEncoding(0)
{
    m_baseHref = "";
    collect (pBuffer);
}

XML::EntityCbRetval URLCollector::href_callback (Tag *pTag, void *pArg)
{
    static const _char *pURLTagName = L("a");
    static const _char *pBase = L("base");
    URLCollector *pThis = (URLCollector *) pArg;
    if (!pThis->m_baseHref.size() && _strcasecmp(pBase, pTag->m_pName) == 0)
    {
        const _char *pVal = pTag->findProperty(L("href"));
        if (pVal)
        {
            int l = _strlen(pVal);
            if (l > 0)
            {
                char *s = new char[sizeof(_char) * (l + 1)];
                strncpy(s, pVal, l + 1);
                pThis->m_baseHref = reinterpret_cast<char *>(s);
                delete s;
            }
        }
        return XML::DROP_SIMPLE;
    }
    if (_strcasecmp(pURLTagName, pTag->m_pName) == 0)
    {
        if (!pTag->m_bEndTag)
        {
            const _char *pVal = pTag->findProperty(L("href"));
            if (pVal)
            {
                if (_strlen(pVal) > pThis->m_iMaxURLLen)
                {
                    pThis->m_iMaxURLLen = _strlen(pVal);
                }
                URL *u = new URL (pVal, pThis->m_baseHref);
                const _char *pRelVal = pTag->findProperty (L("rel"));
                if (pRelVal && !_strcasecmp (pRelVal, L("nofollow")))
                {
                    u->setNoFollow ();
                }
                if (u->getParsestatus_t() == URL::URL_OK)
                {
                    pThis->m_URLList.push_back(u);
                }
            }
        }
    }
    return XML::DROP_SIMPLE;
}

bool URLCollector::text_callback (_char *p, void *pArg)
{
    return false;
}

bool URLCollector::comm_callback (_char *p, void *pArg)
{
    return false;
}

int URLCollector::collect (_char *pBuffer, const char *pEncoding)
{
    setlocale (LC_ALL, pEncoding);
    URLCollector::m_iMaxURLLen = 0;
    XML h (HTMLDoc::s_defaultHTMLTable);
    h.tagCb = &URLCollector::href_callback;
    h.textCb = &URLCollector::text_callback;
    h.commentCb = &URLCollector::comm_callback;
    h.m_pCallbackArg = (void *) this;
    DOMNode *node = DOMNode::create();
    node->setType(DOMNode::ELEMENT);
    node->setName(L("root"));

    node->setLevel(-1);
    h.parse (pBuffer, node);
    delete node;
    return m_URLList.size ();
}

bool URL::operator == (const URL &u) const
{
    return this->equals (u);
}

bool URL::equals (const URL &u) const
{
    bool bSimpleMatch = (
            (getHost () == u.getHost ())
            &&
            (getPort () == u.getPort ())
            );
    if (!bSimpleMatch)
    {
        debug (("Host1: '%s'\n", getHost ().c_str ()));
        debug (("Port1: '%d'\n", getPort ()));
        debug (("Host2: '%s'\n", u.getHost ().c_str ()));
        debug (("Port2: '%d'\n", u.getPort ()));
        return false;
    }
    string p1 = getPath ();
    string p2 = u.getPath ();

    /* 'normalize' the end portion */
    if (p1.at (p1.length () - 1) != '/') { p1 += '/'; }
    if (p2.at (p2.length () - 1) != '/') { p2 += '/'; }

    debug (("Comparing '%s' and '%s'\n", p1.c_str (), p2.c_str ()));
    return (p1 == p2);
}

END_NAMESPACE (hy);
