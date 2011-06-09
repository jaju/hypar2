/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#include "entitystream.hpp"

#include <cassert>
#include <iostream>
#include "debug.hpp"

using namespace std;

BEGIN_NAMESPACE (hy);

EntityStream::EntityStream (_char *pBuffer, long lBufLength) :
    m_pStart(pBuffer),
    m_pEnd(pBuffer + lBufLength), // 1 past the end!!
    m_pCur(pBuffer),
    m_lBufLength(lBufLength),
    m_status(OK),
    m_prevEntity(ELEMENT)
{
    assert (pBuffer != 0);
    assert (lBufLength != 0);
    if (*m_pEnd != L('\0'))
    {
        cerr << "Please use only NULL terminated buffers" << endl;
        assert (0);
    }
}

unsigned long EntityStream::advance (unsigned long lLenToAdvance)
{
    unsigned long retval = 0;
    if (likely (m_pCur + lLenToAdvance < m_pEnd))
    {
        m_pCur += lLenToAdvance;
        return lLenToAdvance;
    }
    else
    {
        retval = m_pEnd - m_pCur;
        m_pCur = m_pEnd;
        m_status = EntityStream::EWCSTREAMEOF;
        return retval;
    }
}

unsigned long EntityStream::rewind (unsigned long lLenToRewind)
{
    assert (m_status != EntityStream::EWCSTREAMINVALID);
    unsigned long retval = 0;
    if (m_pCur - lLenToRewind > m_pStart)
    {
        m_pCur -= lLenToRewind;
        return lLenToRewind;
    }
    else
    {
        retval = m_pCur - m_pStart;
        m_pCur = m_pStart;
        return retval;
    }
}

_char EntityStream::peek ()
{
    assert(m_status != EntityStream::EWCSTREAMINVALID);
    if (likely (m_pCur < m_pEnd))
    {
        return *m_pCur;
    }
    else
    {
        return L('\0');
    }
}

_char EntityStream::get ()
{
    assert(m_status != EntityStream::EWCSTREAMINVALID);
    if (likely (m_pCur < m_pEnd))
    {
        m_pCur++;
        return *(m_pCur - 1);
    }
    else
    {
        m_status = EntityStream::EWCSTREAMEOF;
        return L('\0');
    }
}

const _char * EntityStream::getBuffer ()
{
    if (m_status != EntityStream::EWCSTREAMEOF)
    {
        return m_pCur;
    }
    return 0;
}

// Note: Includes the 'stopChar' when found.
_char *EntityStream::getTill (_char stopChar, long &outLen,
        bool &bFound, bool bAdvance)
{
    if (EntityStream::EWCSTREAMEOF == m_status)
    {
        return 0;
    }

    _char *retLocation = m_pCur;
    long lengthLeft = getRemainingLength ();

    for (
            outLen = 0;
            outLen < lengthLeft && (*(m_pCur + outLen) != stopChar);
            outLen++
        );


    if (outLen == lengthLeft)
    {
        bFound = false;
    }
    else
    {
        bFound = true;
        outLen++;
    }

    if (bAdvance)
    {
        m_pCur += outLen;
        if (outLen == lengthLeft)
        {
            m_status = EntityStream::EWCSTREAMEOF;
        }
    }

    return retLocation;
}

void EntityStream::setPrevEntity (EntityType et)
{
    m_prevEntity = et;
}

    long
EntityStream::getRemainingLength ()
{
    assert(m_pCur <= m_pEnd);
    return (m_pEnd - m_pCur);
}

    EntityStream::EntityType
