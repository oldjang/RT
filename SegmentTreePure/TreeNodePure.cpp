/*
 * TreeNodePure.cpp
 *
 *  Created on: 15 Sep, 2015
 *      Author: jhgan
 */

#include "TreeNodePure.h"
#include "../QueryBase.h"
#include "SegmentTreePure.h"
#include "../IntervalTree/IntervalTree.h"

/***************************************************************
 *  			Tree structure related.
 ***************************************************************/

TreeNodePure::TreeNodePure() {
	// TODO Auto-generated constructor stub

	this->left = NULL;
	this->right = NULL;

	this->minEndpt = 0;
	this->maxEndpt = 0;

	this->ptrType = 1;
	this->structurePtr = NULL;

	TNNUM_SEG++;
}

TreeNodePure::TreeNodePure(int minEndpt, int maxEndpt) {
	this->left = NULL;
	this->right = NULL;

	this->minEndpt = minEndpt;
	this->maxEndpt = maxEndpt;

	this->ptrType = 1;
	this->structurePtr = NULL;
	TNNUM_SEG++;
}

TreeNodePure::TreeNodePure(TreeNodePure* left, TreeNodePure* right) {
	this->left = left;
	this->right = right;

	this->minEndpt = left->minEndpt;
	if (right != NULL) {
		this->maxEndpt = right->maxEndpt;
	} else {
		this->maxEndpt = left->maxEndpt;
	}

	this->ptrType = 1;
	this->structurePtr = NULL;
	TNNUM_SEG++;
}


TreeNodePure::~TreeNodePure() {
	// TODO Auto-generated destructor stub
	TNNUM_SEG--;
}

void TreeNodePure::ReleaseSpace() {
	if (this->structurePtr != NULL) {
		if (this->ptrType == 1) {
			MyVector<QueryBase*>* queryList =
					(MyVector<QueryBase*>*) (this->structurePtr);

			queryList->release_space();

			delete (queryList);
			this->structurePtr = NULL;

		} else if (this->ptrType == 2) {
			SegmentTreePure* nextLevelTree =
					(SegmentTreePure*) (this->structurePtr);
			nextLevelTree->ReleaseSpace();
			delete (nextLevelTree);
			this->structurePtr = NULL;
		} else if (this->ptrType == 3) {
			IntervalTree* nextLevelTree = (IntervalTree*) (this->structurePtr);
			nextLevelTree->ReleaseSpace();
			delete (nextLevelTree);
			this->structurePtr = NULL;
		}
	}
}

void TreeNodePure::ConstructNextLevelTree(int nextLev) {

	if (this->structurePtr != NULL) {
		if (this->ptrType == 1) {
			MyVector<QueryBase*>* queryList =
					(MyVector<QueryBase*>*) (this->structurePtr);

			if (nextLev < DIM) {
				SegmentTreePure* nextLevelTree = new SegmentTreePure(
						*(queryList), nextLev);

				// Change the ptrType of this node.
				this->ptrType = 2;
				this->structurePtr = nextLevelTree;
			} else {
				IntervalTree* nextLevelTree = new IntervalTree(*(queryList),
						nextLev);

				// Change the ptrType of this node.
				this->ptrType = 3;
				this->structurePtr = nextLevelTree;
			}

			queryList->release_space();
			delete (queryList);

		}
	} else {
		// There is no query associating with this node.
		this->ptrType = 2;
		this->structurePtr = NULL;
	}
}

bool TreeNodePure::CheckCover(int min, int max) {
	return (min <= this->minEndpt && max >= this->maxEndpt);
}

bool TreeNodePure::CheckStab(int value) {
	return (value >= this->minEndpt && value <= this->maxEndpt);
}

bool TreeNodePure::CheckIntersect(int min, int max) {
// To check the intersection of the ranges, we need to consider the state of the range.

	return this->CheckStab(min) || this->CheckStab(max);
}

