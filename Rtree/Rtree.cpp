/*
 * Rtree.cpp
 *
 *  Created on: Sep 17, 2014
 *      Author: jhgan
 */

#include "Rtree.h"

//Rtree::Rtree() {
//	this->root = NULL;
//	this->fanout = 12;
//}

Rtree::Rtree(int fanout) {
	this->root = NULL;
	this->fanout = fanout;
	this->aliveNum = 0;
}

Rtree::Rtree(vector<QueryBase*>& queryList, int fanout) {
	this->aliveNum = queryList.size();
	for (int i = 0; i < this->aliveNum; ++i) {
		this->id_pointer_map[queryList[i]->id] = queryList[i];
	}
	this->root = this->ConstructRtree(queryList, fanout);
	this->fanout = fanout;

}

Rtree::~Rtree() {
	this->ReleaseSpace();
}

void Rtree::ReleaseSpace() {
	if (this->root != NULL) {
		// Release the space of R-tree.
		this->root->ReleaseSpace();
		delete (this->root);
		this->root = NULL;
	}
	this->id_pointer_map.clear();
}

void Rtree::ReleaseSpace_All() {
	if (this->root != NULL) {
		// Release the space of R-tree.
		this->root->ReleaseSpace_All();
		delete (this->root);
		this->root = NULL;
	}
	this->id_pointer_map.clear();
}

/*
 *  Compare the Z-order of the two given points.
 *  Return:
 *  	If the Z-order of p1 is <= that of p2, return TRUE. Otherwise, return FALSE.
 */
bool comp_Zorder(const QueryBase* p1, const QueryBase* p2) {
	return compZorderByCoord(p1->range, p2->range, 2 * DIM);
}

RtreeNode* Rtree::ConstructRtree(vector<QueryBase*>& queryList, int fanout,
		int bitNum) {
	int num = queryList.size();
	if (num == 0) {
		ShowError("Error in Rtree::ConstructRtree: the query list is empty!\n");
		return NULL;
	} else if (num == 1) {
		return new RtreeNode(queryList[0]);
	}

	vector<RtreeNode*> treeNodeList;
	vector<RtreeNode*> parentList;

	/*************** Sort the Zorder directly by the coordinates of queries ****************/
	sort(queryList.begin(), queryList.end(), comp_Zorder);

	treeNodeList.resize(num);
	// Construct the leaf level.
	for (int i = 0; i < num; ++i) {
		treeNodeList[i] = new RtreeNode(queryList[i]);
	}

	int treeNodeNum = 0;
	int parentNum = 0;
	int rest = 0;

	while (treeNodeList.size() > 1) {

		treeNodeNum = treeNodeList.size();
		parentNum = ceil(treeNodeNum * 1.0 / fanout);

		parentList.resize(parentNum);

		// The size of the last group of tree nodes.
		rest = treeNodeNum - fanout * (treeNodeNum / fanout);

		for (int i = 0; i < treeNodeNum - rest; i += fanout) {
			parentList[i / fanout] = new RtreeNode();
			for (int j = 0; j < fanout; ++j) {
				parentList[i / fanout]->AddChildNode(treeNodeList[i + j]);
			}
		}

		if (rest > 0) {
			// Add the rest tree nodes to parent.
			parentList[parentNum - 1] = new RtreeNode();
			for (int i = treeNodeNum - rest; i < treeNodeNum; ++i) {
				parentList[parentNum - 1]->AddChildNode(treeNodeList[i]);
			}
		}

		treeNodeList.swap(parentList);
		parentList.clear();
	}

	if (treeNodeList.size() == 0) {
		ShowError("Failure in Rtree::ConstructRtree: level size = 0!\n");
	}

	return treeNodeList[0];
}

