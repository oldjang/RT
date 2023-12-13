/*
 * Rtree.h
 *
 *  Created on: Sep 17, 2014
 *      Author: jhgan
 */

#ifndef RTREE_H_
#define RTREE_H_

#include "RtreeNode.h"
#include "Zorder.h"
#include "../QueryBase.h"
#include "../MyVector/MyVector.h"



#include <algorithm>
#include <queue>
#include <set>
#include <map>
#include <iostream>
using namespace std;

// This value indicates the number of child nodes of an internal node
// is in the range of [fanout/FanoutRatio, fanout].
const int FanoutRatio = 4;

/*
 *  The class for R-tree.
 */
class Rtree {
protected:
	/*
	 *  The root of the R-tree.
	 */
	RtreeNode* root;

	/*
	 *  The fanout of the nodes.
	 */
	int fanout;

	/*
	 *  The number of alive queries.
	 */
	int aliveNum;

	/*
	 *
	 */
	map<int, QueryBase*> id_pointer_map;

public:
//	Rtree();

	Rtree(int fanout = 12);

	Rtree(vector<QueryBase*>& queryList, int fanout = 12);

	~Rtree();

	/*
	 *  Bulk load construction for R-tree.
	 *  Parameter List:
	 *		queryList:		the list of the given queries.
	 *		fanout:			the number of child nodes of each internal node
	 *  	bitNum:			the number of bits to encode each coordinate value
	 */
	void bulkLoadRtree(vector<QueryBase*>& queryList, int fanout, int bitNum =
			WORD);

	/*
	 *  Construct the R-tree for the given query list.
	 *  Parameter List:
	 *		queryList:		the list of the given queries.
	 *		fanout:			the number of child nodes of each internal node
	 *  	bitNum:			the number of bits to encode each coordinate value
	 *  Return:
	 *  	the root of the resulted tree.
	 */
	RtreeNode* ConstructRtree(vector<QueryBase*>& queryList, int fanout,
			int bitNum = WORD);

//	/*
//	 *  Range query for a given query point with given radius.
//	 *  Parameter List:
//	 *  	pt:				the given query point
//	 *  	r:				the given radius
//	 *  	targetPlace:	the target place to store the points within the query range.
//	 */
//	void rangeQuery(Point* pt, double r, vector<Point*>& targetPlace);

	/*
	 *  Insert the target tree node to the target level of this tree.
	 */
	void InsertNode(RtreeNode* toInsNode, int targetLevel);

	/*
	 *  Insert the target tree node to the subtree of subroot at target level.
	 *  Parameter List:
	 *  	subroot:		the root of the target subtree.
	 *  	toInsNode:		the given tree node that to be inserted.
	 *  	targetLevel:	the target level where the given node should be inserted. The leaf level is level - 0.
	 */
	void InsertNode_SubRoutine(RtreeNode* subroot, RtreeNode* toInsNode,
			int targetLevel);

	/*
	 *  Choose the best child node to process the given target node.
	 *  Parameter List:
	 *  	parentNode:		the current tree node.
	 *  	targetNode: 	the given target node.
	 *  Return:
	 *		the best child node of parentNode.
	 */
	RtreeNode* ChooseBestChildNode(RtreeNode* parentNode,
			RtreeNode* targetNode);

	/*
	 *  Handle the given overflowing tree node.
	 *  Parameter List:
	 *  	overflowNode:	the given overflowing tree node.
	 */
	void OverflowHandler(RtreeNode* overflowNode);

	/*
	 *  Insert the query to this tree.
	 *  Parameter List:
	 *  	query:		the query to be inserted.
	 */
	void InsertQuery(QueryBase* query);

	/*
	 *  Delete the query from this tree.
	 *  Parameter List:
	 *  	query:		the query to be deleted.
	 */
	void DeleteQuery(QueryBase* query);


	void DeleteQuery(int delID);

	/*
	 *  Delete the leaf corresponding to the given leaf node to be deleted.
	 *  Parameter List:
	 *  	toDelLeaf:	the leaf node to be deleted.
	 */
	void DeleteLeaf(RtreeNode* toDelLeaf);

	/*
	 *  Find the given node to be deleted in this tree.
	 *  Parameter List:
	 *  	toDelLeaf:	the given leaf node.
	 *  Return:
	 *  	the leaf node in this tree corresponding to the given node.
	 *  	If there does not exist such a leaf node, the returned result is NULL.
	 */
	RtreeNode* FindLeaf(RtreeNode* toDelLeaf);

	/*
	 *  Find the leaf node in the subtree rooted at the given subroot.
	 *  Parameter List:
	 *  	subroot:		the given subroot.
	 *  	toDelLeaf:		the given node to be deleted.
	 *  Return:
	 *  	the leaf node in the subtree rooted at subroot corresponding to the given node.
	 *  	If there does not exist such a leaf node, the returned result is NULL.
	 */
	RtreeNode* FindLeaf_SubRoutine(RtreeNode* subroot, RtreeNode* toDelLeaf);

	/*
	 *  Handle the given underflowing tree node.
	 *  Parameter List:
	 *  	underflowNode:	the given underflowing tree node.
	 */
	void UnderflowHandler(RtreeNode* underflowNode);

	/*
	 *  Insert the given point to all the queries whose ranges are stabbed by this point.
	 *  Parameter List:
	 *  	coords:			the coordinates of the given point.
	 *  	weight:			the weight of the given point.
	 *  	alertIDList:	the target place to store the ID's of alerted queries due to this insertion.
	 */
	void InsertPoint(vector<int>& coords, int weight, MyVector<int>& alertIDList);

	/*
	 *  Delete the given point to all the queries whose ranges are stabbed by this point.
	 *  Parameter List:
	 *  	coords:			the coordinates of the given point.
	 *  	weight:			the weight of the given point.
	 *  	alertIDList:	the target place to store the ID's of alerted queries due to this insertion.
	 */
	void DeletePoint(vector<int>& coords, int weight, MyVector<int>& alertIDList);

	/*
	 *  Show the whole R-tree.
	 */
	void showRtree();

	/*
	 *  Check whether this R-tree is valid. If YES, return TRUE. Otherwise, return FALSE.
	 */
	bool isValid();

	/*
	 *  Release the space of R-tree without releasing the queries.
	 */
	void ReleaseSpace();

	/*
	 *  Release the space of R-tree including the queries.
	 */
	void ReleaseSpace_All();

	/*
	 *  Return the number of alive queries.
	 */
	int GetAliveNum();

	/*
	 *  Compute the space consumption in terms of bytes of this structure.
	 */
	long long ComputeSpaceConsumption();
};

#endif /* RTREE_H_ */
