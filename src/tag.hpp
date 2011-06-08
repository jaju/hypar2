#pragma once
#include "localdefs.h"
#include <list>

typedef std::list <std::pair <_char *, _char *> > TagAttrList;

BEGIN_NAMESPACE(hy);
/**
 * Class encapsulating all information about an individual Tag
 * instance, including name, attributes etc.
 */
class Tag
{
    public:
        const _char *m_pName;
        const _char *m_pStrEnd;
        _char *m_pTagAttrString;
        bool m_bEndTag, m_bSelfClosing;
        TagAttrList m_attrList;

    public:
        Tag ();
        /**
         * Constructor for a simple Tag element with no attributes
         */
        Tag (const _char *pName);
        /**
         * Constructor which takes a tag string
         * 
         * A complete string (without the &lt; and &gt;) which denotes
         * a tag string. The bool attribute tells whether the
         * attributes should be parsed or not. If the attributes are
         * parsed, the values are stored in an internal list object.
         */
        Tag (_char *pString, bool bParseAttributes);
        void reset ();

        /**
         * If the bool argument in the constructor is false, then this
         * function, called anytime later, causes the attributes to be
         * parsed
         */
        int parseFurther ();
        void toString (_string &targetString);

    private:
        int parseTag (_char *pTagString, bool bParseAttributes);

    public:
        int parseAttributes (_char *&pTagAttrString);
        const _char *findProperty (const _char *pName);

    private:
        inline int getQuotedName (_char *&pStr, _char **pName,
                bool bSetNull = false);
        inline int getQuotedValue (_char *&pStr,
                _char **pValue, bool bSetNull = false);
};
END_NAMESPACE(hy);
