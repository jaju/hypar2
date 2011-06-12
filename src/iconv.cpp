/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#include "iconv.hpp"
#include "debug.hpp"

BEGIN_C_DECLS
#include <assert.h>
#include <string.h>
#include <errno.h>
END_C_DECLS

BEGIN_NAMESPACE (hy);

Iconv::Iconv (const char *pFromEncoding, const char *pToEncoding) :
    m_pFromEncoding (pFromEncoding),
    m_pToEncoding (pToEncoding),
    m_iIconvDescriptor ((iconv_t) -1),
    m_status (EICONVINVAL)
{
    m_iIconvDescriptor = iconv_open (m_pToEncoding, m_pFromEncoding);
#if DEBUG
    int _errno = errno;
#endif
    if (m_iIconvDescriptor != (iconv_t) -1)
    {
        m_status = Iconv::OK;
    }
    else
    {
        debug (("iconv_open failed! Error: %s\n", strerror (_errno)));
        debug (("From: '%s'. To: '%s'\n", m_pFromEncoding, m_pToEncoding));
        m_status = Iconv::EICONVINVAL;
#if DEBUG
        assert (0);
#endif
    }
}

Iconv::~Iconv ()
{
    if (m_iIconvDescriptor >= 0)
    {
        assert (iconv_close (m_iIconvDescriptor) == 0);
    }
}

Iconv::status_t Iconv::getStatus ()
{
    return m_status;
}

size_t Iconv::convert (const char **pInbuf, size_t *pInbytesleft,
        char **pOutbuf, size_t *pOutbytesleft)
{
    size_t retval = 0;
    retval = iconv (m_iIconvDescriptor, (char **) pInbuf, pInbytesleft,
            pOutbuf, pOutbytesleft);
#if DEBUG
    int _errno = errno;
#endif

    if (retval == (size_t) -1)
    {
        debug (("Error: '%s'\n", strerror (_errno)));
    }
    return retval;
}

void Iconv::reset ()
{
    iconv (m_iIconvDescriptor, 0, 0, 0, 0);
}

END_NAMESPACE (hy);
