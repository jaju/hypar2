/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

/*
 * WARNING: DO NOT USE unless you know what you are doing, because this class
 * does NOT understand multibyte char-sequences. It works on XML because all
 * special XML chars are single-byte, ASCII.  Look at xml.cpp where this class
 * is (now) being exclusively used, if you need to see EntityStream use in action.
 *
 * OK - with the extreme warning in place, there's some concession - you should
 * be able to use the 'getNextEntity' function without any issues for ASCII and
 * UTF-8. (wide-char mode use is always fine, I believe - the warning is only for
 * multi-byte char processing)
 */

#pragma once

#include "localdefs.h"

BEGIN_NAMESPACE (hy);

#define LANGLE L('<')
#define GANGLE L('>')
#define BANG L('!')
#define MINUS L('-')
#define SLASH L('/')
#define NULLCHAR L('\0')


/**
 * Class for creating a stream of 'entities' from a buffer. The buffer will be
 * edited by this class! Entities are 'tag', 'text' and 'comment', as in the
 * context of XML
 */
class EntityStream
{
    public:
        typedef enum
        {
            EWCSTREAMINVALID = -100,
            EWCSTREAMEOF,
            OK = 0
        }
        status_t;

        typedef enum
        {
            UNINIT = -2,
            END = -1,
            ELEMENT = 0,
            COMMENT,
            CDATA,
            TEXT
        }
        EntityType;

    public:
        EntityStream (_char *pBuffer, long lBufLength);
        ~EntityStream () {}

    public:
        unsigned long advance (unsigned long lLenToAdvance = 1);
        unsigned long rewind (unsigned long lLenToRewind = 1);

        _char peek ();
        _char get ();
        const _char *getBuffer ();
        _char *getTill (_char stopChar, long &lOutLen, bool &bFound,
                bool bAdvance = false);

        void setPrevEntity (EntityType et);
        EntityType getNextEntity (_char **pEntityStr, bool bSetNull = false);

    private:
        _char *m_pStart, *m_pEnd, *m_pCur;
        long m_lBufLength;
        status_t m_status;
        EntityType m_prevEntity;

    private:
        EntityStream () {}
        long getRemainingLength ();

};

END_NAMESPACE (hy);
