/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#include "hypar/charentitystream.hpp"

#include <assert.h>
#include <iostream>
#include "hypar/debug.hpp"

using namespace std;

BEGIN_NAMESPACE (hy);

CharEntityStream::CharEntityStream () :
    m_pStart (0),
    m_pEnd (0),
    m_pCur (0),
    m_lBufLength (0),
    m_status (CharEntityStream::EWCSTREAMINVALID),
    m_prevEntity (CharEntityStream::ELEMENT)
{
}

CharEntityStream::CharEntityStream (_char *pBuffer, long lBufLength)
{
    setSourceBuffer (pBuffer, lBufLength);
}

void CharEntityStream::setSourceBuffer (_char *pBuffer, long lBufLength)
{
    assert (pBuffer != 0);
    assert (lBufLength != 0);
    m_pStart = m_pCur = pBuffer;
    m_lBufLength = lBufLength;
    m_pEnd = m_pStart + m_lBufLength; // 1 past the end!!
    if (*m_pEnd != L('\0'))
    {
        cerr << "Please use only NULL terminated buffers" << endl;
        assert (0);
    }
    m_status = OK;
    m_prevEntity = ELEMENT;
}

void CharEntityStream::reset ()
{
    assert (m_status != EWCSTREAMINVALID);
    m_pCur = m_pStart;
    m_status = CharEntityStream::OK;
}

unsigned long CharEntityStream::advance (unsigned long lLenToAdvance)
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
        m_status = CharEntityStream::EWCSTREAMEOF;
        return retval;
    }
}

unsigned long CharEntityStream::rewind (unsigned long lLenToRewind)
{
    assert (m_status != CharEntityStream::EWCSTREAMINVALID);
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

_char CharEntityStream::peek ()
{
    assert(m_status != CharEntityStream::EWCSTREAMINVALID);
    if (likely (m_pCur < m_pEnd))
    {
        return *m_pCur;
    }
    else
    {
        return L('\0');
    }
}

_char CharEntityStream::get ()
{
    assert(m_status != CharEntityStream::EWCSTREAMINVALID);
    if (likely (m_pCur < m_pEnd))
    {
        m_pCur++;
        return *(m_pCur - 1);
    }
    else
    {
        m_status = CharEntityStream::EWCSTREAMEOF;
        return L('\0');
    }
}

const _char * CharEntityStream::getBuffer ()
{
    if (m_status != CharEntityStream::EWCSTREAMEOF)
    {
        return m_pCur;
    }
    return 0;
}

_char * CharEntityStream::getTill (const _char stopChar, long &outLen,
        bool &bFound, bool bAdvance)
{
    if (CharEntityStream::EWCSTREAMEOF == m_status)
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
            m_status = CharEntityStream::EWCSTREAMEOF;
        }
    }

    return retLocation;
}

_char *CharEntityStream::getTillFirstOf (const _char stopChar_1,
        const _char stopChar_2, long &lOutLen, _char &wcharFound, bool bAdvance)
{
    _char *pCur1, *pCur2;
    long lLenTillStopChar1, lLenTillStopChar2;
    bool bFound1, bFound2;
    pCur1 = getTill (stopChar_1, lLenTillStopChar1, bFound1, false);
    pCur2 = getTill (stopChar_2, lLenTillStopChar2, bFound2, false);

    if (bFound1 || bFound2)
    {
        lOutLen = (lLenTillStopChar1 < lLenTillStopChar2) ?
            lLenTillStopChar1 : lLenTillStopChar2;
        wcharFound = *(pCur1 + lOutLen);
    }
    else
    {
        lOutLen = lLenTillStopChar1;
        wcharFound = L('\0');
    }

    if (bAdvance)
    {
        advance(lOutLen);
    }

    return pCur1;
}

bool CharEntityStream::set (const _char c)
{
    assert(m_status != CharEntityStream::EWCSTREAMINVALID);
    if (m_pCur < m_pEnd)
    {
        *m_pCur = c;
        return true;
    }
    return false;
}

bool CharEntityStream::setNull ()
{
    return set (L('\0'));
}

void CharEntityStream::setPrevEntity (EntityType et)
{
    m_prevEntity = et;
}

    bool
CharEntityStream::putBack (_char c)
{
    bool retval = set (c);
    if (retval)
    {
        return rewind (1) == 1 ? true : false;
    }
    return false;
}

    long
CharEntityStream::getRemainingLength ()
{
    assert(m_pCur <= m_pEnd);
    return (m_pEnd - m_pCur);
}

    long
CharEntityStream::getBufferLength ()
{
    return m_lBufLength;
}

    CharEntityStream::EntityType
CharEntityStream::getNextEntity (_char **pEntityStr, bool bSetNull)
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

    if (CharEntityStream::EWCSTREAMEOF == m_status)
    {
        m_prevEntity = END;
        return END;
    }

    if ((m_prevEntity == TEXT) || (LANGLE == *m_pCur))
    {
        bNextIsElement = true;
        if (L('<') == *m_pCur)
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
