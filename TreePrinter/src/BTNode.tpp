/*
 * BTNode.cpp
 *
 *  Created on: 5 sept. 2019
 *      Author: asens
 */

#include "BTNode.h"
#include <climits>

#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))


template <class T>
typename BTNode<T>::childrenGetterFcn BTNode<T>::childrenGetter = nullptr;

template <class T>
typename BTNode<T>::dataGetterFcn     BTNode<T>::dataGetter = nullptr;

/** **************************************************
 * ***************** Static Adaptor ******************
 */ /*************************************************/
template <class T>
void BTNode<T>::initializeClass(childrenGetterFcn f1, dataGetterFcn f2)
{
	childrenGetter = f1;
	dataGetter     = f2;
}

template <class T>
const std::list<T *> & BTNode<T>::getChildren() {
	return CALL_MEMBER_FN(*_nd,childrenGetter)();
}

template <class T>
const std::string & BTNode<T>::getData() {
	if (!_fake)
		return CALL_MEMBER_FN(*_nd,dataGetter)();
	else
		return std::move(std::string(_wself, ' '));
}


/** **************************************************
 * ****************** Printer class ******************
 */ /*************************************************/

/**
 * Fake / Filler Node Constructor
 */
template <class T>
BTNode<T>::BTNode(lluint len, lluint depth, lluint height) {
	_nd = nullptr;
	_depth = depth;
	_height = height;

	_fcp = 0;
	_lcp = 0;
	_mcp = 0;

	_fbp = 0;
	_lbp = 0;
	_mbp = 0;

	_wself  = len;
	_wchild = 0;
	_wblock = 0;

	_fake = true;
}

/**
 * Node Constructor
 */
template <class T>
BTNode<T>::BTNode(T* node) {
	_nd = node;
	_depth = 0;
	_height = 0;

	_fcp = 0;
	_lcp = 0;
	_mcp = 0;

	_fbp = 0;
	_lbp = 0;
	_mbp = 0;

	_wself  = getData().length();
	_wchild = 0;
	_wblock = 0;

	_fake = false;

	// Create new list of nodes, imitating the structure of the passed node
	const std::list<T *> & ls = getChildren();
	for (auto * child: ls)
	{
		_children.push_back(new BTNode(child));
	}

}

/**
 * Destructor
 */
template <class T>
BTNode<T>::~BTNode() {
}

/**
 * Calculate Depth of Nodes (recursive). Assign the depth initialDepth to the node and
 * assignes a depth of initialDepth+1 to each one of the children.
 * Ideally, the head node hast depth of 0, and each row of children has increasing depth
 */
template <class T>
void BTNode<T>::calcDepth(lluint initialDepth)
{
	_depth = initialDepth;
	for (auto child : _children)
	{
		child->calcDepth(initialDepth+1);
	}
}

/**
 * Calculate Height of Node (Recursive). Tells each node the maximum amount of layers of children
 * below itself. (Eventually we only care about the head node).
 */
template <class T>
lluint BTNode<T>::calcHeight()
{
	_height = 0;
	for (auto child : _children)
	{
		_height = std::max(_height, child->calcHeight());
	}
	return _height + 1;
}

/**
 * Add 1 children to nodes without children until all branches reach the bottom of the tree.
 * Eventually this helps calculating spaces for the lowest row.
 */
template <class T>
void BTNode<T>::fakeFullTree(lluint desiredDepth)
{
	if (_depth == desiredDepth)
	{
		return;
	}
	else // (_depth <= desiredDepth)
	{
		if (_children.empty())
		{
			_children.push_back(new BTNode(_wself, _depth+1, _height-1));
			_children.back()->fakeFullTree(desiredDepth);
		}
		else
		{
			for (BTNode * nd : _children)
			{
				nd->fakeFullTree(desiredDepth);
			}
		}
	}
}

/**
 * Calculates the max width of the node and its children. It takes into account wether the combination
 * of children is bigger or smaller than the parent
 */
template <class T>
lluint BTNode<T>::calcWidth()
{
	if (_children.empty()) // If there are no children
	{
		_wchild = 0; // Width of the children is 0
	}
	else
	{
		_wchild += _children.front()->calcWidth();
		
		auto it = ++_children.begin();
		for (; it != _children.end(); ++it)
		{
			_wchild += 1;
			_wchild += (*it)->calcWidth();
		}
	}
	_wblock = std::max(_wself, _wchild);
	return _wblock;
}

/**
 * Main Algorightm Function. Read comments for detail.
 */
template <class T>
void BTNode<T>::printTree() {
	// --- Preparation for the algorithm ---
	
	// Characterize main node
	calcDepth(0);
	calcHeight(); // NOTE: the height of a node is how many nodes down is the farthest node
	fakeFullTree(_height);
	calcWidth();
	
	// Create a vector with the layers of the tree (layer 0 has the head, layer 1 has its children, ...)
	std::vector<std::list<BTNode *> *> levels;
	for (lluint i = 0; i <= _height; i++)
	{
		levels.push_back(new std::list<BTNode *>());
	}
	groupNodesByDepth(levels, this);
	
	// Load the information of where each node goes into the nodes
	assignPositionsToNodes(levels);
	
	// --- Printing algorithm ---
	
	// Print line of nodes (head and some '_')
	printLine(0, levels);
	
	// Iteratively print an intermediate line (only '/', '\' and '|') followed by the next line of nodes
	for (lluint i = 1; i <= _height; i++)
	{
		printPreline(i, levels);
		printLine(i, levels);
	}
	
	// --- Clean up ---
	
	// Remove layers from the vector
	for (auto ptr : levels)
	{
		delete ptr;
	}
}



