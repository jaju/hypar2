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
    protected:
        TreeNode *_parent, *_prev, *_next, *_child, *_last;

    public:
        int level, horizontalLevel;

    public:
        static TreeNode *create() {
            return new TreeNode();
        }
        int attachAsNext (TreeNode *pNode);
        int attachAsPrevious (TreeNode *pNode);
        int insertAsChild (TreeNode *pNewChild);
        int insertAsParent (TreeNode *pNode);
        int attachAsChild (TreeNode *pNode);
        int initLevel (int iLevel = 0, bool bNext = true);
        int detach ();

    protected:
        TreeNode();
        virtual ~TreeNode();
        virtual void reset ();
};



////////////////////////////////////////////////////////////////////
//////////////            Implementation           /////////////////
////////////////////////////////////////////////////////////////////
template<typename G>
TreeNode<G>::TreeNode() :
    G(), _parent (0), _prev (0), _next (0),
    _child (0), _last (0), level (0), horizontalLevel (0) {}

    template<typename G>
    TreeNode<G>::~TreeNode() {
        TreeNode *pTmpNode = _child;

        while (_child)
        {
            pTmpNode = _child;
            _child = _child->_next;
            delete pTmpNode;
        }
    }

template<typename G>
void TreeNode<G>::reset () {
    _parent = 0;
    _prev = 0;
    _next = 0;
    _child = 0;
    _last = 0;
    level = 0;
    horizontalLevel = 0;
    G::reset();
}

template<typename G>
int TreeNode<G>::detach ()
{
    if (!_parent && !_prev && !_next)
        return -1;
    if (_prev)
    {
        _prev->_next = _next;
    }
    else
    {
        if (_parent)
        {
            _parent->_child = _next;
        }
    }
    if (_next)
    {
        _next->_prev = _prev;
    }
    else
    {
        if (_parent)
        {
            _parent->_last = 0;
        }
    }
    _parent = 0;
    _prev = 0;
    _next = 0;
    return 0;
}

template<typename G>
int TreeNode<G>::initLevel (int iLevel, bool bNext) {
    level = iLevel;
    if (_child)
    {
        _child->initLevel (level + 1, true);
    }
    if (bNext && _next)
    {
        _next->initLevel (level, true);
    }
    return 0;
}

template<typename G>
int TreeNode<G>::attachAsChild (TreeNode *pNode) {
    if (!pNode)
        return -1;
    if (!_child)
    {
        _child = _last = pNode;
        pNode->_parent = this;
        pNode->_prev = pNode->_next = 0;
        pNode->initLevel (level + 1, true);
        return 1;
    }
    else
    {
        int retval = _last->attachAsNext (pNode);
        assert (_last == pNode);
        return retval;
    }
}

template<typename G>
int TreeNode<G>::insertAsParent (TreeNode *pNode) {
    if (!pNode)
    {
        return -1;
    }
    if (_prev)
    {
        _prev->_next = pNode;
    }
    if (_next)
    {
        _next->_prev = pNode;
    }
    if (_parent)
    {
        if (this == _parent->_child)
        {
            _parent->_child = pNode;
        }
        if (this == _parent->_last)
        {
            _parent->_last = pNode;
        }
    }
    _prev = _next = _parent = 0;
    return pNode->attachAsChild (this);
}

template<typename G>
int TreeNode<G>::insertAsChild (TreeNode *pNewChild) {
    TreeNode *pChild = _child;
    while (pChild)
    {
        pChild->_parent = pNewChild;
        pChild = pChild->_next;
    }
    pNewChild->_child = _child;
    pNewChild->_last = _last;
    _child = pNewChild;
    _last = pNewChild;
    _child->initLevel(level + 1, true);
    return 1;
}

template<typename G>
int TreeNode<G>::attachAsPrevious (TreeNode *pNode) {
    if (!pNode)
        return -1;
    pNode->_next = this;
    pNode->_prev = _prev;
    pNode->_parent = _parent;
    if (_prev)
    {
        _prev->_next = pNode;
    }
    else
    {
        if (_parent)
        {
            _parent->_child = pNode;
        }
    }
    _prev = pNode;
    pNode->initLevel(level, false);
    return 0;
}

template<typename G>
int TreeNode<G>::attachAsNext (TreeNode *pNode) {
    if (!pNode)
        return -1;
    pNode->_next = _next;
    pNode->_prev = this;
    pNode->_parent = _parent;
    if (_next)
    {
        _next->_prev = pNode;
    }
    else
    {
        if (_parent)
        {
            _parent->_last = pNode;
        }
    }
    _next = pNode;
    pNode->initLevel (level, false);
    return 0;
}
