/*
 * TreeNode.cpp
 *
 *  Created on: 7 Sep, 2015
 *      Author: jhgan
 */

#include "TreeNode.h"
#include "../Query.h"
#include "SegmentTree.h"
#include "SegmentTreeLog.h"

MinHeap TreeNode::emptyMinHeap;
HeapElement TreeNode::emptyHeapElement;
MyVector<int> TreeNode::emptyQueryIndexList;

/***************************************************************
 *  			Tree structure related.
 ***************************************************************/

TreeNode::TreeNode() {
	// TODO Auto-generated constructor stub

	this->minEndpt = 0;
	this->ptCnt = 0;
	this->structureIndex = -1;

// The number of tree nodes created.
	TNNUM++;
}

TreeNode::TreeNode(int minEndpt) {
	this->minEndpt = minEndpt;
	this->ptCnt = 0;
	this->structureIndex = -1;

// The number of tree nodes created.
	TNNUM++;
}


TreeNode::~TreeNode() {
	// TODO Auto-generated destructor stub

// Decrease the number of tree nodes by 1.
	TNNUM--;
}

void TreeNode::SetEndPoints(int min) {
	this->minEndpt = min;
}

void TreeNode::UpdateHeapElement(int indexInHeap, int thresholdOnSite,
		MyVector<MinHeap>& _minHeapList, MyVector<Query>& _queryList,
		MyVector<int>& _upperElementIndexInHeapList) {
	if (this->structureIndex != -1) {

		MinHeap& emitCntList = _minHeapList[this->structureIndex];

		emitCntList.ModifyKey(indexInHeap, this->ptCnt + thresholdOnSite,
				_queryList, _upperElementIndexInHeapList);
	}
}

void TreeNode::InitializeHeap(MyVector<MinHeap>& _minHeapList,
		MyVector<Query>& _queryList,
		MyVector<int>& _upperElementIndexInHeapList) {

	if (this->structureIndex != -1) {

		MinHeap& emitCntList = _minHeapList[this->structureIndex];
		emitCntList.ShrinkToFit();
		emitCntList.Heapify(_queryList, _upperElementIndexInHeapList);
	}
}

bool TreeNode::CheckEmit(MyVector<MinHeap>& _minHeapList,
		MyVector<HeapElement>& _heapElementList) {
//	double start = getCurrentTime();
	if (this->structureIndex == -1) {
		return false;
	} else {
		MinHeap& emitCntList = _minHeapList[this->structureIndex];
		HeapElement* he = emitCntList.GetTop();

		if (he != NULL) {
			int top = he->getEmitCnt();
			return this->ptCnt >= top;
		} else {
			return false;
		}

	}
}

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
void TreeNode::InsertQuery_Space(Query& query, unsigned int queryIndex,
		unsigned int nodeIndex, void* segTree_Log, int dimLev) {

	SegmentTree_Log* log = (SegmentTree_Log*) segTree_Log;

	if (dimLev == DIM) {
		// This node is a tree node of the segment tree at the last dimension level.

		if (this->structureIndex == -1) {

			this->structureIndex = log->_minHeapList.size();
			log->_minHeapList.push_back(emptyMinHeap);

		}

	} else {
		if (this->structureIndex == -1) {

			this->structureIndex = log->_tempQueryIndexList.size();
			log->_tempQueryIndexList.push_back(emptyQueryIndexList);

		}

		MyVector<int>& queryIndexList =
				log->_tempQueryIndexList[this->structureIndex];

		// Record the pointer of this query.
		queryIndexList.push_back(queryIndex);
	}
}

void TreeNode::FindCanonicalNode_Space(Query& query, int nodeIndex,
		void* segTree_Log, int dimLev, MyVector<int>& nodeIndexList) {

	if (dimLev == DIM) {
		// This node is a tree node of the segment tree at the last dimension level.
		nodeIndexList.push_back(nodeIndex);
		return;
	} else {
		if (this->structureIndex != -1) {
			// This tree node has a secondary segment tree.

			SegmentTree_Log* log = (SegmentTree_Log*) segTree_Log;

			SegmentTree& tree = log->_segTreeList[this->structureIndex];

			tree.FindCanonicalNode_Space(query, segTree_Log, dimLev + 1,
					nodeIndexList);

		}
	}
}

void TreeNode::ConstructNextLevelTree_Space(int subrootIndex, void* segTree_Log,
		int nextLev) {

	if (this->structureIndex != -1) {
		// Note that here we can not use reference because when _tempQueryIndexList reallocate,
		// the reference will become invalid. Thus, we use simple value copy here.

		SegmentTree_Log* log = (SegmentTree_Log*) segTree_Log;

		MyVector<int> queryIndexList =
				log->_tempQueryIndexList[this->structureIndex];

		SegmentTree nextLevelTree(queryIndexList, segTree_Log, nextLev);

		// After constructing the next level tree, the query list is useless.
		// Then release the space.
		queryIndexList.release_space();

		// Note that in the following, we can no longer use "this" pointer because after
		// the recursion, this tree node may already delete as the _treeNodeList enlarges.
		// As a result, we have to use the tree node index to access the corresponding tree node.

		// Change the ptrType of this node.
		log->_treeNodeList[subrootIndex].structureIndex =
				log->_segTreeList.size();

		log->_segTreeList.push_back(nextLevelTree);

	}
}


