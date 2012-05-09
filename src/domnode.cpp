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

const char * const DOMData::defaultName = "root";
DOMData::DOMData() :
    m_pPropertyMap (0),
    m_type(DOMNode::ELEMENT),
    m_pName (defaultName),
    m_pContent (0),
    m_bSelfClosing (true)
{
}

void DOMData::initType (DOMData::NodeType nodeType, const _char *pStr)
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
    //assert(m_pPropertyMap != other.m_pPropertyMap); // TODO - handle this. It fails.
    if (other.m_pPropertyMap)
    {
        PropertyMap::iterator pmx = other.m_pPropertyMap->begin ();
        while (pmx != m_pPropertyMap->end ())
        {
            setProperty ((*pmx).first, (*pmx).second);
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
        m_pPropertyMap->insert (*avx);
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
DOMData::setProperty (const _char *pName, const _char *pValue)
{
    return setProperty(make_pair(pName, pValue));
}
    int
DOMData::setProperty (pair<const _char *, const _char *> p)
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
DOMData::getProperty (const _char *pName) const
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
            targetString += textutils::echoSpaces (node->level());
            targetString += "<";
            targetString += node->name();
            if (node->propertyMap())
            {
                PropertyMap::const_iterator pmx = node->propertyMap()->begin ();
                while (pmx != node->propertyMap()->end ())
                {
                    targetString += " ";
                    targetString += pmx->first;
                    targetString += "=\"";
                    targetString += pmx->second;
                    targetString += "\"";
                    pmx++;
                }
            }
#if 1
            targetString += " level=\"";
            targetString += textutils::_itos (node->level());
            targetString += "\" horlevel=\"";
            targetString += textutils::_itos (node->horizontalLevel());
            targetString += "\"";
#endif
            if (node->child())
            {
                targetString += ">\n";
                toString(node->child(), targetString);
            }
            else
            {
                if (node->selfClosing())
                {
                    targetString += "/>\n";
                }
                else
                {
                    targetString += "></";
                    targetString += node->name();
                    targetString += " selfclosing=\"0\"";
                    targetString += ">\n";
                }
                break;
            }
            targetString += "</";
            targetString += node->name();
            if (node->propertyMap()->find("childrenforceclosed") != node->propertyMap()->end())
            {
                targetString += " culpritchildren=\"1\"";
            }
            else if (node->propertyMap()->find("forceclosed") != node->propertyMap()->end())
            {
                targetString += " forceclosed=\"1\"";
            }
            targetString += ">\n";
            break;
        case DOMData::TEXT:
            if (node->content())
                targetString += node->content();
            break;
        case DOMData::COMMENT:
            if (node->content()) {
                targetString += "<";
                targetString += node->content();
                targetString += ">\n";
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
                targetString += " ";
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
