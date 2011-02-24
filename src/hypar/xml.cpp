/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#include "hypar/xml.hpp"
#include "hypar/charentitystream.hpp"

#include <assert.h>
#include "hypar/debug.hpp"

#include <iostream>
#include <vector>

using std::cerr;
using std::endl;
using std::flush;

BEGIN_NAMESPACE (hy);

#define HYXML_ROOT_NODE L("XML_Root")

static const TagEntry defaultTable[] =
{
    TagEntry (L("root"),  1,  1,    true,   true, 0, 0),
    TagEntry (0, 0, TagEntry::TEXT_ATTR, false, false, 0, 0)
};

void XML::resetUnknownTagEntry ()
{
    m_unknownTagEntry.m_pRparent = m_tagTable.getRootElement ();
    m_unknownTagEntry.m_pFirstRparent = m_unknownTagEntry.m_pRparent;
    m_unknownTagEntry.m_bClosure = true;
}

XML::XML () :
    m_tagTable ((const TagEntry *) &defaultTable),
    m_bDocStarted (false),
    m_bIgnoreUnknownTag (false),
    m_pCurTagEntry (0),
    m_occurenceMap (),
    m_entityStack (),
    m_bMakeValidOnly (false),
    m_pCloneableNode (0),
    m_pRootNode (0),
    m_pCurrentNode (0),
    m_pCurrentParentNode (0),
    m_pCallbackArg (0),
    tagCb (0),
    textCb (0),
    commentCb (0)
{
    doNotClean ();
    resetUnknownTagEntry ();
    m_occurenceMapIter = m_occurenceMap.end ();
}

XML::XML (const TagEntry *pte) :
    m_tagTable (pte),
    m_bDocStarted (false),
    m_bIgnoreUnknownTag (false),
    m_pCurTagEntry (0),
    m_occurenceMap (),
    m_entityStack (),
    m_bMakeValidOnly (false),
    m_pCloneableNode (0),
    m_pRootNode (0),
    m_pCurrentNode (0),
    m_pCurrentParentNode (0),
    m_pCallbackArg (0),
    tagCb (0),
    textCb (0),
    commentCb (0)
{
    resetUnknownTagEntry ();
    m_occurenceMapIter = m_occurenceMap.end ();
}

void XML::reset ()
{
    m_pCloneableNode = 0;
    m_pRootNode = 0;
    m_pCurrentNode = 0;
    m_pCurrentParentNode = 0;
    m_entityStack.clear ();
    m_occurenceMap.clear ();
    m_occurenceMapIter = m_occurenceMap.end ();
}

int XML::initDOM (Tag *pTag)
{
    if (!m_pCloneableNode)
    {
        cerr << "XML::initDOM: Cloneable node not set! Returning ..." << endl;
        return -1;
    }

    m_pRootNode = m_pCloneableNode->clone (DOMNode::ELEMENT,
            pTag ? pTag->m_pName : 0);

    m_pCurrentParentNode = m_pRootNode;
    if (pTag)
    {
        m_pCurrentParentNode->copyAttributes (pTag);
    }
    m_pCurrentNode = 0;
    return 0;
}

int XML::addNode (Tag *pTag, bool bSelfClosing)
{
    addNode (pTag->m_pName, bSelfClosing);
    m_pCurrentParentNode->copyAttributes (pTag);
    return 0;
}

int XML::addNode (const _char *pElementName, bool bSelfClosing)
{
    debug(("Adding XML node...\n"));
    DOMNode *pNode = m_pCloneableNode->clone (DOMNode::ELEMENT, pElementName);
    pNode->selfClosing() = bSelfClosing;
    if (m_pCurrentNode)
    {
        m_pCurrentNode->next() = pNode;
        pNode->previous() = m_pCurrentNode;
        pNode->horizontalLevel() = m_pCurrentNode->horizontalLevel() + 1;
        m_pCurrentNode = 0;
    }
    else
    {
        m_pCurrentParentNode->child() = pNode;
    }
    pNode->parent() = m_pCurrentParentNode;
    m_pCurrentParentNode->last() = pNode;
    pNode->level() = m_pCurrentParentNode->level() + 1;
    m_pCurrentParentNode = pNode;
    return 0;
}

int XML::addNodeSelfContained (DOMNode::NodeType nodeType,
        const _char *pContent)
{
    debug(("Adding self-contained XML node\n"));
    DOMNode *pNode = m_pCloneableNode->clone (nodeType, pContent);
    if (m_pCurrentNode)
    {
        m_pCurrentNode->next() = pNode;
        pNode->previous() = m_pCurrentNode;
        pNode->horizontalLevel() = m_pCurrentNode->horizontalLevel() + 1;
    }
    else
    {
        m_pCurrentParentNode->child() = pNode;
    }
    pNode->parent() = m_pCurrentParentNode;
    pNode->level() = m_pCurrentParentNode->level() + 1;
    m_pCurrentParentNode->last() = pNode;
    m_pCurrentNode = pNode;
    return 0;
}

