/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#ifndef _HY_ICONV_H_
#define _HY_ICONV_H_

#include "hypar/localdefs.h"
#include <iconv.h>

BEGIN_NAMESPACE (hy);

/**
 * Wrapper class for iconv (3)
 */
class Iconv
{
    public:
        typedef enum
        {
            EICONVINVAL = -1,
            OK
        }
        status_t;

    private:
        char const *m_pFromEncoding, *m_pToEncoding;
        iconv_t m_iIconvDescriptor;
        Iconv::status_t m_status;

    private:
        Iconv ();

    public:
        /**
         * Constructor for Iconv with from and to encoding-parameters
         */
        Iconv (const char *pFromEncoding, const char *pToEncoding);
        ~Iconv ();

    public:
        Iconv::status_t getStatus ();

        /**
         * Convert from one encoding to another, with the same meaning for the
         * parameters as in the iconv (3) call, but with the iconv_t parameter
         * not needed.
         */
        size_t convert (const char **pInbuf, size_t *pInbytesleft,
                char **pOutbuf, size_t *pOutbytesleft);

        void reset ();
};

END_NAMESPACE (hy);
#endif // _HY_ICONV_H_
