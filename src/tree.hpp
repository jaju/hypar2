/*
 * tree.hpp
 *
 *  Created on: Nov 28, 2010
 *      Author: jaju
 */

#pragma once
#include <cassert>

template<typename G>
class TreeNode : public G {
    public:
        TreeNode *parent, *prev, *next, *child, *last;
        int level, horizontalLevel;

    public:
        static TreeNode *create() {
            return new TreeNode();
        }

    protected:
        TreeNode() :
            G(),
            parent (0),
            prev (0),
            next (0),
            child (0),
            last (0),
            level (0),
            horizontalLevel (0) {}

        virtual ~TreeNode() {
            TreeNode *pTmpNode = child;

            while (child)
            {
                pTmpNode = child;
                child = child->next;
                delete pTmpNode;
            }
        }

        virtual void reset () {
            parent = 0;
            prev = 0;
            next = 0;
            child = 0;
            last = 0;
            level = 0;
            horizontalLevel = 0;
            G::reset();
        }

    public:
        virtual int detach ()
        {
            if (!parent && !prev && !next)
                return -1;
            if (prev)
            {
                prev->next = next;
            }
            else
            {
                if (parent)
                {
                    parent->child = next;
                }
            }
            if (next)
            {
                next->prev = prev;
            }
            else
            {
                if (parent)
                {
                    parent->last = 0;
                }
            }
            parent = 0;
            prev = 0;
            next = 0;
            return 0;
        }

        int initLevel (int iLevel = 0, bool bNext = true) {
            level = iLevel;
            if (child)
            {
                child->initLevel (level + 1, true);
            }
            if (bNext && next)
            {
                next->initLevel (level, true);
            }
            return 0;
        }

        virtual int attachAsChild (TreeNode *pNode) {
            if (!pNode)
                return -1;
            if (!child)
            {
                child = last = pNode;
                pNode->parent = this;
                pNode->prev = pNode->next = 0;
                pNode->initLevel (level + 1, true);
                return 1;
            }
            else
            {
                int retval = last->attachAsNext (pNode);
                assert (last == pNode);
                return retval;
            }
        }

        virtual int insertAsParent (TreeNode *pNode) {
            if (!pNode)
            {
                return -1;
            }
            if (prev)
            {
                prev->next = pNode;
            }
            if (next)
            {
                next->prev = pNode;
            }
            if (parent)
            {
                if (this == parent->child)
                {
                    parent->child = pNode;
                }
                if (this == parent->last)
                {
                    parent->last = pNode;
                }
            }
            prev = next = parent = 0;
            return pNode->attachAsChild (this);
        }

        virtual int insertAsChild (TreeNode *pNewChild) {
            TreeNode *pChild = child;
            while (pChild)
            {
                pChild->parent = pNewChild;
                pChild = pChild->next;
            }
            pNewChild->child = child;
            pNewChild->last = last;
            child = pNewChild;
            last = pNewChild;
            child->initLevel(level + 1, true);
            return 1;
        }

        virtual int attachAsPrevious (TreeNode *pNode) {
            if (!pNode)
                return -1;
            pNode->next = this;
            pNode->prev = prev;
            pNode->parent = parent;
            if (prev)
            {
                prev->next = pNode;
            }
            else
            {
                if (parent)
                {
                    parent->child = pNode;
                }
            }
            prev = pNode;
            pNode->initLevel(level, false);
            return 0;
        }

        virtual int attachAsNext (TreeNode *pNode) {
            if (!pNode)
                return -1;
            pNode->next = next;
            pNode->prev = this;
            pNode->parent = parent;
            if (next)
            {
                next->prev = pNode;
            }
            else
            {
                if (parent)
                {
                    parent->last = pNode;
                }
            }
            next = pNode;
            pNode->initLevel (level, false);
            return 0;
        }
};
