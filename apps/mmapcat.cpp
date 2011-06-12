/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#include "mmap.hpp"

#include <iostream>

using namespace std;
using hy::Mmap;

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
    Mmap m (argv[1]);

    cout << "----------------------   SLOW mmapcat - spits out one char at a time. ----------------------------" << endl;
    for (unsigned int i = 0; i < m.getBufferLength (); i++)
    {
        cout << *(m.getBuffer() + i) << flush;
    }
    cout << "--------------------------------------------------------------------------------------------------" << endl;

    return 1;
}
