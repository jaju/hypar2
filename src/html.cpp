/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#include "html.hpp"
#include "mmap.hpp"
#include "mbtowide.hpp"
#include "entitystream.hpp"
#include "hash.hpp"
#include "hashset.hpp"
#include "debug.hpp"

#include <iostream>
#include <fstream>
#include <queue>
#include <string>

BEGIN_C_DECLS;
#include <assert.h>
#include <stdio.h>
#include <locale.h>
END_C_DECLS;

USING_NAMESPACE (std);
#if __GNUC__ > 2
USING_NAMESPACE (__gnu_cxx);
#endif

BEGIN_NAMESPACE (hy);

static const _char *ignoreTagArray [] =
{
    L ("script"),
    0
};

typedef HashSet <const _char *, strcasehash, eqcase> StringSet;

static StringSet ignoreTagSet;
static StringSet::iterator itsx;

/*
 * Initialize all static data structures
 */

HTMLNode HTMLDoc::m_sCloneableNode (DOMNode::ELEMENT, L ("cloneable"));
bool HTMLDoc::s_bIsInitialised = false;
bool HTMLDoc::s_bProcessTextData = false;

/*
 * These are the callbacks for various entities parsed by HTMLDoc
 */

XML::EntityCbRetval processHTMLTag (Tag *pTag, void *pArg)
{
    if (!pTag->m_bEndTag)
    {
        itsx = ignoreTagSet.find (pTag->m_pName);
        if (itsx != ignoreTagSet.end ())
        {
            return XML::IGNORE_CHILDREN;
        }
    }
    return XML::OK;
}

bool processHTMLText (_char *pText, void *pArg)
{
    if (HTMLDoc::s_bProcessTextData)
    {
        _string scratchPad = L("");
        HTMLDoc::postProcessText (pText, scratchPad);
        _strncpy (pText, scratchPad.c_str (), scratchPad.length () + 1);
    }
    return true;
}

bool processHTMLComment (_char *pComment, void *pArg)
{
    if (!pComment)
    {
        debug (("Empty comment!\n"));
    }
    return true;
}


HTMLNode::HTMLNode () : DOMNode ()
{
    initType(DOMNode::ELEMENT, "htmlnode");
}

HTMLNode::HTMLNode (DOMNode::NodeType nodeType, const _char *pStr) : DOMNode()
{
        initType(nodeType, pStr);
}


HTMLNode *HTMLNode::clone ()
{
    return new HTMLNode;
}


HTMLNode *HTMLNode::clone (NodeType nodeType, const _char *pStr)
{
    return new HTMLNode(nodeType, pStr);
}


void HTMLNode::toText (_string &targetString, bool bChildOnly)
{
    switch (m_type)
    {
        case ELEMENT:
            if (!_strcasecmp (m_pName, L("br")))
            {
                targetString += L('\n');
            }
            else if (
                    !(_strcasecmp(m_pName, L("style")))
                    ||
                    !(_strcasecmp(m_pName, L("script")))
                    ||
                    !(_strcasecmp(m_pName, L("noscript")))
                    )
            {
            }
            else
            {
                if (_child)
                {
                    ((HTMLNode *) child())->toText (targetString);
                }
            }
            break;
        case TEXT:
            {
                _string processedText = L("");
                /* XXX - Casting might be invalid in the future! */
                HTMLDoc::postProcessText ((_char *) m_pContent, processedText);
                targetString += processedText;
            }
            break;
        case COMMENT:
        default:
            break;
    }
    if (!bChildOnly && _next)
    {
        ((HTMLNode *) next())->toText (targetString);
    }
}

HTMLDoc::HTMLDoc (SourceType sourceType, const char *pSource,
        const char *pLocale) :
    m_sParser (s_defaultHTMLTable),
    m_sourceType (sourceType),
    m_pRootNode (0),
    m_pLocale (pLocale),
    m_pBuffer (0)
{
    if (!s_bIsInitialised)
    {
        const _char **p = ignoreTagArray;
        while (*p)
        {
            ignoreTagSet.insert (*p);
            p++;
        }
        m_sParser.tagCb = &processHTMLTag;
        m_sParser.textCb = &processHTMLText;
        m_sParser.commentCb = &processHTMLComment;
        s_bIsInitialised = true;
    }
    if (m_sourceType == kFILE)
    {
        parseFromFile (pSource);
    }
    else
    {
        parse (pSource, strlen (pSource));
    }
}

