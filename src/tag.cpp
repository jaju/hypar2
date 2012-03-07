/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#include "tag.hpp"
#include "entitystream.hpp"

BEGIN_NAMESPACE (hy);

Tag::Tag () :
    m_pName (0),
    m_pStrEnd (0),
    m_pTagAttrString (0),
    m_bEndTag (false),
    m_bSelfClosing (false)
{
}

Tag::Tag (const _char *pName) :
    m_pName (pName),
    m_pTagAttrString (0),
    m_bEndTag (false),
    m_bSelfClosing (false)
{
    m_pStrEnd = pName + _strlen (pName);
}

Tag::Tag (_char *pTagString, bool bParseAttributes) :
    m_pName (0),
    m_pTagAttrString (0),
    m_bEndTag (false),
    m_bSelfClosing (false)
{
    m_pStrEnd = pTagString + _strlen (pTagString);
    parseTag (pTagString, bParseAttributes);
}

    void
Tag::reset ()
{
    m_pName = m_pStrEnd = m_pTagAttrString = 0;
    m_bEndTag = m_bSelfClosing = false;
}

    int
Tag::parseFurther ()
{
    if (m_pTagAttrString)
    {
        parseAttributes (m_pTagAttrString);
    }
    return m_attrList.size ();
}

    int
Tag::parseTag (_char *pTagString, bool bParseAttributes)
{
    _char *pTmpStr = pTagString;

    while ((pTmpStr < m_pStrEnd) && _isspace (*pTmpStr))
    {
        pTmpStr++;
    }

    if (*pTmpStr == SLASH)
    {
        m_bEndTag = true;
        pTmpStr++;
    }

    while ((pTmpStr < m_pStrEnd) && _isspace (*pTmpStr))
    {
        pTmpStr++;
    }

    m_pName = pTmpStr;

    while ((pTmpStr < m_pStrEnd) && !_isspace(*pTmpStr))
    {
        if (*pTmpStr == SLASH)
        {
            m_bSelfClosing = true;
            *pTmpStr = NULLCHAR;
        }
        else
        {
            *pTmpStr = _tolower (*pTmpStr);
        }
        pTmpStr++;
    }

    if (pTmpStr < m_pStrEnd)
    {
        *pTmpStr = NULLCHAR;
        pTmpStr++;
    }

    while ((pTmpStr < m_pStrEnd) && _isspace (*pTmpStr))
    {
        pTmpStr++;
    }
    if ((pTmpStr < m_pStrEnd))
    {
        _char *pSelfClosing = pTmpStr;
        while ((pSelfClosing + 1) < m_pStrEnd)
        {
            pSelfClosing++;
        }
        while (_isspace (*pSelfClosing))
        {
            pSelfClosing--;
        }
        if (SLASH == *pSelfClosing)
        {
            m_bSelfClosing = true;
            *pSelfClosing = NULLCHAR;
        }
        m_pTagAttrString = pTmpStr;
        if (bParseAttributes)
        {
            parseAttributes (pTmpStr);
        }
    }
    return 0;
}

    void
Tag::toString (_string &targetString)
{
    targetString += LANGLE;
    if (m_bEndTag && !m_bSelfClosing)
    {
        targetString += SLASH;
    }
    if (m_pName)
    {
        targetString += m_pName;
    }

    if (m_attrList.size () > 0)
    {
        TagAttrList::iterator avx = m_attrList.begin ();
        while (avx != m_attrList.end ())
        {
            targetString += ' ';
            targetString += (*avx).first;
            targetString += "=\"";
            targetString += (*avx).second;
            targetString += '\"';
            avx++;
        }
    }
    else if (m_pTagAttrString)
    {
        targetString += ' ';
        targetString += m_pTagAttrString;
    }
    if (m_bSelfClosing && !m_bEndTag)
    {
        targetString += "/>";
    }
    else
    {
        targetString += GANGLE;
    }
}

    inline int
Tag::getQuotedName (_char *&pStr, _char **pName, bool bSetNull)
{
    while ((pStr < m_pStrEnd) && _isspace (*pStr))
    {
        pStr++;
    }
    if (pStr >= m_pStrEnd)
    {
        return -1;
    }
    if ('"' == *pStr || '\'' == *pStr)
    {
        _char wideQuote = *pStr;
        pStr++;
        if (pStr >= m_pStrEnd)
            return -1;
        *pName = pStr;
        while ((pStr < m_pStrEnd) && *pStr != wideQuote)
        {
            pStr++;
        }
        if (pStr >= m_pStrEnd)
            return -1;
        if (*pStr != '=')
        {
            return 1;
        }
        if (bSetNull)
        {
            *pStr = '\0';
            pStr++;
        }
    }
    else
    {
        *pName = pStr;
        while ((pStr < m_pStrEnd) && (*pStr != '=') && !_isspace(*pStr))
        {
            pStr++;
        }
        if (pStr >= m_pStrEnd)
            return -1;
        while ((pStr < m_pStrEnd) && _isspace (*pStr))
        {
            pStr++;
        }
        if (pStr >= m_pStrEnd)
            return -1;
        if (*pStr != '=')
        {
            pStr++;
            return 1;
        }
        if (bSetNull)
        {
            *pStr = '\0';
            pStr++;
        }
    }
    while ((pStr < m_pStrEnd) && _isspace(*pStr))
    {
        pStr++;
    }
    return 0;
}

    inline int
Tag::getQuotedValue (_char *&pStr, _char **pValue, bool bSetNull)
{
    while ((pStr < m_pStrEnd) && _isspace (*pStr))
    {
        pStr++;
    }
    if (pStr >= m_pStrEnd)
    {
        return -1;
    }
    if ('"' == *pStr || '\'' == *pStr)
    {
        _char wideQuote = *pStr;
        pStr++;
        if (pStr >= m_pStrEnd)
            return -1;
        *pValue = pStr;
        while ((pStr < m_pStrEnd) && *pStr != wideQuote)
        {
            pStr++;
        }
        if (bSetNull)
        {
            *pStr = '\0';
        }
        pStr++;
        return 0;
    }
    else
    {
        *pValue = pStr;
        while ((pStr < m_pStrEnd) && !_isspace (*pStr))
        {
            pStr++;
        }
        if (bSetNull)
        {
            *pStr = '\0';
        }
        pStr++;
        return 0;
    }
}

    int
Tag::parseAttributes (_char *&pTagAttrString)
{
    int retval = -1;
    _char *pName, *pValue;
    int propCount = 0;
    while (1)
    {
        retval = getQuotedName (pTagAttrString, &pName, true);
        if (retval < 0)
        {
            break;
        }
        else if (1 == retval)
        {
            continue;
        }
        if (getQuotedValue (pTagAttrString, &pValue, true) < 0)
        {
            break;
        }
        std::pair<_char *, _char *> p (pName, pValue);
        m_attrList.push_back (p);
        propCount++;
    }
    return propCount;
}

const _char *Tag::findProperty (const _char *pName)
{
    TagAttrList::iterator hx = m_attrList.begin ();
    while (hx != m_attrList.end ())
    {
        if (_strcasecmp ((*hx).first, pName) == 0)
        {
            return (*hx).second;
        }
        hx++;
    }
    return 0;
}

END_NAMESPACE (hy);
