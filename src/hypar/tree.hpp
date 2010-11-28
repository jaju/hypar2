/*
 * tree.hpp
 *
 *  Created on: Nov 28, 2010
 *      Author: jaju
 */

#pragma once
#include <cassert>

class TreeNode {
protected:
	TreeNode *m_pParent, *m_pPrev, *m_pNext, *m_pChild, *m_pLast;
	int m_iLevel, m_iHorLevel;

public:
	TreeNode() :
		m_pParent (0),
		m_pPrev (0),
		m_pNext (0),
		m_pChild (0),
		m_pLast (0),
		m_iLevel (0),
		m_iHorLevel (0) {}

	virtual ~TreeNode() {
		TreeNode *pTmpNode = m_pChild;

		while (m_pChild)
		{
			pTmpNode = m_pChild;
			m_pChild = m_pChild->m_pNext;
			delete pTmpNode;
		}
	}

public:
	TreeNode *&parent() { return (TreeNode *&) m_pParent; }
	TreeNode *&child() { return (TreeNode *&) m_pChild; }
	TreeNode *&previous() { return (TreeNode *&) m_pPrev; }
	TreeNode *&next() { return (TreeNode *&) m_pNext; }
	TreeNode *&last() { return (TreeNode *&) m_pLast; }
	int &level() { return m_iLevel; }
	int &horizontalLevel() { return m_iHorLevel; }

	virtual int detach ();
	virtual int initLevel (int iLevel = 0, bool bNext = true);
	virtual int attachAsChild (TreeNode *pNode);
	virtual int insertAsParent (TreeNode *pNode);
	virtual int insertAsChild (TreeNode *pNewChild);
	virtual int attachAsPrevious (TreeNode *pNode);
	virtual int attachAsNext (TreeNode *pNode);
};
