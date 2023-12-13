/*
 * SegmentTreeLog.cpp
 *
 *  Created on: 9 Oct, 2015
 *      Author: jhgan
 */

#include "SegmentTreeLog.h"

/***************************************************************
 *  			Space efficient version related.
 ***************************************************************/
SegmentTree_Log::SegmentTree_Log() {
	// TODO Auto-generated constructor stub
	this->tree = NULL;
	this->aliveNum = 0;
}

SegmentTree_Log::~SegmentTree_Log() {
	// TODO Auto-generated destructor stub
}

long long SegmentTree_Log::ComputeSpaceConsumption() {
	long long spaceCons = 0;

	MyVector<Query>& scQueryList = this->_queryList;
	MyVector<TreeNode>& scTreeNodeList = this->_treeNodeList;
	MyVector<MinHeap>& scMinHeapList = this->_minHeapList;
//	MyVector<HeapElement>& scHeapElementList = this->_heapElementList;
	MyVector<SegmentTree>& scSegTreeList = this->_segTreeList;
	MyVector<int>& scUpperElementIndexInHeapList = this->_upperElementIndexInHeapList;


	spaceCons += sizeof(SegmentTree_Log);

	spaceCons += (sizeof(Query)) * scQueryList.capacity();
	spaceCons += sizeof(int) * 2 * DIM * this->aliveNum;

	spaceCons += sizeof(int) * scUpperElementIndexInHeapList.capacity();

	spaceCons += sizeof(TreeNode) * scTreeNodeList.capacity();

	spaceCons += sizeof(MinHeap) * scMinHeapList.capacity();

	unsigned int tempInt = scMinHeapList.size();
	for (unsigned int j = 0; j < tempInt; ++j) {
		spaceCons += sizeof(HeapElement)
				* scMinHeapList[j].elementList.capacity();
	}

//	spaceCons += sizeof(HeapElement) * scHeapElementList.capacity();

	spaceCons += sizeof(SegmentTree) * scSegTreeList.capacity();

	tempInt = scSegTreeList.size();
	for (unsigned int j = 0; j < tempInt; ++j) {
		spaceCons += sizeof(int) * scSegTreeList[j].levelStartIndex.capacity();
	}

	// One more segment tree that is not in the array.
	// That is the pointer tree in SegmentTree_Log.
	spaceCons += sizeof(SegmentTree);
	spaceCons += sizeof(int) * this->tree->levelStartIndex.capacity();

	spaceCons += 48 * this->id_index_map.size();

	return spaceCons;
}

SegmentTree_Log::SegmentTree_Log(MyVector<Query>& queryList) {
	this->aliveNum = queryList.size();

	// Swap the given query list.
	this->_queryList.swap(queryList);
//	this->_queryRangeList.swap(queryRangeList);

	MyVector<int> queryIndexList;
	queryIndexList.reserve(aliveNum);
	for (int i = 0; i < aliveNum; ++i) {
		queryIndexList.push_back(i);

		// Insert the id, index pair to the map.
		id_index_map[_queryList[i].id] = i;
	}
	this->tree = new SegmentTree(queryIndexList, this);

	queryIndexList.release_space();

	_treeNodeList.shrink_to_fit();
	_segTreeList.shrink_to_fit();
	_minHeapList.shrink_to_fit();
//	_heapElementList.shrink_to_fit();
}

bool SegmentTree_Log::DeleteQuery(int queryID) {
	map<int, int>::iterator it = id_index_map.find(queryID);
	if (it != id_index_map.end()) {
		int index = it->second;

		if (_queryList[index].threshold != -1) {
			if (this->tree != NULL) {
//				this->tree->DeleteQuery_Space(_queryList[index], this);
				// Here we delete the query lazily, namely, set the threshold = -1
				// when it occurs at the top of heap, we delete the heap element.
				_queryList[index].ReleaseSpace();
				aliveNum--;
			}
			id_index_map.erase(it);
			return true;
		}
		id_index_map.erase(it);
		return false;
	}
	return false;
}

void SegmentTree_Log::ObtainExactCntForQuery(MyVector<Query>& targetPlace) {
	int qNum = _queryList.size();

//	int* rangePos = NULL;

	for (int i = 0; i < qNum; ++i) {
		if (_queryList[i].threshold != -1) {
			// this query is alive.

			_queryList[i].RecordExactCounter_Space(this);
			_queryList[i].CleanMySelf_Space();
			targetPlace.push_back(_queryList[i]);
		}
	}
}

SegmentTree_Log* SegmentTree_Log::ConstructStr(MyVector<Query>& queryList) {
	return new SegmentTree_Log(queryList);
}

int SegmentTree_Log::InsertPoint(int* coords, int weight,
		MyVector<int>& alertIDList) {
	int alertNum = 0;
	if (this->tree != NULL) {
		// record the start time of this stab query.
//		StabStart_Ours = getCurrentTime();

		alertNum = this->tree->InsertPoint_Space(coords, weight, 1, alertIDList,
				this);
		this->aliveNum -= alertNum;
	}
	return alertNum;
}


void SegmentTree_Log::ReleaseSpace() {
	if (this->tree != NULL) {
		this->tree->ReleaseSpace_Space(this, 1);
		delete (this->tree);
		this->tree = NULL;
	}

//	_queryRangeList.release_space();
	_queryList.release_space();
	_treeNodeList.release_space();
//	_heapElementList.release_space();
	_tempQueryIndexList.release_space();
	_minHeapList.release_space();
	_segTreeList.release_space();
	_upperElementIndexInHeapList.release_space();
}

void SegmentTree_Log::ReleaseSpace_All() {

	int num = this->_queryList.size();
	for (int i = 0; i < num; ++i) {
		_queryList[i].ReleaseSpace();
	}

	if (this->tree != NULL) {
		this->tree->ReleaseSpace_Space(this, 1);
		delete (this->tree);
		this->tree = NULL;
	}

//	_queryRangeList.release_space();
	_queryList.release_space();
	_treeNodeList.release_space();
//	_heapElementList.release_space();
	_tempQueryIndexList.release_space();
	_minHeapList.release_space();
	_segTreeList.release_space();
	_upperElementIndexInHeapList.release_space();
}
