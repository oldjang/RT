/*
 * IntervalTreeNode.h
 *
 *  Created on: 15 Sep, 2015
 *      Author: jhgan
 */

#ifndef INTERVALTREENODE_H_
#define INTERVALTREENODE_H_

#include "../QueryBase.h"

#include "../MyVector/MyVector.h"

#include <vector>
#include <list>
using namespace std;

struct EndPtElement {

	/*
	 *  The flag of whether this endpoint element is of left endpoint.
	 *  If Yes, this value = 1.
	 *  If this is an element of right endpoint, this value = 0.
	 *  In other cases, this value = -1.
	 */
	int isLeft;

	/*
	 *  One of the endpoints of query q;
	 */
	int endpt;

	/*
	 *  The pointer of query q;
	 */
	QueryBase* q;

	EndPtElement() {
		this->endpt = 0;
		this->q = NULL;
		this->isLeft = -1;
	}

	EndPtElement(QueryBase* q, int endpt, int isLeft) {
		this->isLeft = isLeft;
		this->endpt = endpt;
		this->q = q;
	}

	EndPtElement(const EndPtElement& e) {
		this->endpt = e.endpt;
		this->q = e.q;
		this->isLeft = e.isLeft;
	}

	~EndPtElement(){

	}

	bool operator <(const EndPtElement& e2) const {
		return this->endpt < e2.endpt;
	}
};

/*
 *  The class of tree node of interval tree.
 */
class IntervalTreeNode {
public:
	IntervalTreeNode();

	/*
	 * 	The constructor of an interval tree node.
	 * 	Parameter List:
	 * 		key:			the key value of this node.
	 * 		endPtList:		the list of endpoint elements of the queries stabbed by the key of this node.
	 */
	IntervalTreeNode(int key, vector<EndPtElement>& stabbedEndPtList);

	~IntervalTreeNode();

	/*
	 *  Insert a point to this node.
	 *  Parameter List:
	 *  	value:		the given point.
	 *  	weight:		the weight of the given point.
	 *  Return:
	 *  	the number of queries alerted by inserting this point.
	 */
	int InsertPoint(int value, int weight, MyVector<int>& alertIDList, int dimLev);

	/*
	 *  Release the space of this tree node.
	 */
	void ReleaseSpace();

	/*
	 *  Show the content of the given list.
	 */
	void ShowList(list<QueryBase*>& mylist);

	/*
	 *  Compute the space consumption of this tree node.
	 */
	long long ComputeSpaceConsumption();

	/*
	 *  The key value of this node.
	 */
	int key;

	/*
	 *  The pointer of the left child node.
	 */
	IntervalTreeNode* left;

	/*
	 *  The pointer of the right child node.
	 */
	IntervalTreeNode* right;

	/*
	 *  The list of queries stabbed by key and sorted by *increasing* order of left endpoints.
	 *  It should be noticed that we use std::list here rather than std::vector because we need to
	 *  frequently remove query from the list.
	 */
	list<QueryBase*> leftList;

	/*
	 *  The list of queries stabbed by key and sorted by *decreasing* order of right endpoints.
	 */
	list<QueryBase*> rightList;
};

#endif /* INTERVALTREENODE_H_ */
