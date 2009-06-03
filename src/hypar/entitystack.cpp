/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#include "hypar/entitystack.hpp"
#include <assert.h>

BEGIN_NAMESPACE (hy);

typedef std::deque <const _char *>::iterator repIter;

int EntityStack::size ()
{
    return m_repository.size ();
}

void EntityStack::push (const _char *s)
{
    assert (s);
    m_repository.push_back (s);
}

int EntityStack::pop ()
{
    if (m_repository.size () > 0)
    {
        m_repository.pop_back ();
        return 1;
    }
    return 0;
}

void EntityStack::clear ()
{
    while (pop ());
}

const _char *EntityStack::top () const
{
    if (m_repository.size ())
        return m_repository.back ();
    return 0;
}

int EntityStack::locateFromTop (const _char *s)
{
    assert (s);
    int sz = size ();
    if (sz <= 0)
    {
        return -1;
    }
    for (int i = sz - 1; i >= 0; i--)
    {
        if (_strcasecmp (s, m_repository[i]) == 0)
        {
            return sz - i - 1;
        }
    }
    return -1;
}

int EntityStack::locateAnyFromTop (const std::vector<const _char *> *needle,
        int &index)
{
    int sz = needle->size ();
    int iPosFromTop = 0;
    for (index = 0; index < sz; index++)
    {
        iPosFromTop = locateFromTop (needle->at (index));
        if (iPosFromTop >= 0)
        {
            return iPosFromTop;
        }
    }
    return -1;
}

END_NAMESPACE (hy);
