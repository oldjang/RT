/*
 * TreeNode.h
 *
 *  Created on: 7 Sep, 2015
 *      Author: jhgan
 */

#ifndef TREENODE_H_
#define TREENODE_H_

#include "../headers.h"
#include "../MyHeap/MinHeap.h"
#include "../MyHeap/MaxHeap.h"
//#include "SegmentTree.h"
#include "../MyVector/MyVector.h"
#include "../Query.h"

#include <vector>
//#include <map>
using namespace std;

/*
 *  The class of the internal node of the segment tree of queries.
 */
class TreeNode {
public:
	TreeNode();

	/*
	 *  The constructor of tree node.
	 */
	TreeNode(int minEndpt);

	~TreeNode();

	/*
	 *  The number of points that fall in the range of this node.
	 */
	int ptCnt;

	/*
	 *  The left endpoint of the range corresponds to this node.
	 */
	int minEndpt;

	/*
	 *  The index of the structure in the list of all those structures with the same type.
	 */
	int structureIndex;

	/*
	 *  The following static variables are to avoid repeatedly create objects.
	 */
	static HeapElement emptyHeapElement;
	static MinHeap emptyMinHeap;
	static MyVector<int> emptyQueryIndexList;

	/***************************************************************
	 *  			Tree structure related.
	 ***************************************************************/

	/*
	 *  Construct the next level segment tree on the list of queries covering this node.
	 *  Parameter List:
	 *  	nextLev:		the next level of dimensionality.
	 */
	void ConstructNextLevelTree(int nextLev);

	/*
	 *  Set the end points of the range of this tree node.
	 */
	void SetEndPoints(int min);
	//	void SetEndPoints(int min, int max);

	/*
	 *	Check whether the given range covers the range of this node.
	 *	Parameter List:
	 *		min:		the min endpoint of the given range.
	 *		max:		the max endpoint of the given range.
	 *		maxEndpt:	the max endpoint of the range of this tree node.
	 *	Return:
	 *		If the given range covers the range of this node, then return TRUE. Otherwise, return FALSE.
	 */
//	bool CheckCover(int min, int max, int maxEndpt);
	inline bool CheckCover(int min, int max, int maxEndpt) {
		return (min <= this->minEndpt && max >= maxEndpt);
	}

	/*
	 *  Check whether the given range intersects with the range of this node.
	 *  Parameter List:
	 *  	min:   the min endpoint of the given range.
	 *  	max:   the max endpoint of the given range.
	 *  Return:
	 *  	If the given range intersects with the range of this node, then return TRUE. Otherwise, return FALSE.
	 */
//	bool CheckIntersect(int min, int max, int maxEndpt);
	inline bool CheckIntersect(int min, int max, int maxEndpt) {
		// To check the intersection of the ranges, we need to consider the state of the range.
		return this->CheckStab(min, maxEndpt) || this->CheckStab(max, maxEndpt);
	}

	/*
	 *  Check whether the given value stabs the range of this node.
	 *  Parameter List:
	 *  	value:	the given value.
	 *  Return:
	 *  	If Yes, return TRUE. Otherwise, return FALSE.
	 */
	inline bool CheckStab(int value, int maxEndpt) {
		return (value >= this->minEndpt && value <= maxEndpt);
	}


	/*
	 *  Release the space of this tree node.
	 */
	void ReleaseSpace();

	/*
	 *  Compute the space consumption of the structures under this node.
	 */
	long long CompuateSpaceConsumption();

	/***************************************************************
	 *  			Threshold monitoring related.
	 ***************************************************************/

	/*
	 *  Update the heap element corresponding to the given query.
	 *  Parameter List:
	 *  	heapElement:					the pointer of the target heap element of the corresponding query.
	 *  	thresholdOnSite:				the new value of the threshold which is used to compute next emit count.
	 *  	_minHeapList:					the list of all the min heap objects.
	 *  	_queryList:						the list of all the query objects.
	 *  	_upperElementIndexInHeapList: 	the list of all the index in heap of heap elements.
	 */
	void UpdateHeapElement(int indexInHeap, int thresholdOnSite,
			MyVector<MinHeap>& _minHeapList, MyVector<Query>& _queryList,
			MyVector<int>& _upperElementIndexInHeapList);

