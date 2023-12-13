/*
 * RtreeNode.h
 *
 *  Created on: Sep 17, 2014
 *      Author: jhgan
 */

#ifndef RTREENODE_H_
#define RTREENODE_H_

#include "Rectangle.h"
#include "../QueryBase.h"
#include "../headers.h"
#include "../MyVector/MyVector.h"

#include <algorithm>
using namespace std;

/*
 *  The class for nodes of R-tree.
 */
class RtreeNode {
public:

	/*
	 *  The level of this node in the R-tree. All the leaves are at level-0.
	 */
	int level;

	/*
	 *  The data object of this node if this is a leaf.
	 */
	QueryBase* query;

	/*
	 *  The pointer of the parent tree node of this node.
	 *  If this node is the root, then this pointer is NULL.
	 */
	RtreeNode* parent;

	/*
	 *  The minimum bounding rectangle of the child nodes.
	 */
	Rectangle mbr;

	/*
	 *  The list of child nodes.
	 */
	vector<RtreeNode*> childNodes;

	/*
	 *  A static list that record all the alerted leaf nodes.
	 */
	static vector<RtreeNode*> alertedLeafList;

	/*
	 *  Construct an empty node.
	 */
	RtreeNode();

	/*
	 *  Construct a leaf node by a given query.
	 */
	RtreeNode(QueryBase* query);

	~RtreeNode();

	/*
	 *  If this node overflows, split it into two nodes.
	 *  Return:
	 *  	the newly slipted tree node from this node.
	 */
	RtreeNode* Split();

	/*
	 *  Compute the perimeter of the mbr of the position [start, end - 1] of a given list of tree nodes.
	 *  Parameter List:
	 *  	treeNodeList:		the given list of tree nodes.
	 *  	start:				start index.
	 *  	length:				the length of the faction we consider.
	 *  Return:
	 *  	the resulted perimeter.
	 */
	long long ComputeGroupPerimeter(vector<RtreeNode*>& treeNodeList, int start,
			int length);

	/*
	 *  Check whether this node is a leaf.
	 *  If YES, return TRUE. Otherwise, return FALSE.
	 */
	bool isLeaf();

	/*
	 *  Add a child node to this node.
	 *  Parameter List:
	 *  	child:		the pointer of the given child node.
	 *  Return:
	 *  	the number of child nodes of this node after insertion.
	 */
	int AddChildNode(RtreeNode* child);

	/*
	 *  Delete the given child node from this node.
	 *  Parameter List:
	 *  	child:		the pointer of the given child node.
	 *  Return:
	 *  	the number of child nodes of this node after deletion.
	 */
	int DeleteChildNode(RtreeNode* child);

	/*
	 *  Check whether the mbr of this node is stabbed by the given point.
	 *  Parameter List:
	 *  	coords:		the coordinates of the given point.
	 *  Return:
	 *  	If Yes, return TRUE. Otherwise, return FALSE.
	 */
	bool CheckStab(vector<int>& coords);

	/*
	 *  Insert the given point to the sub-tree rooted at this node.
	 *  Parameter List:
	 *  	coords:			the coordinates of the given point.
	 *  	weight:			the weight of the given point.
	 *  	alertIDList:	the target place to store the alerted queries due to this insertions.
	 */
	void InsertPoint(vector<int>& coords, int weight, MyVector<int>& alertIDList);

	/*
	 *  Show the information of this tree node.
	 */
	void showTreeNode();

	/*
	 *  Release the space of this tree node without releasing the queries.
	 */
	void ReleaseSpace();

	/*
	 *  Release the space of this tree node including the queries.
	 */
	void ReleaseSpace_All();

	/*
	 *  Compute the space consumption in terms of bytes of this structure.
	 */
	long long ComputeSpaceConsumption();

};

#endif /* RTREENODE_H_ */
