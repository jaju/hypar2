/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#include "hypar/tagtable.hpp"
#include <limits.h>
#include <assert.h>

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

    if (!pte)
    {
        assert (0);
    }
    while (pte[iEntryCount].m_pName != 0)
    {
        NameTagPair p (pte[iEntryCount].m_pName, &pte[iEntryCount]);
        m_TERepository.insert (p);
        if (pte[iEntryCount].m_iContextLevel < iRootLevel)
        {
            iRootLevel = pte[iEntryCount].m_iContextLevel;
            m_pRootElementName = pte[iEntryCount].m_pName;
        }
        iEntryCount++;
    }
    assert (m_pRootElementName);
    return iEntryCount;
}

const TagEntry *TagTable::search (const _char *pName)
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

const _char *TagTable::getRootElement ()
{
    return m_pRootElementName;
}

END_NAMESPACE (hy);
