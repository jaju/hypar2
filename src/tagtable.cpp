/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#include "tagtable.hpp"
#include <limits.h>
#include <assert.h>

#include <iostream>
using namespace std;

BEGIN_NAMESPACE (hy);

typedef pair<const _char*, const TagEntry*> NameTagPair;

TagTable::TagTable () :
    m_status (EINIT),
    m_pRootElementName (0),
    m_TERepository ()
{
}

TagTable::TagTable (const TagEntry *pte) :
    m_status (EINIT),
    m_pRootElementName (0)
{
    if (construct (pte) > 0)
    {
        m_status = OK;
    }
}

int TagTable::construct (const TagEntry *pte)
{
    int iEntryCount = 0;
    unsigned int iRootLevel = INT_MAX;

    assert(pte != 0);
    const TagEntry *pEntry;
    while (pte[iEntryCount].name() != 0)
    {
        pEntry = &pte[iEntryCount];
        NameTagPair p (pEntry->name(), pEntry);
        m_TERepository.insert (p);
        if (pEntry->contextLevel() < iRootLevel)
        {
            iRootLevel = pEntry->contextLevel();
            m_pRootElementName = pEntry->name();
        }
        iEntryCount++;
    }
    assert (m_pRootElementName);
    return iEntryCount;
}

const TagEntry *TagTable::find (const _char *pName)
{
    if (likely (pName))
    {
        TERepository::iterator rx = m_TERepository.find (pName);
        if (rx != m_TERepository.end ())
        {
            return rx->second;
        }
    }
    return 0;
}

const _char *TagTable::rootTagName ()
{
    return m_pRootElementName;
}

END_NAMESPACE (hy);
