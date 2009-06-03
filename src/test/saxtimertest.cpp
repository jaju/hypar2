/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#include "hypar/localdefs.h"
#include "hypar/html.hpp"
#include "hypar/url.hpp"

#if !DEBUG
#define DEBUG 0
#endif
#include "hypar/debug.hpp"
#include "hypar/textutils.hpp"
#include "hypar/mmap.hpp"

#include <iostream>
#include <string>
#include <cassert>
#include <vector>
#include <cstdlib>

#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/time.h>

USING_NAMESPACE (std);
USING_NAMESPACE (hy);

_string outString;

XML::EntityCbRetval tagCbFunc (Tag *pTag, void *pArg)
{
    if (
            !(_strcasecmp(pTag->m_pName, L("style")))
            ||
            !(_strcasecmp(pTag->m_pName, L("script")))
            ||
            !(_strcasecmp(pTag->m_pName, L("noscript")))
       )
    {
        return XML::IGNORE_CHILDREN;
    }
    const _char *pHref = 0;
    if ((_strncmp (pTag->m_pName, L("a"), 1)) == 0 &&
            ((pHref = pTag->findProperty (L("href"))) != 0))
    {
        /* URL */
        /* cerr << pHref << endl; */
        /* URL u (pHref); */
    }
    return XML::DROP_SIMPLE;
}

bool textCb (_char *pText, void *pArg)
{
    outString += pText;
    outString += L(" ");
    return false;
}

bool commentCb (_char *pText, void *pArg)
{
    return false;
}

#define DFL_NUMPASSES 100

void usage ()
{
    cerr << "<exe> <html-file> [num-passes: default " << DFL_NUMPASSES << "]" << endl;
}

int main (int argc, char *argv[])
{
    const char *pFilename = 0;
    int iCounter = 0;
    int iNumPasses = DFL_NUMPASSES;
    struct timeval tv_start, tv_end;
    long lSize = 0;

    if (argc < 2 || argc > 3)
    {
        usage ();
        return 255;
    }
    if (argc == 3)
    {
        iNumPasses = atoi (argv[2]);
    }
    pFilename = argv[1];
    Mmap m (pFilename);
    lSize = m.getBufferLength ();
    lSize *= iNumPasses;

    XML h (HTMLDoc::s_defaultHTMLTable);
    h.tagCb = &tagCbFunc;
    h.textCb = &textCb;
    h.commentCb = &commentCb;
    DOMNode node (DOMNode::ELEMENT, L("root"));
    DOMNode *pNode = 0;

    gettimeofday (&tv_start, 0);
    for (iCounter = 0; iCounter != iNumPasses; iCounter++)
    {
        outString.clear ();
        _char *pBuffer = new _char[m.getBufferLength () + 1];
        bzero (pBuffer, m.getBufferLength () + 1);
        memcpy (pBuffer, m.getBuffer (), m.getBufferLength ());
        pNode = h.parse (pBuffer, &node);
        delete pBuffer;
    }
    gettimeofday (&tv_end, 0);

    long diff = ((tv_end.tv_sec * 1000000) + (tv_end.tv_usec));
    diff -= ((tv_start.tv_sec * 1000000) + (tv_start.tv_usec));

    cout << "Time for " << iNumPasses << " passes: " << diff << " microseconds" << endl;
    cout << ((1.0 * iNumPasses * 1000000) / diff) << " passes per second" << endl;
    cout << (lSize / diff) << " Mbytes per second" << endl;

    return 0;
}
