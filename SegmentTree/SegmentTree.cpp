/*
 * SegmentTree.cpp
 *
 *  Created on: 8 Sep, 2015
 *      Author: jhgan
 */

#include "SegmentTree.h"
#include "TreeNode.h"
#include "../Query.h"
#include "SegmentTreeLog.h"

SegmentTree::SegmentTree() {
	// TODO Auto-generated constructor stub
//	this->startIndex = 0;
//	this->length = 0;
	TNUM++;
}

SegmentTree::~SegmentTree() {
	// TODO Auto-generated destructor stub
	TNUM--;
}

/***************************************************************
 *  			Space Efficient version related.
 ***************************************************************/

SegmentTree::SegmentTree(MyVector<int>& queryIndexList, void* segTree_Log,
		int dimLev) {

//	double start = 0;
//	double end = 0;

	SegmentTree_Log* log = (SegmentTree_Log*) segTree_Log;

//	this->startIndex = _treeNodeList.size();
//	this->length = 0;

// Push the start index of the tree node in this tree.
	this->levelStartIndex.push_back(log->_treeNodeList.size());

	this->ConstructSegmentTree_Space(queryIndexList, log->_queryList,
			log->_treeNodeList, dimLev);

	int queryNum = queryIndexList.size();

	if (dimLev < DIM) {
		// Recursively construct segment tree.

		// Insert all the queries to this tree.
		for (int i = 0; i < queryNum; ++i) {

			// Modified on 2015-09-29 4:46PM
			this->InsertQuery_Space(log->_queryList[queryIndexList[i]],
					queryIndexList[i], segTree_Log, dimLev);

		}

		TreeNode& theRoot = this->GetRoot_Space(log->_treeNodeList);
//		int rootIndex = this->length - 1;

		int rootIndex = this->GetRootIndex();
		int rootLev = this->levelStartIndex.size() - 1;

		this->ConstructNextLevSubTree_Space(theRoot, rootIndex, rootLev,
				dimLev + 1, segTree_Log);

	}

//	start = getCurrentTime();
	if (dimLev == 1) {
		// The whole tree has been constructed.

		// The _tempQueryIndexList is now useless.
		log->_tempQueryIndexList.release_space();
		log->_segTreeList.shrink_to_fit();
		log->_treeNodeList.shrink_to_fit();
//		log->_minHeapList.shrink_to_fit();

		for (int i = 0; i < queryNum; ++i) {
//			log->_queryList[queryIndexList[i]].InitializeSites_Space(this,
//					segTree_Log);

			log->_upperElementIndexInHeapList.push_back(queryIndexList[i]);
			Query& query = log->_queryList[queryIndexList[i]];
			query.startIndex = log->_upperElementIndexInHeapList.size() - 1;

			Query::nodeIndexList.clear();
//			Query::nodeIndexList.reserve(query.siteNum);
			this->FindCanonicalNode_Space(query, log, 1, Query::nodeIndexList);

			query.siteNum = Query::nodeIndexList.size();
			query.msgCnt = 0;
			query.ComputeThresholdOnSite_Space();

//			if ((int) Query::nodeIndexList.size() != query.siteNum) {
//				printf("nodeIndexList.size() != siteNum\n");
//			}

			for (int i = 0; i < query.siteNum; i++) {
				TreeNode& node = log->_treeNodeList[Query::nodeIndexList[i]];

				if (node.structureIndex == -1) {

					//			MinHeap minHeap;
					//			this->structureIndex = log->_minHeapList.size();
					//			log->_minHeapList.push_back(minHeap);

					node.structureIndex = log->_minHeapList.size();
					log->_minHeapList.push_back(TreeNode::emptyMinHeap);

					//			// Increase the number of non-empty tree nodes.
					//			NonEmptyNum++;
				}

				MinHeap& emitCntList = log->_minHeapList[node.structureIndex];

				// Add the query info to this node.
				//		HeapElement heapElement(queryIndex);
				//		heapElement.setEmitCnt(query.thresholdOnSite + this->ptCnt);
				////		heapElement.setQueryIndex(queryIndex);

				//		// Note: Here we do not need to maintain the heap because we will initialize the heap later.
				//				emitCntList.Add(heapElement, log->_heapElementList);

				TreeNode::emptyHeapElement.setIndexInQuery(i);
				TreeNode::emptyHeapElement.setQueryStartIndex(query.startIndex);
				TreeNode::emptyHeapElement.setEmitCnt(
						query.thresholdOnSite + node.ptCnt);

				log->_upperElementIndexInHeapList.push_back(
						emitCntList.GetSize());
				// Note: Here we do not need to maintain the heap because we will initialize the heap later.
				emitCntList.Add(TreeNode::emptyHeapElement);
			}
		}

		log->_minHeapList.shrink_to_fit();
		log->_upperElementIndexInHeapList.shrink_to_fit();

//		double start = getCurrentTime();

		// Initialize the heaps of tree nodes.
		this->InitializeTreeNodeHeap_Space(segTree_Log);

//		IniTime_Ours += getCurrentTime() - start;
	}

	TNUM++;
}

