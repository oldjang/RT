/*
 * IntervalTree.cpp
 *
 *  Created on: 15 Sep, 2015
 *      Author: jhgan
 */

#include "IntervalTree.h"

IntervalTree::IntervalTree() {
	// TODO Auto-generated constructor stub
	this->root = NULL;

//	TNUM_INV++;
}

IntervalTree::IntervalTree(MyVector<QueryBase*>& queryList, int dimLev) {

	vector<EndPtElement> endPtList;

	// Generate endpoint element list.
	this->GenerateEndPtElementList(queryList, endPtList, dimLev);

	// Construct interval tree.
	this->root = this->ConstructIntervalTree(endPtList, dimLev);

//	endPtList.release_space();
	endPtList.clear();

	TNUM_INV++;
}

IntervalTree::~IntervalTree() {
	// TODO Auto-generated destructor stub
	TNUM_INV--;
}

void IntervalTree::ReleaseSpace() {
	if (this->root != NULL) {
		this->root->ReleaseSpace();
		delete (this->root);
		this->root = NULL;
	}
}

int IntervalTree::DeletePoint(int* coords, int weight,
		MyVector<int>& alertIDList, int dimLev) {
//	int index = (dimLev - 1);
	int rtn = this->InsertPoint(coords[dimLev], -weight, alertIDList, dimLev);
	return rtn;
}

int IntervalTree::InsertPoint(int* coords, int weight,
		MyVector<int>& alertIDList, int dimLev) {
	int rtn = this->InsertPoint((int) (coords[dimLev - 1]), weight, alertIDList,
			dimLev);
	return rtn;
}

int IntervalTree::InsertPoint(int value, int weight, MyVector<int>& alertIDList,
		int dimLev) {
	int cnt = 0;

	IntervalTreeNode* node = this->root;

	while (node != NULL) {
		// Insert this point to node.
		cnt += node->InsertPoint(value, weight, alertIDList, dimLev);
		if (node->key > value) {
			node = node->left;
		} else if (node->key < value) {
			node = node->right;
		} else {
			// node->key == value
			return cnt;
		}
	}
	return cnt;
}

void IntervalTree::DeleteQuery(QueryBase* query) {
	query->threshold = -1;
}

void IntervalTree::GenerateEndPtElementList(MyVector<QueryBase*>& queryList,
		vector<EndPtElement>& targetPlace, int dimLev) {
	int qNum = queryList.size();
	targetPlace.clear();
	targetPlace.reserve(2 * qNum);

	EndPtElement temp;

	int index = 2 * (dimLev - 1);

	for (int i = 0; i < qNum; ++i) {
		temp.endpt = queryList[i]->range[0 + index];
		temp.q = queryList[i];
		temp.isLeft = 1;
		targetPlace.push_back(temp);

		temp.endpt = queryList[i]->range[1 + index];
		temp.q = queryList[i];
		temp.isLeft = 0;
		targetPlace.push_back(temp);
	}

	// Sort all the endpoint elements.
//	sort(targetPlace.get_list(), targetPlace.get_list() + targetPlace.size());
	sort(targetPlace.begin(), targetPlace.end());

}

int IntervalTree::PickKey(vector<EndPtElement>& endPtList) {
	int key = 0;

	vector<int> distinctValueList;

	int num = endPtList.size();

	distinctValueList.push_back(endPtList[0].endpt);
	for (int i = 1; i < num; ++i) {
		// Skip all the duplicate values.
		while (i < num && endPtList[i - 1].endpt == endPtList[i].endpt) {
			++i;
		}
		if (i < num)
			distinctValueList.push_back(endPtList[i].endpt);
	}

	int num2 = distinctValueList.size();
//	printf("distinct num = %d\n", num2);

// Pick the median value in distinct value list.
	key = distinctValueList[num2 / 2];

//	distinctValueList.release_space();
	distinctValueList.clear();

	return key;
}

void IntervalTree::AssignIntervals(int key, vector<EndPtElement>& endPtList,
		vector<EndPtElement>& leftPart, vector<EndPtElement>& stabbedSet,
		vector<EndPtElement>& rightPart, int dimLev) {

	leftPart.clear();
	stabbedSet.clear();
	rightPart.clear();

	int index = 2 * (dimLev - 1);

	int num = endPtList.size();
	for (int i = 0; i < num; ++i) {
		if (key > endPtList[i].q->range[1 + index]) {
			// Query q is completely to the left of key.
			leftPart.push_back(endPtList[i]);
		} else if (key < endPtList[i].q->range[0 + index]) {
			//Query q is completely to the right of key.
			rightPart.push_back(endPtList[i]);
		} else {
			//Query q is stabbed by key.
			stabbedSet.push_back(endPtList[i]);
		}
	}
}

IntervalTreeNode* IntervalTree::ConstructIntervalTree(
		vector<EndPtElement>& endPtList, int dimLev) {
	if (endPtList.size() == 0) {
		return NULL;
	}

	IntervalTreeNode* subroot = NULL;

	int key = this->PickKey(endPtList);
	vector<EndPtElement> leftPart, stabbedSet, rightPart;

	this->AssignIntervals(key, endPtList, leftPart, stabbedSet, rightPart,
			dimLev);

	endPtList.clear();

	subroot = new IntervalTreeNode(key, stabbedSet);

	subroot->left = this->ConstructIntervalTree(leftPart, dimLev);
	subroot->right = this->ConstructIntervalTree(rightPart, dimLev);

	leftPart.clear();
	stabbedSet.clear();
	rightPart.clear();

	return subroot;
}

int IntervalTree::ComputeTreeSize() {
	return this->ComputeTreeSize_SubRoutine(this->root);
}

int IntervalTree::ComputeTreeSize_SubRoutine(IntervalTreeNode* subroot) {
	if (subroot == NULL) {
		return 0;
	}

	int rtn = subroot->leftList.size();
	rtn += this->ComputeTreeSize_SubRoutine(subroot->left);
	rtn += this->ComputeTreeSize_SubRoutine(subroot->right);
	return rtn;
}

long long IntervalTree::ComputeSpaceConsumption() {
	long long rtn = 0;
	rtn += sizeof(IntervalTree);
	rtn += this->ComputeSpaceConsumption_SubRoutine(this->root);
	return rtn;
}

long long IntervalTree::ComputeSpaceConsumption_SubRoutine(
		IntervalTreeNode* subroot) {
	if (subroot == NULL) {
		return 0;
	}

	long long rtn = 0;
	rtn += subroot->ComputeSpaceConsumption();
	rtn += this->ComputeSpaceConsumption_SubRoutine(subroot->left);
	rtn += this->ComputeSpaceConsumption_SubRoutine(subroot->right);
	return rtn;
}
