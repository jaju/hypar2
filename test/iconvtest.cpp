/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#include "debug.hpp"
#include "iconv.hpp"
#include "mbtowide.hpp"
#include "mmap.hpp"

#include <iostream>

BEGIN_C_DECLS
#include <wchar.h>
END_C_DECLS

USING_NAMESPACE (std);

using hy::Mmap;

void usage(int argc, char *argv[])
{
    cerr << "Usage: " << argv[0] << " <in-encoding> <filename>" << endl;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        usage(argc, argv);
        return -1;
    }

#if USE_WIDECHAR
    _char *pBuffer = 0;
    hy::MbToWide toWideCharConverter(argv[1]);

    if (toWideCharConverter.getStatus() != hy::Iconv::OK)
    {
        cerr << "Error in creating a converter to wide_char from " <<
            argv[1] << endl;
        return -1;
    }

    Mmap m(argv[2]);

    if (m.getStatus () != Mmap::OK)
    {
        cerr << "Error in mmap'ing file " << argv[1] << endl;
        return -1;
    }

    pBuffer = reinterpret_cast<_char *> (
            toWideCharConverter.convert(
                m.getBuffer(), m.getBufferLength()
                )
            );

    if (!pBuffer)
    {
        cerr << "Could not convert to wide_char" << endl;
        return -1;
    }

    _cout << pBuffer << endl;

    int i = 0;
    int bufLen = _strlen(pBuffer);

    while (i < bufLen)
    {
        if (*(pBuffer + i) < 256)
            cout << (char) *(pBuffer + i);
        i++;
    }
    cout << endl;
    cout << "The length of the wide_char buffer is " << _strlen(pBuffer) << endl;
#endif
    return 0;
}