void SegmentTree::ConstructSegmentTree_Space(MyVector<int>& queryIndexList,
		MyVector<Query>& _queryList, MyVector<TreeNode>& _treeNodeList,
		int dimLev) {
	MyVector<int> endPtList;
	int queryNum = queryIndexList.size();

	endPtList.reserve(queryNum * 2);

	// Obtain the endpoint list of corresponding dimension.
	for (int i = 0; i < queryNum; ++i) {

		endPtList.push_back(
				_queryList[queryIndexList[i]].range[2 * (dimLev - 1)]);

		// Note that we set the right endpoint 1 larger.
		endPtList.push_back(
				_queryList[queryIndexList[i]].range[2 * (dimLev - 1) + 1] + 1);

	}

	this->ConstructSegmentTree_Space(endPtList, _treeNodeList);
//	endPtList.clear();
}

// Back up before removing the single path.
void SegmentTree::ConstructSegmentTree_Space(MyVector<int>& endPtList,
		MyVector<TreeNode>& _treeNodeList) {
	int ptNum = endPtList.size();

	int levStartIndex = this->levelStartIndex[0];
	int levSize = 0;

// Sort all the endpoints.
	sort(endPtList.get_list(), endPtList.get_list() + ptNum);

	TreeNode node;

	/*
	 *  This part is for the binary search tree.
	 */
	for (int i = 0; i < ptNum - 1; ++i) {
		// Skip all the duplicate values.
		while (i < ptNum - 1 && endPtList[i] == endPtList[i + 1]) {
			++i;
		}
		if (i < ptNum - 1) {
			node.SetEndPoints(endPtList[i]);
			_treeNodeList.push_back(node);

			levSize++;
		}
	}
	// Create a leaf node for the last endpoint.
	node.SetEndPoints(endPtList[ptNum - 1]);
	_treeNodeList.push_back(node);
	// Increase the length of the fraction of the tree node list.
	levSize++;

	int treeNodeNum = 0;
	bool isOdd = false;
	int endIndex = 0;

	while (levSize > 1) {

		treeNodeNum = levSize;

		if (treeNodeNum % 2 != 0) {
			// if the size of the tree node list is not even, then ignore the last node.
			isOdd = true;
			treeNodeNum--;
		} else {
			isOdd = false;
		}

		endIndex = levStartIndex + treeNodeNum - 1;

		for (int i = levStartIndex; i < endIndex; i += 2) {
			node.SetEndPoints(_treeNodeList[i].minEndpt);

			_treeNodeList.push_back(node);

		}
		if (isOdd) {
			// This is only for the case of isOdd == true;
			// Add the last node directly to parentList.

			// Create a node whose range is exactly the same as that of its left child.
			node.SetEndPoints(
					_treeNodeList[levStartIndex + treeNodeNum].minEndpt);

			_treeNodeList.push_back(node);
		}

		levStartIndex += levSize;
		levSize = ((levSize + 1) >> 1);

		// Record the start index of this level.
		this->levelStartIndex.push_back(levStartIndex);
	}

	if (levSize == 0) {
		ShowError("Failure in ConstructSegmentTree_Array: level size = 0!\n");
	}

	this->levelStartIndex.shrink_to_fit();
}