int XML::addNodeSelfContained (Tag *t)
{
    debug(("Adding self-contained Tag named %s\n", t->m_pName));
    addNodeSelfContained (DOMNode::ELEMENT, t->m_pName);
    m_pCurrentNode->copyAttributes (t);
    return 0;
}

int XML::closeNode ()
{
    debug(("Closing node...\n"));
    if (m_pCurrentParentNode == m_pRootNode)
    {
        debug (("Tried closing more nodes than you opened!"));
        return -1;
    }
    m_pCurrentNode = m_pCurrentParentNode;
    m_pCurrentParentNode = m_pCurrentParentNode->parent();
    return 0;
}

int XML::closeNode (unsigned int iNumTimes)
{
    unsigned int i = 0;
    for (; i < iNumTimes; i++)
    {
        if (closeNode () < 0)
        {
            return -1;
        }
    }
    return i;
}

int XML::handleIgnoreChildren (Tag &tag, CharEntityStream &wideCharStream)
{
    long outLen = 0;
    bool bFound = false;
    _char *pEntity = 0;
    _char *pEnd = 0;
    _char peekedChar;
    int iMoreAdvances = 0; // takes care of futher advance () calls for
                           // re-setting pEnd

goto_HICFurther:
    if (!pEntity)
    {
        pEntity = wideCharStream.getTill (LANGLE, outLen, bFound, false);
        pEnd = pEntity + outLen - 1;
    }
    else
    {
        wideCharStream.getTill (LANGLE, outLen, bFound, false);
        pEnd += outLen + iMoreAdvances;
    }
    if (bFound)
    {
        iMoreAdvances = 0;
        wideCharStream.advance (outLen);
        while ((peekedChar = wideCharStream.peek ()) && _isspace (peekedChar))
        {
            wideCharStream.advance ();
            iMoreAdvances++;
        }
        if ((peekedChar = wideCharStream.peek ()) == SLASH)
        {
            wideCharStream.get ();
            iMoreAdvances++;
            while (_isspace (wideCharStream.peek ()))
            {
                wideCharStream.get ();
                iMoreAdvances++;
            }
            if (_strncasecmp (tag.m_pName, wideCharStream.getBuffer (),
                        _strlen (tag.m_pName)) == 0)
            {
                *pEnd = NULLCHAR;
                addNode (&tag);
                handleText (pEntity);
                closeNode ();
                wideCharStream.getTill (GANGLE, outLen, bFound, true);
                wideCharStream.setPrevEntity (CharEntityStream::ELEMENT);
            }
            else
            {
                if (!peekedChar)
                    return 0;
                goto goto_HICFurther;
            }
        }
        else
        {
            if (!peekedChar)
                return 0;
            goto goto_HICFurther;
        }
    }
    return 1;
}

void XML::doNotClean (bool flag)
{
    m_bMakeValidOnly = flag;
}

/**
 * The 'main' routine. Note: pTextBuffer should be editable!
 */

#if USE_WIDECHAR
DOMNode *XML::parse (char *pTextBuffer, DOMNode *pCloneableNode)
{
    assert (0);
    return 0;
}
#endif

DOMNode *XML::parse (_char *pTextBuffer, DOMNode *pCloneableNode)
{
    /* Define/declare needed variables */
    _char *pEntity;
    bool bDocStarted = false;

    /* Sanity checks */
    if (!pTextBuffer || !pCloneableNode)
    {
        return 0;
    }

    /* Reset all variables */
    reset ();
    CharEntityStream wideCharStream (pTextBuffer, _strlen(pTextBuffer));
    CharEntityStream::EntityType entityType;
    m_pCloneableNode = pCloneableNode;

    /* Now, parse the input and create the tree */
    initDOM ();

    while ((entityType = wideCharStream.getNextEntity (&pEntity, true))
            != CharEntityStream::END)
    {
        if (!m_bMakeValidOnly && bDocStarted && !m_entityStack.size ())
        {
            hy_warn(("Document end encountered, but there's still text in input!"));
            /*
             * The document is not yet over, but we have encountered the end
             * of the root element
             */
            break;
        }
        switch (entityType)
        {
            case CharEntityStream::ELEMENT:
                {
                    EntityCbRetval retval = XML::OK;
                    debug (("Got an element. Tag string: '%s'\n", pEntity));
                    Tag tag (pEntity, true);
                    debug (("Tag name: '%s'\n", tag.m_pName));
                    if (tagCb)
                    {
                        retval = tagCb (&tag, m_pCallbackArg);
                        switch (retval)
                        {
                            case XML::DROP_SIMPLE:
                                debug (("Ignoring '%s' ...\n", tag.m_pName));
                                continue;
                            case XML::IGNORE_CHILDREN:
                                debug (("Ignoring children '%s' ...\n",
                                            tag.m_pName));
                                handleIgnoreChildren (tag, wideCharStream);
                                break;
                            case XML::OK:
                                if (handleElement (&tag, bDocStarted) == 0)
                                {
                                    bDocStarted = true;
                                }
                            default:
                                break;
                        }
                    }
                    else
                    {
                        if (handleElement (&tag, bDocStarted) == 0)
                        {
                            bDocStarted = true;
                        }
                    }
                }
                break;
            case CharEntityStream::TEXT:
                {
                    debug (("Got a text-block\n"));
                    if (textCb && !textCb (pEntity, m_pCallbackArg))
                    {
                        break;
                    }
                    if (unlikely (!bDocStarted))
                    {
                        break;
                    }
                    handleText (pEntity);
                }
                break;
            case CharEntityStream::COMMENT:
                {
                    if (commentCb && !commentCb (pEntity, m_pCallbackArg))
                    {
                        break;
                    }
                    handleComment (pEntity);
                }
                break;
            default:
                break;
        }
    }
    return m_pRootNode;
}

