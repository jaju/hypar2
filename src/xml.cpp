/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#include "xml.hpp"
#include "entitystream.hpp"

#include <assert.h>
#include "debug.hpp"

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

void resetUnknownTagEntry (TagEntry &te, const TagTable &tagTable)
{
    te.setAncestor(tagTable.rootTagName ());
    te.setFirstAncestor(te.ancestor());
    te.setClosure(true);
}

XML::XML () :
    tagCb (0),
    textCb (0),
    commentCb (0),
    m_tagTable ((const TagEntry *) &defaultTable),
    m_bIgnoreUnknownTag (false),
    m_entityStack (),
    m_bMakeValidOnly (false),
    m_pCloneableNode (0),
    m_pRootNode (0),
    m_pCurrentNode (0),
    m_pCurrentParentNode (0)
{
    doNotClean ();
}

XML::XML (const TagEntry *pte) :
    tagCb (0),
    textCb (0),
    commentCb (0),
    m_tagTable (pte),
    m_bIgnoreUnknownTag (false),
    m_entityStack (),
    m_bMakeValidOnly (false),
    m_pCloneableNode (0),
    m_pRootNode (0),
    m_pCurrentNode (0),
    m_pCurrentParentNode (0)
{
}

void XML::reset ()
{
    m_pCloneableNode = 0;
    m_pRootNode = 0;
    m_pCurrentNode = 0;
    m_pCurrentParentNode = 0;
    m_entityStack.clear ();
}

int XML::initDOM (Tag *pTag)
{
    if (!m_pCloneableNode)
    {
        cerr << "XML::initDOM: Cloneable node not set! Returning ..." << endl;
        return -1;
    }

    m_pRootNode = m_pCloneableNode->clone ();
    m_pRootNode->initType(DOMNode::ELEMENT, (pTag ? pTag->name() : 0));

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
    const _char *pElementName = pTag->name();
    debug(("Adding node...\n"));
    DOMNode *pNode = m_pCloneableNode->clone ();
    pNode->initType(DOMNode::ELEMENT, pElementName);
    pNode->setName(pElementName);
    pNode->setSelfClosing(bSelfClosing);
    pNode->copyAttributes(pTag);
    pNode->setLevel(m_pCurrentParentNode->level() + 1);
    if (m_pCurrentNode)
    {
        m_pCurrentNode->attachAsNext(pNode);
        pNode->setHorizontalLevel(m_pCurrentNode->horizontalLevel() + 1);
        m_pCurrentNode = 0;
    }
    else
    {
        m_pCurrentParentNode->attachAsChild(pNode);
    }
    m_pCurrentParentNode = pNode;
    return 0;
}

int XML::addNodeSelfContained (DOMNode::NodeType nodeType, const _char *pContent)
{
    debug(("Adding self-contained XML node\n"));
    DOMNode *pNode = m_pCloneableNode->clone ();
    pNode->initType(nodeType, pContent);
    if (m_pCurrentNode)
    {
        m_pCurrentNode->attachAsNext(pNode);
        pNode->setHorizontalLevel(m_pCurrentNode->horizontalLevel() + 1);
    }
    else
    {
        m_pCurrentParentNode->attachAsChild(pNode);
    }
    pNode->setLevel(m_pCurrentParentNode->level() + 1);
    m_pCurrentNode = pNode;
    return 0;
}

