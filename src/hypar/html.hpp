/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#ifndef _HYPAR_HTML_H_
#define _HYPAR_HTML_H_

#include "hypar/xml.hpp"
#include "hypar/hash.hpp"

BEGIN_NAMESPACE (hy);
/*
 * Specializations
 */


/*
 * Extension for DOMNode - HTMLNode
 */

XML::EntityCbRetval processHTMLTag (Tag *pTag, void *pArg);
bool processHTMLText (_char *pText, void *pArg);
bool processHTMLComment (_char *pComment, void *pArg);

class HTMLNode : public DOMNode
{
    public:
        HTMLNode();
        HTMLNode(DOMNode::NodeType nodeType, const _char *pStr);
        HTMLNode *clone();
        HTMLNode *clone(NodeType nodeType, const _char *pStr);

    public:
        void toText (_string &targetString, bool bChildOnly = false);
};

class HTMLDoc
{
    public:
        typedef enum
        {
            kMEMBUF = 0,
            kFILE
        }
        SourceType;

    public:
        static const TagEntry s_defaultHTMLTable[];
        static bool s_bProcessTextData;


    private:
        static XML m_sParser;
        static HTMLNode m_sCloneableNode;
        static bool s_bIsInitialised;

    private:
        SourceType m_sourceType;
        HTMLNode *m_pRootNode;
        const char *m_pLocale;
        _char *m_pBuffer;

    public:
        HTMLDoc (SourceType, const char *pSource,
                const char *pLocale = "en_US.UTF-8");
        ~HTMLDoc ();

    public:
        HTMLNode *getRootNode () { return m_pRootNode; }
        int toString (string &outString);
        int toText (string &outString);

    private:
        HTMLNode *parseFromFile (const char *pFileName);
        HTMLNode *parse (const char *pContents, unsigned int length);

    public:
        void setProcessText (bool b = true) { s_bProcessTextData = b; }
        static int postProcessText (_char *pTextBufferEditable, _string &out);
};

class URLCollector
{
    public:
        list<hy::URL *> m_URLList;
        unsigned int m_iMaxURLLen;
        string m_baseHref;
        const char *m_pEncoding;

    public:
        URLCollector ();
        URLCollector (DOMNode *pNode);
        URLCollector (_char *pBuffer);

    public:
        int collect (DOMNode *pNode);
        int collect (_char *pBuffer, const char *pEncoding = "ISO-8859-1");

    public:
        static XML::EntityCbRetval href_callback (Tag *pTag, void *pArg);
        static bool text_callback (_char *p, void *pArg);
        static bool comm_callback (_char *p, void *pArg);
};

END_NAMESPACE (hy);
#endif // _HYPAR_HTML_H_
