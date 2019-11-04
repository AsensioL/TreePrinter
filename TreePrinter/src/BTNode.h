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

typedef long long unsigned int lluint;

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
	lluint _depth;

	lluint _wself;
	lluint _wchild;
	lluint _wblock;

	lluint _fcp; // First  Character Position
	lluint _lcp; // Last   Character Position
	lluint _mcp; // Middle Character Position
	lluint _fbp; // First  Block     Position
	lluint _lbp; // Last   Block     Position

	void calcDepth(lluint initialDepth);
	lluint calcWidth();

	void groupNodesByDepth(     std::vector<std::list<BTNode *> *> & levels, BTNode * head);
	lluint assignPositionsToNodes(lluint blockStart);
	
	void printLine(lluint ln,    std::vector<std::list<BTNode *> *> & levels);
	void printPreline(lluint ln, std::vector<std::list<BTNode *> *> & levels);
	
	lluint printNodeStartingAt(lluint c, std::list<BTNode *> * level);
	lluint printNodeData();
	
	bool CisParentFirstCharacter(lluint c, std::list<BTNode *> * level);
	bool CisBetweenChildrenAndParent(lluint c, std::list<BTNode *> * parent);
	bool CisCenterChildren(lluint c, std::list<BTNode *> * children);
	int parentPositionRelToChildren(lluint c, std::list<BTNode *> * parent, std::list<BTNode *> * children);
	
	
	void printSpace();
	void printUnderscore();
	void printLeftChildren();
	void printRightChildren();
	void printVerticalChildren();

};

#include "BTNode.tpp"
#endif /* SRC_BTNODE_H_ */
