/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#include "localdefs.h"
#include "mmap.hpp"
#include "html.hpp"

#include <iostream>
#include <fstream>
#include <queue>
#include <string>

using namespace std;
using namespace hy;

void usage (int argc, char *argv[])
{
    cerr << "Usage: " << argv[0] << " <file>" << endl;
}


int main (int argc, char *argv[])
{
    if (argc != 2)
    {
        usage(argc, argv);
        return -1;
    }
    Mmap m (argv[1]);
    char *pBuffer = m.getBufferCopy();
    HTMLDoc h(HTMLDoc::kMEMBUF, pBuffer);

    cout << toString(h.getRootNode()->child()) << endl;

    delete pBuffer;
    return 0;
}