	/*
	 *  Initialize the heap in this tree node.
	 */
	void InitializeHeap(MyVector<MinHeap>& _minHeapList,
			MyVector<Query>& _queryList,
			MyVector<int>& _upperElementIndexInHeapList);

	/*
	 *  Check whether to notify the corresponding coordinators.
	 */
	bool CheckEmit(MyVector<MinHeap>& _minHeapList,
			MyVector<HeapElement>& _heapElementList);

	/***************************************************************
	 *  			Space Efficient version related.
	 ***************************************************************/

	/*
	 *  Insert a query associating this node.
	 *  Parameter List:
	 *  	query:				the reference of the given query.
	 *  	queryIndex:			the index of the given query in the list of all queries.
	 *  	nodeIndex:			the index of the current node in the list of all tree nodes.
	 *  	_heapElementList:	the list of all heap elements.
	 *  	dimLev:				the level of the dimensionality of this node.
	 */
	void InsertQuery_Space(Query& query, unsigned int queryIndex,
			unsigned int nodeIndex, void* segTree_Log, int dimLev);

	/*
	 *  Find the canonical nodes for the given query in the structure of this node.
	 *  Parameter List:
	 *  	query:			the reference of the given query.
	 *  	nodeIndex:		the index of the current node in the list of all tree nodes.
	 *  	_treeNodeList:	the list of all tree nodes.
	 *  	_segTreeList:	the list of all segment trees.
	 *  	dimLev:			the dimensionality level of this node.
	 *  	nodeIndexList:	the target place to store the index of canonical nodes.
	 */
	void FindCanonicalNode_Space(Query& query, int nodeIndex, void* segTree_Log,
			int dimLev, MyVector<int>& nodeIndexList);

	/*
	 *  Construct the next level segment tree on the list of queries covering this node.
	 *  Parameter List:
	 *  	nextLev:		the next level of dimensionality.
	 */
	void ConstructNextLevelTree_Space(int subrootIndex, void* segTree_Log,
			int nextLev);

	/*
	 *  Delete a query from this node.
	 *  Parameter List:
	 *  	heapElement:	the reference of the target heap element.
	 */
	void DeleteQuery_Space(int indexInHeap, void* segTree_Log);
//	void DeleteQuery_Space(HeapElement& heapElement, void* segTree_Log);

	/*
	 *  Notify the corresponding coordinators.
	 *  Return:
	 *  	the number of queries alerted in this notification.
	 */
	int NotifyCoordinator_Space(MyVector<int>& alertIDList, void* segTree_Log);

	/*
	 *  Insert a point to this node virtually.
	 *  Parameter List:
	 *  	coords:				the coordinates of the given point.
	 *  	dimLev:				the level of dimensionality of this node.
	 *  	weight:				the weight of the given point.
	 *  Return:
	 *  	the number of queries alerted by inserting this given point.
	 */
	int InsertPoint_Space(int* coords, int dimLev, int weight,
			MyVector<int>& alertIDList, void* segTree_Log);

	/*
	 *  Insert a point to this node virtually.
	 *  Return:
	 *  	the number of queries alerted by inserting this given point.
	 */
	int InsertPoint_Space(int weight, MyVector<int>& alertIDList,
			void* segTree_Log);

	/*
	 *  Release the space.
	 */
	void ReleaseSpace_Space(void* segTree_Log, int dimLev);

	/*
	 *  Compute the space consumption of the structures under this node.
	 */
	long long CompuateSpaceConsumption_Space(void* segTree_Log, int dimLev);
};

#endif /* TREENODE_H_ */