inline int XML::handleElement (Tag *pTag, bool bDocStarted)
{
    assert (pTag);
    if (m_bMakeValidOnly)
    {
        resetUnknownTagEntry ();
        m_pCurTagEntry = &m_unknownTagEntry;
        if (pTag->m_bSelfClosing)
        {
            m_unknownTagEntry.m_bClosure = false;
        }
        goto MAKE_VALID_ONLY_GOTO;
    }

    if (m_tagTable.getStatus () != TagTable::OK)
    {
        debug (("Table not in good state!\n"));
        return -1;
    }

    m_pCurTagEntry = m_tagTable.search (pTag->m_pName);
    if (!m_pCurTagEntry)
    {
        if (m_bIgnoreUnknownTag && !m_bMakeValidOnly)
        {
            hy_warn (("Unknown tag. Ignoring -> '%s'\n", pTag->m_pName));
            return -1;
        }
        resetUnknownTagEntry ();
        m_unknownTagEntry.m_pName = pTag->m_pName;
        if (pTag->m_bSelfClosing)
        {
            m_unknownTagEntry.m_bClosure = false;
        }
        m_pCurTagEntry = &m_unknownTagEntry;
    }

    if (unlikely (!bDocStarted))
    {
        debug (("Document not started!\n"));
        if (pTag->m_bEndTag)
        {
            return -1;
        }
        if (_strcasecmp (pTag->m_pName, m_tagTable.getRootElement ()) != 0)
        {
            Tag t (m_tagTable.getRootElement ());
            hy_warn (("(Correction) Adding node '%s'\n", t.m_pName));
            addNode (&t, m_pCurTagEntry->m_bClosure);
            m_occurenceMap[m_tagTable.getRootElement ()] = true;
            m_entityStack.push (m_tagTable.getRootElement ());
        }
        else
        {
            debug (("Adding node '%s'\n", pTag->m_pName));
            addNode (pTag);
            m_occurenceMap[m_tagTable.getRootElement ()] = true;
            m_entityStack.push (pTag->m_pName);
            return 0;
        }
    }

MAKE_VALID_ONLY_GOTO:
    /* Opening tag or closing tag? */
    if (pTag->m_bEndTag)
    {
        /* Is it supposed to end? */
        if (!m_pCurTagEntry->m_bClosure)
        {
            return -1;
        }
        int i = m_entityStack.locateFromTop (pTag->m_pName);
        /* Pop everything */
        if (i < 0)
        {
            hy_warn(("Closing node found without any corresponding opening: '%s'\n", pTag->m_pName));
        }
        else if (i > 0)
        {
            if (m_pCurrentParentNode)
                m_pCurrentParentNode->addProperty(L("childrenforceclosed"), L("1"));
        }
        while (i >= 0)
        {
            if (i != 0)
                m_pCurrentParentNode->addProperty (L ("forceclosed"), L ("10"));
            closeNode ();
            m_entityStack.pop ();
            i--;
        }
    }
    else
    {
        if (!m_bMakeValidOnly)
        {
            correctStack (m_pCurTagEntry, false);
        }
        if (!m_pCurTagEntry->m_bClosure)
        {
            debug (("Adding a self-contained node '%s'\n", pTag->m_pName));
            addNodeSelfContained (pTag);
            pTag->m_bEndTag = true;
        }
        else
        {
            int i;
            i = m_entityStack.locateFromTop (pTag->m_pName);
            m_occurenceMapIter = m_occurenceMap.find (pTag->m_pName);
            if (m_pCurTagEntry->m_bOccurOnce &&
                    (m_occurenceMapIter != m_occurenceMap.end ()) &&
                    (true == m_occurenceMapIter->second))
            {
                hy_warn (("Can't add tag since it can only occur once - '%s'\n", m_pCurTagEntry->m_pName));
                return -1;
            }
            clearCurrentContext (pTag, m_pCurTagEntry, false);
            addNode (pTag);
            if (m_pCurTagEntry == &m_unknownTagEntry)
            {
                m_pCurrentParentNode->addProperty (L ("unknowntag"), L ("1"));
            }
            m_occurenceMap[pTag->m_pName] = true;
            m_entityStack.push (pTag->m_pName);
        }
    }
    return 0;
}

