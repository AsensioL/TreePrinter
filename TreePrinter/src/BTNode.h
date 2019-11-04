/*
 * BTNode.h
 *
 *  Created on: 5 sept. 2019
 *      Author: asens
 *
 *  Known bugs:
 *    * If the parent node is longer than the combination of the children, it will crash
 *
 *  Minor problems:
 *    * Placement of '/' feels skewed to the left on words with even characters to the left of its parent
 */

#ifndef SRC_BTNODE_H_
#define SRC_BTNODE_H_

#include <string>
#include <list>
#include <vector>
#include <memory>

template <class T>
class BTNode {


	/**
	 * Static Adaptor
	 */
  public:
	  // Create typdefs for the the getter functions for the Adaptor
	typedef const std::list<T*> (T::*childrenGetterFcn)( void);
	typedef const std::string   (T::*dataGetterFcn)    ( void);
	static void initializeClass(childrenGetterFcn f1, dataGetterFcn f2);
  private:
	static childrenGetterFcn childrenGetter;
	static dataGetterFcn	 dataGetter;

	const std::list<T *> getChildren();
	const std::string    getData();


	/**
	 * Printer class
	 */
public:
	BTNode(T* node);

	virtual ~BTNode();

	void printTree();

private:

	T * _nd;
	std::list<BTNode *> _children;
	std::size_t _depth;

	std::size_t _wself;
	std::size_t _wchild;
	std::size_t _wblock;

	std::size_t _fcp; // First  Character Position
	std::size_t _lcp; // Last   Character Position
	std::size_t _mcp; // Middle Character Position
	std::size_t _fbp; // First  Block     Position
	std::size_t _lbp; // Last   Block     Position

	void calcDepth(std::size_t initialDepth);
	std::size_t calcWidth();

	void groupNodesByDepth(std::vector<std::unique_ptr<std::list<BTNode *>>> & levels, BTNode * head);
	std::size_t assignPositionsToNodes(std::size_t blockStart);
	
	void printLine(std::size_t ln,    std::vector<std::unique_ptr<std::list<BTNode *>>> & levels);
	void printPreline(std::size_t ln, std::vector<std::unique_ptr<std::list<BTNode *>>> & levels);
	
	std::size_t printNodeStartingAt(std::size_t c, std::unique_ptr<std::list<BTNode *>> & level);
	std::size_t printNodeData();
	
	bool CisParentFirstCharacter(std::size_t c, std::unique_ptr<std::list<BTNode *>> & level);
	bool CisBetweenChildrenAndParent(std::size_t c, std::unique_ptr<std::list<BTNode *>> & parent);
	bool CisCenterChildren(std::size_t c, std::unique_ptr<std::list<BTNode *>> & children);
	int parentPositionRelToChildren(std::size_t c, std::unique_ptr<std::list<BTNode *>> & parent, std::unique_ptr<std::list<BTNode *>> & children);
	
	
	void printSpace();
	void printUnderscore();
	void printLeftChildren();
	void printRightChildren();
	void printVerticalChildren();

};

#include "BTNode.tpp"
#endif /* SRC_BTNODE_H_ */
