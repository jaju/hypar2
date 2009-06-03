/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#include "hypar/mmap.hpp"
#include "hypar/tokenizer.hpp"

#include <iostream>

using namespace std;
using hy::Mmap;
using hy::_CTokenizer;

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
#if USE_WIDECHAR
    cerr << "Not fixed for wide char mode" << endl;
    return -1;
#else
    const _char *token = 0;
    int count = 0;
    Mmap m (argv[1]);
    _CTokenizer tokenizer (m.getBuffer ());

    while ((token = tokenizer.getNext ()) != 0)
    {
        cout << token << endl;
        count ++;
    }
    cout << "The number of tokens found = " << count << endl;

    return 1;
#endif
}
