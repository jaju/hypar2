/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#include "hypar/domnode.hpp"
#include "hypar/textutils.hpp"

#include <assert.h>
#include <iostream>
#include "hypar/debug.hpp"

USING_NAMESPACE (std);

BEGIN_NAMESPACE (hy);

DOMNode::DOMNode (DOMNode::NodeType nodeType, const _char *pStr) :
    m_pPropertyMap (0),
    m_pParent (0),
    m_pPrev (0),
    m_pNext (0),
    m_pChild (0),
    m_pLast (0),
    m_type (nodeType),
    m_pName (0),
    m_pContent (0),
    m_bSelfClosing (true),
    m_iLevel (0),
    m_iHorLevel (0)
{
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

DOMNode::~DOMNode ()
{
    DOMNode *pTmpNode = m_pChild;

    while (m_pChild)
    {
        pTmpNode = m_pChild;
        m_pChild = m_pChild->m_pNext;
        delete pTmpNode;
    }
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

    int
DOMNode::detach ()
{
    if (!m_pParent && !m_pPrev && !m_pNext)
        return -1;
    if (m_pPrev)
    {
        m_pPrev->m_pNext = m_pNext;
    }
    else
    {
        if (m_pParent)
        {
            m_pParent->m_pChild = m_pNext;
        }
    }
    if (m_pNext)
    {
        m_pNext->m_pPrev = m_pPrev;
    }
    else
    {
        if (m_pParent)
        {
            m_pParent->m_pLast = 0;
        }
    }
    m_pParent = 0;
    m_pPrev = 0;
    m_pNext = 0;
    return 0;
}

    int
DOMNode::attachAsChild (DOMNode *pNode)
{
    if (!pNode)
        return -1;
    if (!m_pChild)
    {
        m_pChild = m_pLast = pNode;
        pNode->m_pParent = this;
        pNode->m_pPrev = pNode->m_pNext = 0;
        pNode->initLevel (m_iLevel + 1, true);
        return 1;
    }
    else
    {
        int retval = m_pLast->attachAsNext (pNode);
        assert (m_pLast == pNode);
        return retval;
    }
}

    int
DOMNode::insertAsParent (DOMNode *pNode)
{
    int retval = -1;
    if (!pNode)
    {
        return -1;
    }
    if (m_pPrev)
    {
        m_pPrev->m_pNext = pNode;
    }
    if (m_pNext)
    {
        m_pNext->m_pPrev = pNode;
    }
    if (m_pParent)
    {
        if (this == m_pParent->m_pChild)
        {
            m_pParent->m_pChild = pNode;
        }
        if (this == m_pParent->m_pLast)
        {
            m_pParent->m_pLast = pNode;
        }
    }
    m_pPrev = m_pNext = m_pParent = 0;
    retval = pNode->attachAsChild (this);
    return retval;
}

    int
DOMNode::insertAsChild (DOMNode *pNewChild)
{
    DOMNode *pChild = m_pChild;
    while (pChild)
    {
        pChild->m_pParent = pNewChild;
        pChild = pChild->m_pNext;
    }
    pNewChild->m_pChild = m_pChild;
    pNewChild->m_pLast = m_pLast;
    m_pChild = pNewChild;
    m_pLast = pNewChild;
    m_pChild->initLevel(m_iLevel + 1, true);
    return 1;
}

    int
DOMNode::attachAsPrevious (DOMNode *pNode)
{
    if (!pNode)
        return -1;
    pNode->m_pNext = this;
    pNode->m_pPrev = m_pPrev;
    pNode->m_pParent = m_pParent;
    if (m_pPrev)
    {
        m_pPrev->m_pNext = pNode;
    }
    else
    {
        if (m_pParent)
        {
            m_pParent->m_pChild = pNode;
        }
    }
    m_pPrev = pNode;
    pNode->initLevel(m_iLevel, false);
    return 0;
}

    int
DOMNode::attachAsNext (DOMNode *pNode)
{
    if (!pNode)
        return -1;
    pNode->m_pNext = m_pNext;
    pNode->m_pPrev = this;
    pNode->m_pParent = m_pParent;
    if (m_pNext)
    {
        m_pNext->m_pPrev = pNode;
    }
    else
    {
        if (m_pParent)
        {
            m_pParent->m_pLast = pNode;
        }
    }
    m_pNext = pNode;
    pNode->initLevel (m_iLevel, false);
    return 0;
}

int DOMNode::copyAttributes (Tag *tag)
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
DOMNode::reset ()
{
    if (m_pPropertyMap)
    {
        m_pPropertyMap->clear ();
    }
    m_pParent = 0;
    m_pPrev = 0;
    m_pNext = 0;
    m_pChild = 0;
    m_pLast = 0;
    m_type = END;
    m_pName = 0;
    m_pContent = 0;
    m_iLevel = 0;
    m_iHorLevel = 0;
}

    int
DOMNode::initLevel (int iLevel, bool bNext)
{
    m_iLevel = iLevel;
    if (m_pChild)
    {
        m_pChild->initLevel (m_iLevel + 1, true);
    }
    if (bNext && m_pNext)
    {
        m_pNext->initLevel (m_iLevel, true);
    }
    return 0;
}

    int
DOMNode::addProperty (const _char *pName, const _char *pValue)
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
DOMNode::addProperty (pair<const _char *, const _char *> &p)
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
DOMNode::findProperty (const _char *pName) const
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
DOMNode::recursiveCb (DOMNode::CallBack f)
{
    if (m_pChild)
    {
        m_pChild->recursiveCb (f);
    }

    if (m_pNext)
    {
        m_pNext->recursiveCb (f);
    }

    return;
}

    void
DOMNode::toString (_string &targetString, bool bChildOnly) const
{
    switch (m_type)
    {
        case ELEMENT:
            targetString += textutils::echoSpaces (m_iLevel);
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
            targetString += textutils::_itos (m_iLevel);
            targetString += L("\" horlevel=\"");
            targetString += textutils::_itos (m_iHorLevel);
            targetString += L("\"");
#endif
            if (m_pChild)
            {
                targetString += L(">\n");
                m_pChild->toString(targetString);
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
    if (!bChildOnly && m_pNext)
    {
        m_pNext->toString (targetString);
    }
}

_string DOMNode::toString(bool bChildOnly) const
{
    _string result;
    toString(result, bChildOnly);
    return result;
}


void DOMNode::toText (_string &targetString, bool bChildOnly) const
{
    switch (m_type)
    {
        case ELEMENT:
            if (m_pChild)
            {
                m_pChild->toText (targetString);
            }
            break;
        case TEXT:
            targetString += m_pContent;
            break;
        case COMMENT:
        default:
            break;
    }
    if (!bChildOnly && m_pNext)
    {
        m_pNext->toText (targetString);
    }
}

END_NAMESPACE (hy);