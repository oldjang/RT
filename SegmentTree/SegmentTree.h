/*
 * SegmentTree.h
 *
 *  Created on: 8 Sep, 2015
 *      Author: jhgan
 */

#ifndef SEGMENTTREE_H_
#define SEGMENTTREE_H_

#include "../headers.h"
#include "TreeNode.h"
#include "../Query.h"
#include "../Logarithmic/AbsStructure.h"

#include <vector>
#include <algorithm>
using namespace std;

/*
 *  The class of segment tree of query endpoints.
 */
class SegmentTree {
public:
	SegmentTree();

	/*
	 * 	The constructor of SegmentTree.
	 * 	Parameter List:
	 * 		queryList:		the list of pointers of queries.
	 * 		dimLev:			the level of dimensionality.
	 */
	SegmentTree(MyVector<Query*>& queryList, int dimLev = 1);

	~SegmentTree();

	/*
	 *  The list of start index of each level in the tree array.
	 *  Leaf level is level-0.
	 */
	MyVector<int> levelStartIndex;

	/***************************************************************
	 *  			Space efficient version related.
	 ***************************************************************/

	/*
	 *  Constructor.
	 *  Parameter List:
	 *  	queryIndexList:		the list of index of the queries in _queryList.
	 *  	_queryList:			the list of all the queries.
	 *  	_treeNodeList:		the list of all the tree nodes.
	 *  	_heapElementList:	the list of all the heap elements.
	 *  	dimLev:				the dimensionality level.
	 */
	SegmentTree(MyVector<int>& queryIndexList, void* segTree_Log,
			int dimLev = 1);

	/*
	 *  Construct a segment tree by bulk loading the given endpoint list.
	 *  Parameter List:
	 *  	endPtList:		the list of all the endpoints.
	 */
	void ConstructSegmentTree_Space(MyVector<int>& endPtList,
			MyVector<TreeNode>& _treeNodeList);

	/*
	 *  Construct a segment tree by bulk loading the given query list.
	 *  Parameter List:
	 *  	queryIndexList:	the list of index of queries in _queryList.
	 *  	queryList:		the given query list.
	 *  	dimLev:			the level of dimensionality.
	 */
	void ConstructSegmentTree_Space(MyVector<int>& queryIndexList,
			MyVector<Query>& _queryList, MyVector<TreeNode>& _treeNodeList,
			int dimLev);

	/* Construct a next level segment tree for the each node in the subtree rooted at subroot.
	 * Parameter List:
	 * 		subroot: 			the given subtree root.
	 * 		subrootIndex: 		the index of the subroot in the array of all tree nodes.
	 * 		subrootLev: 		the level of the subroot.
	 * 		nextLev: 			the next level of dimensionality.
	 */
	void ConstructNextLevSubTree_Space(TreeNode& subroot, int subrootIndex,
			int subrootLev, int nextLev, void* segTree_Log);

	/*
	 * 	Insert a query to this segment tree.
	 * 	Parameter List:
	 *  	query:					the reference of the given query.
	 *  	queryIndex:				the index of the given query in the list of all queries.
	 *  	_treeNodeList:			the list of all tree nodes.
	 *  	_heapElementList:		the list of all heap elements.
	 *  	_tempQueryIndexList:	the list of temple query index.
	 *  	_minHeapList:			the list of all min heap objects.
	 *  	dimLev:					the dimensionality level
	 */
	void InsertQuery_Space(Query& query, int queryIndex, void* segTree_Log,
			int dimLev);

	/*
	 *  Insert the query to subtree.
	 *  Parameter List:
	 *  	subroot:			the reference of the subtree root.
	 *  	dimLev:				the dimensionality level of the current subtree.
	 *  	subrootIndex:		the index of this subroot in the array of all tree nodes.
	 *  	subrootLev:			the level of this subroot.
	 *  	maxEndpt:			the max endpoint of the range of subroot.
	 *  	query:				the reference of the given query.
	 *  	queryIndex:			the index of the given query in the list of all queries.
	 *  	_treeNodeList:		the list of all tree nodes.
	 *  	_heapElementList:	the list of all heap elements.
	 *  	min:				the min of the range of the specified dimension of the query.
	 *  	max:				the max of the range of the specified dimension of the query.
	 */
	void InsertQuery_SubRoutine_Space(TreeNode& subroot, int dimLev,
			int subrootIndex, int subrootLev, int maxEndpt, Query& query,
			int queryIndex, void* segTree_Log, int min, int max);

	/*
	 *  Find the canonical nodes for the given query.
	 * 	Parameter List:
	 *  	query:					the reference of the given query.
	 *  	_treeNodeList:			the list of all tree nodes.
	 *  	_segTreeList:			the pointer of the list of all segment trees.
	 *  	dimLev:					the dimensionality level
	 *  	nodeIndexList:			the target place to store the index of canonical nodes.
	 */
	void FindCanonicalNode_Space(Query& query, void* segTree_Log, int dimLev,
			MyVector<int>& nodeIndexList);

	/*
	 *  Find the canonical nodes for the given query in the subtree.
	 *  Parameter List:
	 *  	subroot:			the reference of the subtree root.
	 *  	dimLev:				the dimensionality level of the current subtree.
	 *  	subrootIndex:		the index of this subroot in the array of all tree nodes.
	 *  	subrootLev:			the level of this subroot.
	 *  	maxEndpt:			the max endpoint of the range of subroot.
	 *  	query:				the reference of the given query.
	 *  	_treeNodeList:		the list of all tree nodes.
	 *  	_segTreeList:		the pointer of the list of all segment trees.
	 *  	min:				the min of the range of the specified dimension of the query.
	 *  	max:				the max of the range of the specified dimension of the query.
	 *  	nodeIndexList:		the target place to store the index of canonical nodes.
	 */
	void FindCanonicalNode_SubRoutine_Space(TreeNode& subroot, int dimLev,
			int subrootIndex, int subrootLev, int maxEndpt, Query& query,
			void* segTree_Log, int min, int max, MyVector<int>& nodeIndexList);

