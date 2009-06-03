/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#include "hypar/mmap.hpp"
#include "hypar/mbtowide.hpp"
#include "hypar/charentitystream.hpp"
#include "hypar/html.hpp"

#if !DEBUG
#define DEBUG 0
#endif
#include "hypar/debug.hpp"

#include <iostream>
#include <fstream>
#include <queue>

BEGIN_C_DECLS
#include <assert.h>
#include <stdio.h>
#include <locale.h>
END_C_DECLS

USING_NAMESPACE (std);
USING_NAMESPACE (hy);

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
    setlocale (LC_ALL, "en_IN.UTF-8");
    MbToWide toWideCharConverter ("UTF-8");
    Mmap m (argv[1]);
    _char *pBuffer = reinterpret_cast<_char *> (toWideCharConverter.convert (
                m.getBuffer (), m.getBufferLength ()
                ));
    CharEntityStream wideCharStream (pBuffer, _strlen (pBuffer));
    hy::XML h (HTMLDoc::s_defaultHTMLTable);

    _char *wchar_ptr;
    _string ws, textws;
    _string wsold;
    queue<_char *> entity_queue;

    textws = L("");
    ws = L("");
    char tempLocalLocaleBasedBuffer[2048];
    CharEntityStream::EntityType type;
    while ((type = wideCharStream.getNextEntity (&wchar_ptr, true))
            != CharEntityStream::END)
    {
        if (type == CharEntityStream::TEXT)
        {
            textws.append (wchar_ptr);
        }
        switch(type)
        {
            case CharEntityStream::ELEMENT:
                {
                    hy::Tag t (wchar_ptr, false);
                    _string s;
                    t.toString (s);
                    _cout << s << endl;
                }
                break;
            case CharEntityStream::COMMENT:
                _cout << L("<!--") << wchar_ptr << L("-->") << endl;
                break;
            case CharEntityStream::TEXT:
                _cout << wchar_ptr << endl;
                break;
            default:
                break;
        }
        ws.erase ();
        ws.assign (wchar_ptr);
        bzero (tempLocalLocaleBasedBuffer, 2048);
        wcstombs (tempLocalLocaleBasedBuffer, ws.c_str (), 2048);
    }
    delete pBuffer;
#endif
    return 0;
}