RtreeNode* Rtree::ChooseBestChildNode(RtreeNode* parentNode,
		RtreeNode* targetNode) {
	int bestIndex = -1;
	long long bestValue = MYINFT;
	long long p = 0;

	int childNodeNum = (int) parentNode->childNodes.size();
	for (int i = 0; i < childNodeNum; ++i) {
		p = parentNode->childNodes[i]->mbr.perimeterIncreasement(
				targetNode->mbr);
		if (bestValue > p) {
			bestValue = p;
			bestIndex = i;
		} else if (bestValue == p && bestIndex >= 0) {
			long long p1 = parentNode->childNodes[i]->mbr.computePerimeter();
			long long p2 =
					parentNode->childNodes[bestIndex]->mbr.computePerimeter();
			if (p1 < p2) {
				// If the increasement of perimeter are the same, then pick the one whose perimeter is smaller.
				bestValue = p;
				bestIndex = i;
			}
		}
	}
	return parentNode->childNodes[bestIndex];
}

void Rtree::OverflowHandler(RtreeNode* overflowNode) {
	RtreeNode* u = overflowNode->Split();

	if (overflowNode->parent != NULL) {
		if (overflowNode->parent->AddChildNode(u) > this->fanout) {
			// the parent node overflows after inserting u.
			this->OverflowHandler(overflowNode->parent);
		}
	} else {
		// overflowNode is the root of this tree.
		// then create a new root for this tree.
		RtreeNode* r = new RtreeNode();
		r->AddChildNode(overflowNode);
		r->AddChildNode(u);
		this->root = r;
	}
}

void Rtree::InsertNode_SubRoutine(RtreeNode* subroot, RtreeNode* toInsNode,
		int targetLevel) {
	if (subroot == NULL) {
		ShowError("Error in Rtree::InsertNode_SubRoutine: subroot is NULL!\n");
		return;
	}
	if (toInsNode == NULL) {
		ShowError(
				"Error in Rtree::InsertNode_SubRoutine: toInsNode is NULL!\n");
		return;
	}

	if (targetLevel >= subroot->level) {
		ShowError(
				"Error in Rtree::InsertNode_SubRoutine: targetLevel >= subroot->level!\n");
		return;
	}

	if (subroot->level == targetLevel + 1) {
		if (subroot->AddChildNode(toInsNode) > fanout) {
			// subroot overflows after this insertion. It needs to split.
			this->OverflowHandler(subroot);
		}
	} else {
		// Enlarge the mbr of subroot.
		subroot->mbr.enlarge(toInsNode->mbr);

		RtreeNode* bestChild = this->ChooseBestChildNode(subroot, toInsNode);
		this->InsertNode_SubRoutine(bestChild, toInsNode, targetLevel);
	}
}

void Rtree::InsertNode(RtreeNode* toInsNode, int targetLevel) {
	if (this->root == NULL) {
		this->root = toInsNode;
		return;
	}

	if (this->root->level == targetLevel) {
		// At this time, we need to create a new root.
		RtreeNode* u = new RtreeNode();
		u->AddChildNode(this->root);
		u->AddChildNode(toInsNode);
		this->root = u;
		return;
	} else {
		this->InsertNode_SubRoutine(this->root, toInsNode, targetLevel);
	}
}

void Rtree::InsertQuery(QueryBase* query) {
	this->id_pointer_map[query->id] = query;
	RtreeNode* u = new RtreeNode(query);
	this->InsertNode(u, 0);
	this->aliveNum++;

	// Increase the number of operations.
	OPNUM++;
}

void Rtree::DeleteQuery(QueryBase* query) {
	RtreeNode* toDelLeaf = new RtreeNode(query);

	this->DeleteLeaf(toDelLeaf);

	// release space of toDelNode
	toDelLeaf->ReleaseSpace();
	delete toDelLeaf;
	toDelLeaf = NULL;

//	this->id_pointer_map.erase(query->id);
//	this->aliveNum--;
}

void Rtree::DeleteQuery(int delID) {
	map<int, QueryBase*>::iterator it = this->id_pointer_map.find(delID);
	if (it != this->id_pointer_map.end()) {
		QueryBase* q = (*it).second;
		this->id_pointer_map.erase(it);
		this->DeleteQuery(q);
		this->aliveNum--;

		// Increase the number of operations.
		OPNUM++;
	}
}