inline int XML::handleText (_char *pText)
{
    return addNodeSelfContained (DOMNode::TEXT, pText);
}

inline int XML::handleComment (_char *pComment)
{
    return addNodeSelfContained (DOMNode::COMMENT, pComment);
}

int XML::correctStack (const TagEntry *pTagEntry, bool bCheckOnly)
{
    assert (m_tagTable.getStatus () == TagTable::OK);
    int iCorrections = 0;
    const _char *pAncestor = 0;
    const std::vector <const _char *> *pAncestorList = 0;
    int iDummy = -1, iLocationFromTop = -1;
    bool bParent = false;
    if ((pAncestor = pTagEntry->getRparent ()) != 0)
    {
        pAncestorList = pTagEntry->getRparentList ();
    }
    else
    {
        if (!pTagEntry->m_pParent)
        {
            /* This could be the root element! */
            return iCorrections;
        }
        pAncestor = pTagEntry->getParent ();
        pAncestorList = pTagEntry->getParentList ();
        bParent = true;
    }
    if (pAncestorList)
    {
        iLocationFromTop = m_entityStack.
            locateAnyFromTop (pAncestorList, iDummy);
    }
    else
    {
        iLocationFromTop = m_entityStack.locateFromTop (pAncestor);
    }
    if (iLocationFromTop < 0)
    {
        if (bCheckOnly)
            return 1;
        Tag tag (pAncestor);
        const TagEntry *pt = m_tagTable.search (tag.m_pName);
        if (pt)
        {
            clearCurrentContext (&tag, pt, false);
            iCorrections += correctStack (pt, bCheckOnly);
        }
        addNode (&tag);
        m_pCurrentParentNode->addProperty (L("forceinserted"), L("1"));
        m_entityStack.push (tag.m_pName);
        iCorrections++;
    }
    else if (bParent && iLocationFromTop)
    {
        if (bCheckOnly)
            return 1;
        while (iLocationFromTop)
        {
            m_pCurrentParentNode->addProperty(L("forceclosed"), L("1"));
            closeNode (); m_entityStack.pop ();
            iLocationFromTop--; iCorrections++;
        }
    }
    return iCorrections;
}

int XML::clearCurrentContext (Tag *pTag, const TagEntry *pTagEntry,
        bool bCheckOnly)
{
    assert (m_tagTable.getStatus () == TagTable::OK);
    if (!m_entityStack.size ())
    {
        return 0;
    }
    int retval = 0;
    const TagEntry *pte;
    if ((pTagEntry->m_iContextSwitch == 1) &&
            (m_entityStack.locateFromTop (pTag->m_pName) > 0))
    {
        while (m_entityStack.size () > 0)
        {
            pte = m_tagTable.search (m_entityStack.top ());
            if (pte && (pte->m_iContextLevel < TagEntry::TEXT_ATTR))
            {
                break;
            }
            if (bCheckOnly)
            {
                return 1;
            }
            m_pCurrentParentNode->addProperty (L("forceclosed"), L("1"));
            closeNode (); m_entityStack.pop (); retval++;
        }
    }
    if (!m_entityStack.size ())
    {
        return 0;
    }
    pte = m_tagTable.search (m_entityStack.top ());
    assert (!_strcasecmp (m_entityStack.top (), m_pCurrentParentNode->name()));
    if (pTagEntry->m_iContextLevel < TagEntry::TEXT_ATTR)
    {
        while (pte && (m_entityStack.size () > 0) &&
                (pte->m_iContextLevel >= pTagEntry->m_iContextLevel) &&
                (pte->m_iContextSwitch != 2))
        {
            if ((pte->m_iContextLevel == pTagEntry->m_iContextLevel) &&
                    (pte != pTagEntry))
            {
                break;
            }
            if (bCheckOnly)
            {
                return 1;
            }
            m_pCurrentParentNode->addProperty (L("forceclosed"), L("2"));
            closeNode (); m_entityStack.pop (); retval++;
            if (pte == pTagEntry)
            {
                break;
            }
            pte = m_tagTable.search (m_entityStack.top());
        }
    }
    return retval;
}

END_NAMESPACE (hy);
