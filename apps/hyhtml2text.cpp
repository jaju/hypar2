/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#include "localdefs.h"
#include "url.hpp"
#include "html.hpp"

#if !DEBUG
#define DEBUG 0
#endif
#include "debug.hpp"
#include "textutils.hpp"
#include "mmap.hpp"

#include <iostream>
#include <string>
#include <cassert>
#include <vector>

BEGIN_C_DECLS
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
END_C_DECLS

USING_NAMESPACE (std);
USING_NAMESPACE (hy);

void usage (int argc, char *argv[])
{
    cerr << argv[0] << " <html-file>" << endl;
}

int main (int argc, char *argv[])
{
    const char *pFilename = 0;
    if (argc != 2)
    {
        usage (argc, argv);
        return 255;
    }

    pFilename = argv[1];
    HTMLDoc h (HTMLDoc::kFILE, pFilename);
    _string outString = L("");
    HTMLNode *pNode = h.getRootNode ();

    ((HTMLNode *) pNode->child())->toText (outString);

    cout << outString.c_str () << endl;

    return 0;
}
