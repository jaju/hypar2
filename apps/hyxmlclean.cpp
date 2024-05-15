/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#include "localdefs.h"
#include "mmap.hpp"
#include "entitystream.hpp"
#include "html.hpp"
#include "hash.hpp"
#include "hashset.hpp"

#if !DEBUG
#define DEBUG 0
#endif
#include "debug.hpp"

#include <iostream>
#include <fstream>
#include <queue>
#include <string>

BEGIN_C_DECLS;
#include <assert.h>
#include <stdio.h>
#include <locale.h>
END_C_DECLS;

using namespace std;
using namespace hy;

using hy::Mmap;

typedef HashSet<const _char *, strcasehash, eqcase> StringSet;


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
    setlocale (LC_ALL, "en_IN.UTF-8");
    Mmap m (argv[1]);
    _char *pBuffer = 0;
    int len = m.getBufferLength () + 1;
    pBuffer = new char[len];
    bzero (pBuffer, len);
    memcpy (pBuffer, m.getBuffer (), len - 1);
    XML h;
    DOMNode *node = DOMNode::create();
    DOMNode *pNode = h.parse (pBuffer, node);

    _string text = "";
    toString(pNode->child(), text);

    char *pLocalLocaleBuffer = new char[text.size () * 4 + 4];
    strncpy (pLocalLocaleBuffer, text.c_str (), text.size () * 4 + 4);
    cout << pLocalLocaleBuffer << endl;

    delete pNode;
    delete[] pLocalLocaleBuffer;
    delete pBuffer;
    delete node;

    return 0;
}
