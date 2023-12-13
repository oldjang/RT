/*
 * RtreeNode.cpp
 *
 *  Created on: Sep 17, 2014
 *      Author: jhgan
 */

#include "RtreeNode.h"

vector<RtreeNode*> RtreeNode::alertedLeafList;

/*
 *  axisIndex is in the range of [0, DIM - 1].
 */
int axisIndex = 0;
/*
 *  Compare two given tree node pointers by the specified axisIndex.
 */
bool CompRtreeNodeLess(RtreeNode* u1, RtreeNode* u2) {
	double c1 = (u1->mbr.maxValues[axisIndex] + u1->mbr.minValues[axisIndex])
			* 0.5;
	double c2 = (u2->mbr.maxValues[axisIndex] + u2->mbr.minValues[axisIndex])
			* 0.5;
	return c1 < c2;
}

RtreeNode::RtreeNode() {
	// Means that this node currently is empty and invalid.
	this->level = -1;
	this->query = NULL;
	this->parent = NULL;
}

RtreeNode::RtreeNode(QueryBase* query) {
	this->level = 0;
	this->query = query;
	this->parent = NULL;

	int* minValues = new int[DIM];
	int* maxValues = new int[DIM];

	for (int i = 0; i < DIM; ++i) {
		minValues[i] = query->range[2 * i];
		maxValues[i] = query->range[2 * i + 1];
	}

	// Here copy the content of the arrays.
	this->mbr.setMinCoords(minValues);
	this->mbr.setMaxCoords(maxValues);

	delete[] minValues;
	delete[] maxValues;
}

RtreeNode::~RtreeNode() {
	this->ReleaseSpace();
}

void RtreeNode::ReleaseSpace() {
	this->mbr.ReleaseSpace();

	int num = this->childNodes.size();
	if (num == 0)
		return;
	for (int i = 0; i < num; i++) {
		this->childNodes[i]->ReleaseSpace();
		delete (this->childNodes[i]);
		this->childNodes[i] = NULL;
	}
	this->childNodes.clear();
}

void RtreeNode::ReleaseSpace_All() {
	this->mbr.ReleaseSpace();
	if (this->query != NULL) {
		this->query->ReleaseSpace();
		delete (this->query);
		this->query = NULL;
	}

	int num = this->childNodes.size();
	if (num == 0)
		return;
	for (int i = 0; i < num; i++) {
		this->childNodes[i]->ReleaseSpace_All();
		delete (this->childNodes[i]);
		this->childNodes[i] = NULL;
	}
	this->childNodes.clear();
}

long long RtreeNode::ComputeSpaceConsumption() {
	long long spaceCons = 0;

	spaceCons += sizeof(RtreeNode);
	spaceCons += this->mbr.ComputeSpaceConsumption();

	// The pointers of stored in childNodes.
	spaceCons += sizeof(void*) * this->childNodes.capacity();

	int num = this->childNodes.size();
	for (int i = 0; i < num; ++i) {
		spaceCons += this->childNodes[i]->ComputeSpaceConsumption();
	}

	return spaceCons;
}

long long RtreeNode::ComputeGroupPerimeter(vector<RtreeNode*>& treeNodeList,
		int start, int length) {
	if (start < 0 || start + length > (int) treeNodeList.size() || length < 1) {
		ShowError(
				"Error in RtreeNode::ComputeGroupPerimeter: The given index and length are invalid!\n");
		return -1;
	}

	long long rtn = 0;

	int* minList = new int[DIM];
	int* maxList = new int[DIM];

	for (int k = 0; k < DIM; ++k) {
		minList[k] = this->childNodes[start]->mbr.minValues[k];
		maxList[k] = this->childNodes[start]->mbr.maxValues[k];
	}

	int end = start + length;
	for (int j = start + 1; j < end; ++j) {
		for (int k = 0; k < DIM; ++k) {
			minList[k] = min(minList[k], this->childNodes[j]->mbr.minValues[k]);
			maxList[k] = max(maxList[k], this->childNodes[j]->mbr.maxValues[k]);
		}
	}

	// Compute the perimeter of the mbr defined by minList and maxList.
	for (int i = 0; i < DIM; ++i) {
		rtn += (maxList[i] - minList[i]);
	}
	rtn *= 2;

	delete[] minList;
	delete[] maxList;

	return rtn;
}

RtreeNode * RtreeNode::Split() {
	int childNodeNum = this->childNodes.size();
	int half = childNodeNum / 2;
	int rest = childNodeNum - half;
	long long p1 = 0;
	long long p2 = 0;
	long long bestValue = MYINFT;
	int bestIndex = -1;

	// Find the best split axis.
	for (int i = 0; i < DIM; ++i) {
		// sort the child nodes on each dimension coordinate.
		axisIndex = i;
		sort(this->childNodes.begin(), this->childNodes.end(),
				CompRtreeNodeLess);
		p1 = this->ComputeGroupPerimeter(this->childNodes, 0, half);
		p2 = this->ComputeGroupPerimeter(this->childNodes, half, rest);

		if (bestValue > p1 + p2) {
			// The smaller value the better.
			bestValue = p1 + p2;
			bestIndex = axisIndex;
		}
	}

	// Split this node by bestIndex.
	axisIndex = bestIndex;
	sort(this->childNodes.begin(), this->childNodes.end(), CompRtreeNodeLess);

	vector<RtreeNode*> childList;
	childList.swap(this->childNodes);

	// Reset this node.
	this->childNodes.clear();
	this->mbr.setByRectangle(childList[0]->mbr);
	for (int i = 0; i < half; ++i) {
		this->AddChildNode(childList[i]);
	}

	// Create a new node u.
	RtreeNode* u = new RtreeNode();
	u->parent = this->parent;
	u->level = this->level;
	u->mbr.setByRectangle(childList[half]->mbr);
	for (int i = half; i < childNodeNum; ++i) {
		u->AddChildNode(childList[i]);
	}
	return u;
}

