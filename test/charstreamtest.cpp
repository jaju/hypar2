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

#if !DEBUG
#define DEBUG 0
#endif
#include "debug.hpp"

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

void usage(int argc, char *argv[])
{
    cerr << "Usage: " << argv[0] << " <in-encoding> <file>" << endl;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        usage(argc, argv);
        return -1;
    }
#if USE_WIDECHAR
    //setlocale(LC_ALL, argv[1]);
    setlocale(LC_ALL, "en_IN.UTF-8");
    //setlocale(LC_ALL, "POSIX");
    MbToWide toWideCharConverter (argv[1]);
    Mmap m (argv[2]);
    _char *pBuffer = reinterpret_cast<_char *> (toWideCharConverter.convert (
                m.getBuffer (), m.getBufferLength ()
                ));
    CharEntityStream wideCharStream (pBuffer, _strlen (pBuffer));

#if 0
    _char c;
    while ((c = wideCharStream.get()) != L('\0'))
    {
        _cout << c << flush;
    }
#endif

    _char *wchar_ptr;
    _string ws, textws;
    _string wsold;
    queue<_char *> entity_queue;
    bool bFound;
    long len;
    wofstream wofs ("tmp.out", ios::out);

    textws = L"";
    ws = L"";
    char tempLocalLocaleBasedBuffer[2048];
    while ((wchar_ptr = wideCharStream.getTill (
                    L('<'), len, bFound, true
                    ))
            != 0
          )
    {
        textws.append (wchar_ptr, len - 1);
        if (bFound)
        {
            wsold = ws;
            wideCharStream.rewind (1);
            wchar_ptr = wideCharStream.getTill (
                    L('>'), len, bFound, true
                    );
            if (wchar_ptr)
            {
                ws.erase ();
                ws.assign (wchar_ptr, len);
#if 0
                int i = 0;
                while (i < len)
                {
                    _cout << (wint_t) wchar_ptr[i] << endl;
                    _cout << wchar_ptr[i] << endl;
                    i++;
                }
#endif
                wofs << ws << endl;
                bzero (tempLocalLocaleBasedBuffer, 2048);
                wcstombs (tempLocalLocaleBasedBuffer, ws.c_str (), 2048);
                _cout << tempLocalLocaleBasedBuffer << endl;
                cerr << tempLocalLocaleBasedBuffer << endl;
            }
#if 0
            if (!wofs.good())
            {
                int i = 0;
                int j = 0;
                const _char *abc = ws.c_str();
                for (; i < _strlen(abc); i++)
                {
                    if (j > 10)
                    {
                        j = 0;
                        //cout << endl;
                    }
                    if (static_cast<wint_t>(abc[i]) < 128)
                    {
                        cout << static_cast<char>(abc[i]);
                    }
                    else
                    {
                        //cout << "|" << static_cast<wint_t>(abc[i]) << "|";
                        fprintf(stdout, "%d", (wint_t)abc[i]);
                    }
                    j++;
                }
                cout << endl;
                block();
            }
#endif
        }
    }
    cout << "The remaining length is " << wideCharStream.getRemainingLength ()
        << endl;
#endif
    return 0;
}
