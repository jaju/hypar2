/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#include "hypar/localdefs.h"
#include "hypar/mmap.hpp"
#include "hypar/mbtowide.hpp"
#include "hypar/charentitystream.hpp"
#include "hypar/html.hpp"
#include "hypar/hash.hpp"
#include "hypar/hashset.hpp"

#if !DEBUG
#define DEBUG 0
#endif
#include "hypar/debug.hpp"

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
#if __GNUC__ > 2
using namespace __gnu_cxx;
#endif
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
#if USE_WIDECHAR
    hy::MbToWide toWideCharConverter ("UTF-8");
    pBuffer = reinterpret_cast<_char *> (toWideCharConverter.convert (
                m.getBuffer (), m.getBufferLength ()
                ));
#else
    int len = m.getBufferLength () + 1;
    pBuffer = new char[len];
    bzero (pBuffer, len);
    memcpy (pBuffer, m.getBuffer (), len - 1);
#endif
    XML h;
    DOMNode node (DOMNode::ELEMENT, L("root"));
    node.level() = -1;
    DOMNode *pNode = h.parse (pBuffer, &node);

    _string text = L("");
    pNode->child()->toString (text);

    char *pLocalLocaleBuffer = new char[text.size () * 4 + 4];
    strncpy (pLocalLocaleBuffer, text.c_str (), text.size () * 4 + 4);
    cout << pLocalLocaleBuffer << endl;

    delete pNode;
    delete pLocalLocaleBuffer;
    delete pBuffer;

    return 0;
}
