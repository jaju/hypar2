/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#pragma once
#include "hypar/localdefs.h"
#include "hypar/iconv.hpp"

BEGIN_NAMESPACE (hy);

/**
 * Class to convert from any supported encoding to the wide-char form
 */
class MbToWide
{
    private:
        static const char *m_pToEncoding;
        const char *m_pFromEncoding;
        char *m_pOutbuf;
        Iconv::status_t m_status;
        Iconv m_iconv;

    private:
        MbToWide ();

    public:
        MbToWide (const char *pFromEncoding = "en_US.UTF-8");
        ~MbToWide ();

    public:
        Iconv::status_t getStatus ();

        /**
         * Call this when you want MbToWide to allocate space for
         * you and copy the converted text to the allocated buffer. The caller
         * has to delete the buffer
         */
        char *convert (const char *pInbuf, size_t pInbytesleft);

        /**
         * Call this function when space where the output is to be stored is
         * already allocated by the user
         */
        size_t convert (const char **pInbuf, size_t *pInbytesleft,
                char **pOutbuf, size_t *pOutbytesleft);

};

END_NAMESPACE (hy);