int XML::addNodeSelfContained (Tag *t)
{
    debug(("Adding self-contained Tag named %s\n", t->name()));
    addNodeSelfContained (DOMNode::ELEMENT, t->name());
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

int XML::handleIgnoreChildren (Tag &tag, EntityStream &wideCharStream)
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
            if (_strncasecmp (tag.name(), wideCharStream.getBuffer (),
                        _strlen (tag.name())) == 0)
            {
                *pEnd = NULLCHAR;
                addNode (&tag);
                handleText (pEntity);
                closeNode ();
                wideCharStream.getTill (GANGLE, outLen, bFound, true);
                wideCharStream.setPrevEntity (EntityStream::ELEMENT);
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

DOMNode *XML::parse (_char *pTextBuffer, DOMNode *pCloneableNode, void *pCallbackArg)
{
    /* Define/declare needed variables */
    _char *pEntity;
    bool bDocStarted = false;
    OccurenceMap occurenceMap;

    /* Sanity checks */
    if (!pTextBuffer || !pCloneableNode)
    {
        return 0;
    }

    /* Reset all variables */
    reset ();
    EntityStream wideCharStream (pTextBuffer, _strlen(pTextBuffer));
    EntityStream::EntityType entityType;
    m_pCloneableNode = pCloneableNode;

    /* Now, parse the input and create the tree */
    initDOM ();

    while ((entityType = wideCharStream.getNextEntity (&pEntity, true))
            != EntityStream::END)
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
            case EntityStream::ELEMENT:
                {
                    EntityCbRetval retval = XML::OK;
                    debug (("Got an element. Tag string: '%s'\n", pEntity));
                    Tag tag (pEntity, true);
                    debug (("Tag name: '%s'\n", tag.name()));
                    if (tagCb)
                    {
                        retval = tagCb (&tag, pCallbackArg);
                        switch (retval)
                        {
                            case XML::DROP_SIMPLE:
                                debug (("Ignoring '%s' ...\n", tag.name()));
                                continue;
                            case XML::IGNORE_CHILDREN:
                                debug (("Ignoring children '%s' ...\n",
                                            tag.name()));
                                handleIgnoreChildren (tag, wideCharStream);
                                break;
                            case XML::OK:
                                if (handleElement (&tag, bDocStarted, occurenceMap) == 0)
                                {
                                    bDocStarted = true;
                                }
                            default:
                                break;
                        }
                    }
                    else
                    {
                        if (handleElement (&tag, bDocStarted, occurenceMap) == 0)
                        {
                            bDocStarted = true;
                        }
                    }
                }
                break;
            case EntityStream::TEXT:
                {
                    debug (("Got a text-block\n"));
                    if (textCb && !textCb (pEntity, pCallbackArg))
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
            case EntityStream::COMMENT:
                {
                    if (commentCb && !commentCb (pEntity, pCallbackArg))
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

inline int XML::handleElement (Tag *pTag, bool bDocStarted, OccurenceMap &occurenceMap)
{
    assert (pTag);
    TagEntry unknownTagEntry;
    const TagEntry *pCurTagEntry = 0;
    if (m_bMakeValidOnly)
    {
        resetUnknownTagEntry(unknownTagEntry, m_tagTable);
        pCurTagEntry = &unknownTagEntry;
        if (pTag->selfClosing())
        {
            unknownTagEntry.setClosure(false);
        }
        goto MAKE_VALID_ONLY_GOTO;
    }

    if (m_tagTable.getStatus () != TagTable::OK)
    {
        debug (("Table not in good state!\n"));
        return -1;
    }

    pCurTagEntry = m_tagTable.find (pTag->name());
    if (!pCurTagEntry)
    {
        if (m_bIgnoreUnknownTag && !m_bMakeValidOnly)
        {
            hy_warn (("Unknown tag. Ignoring -> '%s'\n", pTag->name()));
            return -1;
        }
        resetUnknownTagEntry (unknownTagEntry, m_tagTable);
        unknownTagEntry.setName(pTag->name());
        if (pTag->selfClosing())
        {
            unknownTagEntry.setClosure(false);
        }
        pCurTagEntry = &unknownTagEntry;
    }

    if (unlikely (!bDocStarted))
    {
        debug (("Document not started!\n"));
        if (pTag->endTag())
        {
            return -1;
        }
        const _char *rootTagName = m_tagTable.rootTagName();
        if (_strcasecmp (pTag->name(), rootTagName) != 0)
        {
            Tag t (rootTagName);
            hy_warn (("(Correction) Adding node '%s'\n", t.name()));
            addNode (&t, pCurTagEntry->closure());
            occurenceMap[rootTagName] = true;
            m_entityStack.push (rootTagName);
        }
        else
        {
            debug (("Adding node '%s'\n", pTag->name()));
            addNode (pTag);
            occurenceMap[rootTagName] = true;
            m_entityStack.push (pTag->name());
            return 0;
        }
    }

MAKE_VALID_ONLY_GOTO:
    /* Opening tag or closing tag? */
    if (pTag->endTag())
    {
        /* Is it supposed to end? */
        if (!pCurTagEntry->closure())
        {
            return -1;
        }
        int i = m_entityStack.locateFromTop (pTag->name());
        /* Pop everything */
        if (i < 0)
        {
            hy_warn(("Closing node found without any corresponding opening: '%s'\n", pTag->name()));
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
            correctStack (pCurTagEntry, false);
        }
        if (!pCurTagEntry->closure())
        {
            debug (("Adding a self-contained node '%s'\n", pTag->name()));
            addNodeSelfContained (pTag);
            pTag->setEndTag(true);
        }
        else
        {
            OccurenceMap::iterator occurenceMapIter = occurenceMap.find (pTag->name());
            if (pCurTagEntry->occurOnce() &&
                    (occurenceMapIter != occurenceMap.end ()) &&
                    (true == occurenceMapIter->second))
            {
                hy_warn (("Can't add tag since it can only occur once - '%s'\n", pCurTagEntry->name()));
                return -1;
            }
            clearCurrentContext (pTag, pCurTagEntry, false);
            addNode (pTag);
            if (pCurTagEntry == &unknownTagEntry)
            {
                m_pCurrentParentNode->addProperty (L ("unknowntag"), L ("1"));
            }
            occurenceMap[pTag->name()] = true;
            m_entityStack.push (pTag->name());
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
    if ((pAncestor = pTagEntry->getAncestor ()) != 0)
    {
        pAncestorList = pTagEntry->getAncestorList ();
    }
    else
    {
        if (!pTagEntry->parent())
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
        const TagEntry *pt = m_tagTable.find (tag.name());
        if (pt)
        {
            clearCurrentContext (&tag, pt, false);
            iCorrections += correctStack (pt, bCheckOnly);
        }
        addNode (&tag);
        m_pCurrentParentNode->addProperty (L("forceinserted"), L("1"));
        m_entityStack.push (tag.name());
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
    if ((pTagEntry->contextSwitch() == 1) &&
            (m_entityStack.locateFromTop (pTag->name()) > 0))
    {
        while (m_entityStack.size () > 0)
        {
            pte = m_tagTable.find (m_entityStack.top ());
            if (pte && (pte->contextLevel() < TagEntry::TEXT_ATTR))
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
    pte = m_tagTable.find (m_entityStack.top ());
    assert (!_strcasecmp (m_entityStack.top (), m_pCurrentParentNode->name()));
    if (pTagEntry->contextLevel() < TagEntry::TEXT_ATTR)
    {
        while (pte && (m_entityStack.size () > 0) &&
                (pte->contextLevel() >= pTagEntry->contextLevel()) &&
                (pte->contextSwitch() != 2))
        {
            if ((pte->contextLevel() == pTagEntry->contextLevel()) &&
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
            pte = m_tagTable.find (m_entityStack.top());
        }
    }
    return retval;
}

END_NAMESPACE (hy);
