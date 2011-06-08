#include <tree.hpp>

int TreeNode::detach ()
{
	if (!m_pParent && !m_pPrev && !m_pNext)
		return -1;
	if (m_pPrev)
	{
		m_pPrev->m_pNext = m_pNext;
	}
	else
	{
		if (m_pParent)
		{
			m_pParent->m_pChild = m_pNext;
		}
	}
	if (m_pNext)
	{
		m_pNext->m_pPrev = m_pPrev;
	}
	else
	{
		if (m_pParent)
		{
			m_pParent->m_pLast = 0;
		}
	}
	m_pParent = 0;
	m_pPrev = 0;
	m_pNext = 0;
	return 0;
}

int TreeNode::initLevel (int iLevel, bool bNext) {
	m_iLevel = iLevel;
	if (m_pChild)
	{
		m_pChild->initLevel (m_iLevel + 1, true);
	}
	if (bNext && m_pNext)
	{
		m_pNext->initLevel (m_iLevel, true);
	}
	return 0;
}

int TreeNode::attachAsChild (TreeNode *pNode) {
	if (!pNode)
		return -1;
	if (!m_pChild)
	{
		m_pChild = m_pLast = pNode;
		pNode->m_pParent = this;
		pNode->m_pPrev = pNode->m_pNext = 0;
		pNode->initLevel (m_iLevel + 1, true);
		return 1;
	}
	else
	{
		int retval = m_pLast->attachAsNext (pNode);
		assert (m_pLast == pNode);
		return retval;
	}
}

int TreeNode::insertAsParent (TreeNode *pNode) {
	if (!pNode)
	{
		return -1;
	}
	if (m_pPrev)
	{
		m_pPrev->m_pNext = pNode;
	}
	if (m_pNext)
	{
		m_pNext->m_pPrev = pNode;
	}
	if (m_pParent)
	{
		if (this == m_pParent->m_pChild)
		{
			m_pParent->m_pChild = pNode;
		}
		if (this == m_pParent->m_pLast)
		{
			m_pParent->m_pLast = pNode;
		}
	}
	m_pPrev = m_pNext = m_pParent = 0;
	return pNode->attachAsChild (this);
}

int TreeNode::insertAsChild (TreeNode *pNewChild) {
	TreeNode *pChild = m_pChild;
	while (pChild)
	{
		pChild->m_pParent = pNewChild;
		pChild = pChild->m_pNext;
	}
	pNewChild->m_pChild = m_pChild;
	pNewChild->m_pLast = m_pLast;
	m_pChild = pNewChild;
	m_pLast = pNewChild;
	m_pChild->initLevel(m_iLevel + 1, true);
	return 1;
}

int TreeNode::attachAsPrevious (TreeNode *pNode) {
	if (!pNode)
		return -1;
	pNode->m_pNext = this;
	pNode->m_pPrev = m_pPrev;
	pNode->m_pParent = m_pParent;
	if (m_pPrev)
	{
		m_pPrev->m_pNext = pNode;
	}
	else
	{
		if (m_pParent)
		{
			m_pParent->m_pChild = pNode;
		}
	}
	m_pPrev = pNode;
	pNode->initLevel(m_iLevel, false);
	return 0;
}

int TreeNode::attachAsNext (TreeNode *pNode) {
	if (!pNode)
		return -1;
	pNode->m_pNext = m_pNext;
	pNode->m_pPrev = this;
	pNode->m_pParent = m_pParent;
	if (m_pNext)
	{
		m_pNext->m_pPrev = pNode;
	}
	else
	{
		if (m_pParent)
		{
			m_pParent->m_pLast = pNode;
		}
	}
	m_pNext = pNode;
	pNode->initLevel (m_iLevel, false);
	return 0;
}