EntityStream::getNextEntity (_char **pEntityStr, bool bSetNull)
{
    /*
     * Omit everything till the first '<' character
     */
    bool bNextIsElement = false;
    unsigned int uSpaceCount = 0;

    if (m_prevEntity != TEXT)
    {
        while (_isspace (*m_pCur) && advance ())
        {
            uSpaceCount++;
        }
    }

    if (EntityStream::EWCSTREAMEOF == m_status)
    {
        m_prevEntity = END;
        return END;
    }

    if ((m_prevEntity == TEXT) || (LANGLE == *m_pCur))
    {
        bNextIsElement = true;
        if (LANGLE == *m_pCur)
        {
            m_pCur++;
        }
    }

    if (bNextIsElement)
    {
        _char *pFirstGangle = 0;
        bool bFirstGangleFound = false;
        _char quoteChar = 0;
        if (likely ((*m_pCur != BANG) && (*m_pCur != L('?'))))
        {
            /* Tag-element */
            bool bInQuote = false;
            bool bNewLine = false;
            _char *pNewLine = 0;
            *pEntityStr = m_pCur;

            while ((m_pCur != m_pEnd) && (L('>') != *m_pCur))
            {
                if (((*m_pCur == L('"')) || (*m_pCur == L('\'')))
                        && (*(m_pCur - 1) != L('\\')))
                {
                    bInQuote = !bInQuote;
                    quoteChar = *m_pCur;
                    while ((m_pCur != m_pEnd) && (*m_pCur != quoteChar))
                    {
                        if ((L('\\') != *(m_pCur - 1)) && (L('\n') == *m_pCur))
                        {
                            bNewLine = true;
                            pNewLine = m_pCur;
                        }
                        else if (!bFirstGangleFound && (GANGLE == *m_pCur))
                        {
                            bFirstGangleFound = true;
                            pFirstGangle = m_pCur;
                        }
                        advance();
                    }
                    if (*m_pCur == quoteChar)
                    {
                        bInQuote = !bInQuote;
                    }
                }
                m_pCur++;
            }
            if (bInQuote)
            {
                if (bFirstGangleFound)
                {
                    m_pCur = pFirstGangle;
                }
                else
                {
                    while ((m_pCur != m_pEnd) && (GANGLE != *m_pCur))
                    {
                        m_pCur++;
                    }
                    if ((GANGLE != *m_pCur) && bNewLine)
                    {
                        m_pCur = pNewLine;
                    }
                    else
                    {
                        m_status = EWCSTREAMEOF;
                        m_prevEntity = END;
                        return END;
                    }
                }
            }
            if (bSetNull && (m_pCur != m_pEnd))
            {
                *(m_pCur) = L('\0');
            }
            advance ();
            if (*pEntityStr == m_pCur - 1)
            {
                m_status = EWCSTREAMEOF;
                m_prevEntity = END;
                return END;
            }
            m_prevEntity = ELEMENT;
            return ELEMENT;
        }
        else
        {
            /* Comment */
            bool bIsPureComment = false;
            *pEntityStr = m_pCur;

            if (L('-') == *(m_pCur + 1))
            {
                bIsPureComment = true;
            }
goto_getOverComment:
            while (*m_pCur && *m_pCur != GANGLE)
            {
                m_pCur++;
            }
            if (m_pCur == m_pEnd)
            {
                m_status = EWCSTREAMEOF;
                m_prevEntity = END;
                return END;
            }
            if (bIsPureComment && (_strncmp (m_pCur - 1, L("->"), 2) != 0))
            {
                advance();
                goto goto_getOverComment;
            }
            if (bSetNull && (m_pCur != m_pEnd))
            {
                *m_pCur = L('\0');
            }
            advance();
            m_prevEntity = COMMENT;
            return COMMENT;
        }
    }
    else
    {
        /* Text */
        /* We have already skipped all initial whites */
        rewind (uSpaceCount);
        *pEntityStr = m_pCur;
        while ((LANGLE != *m_pCur) && advance()) {}
        if (bSetNull)
        {
            if (m_pCur != m_pEnd)
                *(m_pCur) = L('\0');
        }
        advance ();
        m_prevEntity = TEXT;
        return TEXT;
    }
    /* Not reached */
}

END_NAMESPACE (hy);
