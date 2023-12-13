/*
 * Query.cpp
 *
 *  Created on: 7 Sep, 2015
 *      Author: jhgan
 */

#include "Query.h"
#include "./SegmentTree/TreeNode.h"
#include "./SegmentTree/SegmentTree.h"
#include "./SegmentTree/SegmentTreeLog.h"

MyVector<int> Query::nodeIndexList;

Query::Query() :
		QueryBase() {
	// TODO Auto-generated constructor stub
	this->thresholdOnSite = 0;
	this->msgCnt = 0;
	this->siteNum = 0;
	this->startIndex = -1;
	QNUM++;
}

Query::Query(int id, int* range, int threshold) :
		QueryBase(id, range, threshold) {

	this->thresholdOnSite = 0;
	this->msgCnt = 0;
	this->siteNum = 0;
	this->startIndex = -1;

	QNUM++;
}

Query::Query(int id, MyVector<int>& list, int threshold) :
		QueryBase(id, list, threshold) {
	this->thresholdOnSite = 0;
	this->msgCnt = 0;
	this->siteNum = 0;
	this->startIndex = -1;

	QNUM++;
}

Query::Query(const QueryBase* q) :
		QueryBase(q) {
	this->thresholdOnSite = 0;
	this->msgCnt = 0;
	this->siteNum = 0;
	this->startIndex = -1;

	QNUM++;
}

Query::Query(const QueryBase& q) :
		QueryBase(q) {
	this->thresholdOnSite = 0;
	this->msgCnt = 0;
	this->siteNum = 0;
	this->startIndex = -1;

	QNUM++;
}

Query::~Query() {
	// TODO Auto-generated destructor stub

//	this->ReleaseSpace();

	QNUM--;
}

void Query::Alert() {

#ifdef SHOWALERT
	printf("Query ID = %d: ", this->id);
	this->ShowRange();
	printf(" has reached its threshold %d\n", this->threshold);
	fflush(stdout);
#endif

}

int Query::ShowQueryState() {
	// If the lower list is empty, the query alerts only when it collects the exact count
	// and this exact >= threshold.
	if (this->msgCnt == 0 && this->evlCnt >= this->threshold) {
		return 1;
	}

	return 0;
}

/***************************************************************
 *  			Space Efficient version related.
 ***************************************************************/

void Query::DeleteMyself_Space(void* segTree_Log) {
//	int siteNum = this->upperElementIndexList.size();

	nodeIndexList.clear();
	nodeIndexList.reserve(siteNum);

	SegmentTree_Log* log = (SegmentTree_Log*) segTree_Log;

	log->tree->FindCanonicalNode_Space(*this, segTree_Log, 1, nodeIndexList);

	for (int i = 0; i < this->siteNum; ++i) {
		log->_treeNodeList[nodeIndexList[i]].DeleteQuery_Space(
				log->_upperElementIndexInHeapList[this->startIndex + 1 + i],
				segTree_Log);
	}
}

void Query::CleanMySelf_Space() {
	// reset the value of msgCnt.
	this->msgCnt = 0;
	this->evlCnt = 0;
}

void Query::RecordExactCounter_Space(void* segTree_Log) {

	this->threshold -= this->CollectInfoFromSites_Space(segTree_Log);
	this->evlCnt = 0;
}

void Query::RecordExactCounter() {
	this->threshold -= this->evlCnt;
	this->evlCnt = 0;
	this->siteNum = 0;
}

int Query::CollectInfoFromSites_Space(void* segTree_Log) {

	SegmentTree_Log* log = (SegmentTree_Log*) segTree_Log;


	nodeIndexList.clear();
	nodeIndexList.reserve(this->siteNum);

	log->tree->FindCanonicalNode_Space(*this, segTree_Log, 1, nodeIndexList);

	// Collect all the exact counts from canonical nodes.
	int ptCnt = 0;
	for (int i = 0; i < siteNum; ++i) {
		ptCnt += log->_treeNodeList[nodeIndexList[i]].ptCnt;
	}
	return ptCnt;
}

void Query::ComputeThresholdOnSite_Space() {
	if (this->siteNum <= 0) {
		ShowError(
				"Error in Query::ComputeThresholdOnSite_Space: The siteNum of a query is <= 0!\n");
		return;
	}

	// This is the threshold monitoring algorithm.

	int rest = this->threshold - this->evlCnt;

	this->thresholdOnSite = rest / (2 * this->siteNum);

	if (this->thresholdOnSite < 1) {
		this->thresholdOnSite = 1;
	}
}