int TreeNodePure::InsertPoint(int* coords, int dimLev, int weight,
		MyVector<int>& alertIDList) {
	int cnt = 0;

	if (dimLev == DIM) {
		cnt = this->InsertPoint(weight, alertIDList);
	} else {
		if (this->structurePtr != NULL) {
			// Insert this point to next level tree.
			if (this->ptrType == 2) {
				SegmentTreePure* nextLevelTree =
						(SegmentTreePure*) (this->structurePtr);
				cnt = nextLevelTree->InsertPoint(coords, weight, alertIDList);
			} else if (this->ptrType == 3) {
				IntervalTree* nextLevelTree =
						(IntervalTree*) (this->structurePtr);
//				double start = getCurrentTime();
				cnt = nextLevelTree->InsertPoint(coords[DIM - 1], weight,
						alertIDList, DIM);
//				TestTime += getCurrentTime() - start;

			} else {
				ShowError(
						"Error in TreeNodePure::InsertPoint(vector<int>& coords, int dimLev): The ptrType is incorrect!\n");
			}
		}
	}
	return cnt;
}

int TreeNodePure::InsertPoint(int weight, MyVector<int>& alertIDList) {
	int alertCnt = 0;

	if (this->structurePtr != NULL && this->ptrType == 1) {
		MyVector<QueryBase*>& queryList =
				*((MyVector<QueryBase*>*) (this->structurePtr));
		QueryBase* q = NULL;
		for (int i = 0; i < (int) queryList.size(); ++i) {
			q = (QueryBase*) (queryList[i]);

			if (q->threshold == -1) {
				// Delete this query from the list as it has alerted.
				// Move the last query pointer to index i and pop back.
				i = this->DeleteFromList(i);
			} else {

				// Increase the StabNum by one.
				StabNum++;

				if (q->InsertPoint(weight)) {
					// Query q alerts. Delete q from the list.

					q->ReleaseSpace();
					// This step is important because q may be stored in other nodes.
					q->threshold = -1;

					alertCnt++;

					alertIDList.push_back(q->id);
					i = this->DeleteFromList(i);
				}
			}
		}
	}

	return alertCnt;
}

void TreeNodePure::ShowRange() {
// Print range.
	printf("node range: ");
	printf("[%d, %d]\n", this->minEndpt, this->maxEndpt);
}

void TreeNodePure::ShowInfo() {
	// Print the range.
	this->ShowRange();
}

void TreeNodePure::InsertQuery(QueryBase* query, int dimLev) {

	if (this->structurePtr == NULL) {

		this->ptrType = 1;

		// When the query list is not initialized, initialize it.
		this->structurePtr = new MyVector<QueryBase*>();
	}

	if (this->ptrType == 1) {
		MyVector<QueryBase*>* queryList =
				(MyVector<QueryBase*>*) (this->structurePtr);

		// Record the pointer of this query.
		queryList->push_back(query);
	} else {
		ShowError(
				"Error in TreeNodePure::InsertQuery: The ptrType is incorrect!\n");
	}
}

int TreeNodePure::DeleteFromList(int index) {

	MyVector<QueryBase*>& queryList =
			*((MyVector<QueryBase*>*) (this->structurePtr));

	if (index >= (int) queryList.size() || index < 0) {
		ShowError(
				"Error in TreeNodePure::DeleteFromList: the index exceeds the length of the list!\n");
		return -1;
	}

	queryList[index] = queryList[queryList.size() - 1];
	queryList.pop_back();

	return index - 1;
}

long long TreeNodePure::ComputeSpaceConsumption() {
	long long rtn = 0;
	rtn += sizeof(TreeNodePure);
	if (this->structurePtr != NULL) {
		if (this->ptrType == 2) {
			rtn +=
					((SegmentTreePure*) (this->structurePtr))->ComputeSpaceConsumption();
		} else if (this->ptrType == 3) {
			rtn +=
					((IntervalTree*) (this->structurePtr))->ComputeSpaceConsumption();
		}
	}
	return rtn;
}