	/*
	 *  Delete the given query from this tree.
	 */
	void DeleteQuery_Space(Query& queryToDel, void* segTree_Log);

	/*
	 *  Initialize the heaps of the tree nodes at the last level.
	 */
	void InitializeTreeNodeHeap_Space(void* segTree_Log);

	/*
	 *  The sub-routine for initializing the heaps of the tree nodes.
	 *  Parameter List:
	 *  	subtree:		the pointer of the next level subtree.
	 *  	dimLev:			the dimensionality level of the specified subtree.
	 */
	void InitializeTreeNodeHeap_SubRoutine_Space(SegmentTree* subtree,
			int dimLev, void* segTree_Log);

	/*
	 *  Insert the given point to this tree.
	 *  Parameter List:
	 *  	coords:		the coordinates of the given point.
	 *  	weight:		the weight of the given point.
	 *  	dimLev:		the dimensionality level of the current tree.
	 */
	int InsertPoint_Space(int* coords, int weight, int dimLev,
			MyVector<int>& alertIDList, void* segTree_Log);

	/*
	 *  Release the space of this segment tree.
	 */
	void ReleaseSpace_Space(void* segTree_Log, int dimLev);

	/*
	 *  Construct a new segment tree.
	 */
	SegmentTree* ConstructStr_Space(MyVector<int>& queryIndexList,
			void* segTree_Log);

	/*
	 *  Compute the space consumption of the structures in this tree.
	 */
	long long ComputeSpaceConsumption_Space(void* segTree_Log, int dimLev);

	/*
	 *  Get the root of this tree.
	 */
	TreeNode& GetRoot_Space(MyVector<TreeNode>& _treeNodeList) {
		int rootIndex = this->GetRootIndex();
		if (rootIndex == -1) {
			ShowError(
					"Error in SegmentTree::GetRoot_Space: root index is -1!\n");
		}
		return _treeNodeList[rootIndex];
	}

	/*
	 *  Return the index of the root.
	 */
	int GetRootIndex() {
		int lastIndex = levelStartIndex.size() - 1;

		if (lastIndex < 0) {
			return -1;
		}

		return this->levelStartIndex[lastIndex];
	}

	/*
	 *  Return the max endpoint of the range of the root.
	 */
	int GetRootMaxEndpt(MyVector<TreeNode>& _treeNodeList) {
		TreeNode& root = this->GetRoot_Space(_treeNodeList);
		int rootLev = this->levelStartIndex.size() - 1;
		int rootMax = -1;
		if (rootLev == 0) {
			// In this case, the root is the only one node in the tree.
			rootMax = root.minEndpt;
		} else {
			// This tree has at least two level.
			int lastLeafIndex = this->levelStartIndex[1] - 1;
			// The range of the last leaf node is [minEndpt, minEndpt].
			rootMax = _treeNodeList[lastLeafIndex].minEndpt;
		}

		return rootMax;
	}

	/*
	 *  Compute the index of the left child of the tree node at the given index.
	 *  Parameter List:
	 *  	index:		the index of the specified tree node.
	 *  	lev:		the level of the specified tree node.
	 *  Return:
	 *  	the index of the left child.
	 */
	int GetLeftChildIndex(int index, int lev) {
		int rtn = -1;

		if (lev == 0) {
			return -1;
		}
		rtn = this->levelStartIndex[lev - 1]
				+ 2 * (index - this->levelStartIndex[lev]);
		return rtn;
	}

	/*
	 *  Compute the max endpoint of the left child node.
	 *  Parameter List:
	 *  	leftIndex:		the index of the left child node.
	 *  	rightIndex:		the index of the right child node.
	 *  	parentMax:		the max endpoint of the parent node.
	 *  Return:
	 *  	the value of the max endpoint of the left child node.
	 */
	int GetLeftChildMaxEndpt(int leftIndex, int rightIndex, int parentMax,
			MyVector<TreeNode>& _treeNodeList) {
		int leftMax = -1;

		if (rightIndex == -1) {
			// subroot has only one child node and this child node must have the same range as its.
			leftMax = parentMax;
		} else {
			// subroot has right child node.
			// The max endpoint of the left child node is one less than the min endpoint of the right child node.
			leftMax = _treeNodeList[rightIndex].minEndpt - 1;
		}

		return leftMax;
	}

	/*
	 *  Compute the index of the right child of the tree node at the given index.
	 *  Parameter List:
	 *  	index:		the index of the specified tree node.
	 *  	lev:		the level of the specified tree node.
	 *  Return:
	 *  	the index of the right child.
	 */
	int GetRightChildIndex(int index, int lev) {
		int rtn = -1;
		if (lev == 0) {
			return -1;
		}

		rtn = this->levelStartIndex[lev - 1]
				+ 2 * (index - this->levelStartIndex[lev]) + 1;

		if (rtn >= this->levelStartIndex[lev]) {
			// In this case, the right child does not exist.
			rtn = -1;
		}
		return rtn;
	}

	/***************************************************************/
};

#endif /* SEGMENTTREE_H_ */
