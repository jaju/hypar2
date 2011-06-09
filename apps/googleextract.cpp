/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#include "xml.hpp"
#include "html.hpp"
#include "domutils.hpp"

#if !DEBUG
#define DEBUG 0
#endif
#include "debug.hpp"

#include <iostream>
#include <fstream>
#include <queue>
#include <string>

BEGIN_C_DECLS
#include <assert.h>
#include <stdio.h>
#include <locale.h>
END_C_DECLS

USING_NAMESPACE (std);
#if __GNUC__ > 2
USING_NAMESPACE (__gnu_cxx);
#endif

void usage (int argc, char *argv[])
{
    cerr << "Usage: " << argv[0] << " <file>" << endl;
}

int main (int argc, char *argv[])
{
    if (argc != 2)
    {
        usage (argc, argv);
        return -1;
    }
    hy::HTMLDoc h (hy::HTMLDoc::kFILE, argv[1]);
    hy::NodeList nodeList;
    hy::HTMLNode *pRoot = h.getRootNode ();
    pRoot->collectNodesByName(L("h2"), &nodeList);
    nodeList = hy::filterNodesByAttrVal (L("class"), L("r"), nodeList);
    hy::NodeList aNodeList;
    hy::NodeList::iterator x = nodeList.begin ();
    for (; x != nodeList.end (); x++)
    {
        (*x)->collectNodesByName (L("a"), &aNodeList);
    }
    aNodeList = hy::filterNodesByAttrVal (L("class"), L("l"), aNodeList);
    x = aNodeList.begin ();
    for (; x != aNodeList.end (); x++)
    {
        hy::HTMLNode *pNode = (hy::HTMLNode *) *x;
        const _char *href = pNode->findProperty (L("href"));
        if (href)
        {
            _string s; pNode->toText (s, true);
        cout << s << " - " << href << endl;
        }
    }
}
