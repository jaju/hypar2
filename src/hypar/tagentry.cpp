/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#include "hypar/tagentry.hpp"
#include "hypar/hash.hpp"
#include "hypar/debug.hpp"
#include <malloc.h>
#include <assert.h>

BEGIN_NAMESPACE (hy);

int
parseAndStoreElementNamesFromCommaDelimitedString (
        _char *pString, std::vector <const _char *> *pStorage
        )
{
    bool bFinished = false;
    _char *pStart;
    int iNumElements = 0;
    while (!bFinished)
    {
        while (*pString && (_isspace (*pString) || (L(',') == *pString)))
        {
            pString++;
        }
        pStart = pString;
        while (*pString && (L(',') != *pString) && !_isspace (*pString))
        {
            pString++;
        }
        if (L(',') == *pString)
        {
            *pString = L('\0');
            pString++;
        }
        else
        {
            *pString = L('\0');
            while (*pString && (L(',') != *pString))
            {
                pString++;
            }
        }
        pStorage->push_back (pStart);
        iNumElements++;
        if (!*pString)
        {
            bFinished = true;
        }
    }
    return iNumElements;
}

TagEntry::TagEntry ()
    :
m_pName (0),
        m_iContextSwitch (1),
        m_iContextLevel (TagEntry::TEXT_ATTR),
        m_bClosure (true),
        m_bOccurOnce (false),
        m_pParent (0),
        m_pRparent (0),
        m_pFirstParent (0),
        m_pFirstRparent (0),
        m_pParentNew (0),
        m_pRparentNew (0),
        m_pParentList (0),
        m_pRparentList (0)
{
}

TagEntry::TagEntry (
        const _char *pName, unsigned int iContextSwitch,
        unsigned int iContextLevel, bool bClosure,
        bool bOccurOnce, const _char *pParent, const _char *pRparent
        ) :
    m_pName (pName),
    m_iContextSwitch (iContextSwitch),
    m_iContextLevel (iContextLevel),
    m_bClosure (bClosure),
    m_bOccurOnce (bOccurOnce),
    m_pParent (pParent),
    m_pRparent (pRparent),
    m_pFirstParent (0),
    m_pFirstRparent (0),
    m_pParentNew (0),
    m_pRparentNew (0),
    m_pParentList (0),
    m_pRparentList (0)
{
    if (m_pParent != 0)
    {
        const _char *constTmpStrPtr = _strrchr (m_pParent, ',');
        if (!constTmpStrPtr)
        {
            m_pFirstParent = m_pParent;
        }
        else
        {
            m_pParentNew = _strdup (m_pParent);
            assert (m_pParentNew);
            _char *tmpStrPtr = m_pParentNew;
            while (*tmpStrPtr)
            {
                *tmpStrPtr = _tolower(*tmpStrPtr);
                tmpStrPtr++;
            }
            m_pParentList = new vector <const _char *>;
            parseAndStoreElementNamesFromCommaDelimitedString (m_pParentNew,
                    m_pParentList);
            m_pFirstParent = m_pParentList->at (0);
        }
    }

    if (m_pRparent != 0)
    {
        const _char *constTmpStrPtr = _strrchr(m_pRparent, ',');
        if (!constTmpStrPtr)
        {
            m_pFirstRparent = m_pRparent;
        }
        else
        {
            m_pRparentNew = _strdup (m_pRparent);
            assert (m_pRparentNew);
            _char *tmpStrPtr = m_pRparentNew;
            while (*tmpStrPtr)
            {
                *tmpStrPtr = _tolower (*tmpStrPtr);
                tmpStrPtr++;
            }
            m_pRparentList = new vector<const _char *>;
            parseAndStoreElementNamesFromCommaDelimitedString (m_pRparentNew,
                    m_pRparentList);
            m_pFirstRparent = m_pRparentList->at (0);
        }
    }
}

TagEntry::~TagEntry ()
{
    if (m_pParentNew)
    {
        delete m_pParentList;
        m_pParentList = 0;
        free (m_pParentNew);
        m_pParentNew = 0;
    }
    if (m_pRparentNew)
    {
        delete m_pRparentList;
        m_pRparentList = 0;
        free (m_pRparentNew);
        m_pRparentNew = 0;
    }
}

inline bool
TagEntry::findSubstr (const _char *needle, const _char *haystack) const
{
    if (_strstr (needle, haystack))
    {
        return true;
    }
    return false;
}

inline int
TagEntry::findInList (const _char *needle,
        std::vector <const _char *> *haystack) const
{
    int sz = haystack->size ();
    int i = 0;
    while (i < sz)
    {
        if (_strcasecmp (haystack->at (i), needle) == 0)
        {
            return i;
        }
        i++;
    }
    return -1;
}

    const _char *
TagEntry::getName ()
{
    return m_pName;
}

const _char *
TagEntry::getParent () const
{
    return m_pFirstParent;
}

const _char *
TagEntry::getRparent () const
{
    return m_pFirstRparent;
}

const std::vector<const _char *> *
TagEntry::getParentList () const
{
    return m_pParentList;
}

const std::vector<const _char *> *
TagEntry::getRparentList () const
{
    return m_pRparentList;
}

    bool
TagEntry::isParent (const _char *pParent)
{
    if (m_pParentNew)
    {
        return findInList (pParent, m_pParentList) > -1 ?
            true : false;
    }
    return _strcasecmp (pParent, m_pParent) == 0 ? true : false;
}

    bool
TagEntry::isRparent (const _char *pRparent)
{
    if (m_pRparentNew)
    {
        return findInList (pRparent, m_pRparentList) > -1 ?
            true : false;
    }
    return _strcasecmp (pRparent, m_pRparent) == 0 ? true : false;
}

END_NAMESPACE (hy);
