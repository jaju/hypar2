#ifndef _HYPAR_TAGTABLE_H_
#define _HYPAR_TAGTABLE_H_
#include "hypar/localdefs.h"
#include "hypar/tagentry.hpp"
#include "hypar/hash.hpp"
#include "hypar/hashmap.hpp"

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
        const TagEntry *search (const _char *pName);
        const _char *getRootElement ();
        inline status_t getStatus () { return m_status; }

    protected:
        status_t m_status;
        const _char *m_pRootElementName;
        typedef HashMap <const _char *, const TagEntry *, strcasehash, eqcase>
            TERepository;
        TERepository m_TERepository;
};
END_NAMESPACE(hy);
#endif // _HYPAR_TAGTABLE_H_