void SegmentTree::ConstructNextLevSubTree_Space(TreeNode& subroot,
		int subrootIndex, int subrootLev, int nextLev, void* segTree_Log) {

	// Construct for subroot.
	subroot.ConstructNextLevelTree_Space(subrootIndex, segTree_Log, nextLev);

	if (subrootLev > 0) {

		SegmentTree_Log* log = (SegmentTree_Log*) segTree_Log;

		// Recursively construct on the child nodes.
		int index = this->GetLeftChildIndex(subrootIndex, subrootLev);

		if (index != -1) {
			this->ConstructNextLevSubTree_Space(log->_treeNodeList[index],
					index, subrootLev - 1, nextLev, segTree_Log);

		}

		index = this->GetRightChildIndex(subrootIndex, subrootLev);
		if (index != -1) {
			this->ConstructNextLevSubTree_Space(log->_treeNodeList[index],
					index, subrootLev - 1, nextLev, segTree_Log);

		}
	}
}

void SegmentTree::InsertQuery_Space(Query& query, int queryIndex,
		void* segTree_Log, int dimLev) {

	int min = query.range[2 * (dimLev - 1)];
	int max = query.range[2 * (dimLev - 1) + 1];

	SegmentTree_Log* log = (SegmentTree_Log*) segTree_Log;

//	int rootIndex = this->length - 1;
	int rootIndex = this->GetRootIndex();
	int rootLev = this->levelStartIndex.size() - 1;
	int rootMax = this->GetRootMaxEndpt(log->_treeNodeList);

	this->InsertQuery_SubRoutine_Space(this->GetRoot_Space(log->_treeNodeList),
			dimLev, rootIndex, rootLev, rootMax, query, queryIndex, segTree_Log,
			min, max);

}

void SegmentTree::InsertQuery_SubRoutine_Space(TreeNode& subroot, int dimLev,
		int subrootIndex, int subrootLev, int maxEndpt, Query& query,
		int queryIndex, void* segTree_Log, int min, int max) {

	if (subroot.CheckCover(min, max, maxEndpt)) {
		// the range of query covers the range of node.
		subroot.InsertQuery_Space(query, queryIndex, subrootIndex, segTree_Log,
				dimLev);

		return;

	} else {

		SegmentTree_Log* log = (SegmentTree_Log*) segTree_Log;

		int leftIndex = this->GetLeftChildIndex(subrootIndex, subrootLev);
		int rightIndex = this->GetRightChildIndex(subrootIndex, subrootLev);

		int leftMax = this->GetLeftChildMaxEndpt(leftIndex, rightIndex,
				maxEndpt, log->_treeNodeList);

		// The max endpoint of the right child node is the same as its parent.
		int rightMax = maxEndpt;

		if (leftIndex != -1) {
			TreeNode& node = log->_treeNodeList[leftIndex];
			if (node.CheckIntersect(min, max, leftMax)
					|| node.CheckCover(min, max, leftMax)) {
				// the range of query intersects with the range of left.
				if (subrootLev > 0) {
					this->InsertQuery_SubRoutine_Space(node, dimLev, leftIndex,
							subrootLev - 1, leftMax, query, queryIndex,
							segTree_Log, min, max);
				}
			}
		}

		if (rightIndex != -1) {
			TreeNode& node = log->_treeNodeList[rightIndex];
			if (node.CheckIntersect(min, max, rightMax)
					|| node.CheckCover(min, max, rightMax)) {
				// the range of query intersects with the range of left.
				if (subrootLev > 0) {
					this->InsertQuery_SubRoutine_Space(node, dimLev, rightIndex,
							subrootLev - 1, rightMax, query, queryIndex,
							segTree_Log, min, max);
				}
			}
		}
	}
}

