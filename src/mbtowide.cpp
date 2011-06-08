/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#include <iostream>
#include "mbtowide.hpp"
#include "debug.hpp"

BEGIN_C_DECLS
#include <assert.h>
#include <strings.h>
END_C_DECLS

BEGIN_NAMESPACE (hy);

const char *MbToWide::m_pToEncoding = "WCHAR_T";

MbToWide::MbToWide (const char *pFromEncoding) :
    m_pFromEncoding (pFromEncoding),
    m_pOutbuf (0),
    m_iconv (m_pFromEncoding, m_pToEncoding)
{
}

MbToWide::~MbToWide ()
{
}

Iconv::status_t MbToWide::getStatus ()
{
    return m_iconv.getStatus ();
}

char *MbToWide::convert (const char *pInbuf, size_t inbytesLeft)
{
    size_t outbufLen = sizeof (_char) * (inbytesLeft + 1); // What a waste!
    char *pTempOutBuf;
    m_pOutbuf = new char[outbufLen];
    assert (m_pOutbuf);
    bzero (m_pOutbuf, outbufLen);
    pTempOutBuf = m_pOutbuf;
    convert (&pInbuf, &inbytesLeft, &pTempOutBuf, &outbufLen);
    return m_pOutbuf; // To be deleted by caller!
}

size_t MbToWide::convert (const char **pInbuf, size_t *pInbytesleft,
        char **pOutbuf, size_t *pOutbytesleft)
{
#ifdef USE_WIDECHAR
# ifndef WIN32
    return m_iconv.convert (pInbuf, pInbytesleft, pOutbuf, pOutbytesleft);
# else
    mbstate_t conv_state;
    memset (&conv_state, 0, sizeof (mbstate_t));
    setlocale (LC_ALL, m_pFromEncoding); // FIXME - Introduce locking ...
    return mbsrtowcs ((_char *) *m_pOutbuf, *pInbuf, *pOutbytesleft,
            &conv_state);
# endif
#else
    strncpy (*pOutbuf, *pInbuf, *pInbytesleft);
    *pOutbytesleft -= *pInbytesleft;
    return 0;
#endif
}

END_NAMESPACE (hy);
