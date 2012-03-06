/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#pragma once
#include "xml.hpp"
#include "hash.hpp"

BEGIN_NAMESPACE (hy);
/*
 * Specializations
 */


/*
 * Extension for DOMNode - DOMNode
 */

XML::EntityCbRetval processHTMLTag (Tag *pTag, void *pArg);
bool processHTMLText (_char *pText, void *pArg);
bool processHTMLComment (_char *pComment, void *pArg);

class HTMLDoc
{
    private:
        typedef std::string string;
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
        XML m_sParser;
        static DOMNode *m_sCloneableNode;
        static bool s_bIsInitialised;

    private:
        SourceType m_sourceType;
        DOMNode *m_pRootNode;
        const char *m_pLocale;
        _char *m_pBuffer;

    public:
        HTMLDoc (SourceType, const char *pSource,
                const char *pLocale = "en_US.UTF-8");
        ~HTMLDoc ();

    public:
        DOMNode *getRootNode () { return m_pRootNode; }
        int toText (string &outString);

    private:
        DOMNode *parseFromFile (const char *pFileName);
        DOMNode *parse (const char *pContents, unsigned int length);

    public:
        void setProcessText (bool b = true) { s_bProcessTextData = b; }
        static int postProcessText (_char *pTextBufferEditable, _string &out);
};

class URLCollector
{
    private:
        typedef std::string string;
    public:
        std::list<hy::URL *> m_URLList;
        const char *m_pEncoding;
        unsigned int m_iMaxURLLen;
        string m_baseHref;

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
