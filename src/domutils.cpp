/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#include "localdefs.h"
#include "domutils.hpp"
#include "debug.hpp"

#include <assert.h>
#include <iostream>
using std::cerr;
using std::endl;

BEGIN_NAMESPACE (hy);

int DOMNode::collectNodesByName (const _char *pName, NodeList *pNodeList, bool siblings)
{
    assert (pName);
    int retval = 0;
    if (name() && (_strcasecmp (pName, name()) == 0))
    {
        pNodeList->push_back (this);
        retval++;
    }
    if (siblings && next())
    {
        retval += next()->collectNodesByName (pName, pNodeList, true);
    }
    if (child())
    {
        retval += child()->collectNodesByName (pName, pNodeList, true);
    }
    return retval;
}


int DOMNode::collectNodesByAttrVal (const _char *pAttr, const _char *pVal,
        NodeList *pNodeList, bool siblings)
{
    assert (pAttr);
    int retval = 0;
    const _char *pThisAttrVal = 0;

    pThisAttrVal = findProperty (pAttr);
    if (pThisAttrVal)
    {
        if (pVal && (_strcmp (pVal, pThisAttrVal) == 0))
        {
            pNodeList->push_back (this);
        }
        retval++;
    }
    if (siblings && next())
    {
        retval += next()->collectNodesByAttrVal (pAttr, pVal, pNodeList, true);
    }
    if (child())
    {
        retval += child()->collectNodesByAttrVal (pAttr, pVal, pNodeList, true);
    }
    return retval;
}


int DOMNode::collectChildrenByName (const _char *pName, NodeList *pNodeList)
{
    DOMNode *pChildNode = child();
    assert (pName);
    if (pNodeList->size ())
    {
        cerr << "NodeList not empty!" << endl;
        pNodeList->clear ();
    }

    while (pChildNode)
    {
        if ((hy::DOMNode::ELEMENT == pChildNode->type()) &&
                pChildNode->name() &&
                (_strcasecmp (pName, pChildNode->name()) == 0))
        {
            debug (("Child node: '%s'\n", pChildNode->name()));
            pNodeList->push_back (pChildNode);
            debug (("Added"));
        }
        pChildNode = pChildNode->next();
    }
    return pNodeList->size ();
}

int DOMNode::collectChildrenByType (DOMNode::NodeType type,
        NodeList *pNodeList)
{
    DOMNode *pChildNode = child();
    if (pNodeList->size ())
    {
        cerr << "NodeList not empty!" << endl;
        pNodeList->clear ();
    }
    while (pChildNode)
    {
        if (type == pChildNode->type())
        {
            pNodeList->push_back (pChildNode);
        }
        pChildNode = pChildNode->next();
    }
    return pNodeList->size ();
}

DOMNode *DOMNode::findFirstChild (const _char *pName)
{
    if (!child()) return 0;
    return child()->findNext (pName, true);
}

DOMNode *DOMNode::findFirstChild (NodeType type)
{
    if (!child()) return 0;
    return child()->findNext (type, true);
}

DOMNode *DOMNode::findNext (const _char *pName, bool bConsiderThis)
{
    if (!pName) return 0;
    DOMNode *pNode = (bConsiderThis ? this : next());
    while (pNode && pNode->name() && (_strcasecmp (pName, pNode->name())))
        pNode = pNode->next();
    return pNode;
}

DOMNode *DOMNode::findNext (NodeType type, bool bConsiderThis)
{
    DOMNode *pNode = (bConsiderThis ? this : next());
    while (pNode && (type != pNode->type()))
        pNode = pNode->next();
    return pNode;
}

NodeList filterNodesByName (const _char *pName, NodeList &_nodeList)
{
    NodeList nodeList;
    NodeList::iterator _x = _nodeList.begin ();

    assert (pName);

    while (_x != _nodeList.end ())
    {
        if ((*_x)->name() && (_strcasecmp (pName, (*_x)->name()) == 0))
        {
            nodeList.push_back (*_x);
        }
        _x++;
    }
    return nodeList;
}

NodeList filterNodesByAttrVal (const _char *pAttr, const _char *pVal,
        NodeList &_nodeList)
{
    NodeList nodeList;
    NodeList::iterator _x = _nodeList.begin ();
    assert (pAttr);
    const _char *pThisAttrVal = 0;

    while (_x != _nodeList.end ())
    {
        pThisAttrVal = (*_x)->findProperty (pAttr);
        if (pThisAttrVal)
        {
            if (pVal && (_strcmp (pVal, pThisAttrVal) == 0))
            {
                //cout << "MATCHED: pVal = " << pVal << " and this attribute = " << pThisAttrVal << endl;
                nodeList.push_back (*_x);
            }
            else
            {
                nodeList.push_back (*_x);
            }
        }
        _x++;
    }
    return nodeList;
}

NodeList filterNodesByLevel (int iLevel, int iHorLevel, NodeList &_nodeList)
{
    NodeList nodeList;
    NodeList::iterator _x = _nodeList.begin ();

    assert (iLevel >= 0 || iHorLevel >= 0);

    while (_x != _nodeList.end ())
    {
        if (iLevel >= 0)
        {
            if (iLevel == (*_x)->level())
            {
                if (iHorLevel >= 0 && (iHorLevel == (*_x)->horizontalLevel()))
                {
                    nodeList.push_back (*_x);
                }
                else
                {
                    nodeList.push_back (*_x);
                }
            }
        }
        else if (iHorLevel >= 0 && (iHorLevel == (*_x)->horizontalLevel()))
        {
            nodeList.push_back (*_x);
        }
        _x++;
    }
    return nodeList;
}

int toText (DOMNode *pNode, _string &outString)
{
    if (DOMNode::TEXT == pNode->type() && pNode->content())
    {
        outString += pNode->content();
        outString += L(" ");
        return 1;
    }
    else if (DOMNode::ELEMENT == pNode->type())
    {
        int retval = 0;
        DOMNode *pChild = pNode->child();
        while (pChild)
        {
            retval += toText (pChild, outString);
            pChild = pChild->next();
        }
        return retval;
    }
    return 0;
}

END_NAMESPACE (hy);
