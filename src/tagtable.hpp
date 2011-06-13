#pragma once
#include "localdefs.h"
#include "tagentry.hpp"
#include "hash.hpp"
#include "hashmap.hpp"

BEGIN_NAMESPACE(hy);
/**
 * TagTable - The table which contains all the valid Tags and rules
 *
 * This contains all valid tags for a particular instance of XML. It
 * is initialised using various TagEntry-objects, and combined
 * together, these TagEntry-s define the rules which any document that
 * XML parses is made to follow
 */
class TagTable
{
    public:
        typedef enum
        {
            EINIT,
            OK
        }
        status_t;

    public:
        TagTable ();
        /**
         * Constructor which takes an array of TagEntry-s for
         * constructing the internal rule-table
         */
        TagTable (const TagEntry *pte);

    private:
        int construct (const TagEntry *pte);

    public:
        const TagEntry *find (const _char *pName) const;
        const _char *rootTagName () const;
        inline status_t getStatus () const { return m_status; }

    protected:
        status_t m_status;
        const _char *m_pRootElementName;
        typedef HashMap <const _char *, const TagEntry *, strcasehash, eqcase>
            TERepository;
        TERepository m_TERepository;
};
END_NAMESPACE(hy);