void Rtree::DeleteLeaf(RtreeNode* toDelLeaf) {
	RtreeNode* rtnLeaf = this->FindLeaf(toDelLeaf);

	if (rtnLeaf != NULL) {
		// Finally, I decide to leave the space release of the query to be deleted to the query management outside here.
		//		//release the space of the query to be deleted.
		//		rtnNode->query->ReleaseSpace();
		//		delete (rtnNode->query);
		//		rtnNode->query = NULL;

		RtreeNode* parent = rtnLeaf->parent;
		if (parent != NULL) {
			if (parent->DeleteChildNode(rtnLeaf) < this->fanout / FanoutRatio) {
				// parent underflows.
				this->UnderflowHandler(parent);
			}
		} else {
			// rtnLeaf is the root of this tree.
			rtnLeaf->ReleaseSpace();
			delete rtnLeaf;
			rtnLeaf = NULL;
			this->root = NULL;
		}
	}
}

RtreeNode* Rtree::FindLeaf(RtreeNode* toDelLeaf) {
	return this->FindLeaf_SubRoutine(this->root, toDelLeaf);
}

RtreeNode* Rtree::FindLeaf_SubRoutine(RtreeNode* subroot,
		RtreeNode* toDelLeaf) {
	if (subroot == NULL) {
		return NULL;
	}

	if (subroot->level == 0) {
		// subroot is a leaf node.
		if (subroot->query->id == toDelLeaf->query->id)
			return subroot;
		else
			return NULL;
	}

	RtreeNode* rtn = NULL;
	int childNum = subroot->childNodes.size();
	for (int i = 0; i < childNum; ++i) {
		if (subroot->childNodes[i]->mbr.contain(toDelLeaf->mbr)) {
			// the mbr of this child node fully contains the mbr of toDelNode.
			rtn = this->FindLeaf_SubRoutine(subroot->childNodes[i], toDelLeaf);
			if (rtn != NULL) {
				return rtn;
			}
		}
	}
	return NULL;
}

void Rtree::UnderflowHandler(RtreeNode* underflowNode) {
	if (underflowNode == NULL) {
		return;
	}

	RtreeNode* parent = underflowNode->parent;
	if (parent == NULL) {
		// underflowNode is the root of this tree.

		int num = underflowNode->childNodes.size();
		if (num > 1) {
			// underflowNode is a legal root.
			return;
		} else {
			// underflowNode is an illegal root.
			if (num == 1) {
				// Let the only child of underflowNode to be the new root.
				this->root = underflowNode->childNodes[0];
				this->root->parent = NULL;

				// This step is necessary. Otherwise, underflowNode->ReleaseSpace() will delete the whole tree.
				underflowNode->childNodes.clear();
			} else {
				this->root = NULL;
			}
			underflowNode->ReleaseSpace();
			delete underflowNode;
			underflowNode = NULL;
		}
	} else {
		// underflowNode is not the root of this tree.

		// Get all the child nodes of underflowNode.
		vector<RtreeNode*> reInsList;
		reInsList.swap(underflowNode->childNodes);

		// Delete underflowNode from its parent.
		if (parent->DeleteChildNode(underflowNode) < fanout / FanoutRatio) {
			// parent underflows after deleting underflowNode.
			// Handle the underflowing event of parent.
			this->UnderflowHandler(parent);
		}

		// Reinsert all the child nodes of underflowNode.
		int reInsNum = reInsList.size();
		for (int i = 0; i < reInsNum; ++i) {
			this->InsertNode(reInsList[i], reInsList[i]->level);
		}
	}
}

