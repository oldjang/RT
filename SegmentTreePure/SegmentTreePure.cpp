/*
 * SegmentTreePure.cpp
 *
 *  Created on: 15 Sep, 2015
 *      Author: jhgan
 */

#include "SegmentTreePure.h"
#include "../QueryBase.h"

SegmentTreePure::SegmentTreePure() {
	// TODO Auto-generated constructor stub
	this->root = NULL;
	this->dimLev = 1;
	TNUM_SEG++;
}

SegmentTreePure::SegmentTreePure(MyVector<QueryBase*>& queryList, int dimLev) {

	int queryNum = queryList.size();

	this->dimLev = dimLev;
	this->root = this->ConstructSegmentTree(queryList, dimLev);

//	start = getCurrentTime();
	// Insert all the queries to this tree.
	for (int i = 0; i < queryNum; ++i) {
		this->InsertQuery((queryList[i]));
	}
//	end = getCurrentTime();
//	InsQTime_Seg += end - start;

	if (dimLev < DIM) {
		// Recursively construct segment tree.
		this->ConstructNextLevSubTree(this->root, dimLev + 1);
	}

	TNUM_SEG++;
}

SegmentTreePure::~SegmentTreePure() {
	// TODO Auto-generated destructor stub
	TNUM_SEG--;
}

void SegmentTreePure::ReleaseSpace() {

	this->ReleaseSpace_SubRoutine(this->root);
}

void SegmentTreePure::ReleaseSpace_SubRoutine(TreeNodePure* subroot) {
	if (subroot != NULL) {
		subroot->ReleaseSpace();

		this->ReleaseSpace_SubRoutine(subroot->left);
		this->ReleaseSpace_SubRoutine(subroot->right);

		delete subroot;
		subroot = NULL;
	}
}

void ShowSubtreeInfo(TreeNodePure* subroot) {
	if (subroot == NULL)
		return;
	subroot->ShowInfo();
	ShowSubtreeInfo(subroot->left);
	ShowSubtreeInfo(subroot->right);
}

void SegmentTreePure::ShowInfo() {
	ShowSubtreeInfo(this->root);
	fflush(stdout);
}

void SegmentTreePure::ConstructNextLevSubTree(TreeNodePure* subroot,
		int nextLev) {
	if (subroot != NULL) {
		// Construct for subroot.
		subroot->ConstructNextLevelTree(nextLev);

		// Recursively construct on the child nodes.
		this->ConstructNextLevSubTree(subroot->left, nextLev);
		this->ConstructNextLevSubTree(subroot->right, nextLev);
	}
}

TreeNodePure* SegmentTreePure::ConstructSegmentTree(
		MyVector<QueryBase*>& queryList, int dimLev) {
	vector<int> endPtList;
	int queryNum = queryList.size();
	QueryBase* query = NULL;

	// Obtain the endpoint list of corresponding dimension.
	for (int i = 0; i < queryNum; ++i) {
		query = (QueryBase*) (queryList[i]);
		endPtList.push_back(query->range[2 * (dimLev - 1)]);

		// Note that we set the endpoint 1 larger.
		endPtList.push_back(query->range[2 * (dimLev - 1) + 1] + 1);
	}

	TreeNodePure* rtn = this->ConstructSegmentTree(endPtList);
	endPtList.clear();
	return rtn;
}

TreeNodePure* SegmentTreePure::ConstructSegmentTree(vector<int>& endPtList) {
	int ptNum = endPtList.size();

	// Sort all the endpoints.
	sort(endPtList.begin(), endPtList.end());

	vector<TreeNodePure*> treeNodeList;
	vector<TreeNodePure*> parentList;

	/*
	 *  This part is for the binary search tree.
	 */
	for (int i = 0; i < ptNum - 1; ++i) {
		// Skip all the duplicate values.
		while (i < ptNum - 1 && endPtList[i] == endPtList[i + 1]) {
			++i;
		}
		if (i < ptNum - 1) {
			treeNodeList.push_back(
					new TreeNodePure(endPtList[i], endPtList[i + 1] - 1));
		}
	}
	// Create a leaf node for the last endpoint.
	treeNodeList.push_back(
			new TreeNodePure(endPtList[ptNum - 1], endPtList[ptNum - 1]));

	int treeNodeNum = 0;
	bool isOdd = false;

	while (treeNodeList.size() > 1) {

		treeNodeNum = treeNodeList.size();

		if (treeNodeNum % 2 != 0) {
			// if the size of the tree node list is not even, then ignore the last node.
			isOdd = true;
			treeNodeNum--;
			parentList.resize((treeNodeNum >> 1) + 1);
		} else {
			isOdd = false;
			parentList.resize(treeNodeNum >> 1);
		}

		for (int i = 0; i < treeNodeNum - 1; i += 2) {
			parentList[(i >> 1)] = new TreeNodePure(treeNodeList[i],
					treeNodeList[i + 1]);
		}
		if (isOdd) {
			// This is only for the case of isOdd == true;
			// Add the last node directly to parentList.
			int lastIndex = (treeNodeNum >> 1);
			parentList[lastIndex] = treeNodeList[treeNodeNum];
		}

		treeNodeList.swap(parentList);
		parentList.clear();
	}

	if (treeNodeList.size() == 0) {
		ShowError(
				"Error in SegmentTreePure::ConstructSegmentTree: level size = 0!\n");
	}

	return treeNodeList[0];
}

