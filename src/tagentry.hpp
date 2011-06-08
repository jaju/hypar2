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
        const _char *m_pName;
        unsigned int m_iContextSwitch;
        unsigned int m_iContextLevel;
        bool m_bClosure;
        bool m_bOccurOnce;

        enum
        {
            TEXT_ATTR = 1000
        };

    public:
        const _char *m_pParent;
        const _char *m_pRparent;
        const _char *m_pFirstParent;
        const _char *m_pFirstRparent;

    private:
        _char *m_pParentNew, *m_pRparentNew; /* strdup()'ed strings */
        std::vector <const _char *> *m_pParentList, *m_pRparentList;

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
                const _char *pRparent);
        ~TagEntry ();

    public:
        const _char *getName ();
        bool isParent (const _char *pParent);
        bool isRparent (const _char *pRparent);
        const _char *getParent () const;
        const _char *getRparent () const;
        const std::vector <const _char *> *getParentList () const;
        const std::vector <const _char *> *getRparentList () const;
};
END_NAMESPACE(hy);
