/*
 * IntervalTreeNode.cpp
 *
 *  Created on: 15 Sep, 2015
 *      Author: jhgan
 */

#include "IntervalTreeNode.h"

IntervalTreeNode::IntervalTreeNode() {
	// TODO Auto-generated constructor stub
	this->key = 0;
	this->left = NULL;
	this->right = NULL;

	TNNUM_INV++;
}

IntervalTreeNode::IntervalTreeNode(int key,
		vector<EndPtElement>& stabbedEndPtList) {
	this->key = key;

	int num = stabbedEndPtList.size();
	for (int i = 0; i < num; i++) {
		if (stabbedEndPtList[i].isLeft == 1) {
			// This is a left endpoint.
			this->leftList.push_back(stabbedEndPtList[i].q);
		} else if (stabbedEndPtList[i].isLeft == 0) {
			// This is a right endpoint.
			this->rightList.push_front(stabbedEndPtList[i].q);
		} else {
			ShowError(
					"Error in IntervalTreeNode::IntervalTreeNode: The flag of EndptElement is not initialized!\n");
		}
	}
	this->left = NULL;
	this->right = NULL;

	TNNUM_INV++;
}

IntervalTreeNode::~IntervalTreeNode() {
	// TODO Auto-generated destructor stub
	TNNUM_INV--;
}

void IntervalTreeNode::ReleaseSpace() {
	if (this->left != NULL) {
		this->left->ReleaseSpace();
		delete (this->left);
		this->left = NULL;
	}

	if (this->right != NULL) {
		this->right->ReleaseSpace();
		delete (this->right);
		this->right = NULL;
	}

	this->leftList.clear();
	this->rightList.clear();
}

int IntervalTreeNode::InsertPoint(int value, int weight,
		MyVector<int>& alertIDList, int dimLev) {
	int cnt = 0;

	int index = 2 * (dimLev - 1);

	bool tempState = false;

	list<QueryBase*>::iterator it;
	if (value <= key) {

		// Process by the left list.
		it = this->leftList.begin();
		while (it != this->leftList.end()) {

			while ((*it)->threshold == -1) {

				// This query has alerted. Remove it from the list directly.
				it = this->leftList.erase(it);

				if (it == this->leftList.end()) {
					// The left list is retrieved.
					return cnt;
				}
			}

			// As after the erase operation, the iterator *it* will point to the next element.
			if ((*it)->range[index] <= value) {
//				InsStart = getCurrentTime();
				tempState = (*it)->InsertPoint(weight);
//				InsPtTime_Inv += getCurrentTime() - InsStart;

				if (tempState) {
					// If this query alerts after inserting this point,
					// then remove this query from the list.

					alertIDList.push_back((*it)->id);
					cnt++;

					(*it)->ReleaseSpace();
					it = this->leftList.erase(it);

				} else {
					it++;
				}

				// Increase the StabNum by one.
				StabNum++;

			} else {
				// Meet the first query whose left endpoint > value.
				break;
			}
		}

	} else {

		// Process by the right list.
		it = this->rightList.begin();
		while (it != this->rightList.end()) {

			while ((*it)->threshold == -1) {

				// This query has alerted. Remove it from the list directly.
				it = this->rightList.erase(it);

				if (it == this->rightList.end()) {
					// The right list is retrieved.
					return cnt;
				}
			}

			// As after the erase operation, the iterator *it* will point to the next element.
			if ((*it)->range[1 + index] >= value) {

//				InsStart = getCurrentTime();
				tempState = (*it)->InsertPoint(weight);
//				InsPtTime_Inv += getCurrentTime() - InsStart;

				if (tempState) {
					// If this query alerts after inserting this point,
					// then remove this query from the list.

					alertIDList.push_back((*it)->id);

					cnt++;
					(*it)->ReleaseSpace();
					it = this->rightList.erase(it);

				} else {
					it++;
				}
				// Increase the StabNum by one.
				StabNum++;
			} else {
				// Meet the first query whose right endpoint < value.
				break;
			}
		}

	}

//	end = getCurrentTime();
//	ScanTime_Inv += end - start;

	return cnt;
}

void IntervalTreeNode::ShowList(list<QueryBase*>& mylist) {
	printf("\n");
	list<QueryBase*>::iterator it = mylist.begin();
	while (it != mylist.end()) {
		printf("Query %d:\t[%d, %d]\n", (*it)->id, (*it)->range[0],
				(*it)->range[1]);
		it++;
	}
	printf("\n");
	fflush(stdout);
}

long long IntervalTreeNode::ComputeSpaceConsumption() {
	long long rtn = 0;
	rtn += sizeof(IntervalTreeNode);

	// In the STL list, each element has two points to next and previous.
	// Since each element also has a pointer of QueryBase here, the size should be 24 bytes.
	// However, according to the memory allocation of the system, each allocation is at least 32 bytes.
	// Thus, the memory consumption is 32 * number of elements here.
	rtn += 32 * leftList.size();
	rtn += 32 * rightList.size();

	return rtn;
}