HTMLDoc::~HTMLDoc ()
{
    safe_delete (m_pRootNode);
    safe_delete (m_pBuffer);
}

HTMLNode *HTMLDoc::parseFromFile (const char *pFileName)
{
    if (!pFileName)
    {
        return 0;
    }

    Mmap m (pFileName);
    return parse (m.getBuffer (), m.getBufferLength ());
}

HTMLNode *HTMLDoc::parse (const char *pBuffer, unsigned int length)
{
#ifdef USE_WIDECHAR
    MbToWide toWideCharConverter (m_pLocale);
    m_pBuffer = reinterpret_cast<wchar_t *>
        (toWideCharConverter.convert (pBuffer, length));
#else
    m_pBuffer = new char[length + 1];
    bzero(m_pBuffer, length + 1);
    memcpy (m_pBuffer, pBuffer, length);
#endif
    m_pRootNode = reinterpret_cast<HTMLNode *>
        (m_sParser.parse (m_pBuffer, &m_sCloneableNode));
    return m_pRootNode;
}

typedef struct _NVStringPair
{
    const _char *m_pName, *m_pValue;
}
NVStringPair;


static const NVStringPair HTMLTextSymTabArray [] =
{
    {L ("nbsp"), L (" ")},
    {L ("gt"),   L (">")},
    {L ("lt"),   L ("<")},
    {L ("amp"),  L ("&")},
    {L ("#39"),  L ("'")},
    {L ("copy"),  L ("©")},
    {L ("quot"),  L ("\"")},
    {L ("raquo"), L("»")},
    {0, 0}
};


/* The following static function is NOT thread safe! */

    int
HTMLDoc::postProcessText (_char *pTextBufferEditable, _string &processedText)
{
    static bool bInitialized = false;
    static PropertyMap NVPairMap;
    static int iMaxLen = 0, iMinLen = 1024;

    if (!bInitialized)
    {
        const _char *pName = 0, *pValue = 0;
        int counter = 0;
        int iLen = 0;

        while (HTMLTextSymTabArray[counter].m_pName)
        {
            pName = HTMLTextSymTabArray[counter].m_pName;
            pValue = HTMLTextSymTabArray[counter].m_pValue;
            NVPairMap[pName] = pValue;
            iLen = _strlen (pName);
            if (iMaxLen < iLen)
            {
                iMaxLen = iLen;
            }
            if (iMinLen > iLen)
            {
                iMinLen = iLen;
            }
            counter++;
        }
        assert (iMinLen <= iMaxLen);
        bInitialized = true;
    }

    /* Now, starts the processing */
    _char *pChunkStart = pTextBufferEditable;
    _char *pChunkEnd = pTextBufferEditable;
    const _char ampersand = L('&');
    const _char semicolon = L(';');

    while (*pChunkStart)
    {
        while (*pChunkEnd && *pChunkEnd != ampersand)
        {
            pChunkEnd++;
        }
        if (*pChunkEnd == ampersand)
        {
            /* First, verify if it *is* a special string */
            bool bIsSpecial = false;
            int i;
            for (i = iMinLen + 1; i <= iMaxLen + 1; i++)
            {
                if (pChunkEnd[i] == semicolon)
                {
                    bIsSpecial = true;
                    break;
                }
            }
            if (bIsSpecial)
            {
                PropertyMap::iterator pmx;
                pChunkEnd[i] = NULLCHAR;

                pmx = NVPairMap.find (pChunkEnd + 1);
                if (pmx != NVPairMap.end ())
                {
                    *pChunkEnd = NULLCHAR;
                    processedText += pChunkStart;
                    processedText += pmx->second;
                    pChunkStart = &pChunkEnd[i + 1];
                    *pChunkEnd = ampersand;
                    pChunkEnd[i] = semicolon;
                    pChunkEnd = pChunkStart;
                }
                else
                {
                    debug (("Symbol not found: %s\n", pChunkEnd + 1));
                    pChunkEnd[i] = semicolon;
                    pChunkEnd += i + 1;
                }
            }
            else
            {
                pChunkEnd += i - 1;
            }
        }
        else
        {
            processedText += pChunkStart;
            pChunkStart = pChunkEnd;
        }
    }
    return 0;
}

END_NAMESPACE (hy);