void SegmentTree::FindCanonicalNode_Space(Query& query, void* segTree_Log,
		int dimLev, MyVector<int>& nodeIndexList) {

	int min = query.range[2 * (dimLev - 1)];
	int max = query.range[2 * (dimLev - 1) + 1];

	SegmentTree_Log* log = (SegmentTree_Log*) segTree_Log;

//	int rootIndex = this->length - 1;
	int rootIndex = this->GetRootIndex();
	int rootLev = this->levelStartIndex.size() - 1;
	int rootMax = this->GetRootMaxEndpt(log->_treeNodeList);

	this->FindCanonicalNode_SubRoutine_Space(
			this->GetRoot_Space(log->_treeNodeList), dimLev, rootIndex, rootLev,
			rootMax, query, segTree_Log, min, max, nodeIndexList);

}

void SegmentTree::FindCanonicalNode_SubRoutine_Space(TreeNode& subroot,
		int dimLev, int subrootIndex, int subrootLev, int maxEndpt,
		Query& query, void* segTree_Log, int min, int max,
		MyVector<int>& nodeIndexList) {
	if (subroot.CheckCover(min, max, maxEndpt)) {
		// the range of query covers the range of node.
		subroot.FindCanonicalNode_Space(query, subrootIndex, segTree_Log,
				dimLev, nodeIndexList);
		return;

	} else {

		int leftIndex = this->GetLeftChildIndex(subrootIndex, subrootLev);
		int rightIndex = this->GetRightChildIndex(subrootIndex, subrootLev);

		SegmentTree_Log* log = (SegmentTree_Log*) segTree_Log;

		int leftMax = this->GetLeftChildMaxEndpt(leftIndex, rightIndex,
				maxEndpt, log->_treeNodeList);

		// The max endpoint of the right child node is the same as its parent.
		int rightMax = maxEndpt;

		if (leftIndex != -1) {
			TreeNode& node = log->_treeNodeList[leftIndex];
			if (node.CheckIntersect(min, max, leftMax)
					|| node.CheckCover(min, max, leftMax)) {
				// the range of query intersects with the range of left.
				if (subrootLev > 0) {
					this->FindCanonicalNode_SubRoutine_Space(node, dimLev,
							leftIndex, subrootLev - 1, leftMax, query,
							segTree_Log, min, max, nodeIndexList);
				}
			}
		}

		if (rightIndex != -1) {
			TreeNode& node = log->_treeNodeList[rightIndex];
			if (node.CheckIntersect(min, max, rightMax)
					|| node.CheckCover(min, max, rightMax)) {
				// the range of query intersects with the range of left.
				if (subrootLev > 0) {
					this->FindCanonicalNode_SubRoutine_Space(node, dimLev,
							rightIndex, subrootLev - 1, rightMax, query,
							segTree_Log, min, max, nodeIndexList);
				}
			}
		}
	}

}

void SegmentTree::DeleteQuery_Space(Query& queryToDel, void* segTree_Log) {
	queryToDel.ReleaseSpace_Space(segTree_Log);
	queryToDel.threshold = -1;
}

void SegmentTree::InitializeTreeNodeHeap_Space(void* segTree_Log) {
	this->InitializeTreeNodeHeap_SubRoutine_Space(this, 1, segTree_Log);
}