int Query::ReceiveNotification_Space(void* segTree_Log, int ptCntDelta) {
	int msgNum = ptCntDelta / this->thresholdOnSite;

	this->evlCnt += this->thresholdOnSite * msgNum;
	if (this->evlCnt >= this->threshold) {
		//			this->Alert();
		return 1;
	}

	this->msgCnt += msgNum;
	if (this->thresholdOnSite > 1 && this->msgCnt >= this->siteNum) {

		// Collect all the exact counts from canonical nodes.
		this->evlCnt = this->CollectInfoFromSites_Space(segTree_Log);

		if (this->evlCnt >= this->threshold) {
			return 1;
		}

		this->EnterNextRound_Space(segTree_Log);
		return 2;
	}
	return 0;
}

int Query::ThresholdMonitoring_Action_Space(void* segTree_Log, int ptCntDelta) {
	// This is the threshold monitoring algorithm
	int msgNum = ptCntDelta / this->thresholdOnSite;
	this->msgCnt += msgNum;
	if (this->thresholdOnSite > 1 && this->msgCnt >= this->siteNum) {

		// Collect all the exact counts from canonical nodes.
		this->evlCnt = this->CollectInfoFromSites_Space(segTree_Log);

		if (this->evlCnt >= this->threshold) {
			this->Alert();
			return 1;
		}

		this->EnterNextRound_Space(segTree_Log);
		return 2;
	} else {
		this->evlCnt += this->thresholdOnSite * msgNum;
		if (this->evlCnt >= this->threshold) {
			this->Alert();
			return 1;
		}
	}
	return 0;
}

void Query::EnterNextRound_Space(void* segTree_Log) {
	this->msgCnt = 0;
	this->ComputeThresholdOnSite_Space();
	this->BroadCastThresholdOnSite_Space(segTree_Log);
}

void Query::BroadCastThresholdOnSite_Space(void* segTree_Log) {

	SegmentTree_Log* log = (SegmentTree_Log*) segTree_Log;

	for (int i = 0; i < this->siteNum; ++i) {

		// Here we do not need to compute nodeIndexList, because we have computed it
		// in function CollectInfoFromSites_Space.
		log->_treeNodeList[nodeIndexList[i]].UpdateHeapElement(
				log->_upperElementIndexInHeapList[this->startIndex + 1 + i],
				this->thresholdOnSite, log->_minHeapList, log->_queryList,
				log->_upperElementIndexInHeapList);
	}
}

void Query::ReleaseSpace_Space(void* segTree_Log) {
	if (this->range != NULL) {
		delete[] range;
		this->range = NULL;
	}

	this->DeleteMyself_Space(segTree_Log);
	this->threshold = -1;
}

void Query::ReleaseSpace() {
	if (this->range != NULL) {
		delete[] range;
		this->range = NULL;
	}

	this->threshold = -1;
}

long long Query::ComputeSpaceConsumption_Space() {
	long long rtn = 0;

	rtn += sizeof(Query);
	rtn += sizeof(int) * DIM * 2;

	return rtn;
}


//void Query::InitializeSites_Space(void* segTree, void* segTree_Log) {
////	this->msgCnt = 0;
////	this->ComputeThresholdOnSite_Space();
////
////	SegmentTree_Log* log = (SegmentTree_Log*) segTree_Log;
////
////	this->upperElementIndexList.shrink_to_fit();
////	int siteNum = this->upperElementIndexList.size();
////
////	nodeIndexList.clear();
////	nodeIndexList.reserve(siteNum);
////
//////	printf("query %d: [%d, %d]\n", this->id, this->range[0], this->range[1]);
////	SegmentTree* tree = (SegmentTree*) segTree;
////	tree->FindCanonicalNode_Space(*this, segTree_Log, 1, nodeIndexList);
//////	printf("\n");
////
////	// Initialize the heap elements but not maintain the heap.
////	for (int i = 0; i < siteNum; ++i) {
////		//		log->_heapElementList[this->upperElementIndexList[i]].emitCnt =
////		//				this->thresholdOnSite;
////
//////		log->_heapElementList[this->upperElementIndexList[i]].setEmitCnt(
//////				this->thresholdOnSite);
////
////		TreeNode& node = log->_treeNodeList[nodeIndexList[i]];
//////		printf("%d ", node.minEndpt);
////
////		MinHeap& minHeap = log->_minHeapList[node.structureIndex];
////		minHeap.elementList[this->upperElementIndexList[i]].setEmitCnt(
////				this->thresholdOnSite);
////
////	}
////
//////	printf("\n\n");
//}
