/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#pragma once
#include "localdefs.h"
#include "tag.hpp"
#include "tagentry.hpp"
#include "tagtable.hpp"
#include "entitystack.hpp"
#include "domnode.hpp"

BEGIN_NAMESPACE (hy);

class EntityStream;
typedef HashMap <const _char *, bool, strcasehash, eqcase> OccurenceMap;

/**
 * class XML : The main class of HyParSuite
 * This is the main class, and the 'main' function to use is 'parse'
 */

class XML
{
    public:
        /**
         * enum EntityCbRetval - Return value of 'entity callback functions'
         */
        typedef enum
        {
            DROP_SIMPLE = -100,
            DROP_CHILDREN,
            IGNORE_CHILDREN,
            OK = 0
        }
        EntityCbRetval;

    private:
        TagTable m_tagTable;
        bool m_bDocStarted, m_bIgnoreUnknownTag;
        const TagEntry *m_pCurTagEntry;
        OccurenceMap m_occurenceMap;
        EntityStack m_entityStack;
        bool m_bMakeValidOnly;

    private:
        DOMNode *m_pCloneableNode, *m_pRootNode, *m_pCurrentNode,
                *m_pCurrentParentNode;

    public:
        XML ();
        XML (const TagEntry *pte);

    public:
        void doNotClean (bool flag = true);
        /**
         * The 'main' call.
         *
         * It does everything, and returns a 'DOM' tree to the caller, with the
         * supplied pCloneableNode as the root of this 'DOM' tree.
         * NOTE the quotes - this isn't the standards-compliant DOM.
         */
        DOMNode *parse (_char *pTextBuffer, DOMNode *pCloneableNode);

    private:
        void reset ();

        inline int handleElement (Tag *t, bool bDocStarted);
        inline int handleIgnoreChildren (Tag &t, EntityStream &wcs);
        inline int handleText (_char *pText);
        inline int handleComment (_char *pComment);

        int correctStack (const TagEntry *pTagEntry, bool bCheckOnly = false);
        int clearCurrentContext (Tag *t, const TagEntry *pTagEntry,
                bool bCheckOnly = false);

        int initDOM (Tag *tag = 0);
        int addNode (Tag *tag, bool bClosure = false);
        int addNodeSelfContained (DOMNode::NodeType nodeType,
                const _char *pContent);
        int addNodeSelfContained (Tag *t);
        int closeNode ();
        int closeNode (unsigned int iNumTimes);

    public:
        void *m_pCallbackArg;
        /**
         * A call-back function called when 'tags' are encountered
         *
         * Set this to some function you define, and everytime a tag is
         * encountered in the text, this function is called with the parsed Tag
         * element. Further processing path depends on the return value of this
         * function. XML proceeds normally only if XML::OK is returned. If
         * XML::DROP_SIMPLE is returned, this Tag is simply ignored, as if
         * it never occured in the text. If XML::IGNORE_CHILDREN is returned,
         * all text upto the closing tag of the current tag-entity is ignored.
         * The IGNORE_CHILDREN way of ignoring is used to tell XML not to do
         * anything with Javascript till &lt;/script&gt; is encountered, for
         * example.
         * <p>
         * Similarly, DROP_SIMPLE can be used, say, when one wants to only
         * gather the links in a document, and does not want the tree to be
         * created.
         */
        EntityCbRetval (*tagCb) (Tag *pTag, void *x);
        /**
         * Call back for text entities. Text is ignored if return value is
         * false
         */
        bool (*textCb) (_char *pText, void *x);
        /**
         * Call back for comments. Comment is ignored if return value is false
         */
        bool (*commentCb) (_char *pComment, void *x);
};

END_NAMESPACE (hy);