bool RtreeNode::isLeaf() {
	return this->level == 0;
}

bool RtreeNode::CheckStab(vector<int>& coords) {
	return this->mbr.CheckStab(coords);
}

void RtreeNode::InsertPoint(vector<int>& coords, int weight,
		MyVector<int>& alertIDList) {

	if (this->level == 0) {
		// The root is the only one node in the tree.
		if (this->query->InsertPoint(weight)) {
			// This query alerts.
//#ifdef CHECK
//			// Just for test.
//			// Add ID to AlertIDList5.
//			RT_List.push_back(this->query->id);
//			//========================
//#endif
			alertIDList.push_back(this->query->id);

			// Delete this query.
			this->query->ReleaseSpace();
			delete (this->query);
			this->query = NULL;

			// Add the alerted leaf node to the list.
			RtreeNode::alertedLeafList.push_back(this);
		}
	}

	if (this->level == 1) {
		// This node is a parent node of leaf nodes.

		for (int i = 0; i < (int) this->childNodes.size(); ++i) {
			if (this->childNodes[i]->CheckStab(coords)) {
				if (this->childNodes[i]->query->InsertPoint(weight)) {
					// The query in this child leaf node alerts.
//#ifdef CHECK
//					// Just for test.
//					// Add ID to AlertIDList5.
//					RT_List.push_back(this->childNodes[i]->query->id);
//					//========================
//#endif

					alertIDList.push_back(this->childNodes[i]->query->id);

					// Delete this query.
					this->childNodes[i]->query->ReleaseSpace();
					delete (this->childNodes[i]->query);
					this->childNodes[i]->query = NULL;

					// Add the alerted leaf node to the list.
					RtreeNode::alertedLeafList.push_back(this->childNodes[i]);

//					// Good version on 2015-10-06.
//					// Release the space of this leaf node.
//					this->childNodes[i]->ReleaseSpace();
//					delete (this->childNodes[i]);
//
//					// swap this child node with the last child node.
//					this->childNodes[i] =
//							this->childNodes[(int) this->childNodes.size() - 1];
//
//					// delete the last element from the child node list.
//					this->childNodes.pop_back();
//
//					// In order to recheck the "new" element at the index i.
//					--i;
				}
			}
		}
	} else {
		// This node is not a parent node of leaf nodes.
		for (int i = 0; i < (int) this->childNodes.size(); ++i) {

			if (this->childNodes[i]->CheckStab(coords)) {
				this->childNodes[i]->InsertPoint(coords, weight, alertIDList);
			}

//			// Good version on 2015-10-06
//			if (this->childNodes[i]->childNodes.size() == 0) {
//				// This child node is empty.
//
//				// Release the space of this empty child node.
//				this->childNodes[i]->ReleaseSpace();
//				delete (this->childNodes[i]);
//
//				// Swap this child node with the last child node.
//				this->childNodes[i] =
//						this->childNodes[(int) this->childNodes.size() - 1];
//
//				// Delete the last element from the child node list.
//				this->childNodes.pop_back();
//
//				// In order to recheck the "new" element at the index i.
//				--i;
//			} else {
//				if (this->childNodes[i]->CheckStab(coords)) {
//					this->childNodes[i]->InsertPoint(coords, weight);
//				}
//			}
		}
	}
}

int RtreeNode::AddChildNode(RtreeNode* child) {
// Update the MBR for the parent node.
	if (this->level == -1) {
		this->mbr.setByRectangle(child->mbr);
		this->level = child->level + 1;
	}

	this->mbr.enlarge(child->mbr);
	this->childNodes.push_back(child);

	// Record the pointer of parent node.
	child->parent = this;

	return (int) this->childNodes.size();
}

int RtreeNode::DeleteChildNode(RtreeNode* child) {
	int num = this->childNodes.size();
	if (num == 0) {
		ShowError(
				"Error in RtreeNode::deleteChildNode: The child node list is empty!\n");
	}
	for (int i = 0; i < num; ++i) {
		if (this->childNodes[i] == child) {
			// delete this child node
			this->childNodes[i]->ReleaseSpace();
			delete this->childNodes[i];
			// Move the last child node to index i and pop_back.
			this->childNodes[i] = this->childNodes[num - 1];
			this->childNodes[num - 1] = NULL;
			this->childNodes.pop_back();
			break;
		}
	}
	return (int) this->childNodes.size();
}

void RtreeNode::showTreeNode() {
	printf("========================================================\n");
	printf("Level: %d\n", this->level);
	this->mbr.showRectangle();
}