/** **************************************************
 * ***** Helper Functions for the Main Algorithm *****
 */ /*************************************************/

/**
 * 
 */
template <class T>
void BTNode<T>::groupNodesByDepth(std::vector<std::list<BTNode *> *> & levels, BTNode * head) {
	std::list<BTNode *> queue;
	queue.push_back(head);
	BTNode * nd;
	
	while (!queue.empty())
	{
		// Traverse
		nd = queue.front();
		queue.pop_front();
		for (BTNode * & child : nd->_children)
		{
			queue.push_back(child);
		}
		// Group by depth
		levels[nd->_depth]->push_back(nd);
	}
}

template <class T>
void BTNode<T>::assignPositionsToNodes(std::vector<std::list<BTNode *> *> & levels)
{
	if (levels.empty()) {return;}
	lluint c = 0;
	for (BTNode * & nd : *(levels.back()))
	{
		nd->_fcp = c;
		nd->_lcp = c + nd->_wself - 1;
		nd->_mcp = (nd->_fcp + nd->_lcp)/2;

		nd->_fbp = nd->_fcp;
		nd->_lbp = nd->_lcp;
		nd->_mbp = (nd->_fbp + nd->_lbp)/2;
		c        = c + nd->_wself + 1;
	}
	auto it = levels.rbegin();
	++it;
	for (; it != levels.rend(); ++it) // For each other layer in reverse order
	{
		for (BTNode * & nd : **it) // And each of its nodes
		{
			nd->_fbp = nd->_children.front()->_fcp;
			nd->_lbp = nd->_children.back( )->_lcp;
			nd->_mbp = (nd->_fbp + nd->_lbp) / 2;

			nd->_mcp = (nd->_children.front()->_mcp + nd->_children.back()->_mcp) / 2;
			nd->_fcp = nd->_mcp - nd->_wself / 2;
			nd->_lcp = nd->_fcp + nd->_wself -1;
		}
	}
}

template <class T>
void BTNode<T>::printLine(lluint ln, std::vector<std::list<BTNode *> *> & levels) {
	for (lluint c = 0; c < _wblock; c++)
	{
		if (CisParentFirstCharacter(c, levels[ln]))
		{
			c += printNodeStartingAt(c, levels[ln]) -1;
		}
		else if (((ln+1) < (levels.size())) && CisBetweenChildrenAndParent(c, levels[ln]))
		{
			printUnderscore();
		}
		else
		{
			printSpace();
		}
	}

	std::cout << std::endl;
}

template <class T>
bool BTNode<T>::CisParentFirstCharacter(lluint c, std::list<BTNode *> * level)
{
	for (BTNode * & nd : *level)
	{
		if (nd->_fcp == c)
			return true;
	}
	return false;
}

template <class T>
lluint BTNode<T>::printNodeStartingAt(lluint c, std::list<BTNode *> * level)
{
	// Find corresponding node
	for (BTNode * & nd : * level)
	{
		if (nd->_fcp == c)
		{
			// Return the number of characters printed
			return nd->printNodeData();
		}
	}
	return 0; // TODO: this should be an error
}

template <class T>
bool BTNode<T>::CisBetweenChildrenAndParent(lluint c, std::list<BTNode *> * parent)
{
	for (BTNode * & nd : *parent)
	{
		if (nd->_children.empty())
		{
			continue;
		}
		lluint minCp = nd->_children.front()->_mcp;
		lluint maxCp = nd->_children.back( )->_mcp;
		if (c > minCp && c < maxCp)
			return true;
	}
	return false;
}

template <class T>
void BTNode<T>::printPreline(lluint ln, std::vector<std::list<BTNode *> *> & levels) {
	for (lluint c = 0; c < _wblock; c++)
	{
		if (CisCenterParent(c, levels[ln]))
		{
			switch (parentPositionRelToChildren(c, levels[ln-1], levels[ln]))
			{
				case -1:
					printLeftChildren();
					break;
				case 0:
					printVerticalChildren();
					break;
				case 1:
					printRightChildren();
					break;
			}
		}
		else
		{
			printSpace();
		}
	}
	std::cout << std::endl;
}

template <class T>
bool BTNode<T>::CisCenterParent(lluint c, std::list<BTNode *> * level)
{
	for (BTNode * & nd : * level)
	{
		if (nd->_mcp == c && !nd->_fake)
			return true;
	}
	return false;
}

template <class T>
int BTNode<T>::parentPositionRelToChildren(lluint c, std::list<BTNode *> * parent, std::list<BTNode *> * children)
{
	BTNode * dad = nullptr;
	BTNode * kid = nullptr;
	for (BTNode * & nd : *parent)
	{
		if ((c >= nd->_fbp) && (c <= nd->_lbp))
		{
			dad = nd;
		}
	}
	for (BTNode * & nd : *children)
	{
		if ((c >= nd->_fbp) && (c <= nd->_lbp))
		{
			kid = nd;
		}
	}
	// relative position of the kid w/r to the dad
	if (kid->_mcp < dad->_mcp)
		return -1;
	else if (kid->_mcp == dad->_mcp)
		return 0;
	else
		return 1;
}


template <class T>
lluint BTNode<T>::printNodeData() {
	std::cout << getData();
	return _wself; // Return the number of characters printed
}

template <class T>
void BTNode<T>::printSpace() {
	std::cout << " ";
}

template <class T>
void BTNode<T>::printUnderscore() {
	std::cout << "_";
}

template <class T>
void BTNode<T>::printLeftChildren() {
	std::cout << "/";
}

template <class T>
void BTNode<T>::printRightChildren() {
	std::cout << "\\";
}

template <class T>
void BTNode<T>::printVerticalChildren() {
	std::cout << "|";
}
