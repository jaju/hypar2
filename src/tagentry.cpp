/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#include "tagentry.hpp"
#include "hash.hpp"
#include "debug.hpp"
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

TagEntry::TagEntry () :
        m_iContextSwitch (1),
        m_iContextLevel (TagEntry::TEXT_ATTR),
        m_bClosure (true),
        m_bOccurOnce (false),
        m_pName (0),
        m_pParent (0),
        m_pAncestor (0),
        m_pFirstParent (0),
        m_pFirstAncestor (0),
        m_pParentNew (0),
        m_pAncestorNew (0),
        m_pParentList (0),
        m_pAncestorList (0)
{
}

TagEntry::TagEntry (
        const _char *pName, unsigned int iContextSwitch,
        unsigned int iContextLevel, bool bClosure,
        bool bOccurOnce, const _char *pParent, const _char *pAncestor
        ) :
    m_iContextSwitch (iContextSwitch),
    m_iContextLevel (iContextLevel),
    m_bClosure (bClosure),
    m_bOccurOnce (bOccurOnce),
    m_pName (pName),
    m_pParent (pParent),
    m_pAncestor (pAncestor),
    m_pFirstParent (0),
    m_pFirstAncestor (0),
    m_pParentNew (0),
    m_pAncestorNew (0),
    m_pParentList (0),
    m_pAncestorList (0)
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

    if (m_pAncestor != 0)
    {
        const _char *constTmpStrPtr = _strrchr(m_pAncestor, ',');
        if (!constTmpStrPtr)
        {
            m_pFirstAncestor = m_pAncestor;
        }
        else
        {
            m_pAncestorNew = _strdup (m_pAncestor);
            assert (m_pAncestorNew);
            _char *tmpStrPtr = m_pAncestorNew;
            while (*tmpStrPtr)
            {
                *tmpStrPtr = _tolower (*tmpStrPtr);
                tmpStrPtr++;
            }
            m_pAncestorList = new vector<const _char *>;
            parseAndStoreElementNamesFromCommaDelimitedString (m_pAncestorNew,
                    m_pAncestorList);
            m_pFirstAncestor = m_pAncestorList->at (0);
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
    if (m_pAncestorNew)
    {
        delete m_pAncestorList;
        m_pAncestorList = 0;
        free (m_pAncestorNew);
        m_pAncestorNew = 0;
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

const _char *TagEntry::name () const
{
    return m_pName;
}

const _char *
TagEntry::getParent () const
{
    return m_pFirstParent;
}

const _char *
TagEntry::getAncestor () const
{
    return m_pFirstAncestor;
}

const std::vector<const _char *> *
TagEntry::getParentList () const
{
    return m_pParentList;
}

const std::vector<const _char *> *
TagEntry::getAncestorList () const
{
    return m_pAncestorList;
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
TagEntry::isAncestor (const _char *pAncestor)
{
    if (m_pAncestorNew)
    {
        return findInList (pAncestor, m_pAncestorList) > -1 ?
            true : false;
    }
    return _strcasecmp (pAncestor, m_pAncestor) == 0 ? true : false;
}

END_NAMESPACE (hy);
