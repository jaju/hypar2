/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#include "domnode.hpp"
#include "textutils.hpp"

#include <cassert>
#include <iostream>
#include "debug.hpp"

USING_NAMESPACE (std);

BEGIN_NAMESPACE (hy);

DOMData::DOMData() :
    m_pPropertyMap (0),
    m_pName (0),
    m_pContent (0),
    m_bSelfClosing (true)
{
}

void DOMData::initData (DOMData::NodeType nodeType, const _char *pStr)
{
    m_type = nodeType;
    switch (m_type)
    {
        case ELEMENT:
            m_pName = pStr;
            break;
        case TEXT:
        case COMMENT:
            m_pContent = pStr;
            break;
        default:
            break;
    }
}

DOMData::~DOMData ()
{
    safe_delete (m_pPropertyMap);
}


DOMData::DOMData (const DOMData &other)
{
    (*this) = other; // Copies the basic types.
    if (other.m_pPropertyMap)
    {
        PropertyMap::iterator pmx = other.m_pPropertyMap->begin ();
        while (pmx != m_pPropertyMap->end ())
        {
            addProperty ((*pmx).first, (*pmx).second);
            pmx++;
        }
    }
}

int DOMData::copyAttributes (Tag *tag)
{
    int retval = 0;
    if (!m_pPropertyMap)
    {
        m_pPropertyMap = new PropertyMap;
        assert (m_pPropertyMap);
    }
    TagAttrList::const_iterator avx = tag->m_attrList.begin ();
    while (avx != tag->m_attrList.end ())
    {
        pair <_char *, _char *> p (avx->first, avx->second);
        m_pPropertyMap->insert (p);
        avx++;
        retval++;
    }
    return retval;
}

    void
DOMData::reset ()
{
    if (m_pPropertyMap)
    {
        m_pPropertyMap->clear ();
    }
    m_type = END;
    m_pName = 0;
    m_pContent = 0;
}


    int
DOMData::addProperty (const _char *pName, const _char *pValue)
{
    if (!m_pPropertyMap)
    {
        m_pPropertyMap = new PropertyMap;
        m_pPropertyMap->clear ();
        if (!m_pPropertyMap)
            return -1;
    }
    (*m_pPropertyMap)[pName] = pValue;
    return 0;
}

    int
DOMData::addProperty (pair<const _char *, const _char *> &p)
{
    if (!m_pPropertyMap)
    {
        m_pPropertyMap = new PropertyMap;
    }
    if ((m_pPropertyMap->insert (p)).second == true)
    {
        return 0;
    }
    return -1;
}

    const _char *
DOMData::findProperty (const _char *pName) const
{
    if (m_pPropertyMap)
    {
        PropertyMap::iterator px;
        px = m_pPropertyMap->find (pName);
        if (px != m_pPropertyMap->end ())
        {
            return (*px).second;
        }
    }
    return 0;
}

    void
toString (DOMNode *node, _string &targetString, bool bChildOnly)
{
    switch (node->type())
    {
        case DOMData::ELEMENT:
            targetString += textutils::echoSpaces (node->level);
            targetString += L("<");
            targetString += node->name();
            /* TODO - Add the attributes */
            if (node->getPropertyMap())
            {
                PropertyMap::const_iterator pmx = node->getPropertyMap()->begin ();
                while (pmx != node->getPropertyMap()->end ())
                {
                    targetString += L(" ");
                    targetString += pmx->first;
                    targetString += L("=\"");
                    targetString += pmx->second;
                    targetString += L("\"");
                    pmx++;
                }
            }
#if 1
            targetString += L(" level=\"");
            targetString += textutils::_itos (node->level);
            targetString += L("\" horlevel=\"");
            targetString += textutils::_itos (node->horizontalLevel);
            targetString += L("\"");
#endif
            if (node->child())
            {
                targetString += L(">\n");
                toString(node->child(), targetString);
            }
            else
            {
                if (node->selfClosing())
                {
                    targetString += L("/>\n");
                }
                else
                {
                    targetString += L("></");
                    targetString += node->name();
                    targetString += " selfclosing=\"1\"";
                    targetString += L(">\n");
                }
                break;
            }
            targetString += L("</");
            targetString += node->name();
            if (node->getPropertyMap()->find(L("childrenforceclosed")) != node->getPropertyMap()->end())
            {
                targetString += " culpritchildren=\"1\"";
            }
            else if (node->getPropertyMap()->find(L("forceclosed")) != node->getPropertyMap()->end())
            {
                targetString += " forceclosed=\"1\"";
            }
            targetString += L(">\n");
            break;
        case DOMData::TEXT:
            if (node->content())
            targetString += node->content();
            break;
        case DOMData::COMMENT:
            if (node->content()) {
            targetString += L("<");
            targetString += node->content();
            targetString += L(">\n");
            }
            break;
        default:
            break;
    }
    if (!bChildOnly && node->next())
    {
        toString (node->next(), targetString);
    }
}

_string toString(DOMNode *node, bool bChildOnly)
{
    _string result;
    toString(node, result, bChildOnly);
    return result;
}


void toText (DOMNode *node, _string &targetString, bool bChildOnly)
{
    switch (node->type())
    {
        case DOMData::ELEMENT:
            if (node->child())
            {
                toText (node->child(), targetString);
            }
            break;
        case DOMData::TEXT:
            if (node->content()) {
                targetString += node->content();
                targetString += L(" ");
            }
            break;
        case DOMData::COMMENT:
        default:
            break;
    }
    if (!bChildOnly && node->next())
    {
        toText (node->next(), targetString);
    }
}

END_NAMESPACE (hy);
