#pragma once
#include "hypar/localdefs.h"
#include <deque>
#include <vector>

BEGIN_NAMESPACE(hy);

class EntityStack
{
    protected:
        std::deque <const _char *> m_repository;

    public:
        EntityStack () : m_repository () {}
        ~EntityStack () {}
    public:
        int size ();
        void push (const _char *);
        int pop ();
        void clear ();
        const _char *top () const;
        int locateFromTop (const _char *needle);
        int locateAnyFromTop (const std::vector <const _char *> *needle,
                int &index);
        const _char *operator[] (int i)
        {
            return m_repository[i];
        }
};

END_NAMESPACE(hy);
