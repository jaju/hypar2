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
        pThisAttrVal = (*_x)->getProperty (pAttr);
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

END_NAMESPACE (hy);