void Rtree::InsertPoint(vector<int>& coords, int weight,
		MyVector<int>& alertIDList) {
	if (this->root == NULL) {
		return;
	}
	if (this->root->CheckStab(coords)) {
		// Clear the list first.
		RtreeNode::alertedLeafList.clear();

		this->root->InsertPoint(coords, weight, alertIDList);

		int alertNum = alertIDList.size();
		this->aliveNum -= alertNum;

		// Increase the number of operations.
		OPNUM++;
		OPNUM+= alertNum;

		for (int i = 0; i < alertNum; ++i) {
			// Remove the entry of the alerted queries from the map.
			this->id_pointer_map.erase(alertIDList[i]);
		}

//		this->aliveNum -= RT_List.size();
//
//		for (int i = 0; i < RT_List.size(); ++i) {
//			this->id_pointer_map.erase(RT_List[i]);
//		}

		// Delete all the alerted leaf nodes.
		int num = RtreeNode::alertedLeafList.size();
		RtreeNode* curNode = NULL;
		RtreeNode* parent = NULL;
		for (int i = 0; i < num; ++i) {
			curNode = RtreeNode::alertedLeafList[i];
			parent = curNode->parent;
			if (parent != NULL) {
				parent->DeleteChildNode(curNode);
				while (i < num - 1
						&& parent == RtreeNode::alertedLeafList[i + 1]->parent) {
					curNode = RtreeNode::alertedLeafList[i + 1];
					parent->DeleteChildNode(curNode);
					++i;
				}
				if ((int) parent->childNodes.size() < fanout / FanoutRatio) {
					// Handle the underflowing event of parent.
					this->UnderflowHandler(parent);
				}
			} else {
				// parent == NULL means curNode is the root and it needs to be deleted.
				curNode->ReleaseSpace();
				delete curNode;
				this->root = NULL;
			}

		}
	}
}

void Rtree::DeletePoint(vector<int>& coords, int weight,
		MyVector<int>& alertIDList) {
	this->InsertPoint(coords, -weight, alertIDList);
}

void Rtree::showRtree() {
	RtreeNode* curNode = this->root;
	if (curNode == NULL) {
		ShowError("Error in Rtree::showRtree: The root is NULL!\n");
	}

	int childNum = 0;

	int objID = 1;
//	bool first = true;
	int lev = 0;

//	printf("set style line 1 bgnd\n");
//	printf("set style line 2 lt rgb \"cyan\"\n");

	char rs[200] =
			"/home/jhgan/Desktop/ThresholdMonitor_Experiments/2015-10-28_GQuery_UPoint/d=2/rtree_plot";

	FILE* file = fopen(rs, "wt");
	if (file == NULL) {
		ShowError(
				"Error in function main:\nResult file open failed! Please check your file path!\n");
	}

	queue<RtreeNode*> que;
	que.push(curNode);
	while (!que.empty()) {
		curNode = que.front();
		que.pop();

//		curNode->showTreeNode();

//		if (curNode->level == 0) {
//			if (first) {
//				printf("plot \'-\' w p ls 10 lc rgb \'black\'\n");
//				first = false;
//			}
//			printf("%d, %d\n", curNode->mbr.minValues[0],
//					curNode->mbr.minValues[1]);
//		}

//		if (curNode->level >= 1) {
//			if (lev != curNode->level) {
//				printf("\n\n===========================\n");
//				printf("This is level %d\n", curNode->level);
//				lev = curNode->level;
//			}
//
//			// Print the gnuplot script.
//			printf("set object %d rect from ", objID++);
//			printf("%d", curNode->mbr.minValues[0]);
//			for (int i = 1; i < DIM; ++i) {
//				printf(", %d", curNode->mbr.minValues[i]);
//			}
//			printf(" to %d", curNode->mbr.maxValues[0]);
//			for (int i = 1; i < DIM; ++i) {
//				printf(", %d", curNode->mbr.maxValues[i]);
//			}
//			printf(" fs empty border lc rgb \'red\'\n");
////			printf("\n");
////		printf("fc lt 2\n");
//		}

		if (curNode->level == 1) {
			if (lev != curNode->level) {
//				printf("\n\n===========================\n");
//				printf("This is level %d\n", curNode->level);
				fprintf(file, "\n\n===========================\n");
				fprintf(file, "This is level %d\n", curNode->level);

				lev = curNode->level;
			}

			fprintf(file, "set object %d rect from ", objID++);
			fprintf(file, "%d", curNode->mbr.minValues[0]);
			for (int i = 1; i < DIM; ++i) {
				fprintf(file, ", %d", curNode->mbr.minValues[i]);
			}
			fprintf(file, " to %d", curNode->mbr.maxValues[0]);
			for (int i = 1; i < DIM; ++i) {
				fprintf(file, ", %d", curNode->mbr.maxValues[i]);
			}
			fprintf(file, " fs empty border lc rgb \'red\'\n");
			//			printf("\n");
			//		printf("fc lt 2\n");

			vector<RtreeNode*>& childList = curNode->childNodes;
			childNum = childList.size();
			for (int j = 0; j < childNum; ++j) {
				//			que.push(childList[i]);

				fprintf(file, "set object %d rect from ", objID++);
				fprintf(file, "%d", childList[j]->mbr.minValues[0]);
				for (int i = 1; i < DIM; ++i) {
					fprintf(file, ", %d", childList[j]->mbr.minValues[i]);
				}
				fprintf(file, " to %d", childList[j]->mbr.maxValues[0]);
				for (int i = 1; i < DIM; ++i) {
					fprintf(file, ", %d", childList[j]->mbr.maxValues[i]);
				}
				fprintf(file, " fs empty border lc rgb \'red\'\n");
				//			printf("\n");
			}
			break;

			// Print the gnuplot script.
//			printf("set object %d rect from ", objID++);
//			printf("%d", curNode->mbr.minValues[0]);
//			for (int i = 1; i < DIM; ++i) {
//				printf(", %d", curNode->mbr.minValues[i]);
//			}
//			printf(" to %d", curNode->mbr.maxValues[0]);
//			for (int i = 1; i < DIM; ++i) {
//				printf(", %d", curNode->mbr.maxValues[i]);
//			}
//			printf(" fs empty border lc rgb \'red\'\n");
//			//			printf("\n");
//			//		printf("fc lt 2\n");
		}

		vector<RtreeNode*>& childList = curNode->childNodes;
		childNum = childList.size();
		for (int j = 0; j < childNum; ++j) {
			que.push(childList[j]);
		}
	}
//	printf("e\n");
//	fprintf(file, "e\n");

	fclose(file);
}