void SegmentTree::InitializeTreeNodeHeap_SubRoutine_Space(SegmentTree* subtree,
		int dimLev, void* segTree_Log) {

	if (subtree != NULL) {

		SegmentTree_Log* log = (SegmentTree_Log*) segTree_Log;

		// The root is the last node in the fraction of tree node array.
		int endIndex = subtree->GetRootIndex() + 1;
		int startIndex = subtree->levelStartIndex[0];

		if (dimLev == DIM) {

			for (int i = startIndex; i < endIndex; ++i) {
				//				subtree->wholeTree[i].InitializeHeap();
				log->_treeNodeList[i].InitializeHeap(log->_minHeapList,
						log->_queryList, log->_upperElementIndexInHeapList);
			}
		} else {
			SegmentTree* segTree = NULL;

			for (int i = startIndex; i < endIndex; ++i) {
				if (log->_treeNodeList[i].structureIndex != -1) {
					// This tree node has a next level tree.

					segTree =
							&(log->_segTreeList[log->_treeNodeList[i].structureIndex]);

					this->InitializeTreeNodeHeap_SubRoutine_Space(segTree,
							dimLev + 1, segTree_Log);
				}
			}
		}

	}
}

int SegmentTree::InsertPoint_Space(int* coords, int weight, int dimLev,
		MyVector<int>& alertIDList, void* segTree_Log) {

	int alertCnt = 0;

	SegmentTree_Log* log = (SegmentTree_Log*) segTree_Log;

	TreeNode* node = &(this->GetRoot_Space(log->_treeNodeList));

	if (node != NULL) {
		// The coordinate value of the given point in the $dimLev$-th dimension.
		int value = coords[dimLev - 1];

//		int index = this->length - 1;
		int index = GetRootIndex();
		int lev = this->levelStartIndex.size() - 1;
		int maxEndpt = this->GetRootMaxEndpt(log->_treeNodeList);
		int leftIndex = -1;
		int rightIndex = -1;
		int leftMax = -1;

		if (index != -1 && node->CheckStab(value, maxEndpt)) {
			while (index != -1) {

				TreeNode& curNode = log->_treeNodeList[index];

				// Insert the point to node.
				alertCnt += curNode.InsertPoint_Space(coords, dimLev, weight,
						alertIDList, segTree_Log);

				leftIndex = this->GetLeftChildIndex(index, lev);
				rightIndex = this->GetRightChildIndex(index, lev);

				leftMax = this->GetLeftChildMaxEndpt(leftIndex, rightIndex,
						maxEndpt, log->_treeNodeList);

				if (leftIndex != -1
						&& log->_treeNodeList[leftIndex].CheckStab(value,
								leftMax)) {
					index = leftIndex;
					maxEndpt = leftMax;
				} else {
					// If node->left == NULL, then node->right == NULL as well.
					// If node->left is not stabbed by value, since value stabs node, value must stab node->right.
					index = this->GetRightChildIndex(index, lev);
				}

				lev--;
				if (lev < 0) {
					break;
				}
			}
		}
	}
	return alertCnt;
}

void SegmentTree::ReleaseSpace_Space(void* segTree_Log, int dimLev) {

	// The root is the last node in the fractio of tree node array.
	int endIndex = GetRootIndex() + 1;

	SegmentTree_Log* log = (SegmentTree_Log*) segTree_Log;

	for (int i = this->levelStartIndex[0]; i < endIndex; ++i) {
		log->_treeNodeList[i].ReleaseSpace_Space(segTree_Log, dimLev);
	}

	levelStartIndex.release_space();
}

SegmentTree* SegmentTree::ConstructStr_Space(MyVector<int>& queryIndexList,
		void* segTree_Log) {
	SegmentTree* rtn = new SegmentTree(queryIndexList, segTree_Log, 1);
	return rtn;
}

long long SegmentTree::ComputeSpaceConsumption_Space(void* segTree_Log,
		int dimLev) {
	long long rtn = 0;
	rtn += sizeof(SegmentTree);
//	TestTNUM++;
	rtn += sizeof(int) * this->levelStartIndex.capacity();

	SegmentTree_Log* log = (SegmentTree_Log*) segTree_Log;

	int endIndex = GetRootIndex() + 1;
	for (int i = this->levelStartIndex[0]; i < endIndex; ++i) {
		rtn += log->_treeNodeList[i].CompuateSpaceConsumption_Space(segTree_Log,
				dimLev);
	}

	return rtn;
}
