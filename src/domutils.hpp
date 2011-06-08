/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#pragma once
#include "localdefs.h"
#include "xml.hpp"

#include <list>

using std::list;

BEGIN_NAMESPACE (hy);

typedef list <DOMNode * > NodeList;
NodeList filterNodesByLevel (int iLevel, int iHorLevel, NodeList &n);
NodeList filterNodesByName (const _char *pName, NodeList &_nodeList);
NodeList filterNodesByAttrVal (const _char *pAttr, const _char *pVal,
        NodeList &_nodeList);

int toText (DOMNode *pRootNode, _string &outString);

END_NAMESPACE (hy);