bool Rtree::isValid() {
	return this->root != NULL;
}

void Rtree::bulkLoadRtree(vector<QueryBase*>& queryList, int fanout,
		int bitNum) {

	int num = queryList.size();
	if (num == 0) {
		ShowError("Error in Rtree::bulkLoadRtree: the point list is empty!\n");
		return;
	} else if (num == 1) {
		this->root = new RtreeNode(queryList[0]);
		return;
	}

	/*************** Codes for debug ****************/
//	set<Point*, ltstr> bst;
//	for (int i = 0; i < num; i++) {
//		bst.insert(ptList[i]);
//	}
//	set<Point*, ltstr>::iterator it = bst.find(ptList[4]);
//	while (it != bst.end()) {
//		(*it)->showCoords();
//		it--;
//	}
//
//	printf("\n");
	/*************************************************/

	queue<RtreeNode*> que, que2;
	queue<RtreeNode*>& work = que;
	queue<RtreeNode*>& store = que2;

	/*************** Sort directly by the coordinates of points ****************/

	sort(queryList.begin(), queryList.end(), comp_Zorder);
	for (int i = 0; i < num; i++) {
		que.push(new RtreeNode(queryList[i]));
	}

//	/*************** Codes for debug ****************/
//	for (int i = 0; i < num; i++) {
//		ptList[i]->showCoords();
//	}
//	printf("\n");
	/*************************************************/

	RtreeNode* parent = NULL;
	bool flag = false;
	while (work.size() + store.size() != 1) {
		parent = new RtreeNode();

		for (int i = 0; i < fanout; i++) {
			if (!work.empty()) {
				parent->AddChildNode(work.front());
				work.pop();
			} else {
				flag = true;
				break;
			}
		}

		if (flag) {
			if (parent->childNodes.size() > 0) {
				store.push(parent);
			} else {
				delete (parent);
			}
			std::swap(work, store);
			flag = false;
		} else {
			store.push(parent);
		}
	}
	if (work.empty())
		this->root = store.front();
	else
		this->root = work.front();
}

int Rtree::GetAliveNum() {
	return this->aliveNum;
}

long long Rtree::ComputeSpaceConsumption() {
	long long spaceCons = 0;

	spaceCons += sizeof(Rtree);

	// Each element in this map takes 64 bytes.
	spaceCons += 64 * this->id_pointer_map.size();

	// The space of query list.
	spaceCons += (sizeof(QueryBase) + 2 * DIM * sizeof(int)) * this->aliveNum;

	if (this->root != NULL) {
		spaceCons += this->root->ComputeSpaceConsumption();
	}

	return spaceCons;
}

