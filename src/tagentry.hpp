#pragma once
#include "localdefs.h"

#include <vector>

BEGIN_NAMESPACE(hy);
/**
 * TagEntry - All information about a valid 'Tag'
 *
 * For each valid tag, the name, information about where it can occur
 * in the DOM structure, whether it can occur only once, its parent as
 * well as 'remote' parents (either one) etc.
 */
class TagEntry
{
    public:

        enum
        {
            TEXT_ATTR = 1000
        };

    public:
        const _char *name () const;
        const _char *parent() const { return m_pParent; }
        const _char *ancestor() const { return m_pAncestor; }
        const _char *firstParent() const { return m_pFirstParent; }
        const _char *firstAncestor() const { return m_pFirstAncestor; }
        unsigned int contextSwitch() const { return m_iContextSwitch; }
        unsigned int contextLevel() const { return m_iContextLevel; }
        bool closure() const { return m_bClosure; }
        bool occurOnce() const { return m_bOccurOnce; }

        void setName(const _char *x) { m_pName = x; }
        void setParent(const _char *x) { m_pParent = x; }
        void setAncestor(const _char *x) { m_pAncestor = x; }
        void setFirstParent(const _char *x) { m_pFirstParent = x; }
        void setFirstAncestor(const _char *x) { m_pFirstAncestor = x; }
        void setClosure(bool b) { m_bClosure = b; }

    private:
        unsigned int m_iContextSwitch;
        unsigned int m_iContextLevel;
        bool m_bClosure;
        bool m_bOccurOnce;
        const _char *m_pName;
        const _char *m_pParent;
        const _char *m_pAncestor;
        const _char *m_pFirstParent;
        const _char *m_pFirstAncestor;
        _char *m_pParentNew, *m_pAncestorNew; /* strdup()'ed strings */
        std::vector <const _char *> *m_pParentList, *m_pAncestorList;

    private:
        inline bool findSubstr (const _char *needle,
                const _char *haystack) const;
        inline int findInList (const _char *needle,
                std::vector <const _char *> *haystack) const;

    public:
        TagEntry ();
        TagEntry (const _char *pName, unsigned int iContextSwitch,
                unsigned int iContextLevel, bool bClosure,
                bool bOccurOnce, const _char *pParent,
                const _char *pAncestor);
        ~TagEntry ();

    public:
        bool isParent (const _char *pParent);
        bool isAncestor (const _char *pAncestor);
        const _char *getParent () const;
        const _char *getAncestor () const;
        const std::vector <const _char *> *getParentList () const;
        const std::vector <const _char *> *getAncestorList () const;
};
END_NAMESPACE(hy);
