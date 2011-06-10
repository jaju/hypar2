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


DOMNode *DOMNode::clone () const
{
    DOMNode *pRetNode = clone (m_type, m_pName);
    if (m_pPropertyMap)
    {
        PropertyMap::iterator pmx = m_pPropertyMap->begin ();
        while (pmx != m_pPropertyMap->end ())
        {
            pRetNode->addProperty ((*pmx).first, (*pmx).second);
            pmx++;
        }
    }
    return pRetNode;
}

    DOMNode *
DOMNode::clone (NodeType nodeType, const _char *pName) const
{
    if (!pName) { pName = m_pName; }
    DOMNode *pRetNode = new DOMNode (nodeType, pName);
    return pRetNode;
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

void DOMNode::reset() {
    TreeNode<DOMData>::reset();
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
        m_pPropertyMap->clear ();
        if (!m_pPropertyMap)
            return -1;
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
DOMNode::recursiveCb (DOMData::CallBack f)
{
    if (TreeNode::child)
    {
        child()->recursiveCb (f);
    }

    if (TreeNode::next)
    {
        next()->recursiveCb (f);
    }

    return;
}

    void
DOMNode::toString (_string &targetString, bool bChildOnly)
{
    switch (m_type)
    {
        case ELEMENT:
            targetString += textutils::echoSpaces (level);
            targetString += L("<");
            targetString += m_pName;
            /* TODO - Add the attributes */
            if (m_pPropertyMap)
            {
                PropertyMap::iterator pmx = m_pPropertyMap->begin ();
                while (pmx != m_pPropertyMap->end ())
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
            targetString += textutils::_itos (level);
            targetString += L("\" horlevel=\"");
            targetString += textutils::_itos (horizontalLevel);
            targetString += L("\"");
#endif
            if (TreeNode::child)
            {
                targetString += L(">\n");
                child()->toString(targetString);
            }
            else
            {
                if (m_bSelfClosing)
                {
                    targetString += L("/>\n");
                }
                else
                {
                    targetString += L("></");
                    targetString += m_pName;
                    targetString += " selfclosing=\"1\"";
                    targetString += L(">\n");
                }
                break;
            }
            targetString += L("</");
            targetString += m_pName;
            if (m_pPropertyMap->find(L("childrenforceclosed")) != m_pPropertyMap->end())
            {
                targetString += " culpritchildren=\"1\"";
            }
            else if (m_pPropertyMap->find(L("forceclosed")) != m_pPropertyMap->end())
            {
                targetString += " forceclosed=\"1\"";
            }
            targetString += L(">\n");
            break;
        case TEXT:
            targetString += m_pContent;
            break;
        case COMMENT:
            targetString += L("<");
            targetString += m_pContent;
            targetString += L(">\n");
            break;
        default:
            break;
    }
    if (!bChildOnly && TreeNode::next)
    {
        next()->toString (targetString);
    }
}

_string DOMNode::toString(bool bChildOnly)
{
    _string result;
    toString(result, bChildOnly);
    return result;
}


void DOMNode::toText (_string &targetString, bool bChildOnly)
{
    switch (m_type)
    {
        case ELEMENT:
            if (TreeNode::child)
            {
                child()->toText (targetString);
            }
            break;
        case TEXT:
            targetString += m_pContent;
            break;
        case COMMENT:
        default:
            break;
    }
    if (!bChildOnly && TreeNode::next)
    {
        next()->toText (targetString);
    }
}

END_NAMESPACE (hy);
