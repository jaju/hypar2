/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#ifndef _TXT_TOKENIZER_H_
#define _TXT_TOKENIZER_H_

#include "hypar/localdefs.h"

#include <string>
using std::string;
using std::wstring;

BEGIN_NAMESPACE (hy);

struct TokenStartIdentifier
{
    TokenStartIdentifier () {}
    ~TokenStartIdentifier () {}
    bool operator () (const char c)
    {
        if (!c)
        {
            return false;
        }
        return (isspace (c) ? false : true);
    }
#if USE_WIDECHAR
    bool operator () (const wchar_t wc)
    {
        if (!wc)
        {
            return false;
        }
        return (iswspace (wc) ? false : true);
    }
#endif
};

struct TokenEndIdentifier
{
    TokenEndIdentifier () {}
    ~TokenEndIdentifier () {}
    bool operator () (const char c)
    {
        return (isspace (c) ? true : false);
    }
#if USE_WIDECHAR
    bool operator () (const wchar_t wc)
    {
        return (iswspace (wc) ? true : false);
    }
#endif
};

template <typename _Unit, typename _Container,
         class startIdentifier = TokenStartIdentifier,
         class endIdentifier = TokenEndIdentifier>
class Tokenizer
{
    private:
        _Container m_currentToken;
        const _Unit *m_pSource;
        const _Unit *m_pCurPos;
        startIdentifier m_TrueToStartFunc;
        endIdentifier m_FalseToEndFunc;

    public:
        Tokenizer () {}
        Tokenizer (const _Unit *pSource)
        {
            setSource (pSource);
        }
        ~Tokenizer ()
        {
        }

    public:
        void setSource (const _Unit *pSource)
        {
            m_pSource = pSource;
            m_pCurPos = pSource;
        }
        const _Unit *getNext ()
        {
            const char *pBegin = 0;
            m_currentToken.erase ();

            while (!m_TrueToStartFunc (*m_pCurPos))
            {
                if (!*m_pCurPos)
                {
                    return 0;
                }
                m_pCurPos++;
            }
            pBegin = m_pCurPos;
            while (*m_pCurPos && !m_FalseToEndFunc (*m_pCurPos))
            {
                m_pCurPos++;
            }
            m_currentToken.assign (pBegin, m_pCurPos - pBegin);
            return m_currentToken.c_str ();
        }
        void reset ()
        {
            m_pCurPos = m_pSource;
            m_currentToken.erase ();
        }
};

#define C_TEMPLATE_SPEC <char, string>
typedef Tokenizer C_TEMPLATE_SPEC CTokenizer;

#if USE_WIDECHAR
#define WC_TEMPLATE_SPEC <wchar_t, wstring>
typedef Tokenizer WC_TEMPLATE_SPEC WCTokenizer;
#endif

#if USE_WIDECHAR
#define _CTokenizer WCTokenizer
#else
#define _CTokenizer CTokenizer
#endif

END_NAMESPACE (hy);

#endif // _TXT_TOKENIZER_H_