void TreeNode::DeleteQuery_Space(int indexInHeap, void* segTree_Log) {
	// Delete the heap element from heap.

	if (this->structureIndex != -1) {
		SegmentTree_Log* log = (SegmentTree_Log*) segTree_Log;
		MinHeap& emitCntList = log->_minHeapList[this->structureIndex];

		emitCntList.Delete(indexInHeap, log->_queryList,
				log->_upperElementIndexInHeapList);
	}

}

int TreeNode::NotifyCoordinator_Space(MyVector<int>& alertIDList,
		void* segTree_Log) {

	int alertCnt = 0;

	// Notify coordinators.
	if (this->structureIndex != -1) {
//		double start = 0;
//		double end = 0;

		SegmentTree_Log* log = (SegmentTree_Log*) segTree_Log;

		MinHeap& emitCntList = log->_minHeapList[this->structureIndex];

		Query* query = NULL;
//		HeapElement* he = NULL;
		int ptCntDelta = 0;
		int emitCnt = 0;
		int temp = 0;

//		HeapElement* minElement = emitCntList.GetTop(log->_heapElementList);
		HeapElement* minElement = emitCntList.GetTop();
		if (minElement != NULL) {
			emitCnt = minElement->getEmitCnt();

			while (this->ptCnt >= emitCnt) {

				query =
						&(log->_queryList[log->_upperElementIndexInHeapList[minElement->getQueryStartIndex()]]);

				if (query->threshold == -1) {
//					start = getCurrentTime();

					// The current heap top has alerted.
					emitCntList.DeleteTop(log->_queryList,
							log->_upperElementIndexInHeapList);


					// Get the next min.
					if (emitCntList.GetSize() > 0) {
//						minElement = emitCntList.GetTop(log->_heapElementList);
						minElement = emitCntList.GetTop();
						emitCnt = minElement->getEmitCnt();
					} else {
						break;
					}
					continue;
				}

				ptCntDelta = this->ptCnt - emitCnt + query->thresholdOnSite;

				temp = query->ReceiveNotification_Space(segTree_Log,
						ptCntDelta);

				if (temp == 0) {
					// The query neither alerted nor entered next round.
					// Update the key of the heap element.
					emitCnt += (ptCntDelta / query->thresholdOnSite
							* query->thresholdOnSite);

					minElement->setEmitCnt(emitCnt);

					// Maintain the heap.
					emitCntList.BubbleDown(0, log->_queryList,
							log->_upperElementIndexInHeapList);

				} else if (temp == 1) {
					// This the exact insertion only version and this query alerts.

					// Delete this heap element on top.
					emitCntList.DeleteTop(log->_queryList,
							log->_upperElementIndexInHeapList);

					query->ReleaseSpace();

					alertCnt++;

					alertIDList.push_back(query->id);

				}

				// Get the next min.
				if (emitCntList.GetSize() > 0) {
//					minElement = emitCntList.GetTop(log->_heapElementList);
					minElement = emitCntList.GetTop();
					emitCnt = minElement->getEmitCnt();
				} else {
					break;
				}
			}
		}
	}

	return alertCnt;
}


int TreeNode::InsertPoint_Space(int* coords, int dimLev, int weight,
		MyVector<int>& alertIDList, void* segTree_Log) {

	int alertCnt = 0;

	if (dimLev == DIM) {
		// Record the running time of this stab query.
//		StabTime_Ours += getCurrentTime() - StabStart_Ours;

		alertCnt = this->InsertPoint_Space(weight, alertIDList, segTree_Log);
	} else {
		// Insert this point to next level tree.
		if (this->structureIndex != -1) {

			SegmentTree_Log* log = (SegmentTree_Log*) segTree_Log;

			SegmentTree& nextLevelTree = log->_segTreeList[this->structureIndex];
			alertCnt = nextLevelTree.InsertPoint_Space(coords, weight,
					dimLev + 1, alertIDList, segTree_Log);

		}
	}
	return alertCnt;
}

int TreeNode::InsertPoint_Space(int weight, MyVector<int>& alertIDList,
		void* segTree_Log) {

	int alertCnt = 0;
	this->ptCnt += weight;

	alertCnt = this->NotifyCoordinator_Space(alertIDList, segTree_Log);

	return alertCnt;
}

void TreeNode::ReleaseSpace_Space(void* segTree_Log, int dimLev) {

	if (this->structureIndex != -1) {
		SegmentTree_Log* log = (SegmentTree_Log*) segTree_Log;

		if (dimLev < DIM) {

			SegmentTree& nextLevelTree =
					(log->_segTreeList)[this->structureIndex];
			nextLevelTree.ReleaseSpace_Space(segTree_Log, dimLev + 1);
			this->structureIndex = -1;
		} else {
			MinHeap& emitCntList = log->_minHeapList[this->structureIndex];
			emitCntList.ReleaseSpace();
			this->structureIndex = -1;
		}
	}
}

long long TreeNode::CompuateSpaceConsumption_Space(void* segTree_Log,
		int dimLev) {

	long long rtn = 0;

	rtn += sizeof(TreeNode);

	if (this->structureIndex != -1) {

		SegmentTree_Log* log = (SegmentTree_Log*) segTree_Log;

		if (dimLev < DIM) {
			SegmentTree& nextLevelTree =
					(log->_segTreeList)[this->structureIndex];
			rtn += nextLevelTree.ComputeSpaceConsumption_Space(segTree_Log,
					dimLev + 1);
		} else {
			MinHeap& emitCntList = log->_minHeapList[this->structureIndex];
			rtn += sizeof(MinHeap);
			rtn += sizeof(int) * emitCntList.elementList.capacity();

		}
	}

	return rtn;
}
