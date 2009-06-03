/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#include "hypar/html.hpp"


#if !DEBUG
#define DEBUG 0
#endif
#include "hypar/debug.hpp"

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
    string text;
    h.toString (text);

    cout << text.c_str () << endl;

    return 0;
}
