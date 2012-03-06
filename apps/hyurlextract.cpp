/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#include "mmap.hpp"
#include "mbtowide.hpp"
#include "entitystream.hpp"
#include "html.hpp"
#include "url.hpp"
#include "hash.hpp"

#if !DEBUG
#define DEBUG 0
#endif
#include "debug.hpp"

#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <list>

BEGIN_C_DECLS
#include <assert.h>
#include <stdio.h>
#include <locale.h>
END_C_DECLS

using namespace std;
#if __GNUC__ > 2
using namespace __gnu_cxx;
#endif
using hy::Mmap;
using hy::URL;
unsigned int iMaxURLLen = 0;

void usage(int argc, char *argv[])
{
    cerr << "Usage: " << argv[0] << " <file>" << endl;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        usage(argc, argv);
        return -1;
    }
    setlocale (LC_ALL, "ISO-8859-1");
    Mmap m (argv[1]);
    _char *pBuffer = 0;
    pBuffer = new char[m.getBufferLength () + 1];
    bzero (pBuffer, m.getBufferLength () + 1);
    memcpy (pBuffer, m.getBuffer (), m.getBufferLength ());
    hy::URLCollector url_collector (pBuffer);
    list<URL *>::iterator urlx = url_collector.m_URLList.begin ();
    while (urlx != url_collector.m_URLList.end ())
    {
        if ((*urlx)->isNoFollowSet ())
            cout << "[No-Follow] ";
        cout << (*urlx)->getCanonical () << endl;
        urlx++;
    }
    delete pBuffer;
    return 0;
}