void SegmentTreePure::InsertQuery_SubRoutine(TreeNodePure* subroot,
		QueryBase* query, int min, int max) {
	if (subroot == NULL)
		return;

	if (subroot->CheckCover(min, max)) {
		// the range of query covers the range of node.
		subroot->InsertQuery(query, this->dimLev);
		return;
	} else {

		TreeNodePure* left = subroot->left;
		if (left != NULL
				&& (left->CheckIntersect(min, max) || left->CheckCover(min, max))) {
			// the range of query intersects with the range of left.
			this->InsertQuery_SubRoutine(left, query, min, max);
		}

		TreeNodePure* right = subroot->right;
		if (right != NULL
				&& (right->CheckIntersect(min, max)
						|| right->CheckCover(min, max))) {
			// the range of query intersects with the range of right.
			this->InsertQuery_SubRoutine(right, query, min, max);
		}
	}
}

void SegmentTreePure::InsertQuery(QueryBase* query) {

	if (this->root != NULL) {
//		QueryBase* q = (QueryBase*) query;
		int min = query->range[2 * (this->dimLev - 1)];
		int max = query->range[2 * (this->dimLev - 1) + 1];
		this->InsertQuery_SubRoutine(this->root, query, min, max);
	}

}

int SegmentTreePure::InsertPoint(int* coords, int weight,
		MyVector<int>& alertIDList) {
//	return this->InsertPoint_Array(coords, weight, alertIDList);

	int cnt = 0;

	if (this->root != NULL) {
		// The coordinate value of the given point in the $dimLev$-th dimension.
		int value = coords[this->dimLev - 1];
		TreeNodePure* node = this->root;

		if (node != NULL && node->CheckStab(value)) {
			while (node != NULL) {

				// Insert the point to node.
				cnt += node->InsertPoint(coords, this->dimLev, weight,
						alertIDList);

				if (node->left != NULL && node->left->CheckStab(value)) {
					node = node->left;
				} else {
					// If node->left == NULL, then node->right == NULL as well.
					// If node->left is not stabbed by value, since value stabs node, value must stab node->right.
					node = node->right;
				}
			}
		}
	}

	return cnt;
}

int SegmentTreePure::DeletePoint(int* coords, int weight,
		MyVector<int>& alertIDList) {
	return this->InsertPoint(coords, -weight, alertIDList);
}

SegmentTreePure* SegmentTreePure::ConstructStr(
		MyVector<QueryBase*>& queryList) {
//	double start = getCurrentTime();
	SegmentTreePure* rtn = new SegmentTreePure(queryList);
//	double end = getCurrentTime();
//	CONSTIME_SEG += end - start;
	return rtn;
}

void SegmentTreePure::DeleteQuery(QueryBase& query) {
	query.threshold = -1;
}

long long SegmentTreePure::ComputeSpaceConsumption() {
	long long rtn = 0;
	rtn += sizeof(SegmentTreePure);

	rtn += this->ComputeSpaceConsumption_SubRoutine(this->root);

	return rtn;
}

long long SegmentTreePure::ComputeSpaceConsumption_SubRoutine(
		TreeNodePure* subroot) {
	if (subroot == NULL) {
		return 0;
	}

	long long rtn = 0;
	rtn += subroot->ComputeSpaceConsumption();
	rtn += ComputeSpaceConsumption_SubRoutine(subroot->left);
	rtn += ComputeSpaceConsumption_SubRoutine(subroot->right);

	return rtn;
}
