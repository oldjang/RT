/*
 * IntervalTree.h
 *
 *  Created on: 15 Sep, 2015
 *      Author: jhgan
 */

#ifndef INTERVALTREE_H_
#define INTERVALTREE_H_

#include "IntervalTreeNode.h"
#include "../QueryBase.h"
#include "../Logarithmic/AbsStructure.h"

#include "../MyVector/MyVector.h"

#include <vector>
#include <algorithm>
using namespace std;

/*
 *  The class of interval tree.
 */
class IntervalTree {
public:
	IntervalTree();

	/*
	 *  The constructor of IntervalTree.
	 *  Parameter List:
	 *  	queryList:		the list of pointers of basic queries.
	 */
	IntervalTree(MyVector<QueryBase*>& queryList, int dimLev = 1);

	~IntervalTree();

	/*
	 *  The root of this tree.
	 */
	IntervalTreeNode* root;

	/*
	 *  Insert a point to this interval tree.
	 *  Parameter List:
	 *  	value:		the point value.
	 *  	weight:		the weight of the given point.
	 *  Return:
	 *  	the number of queries alerted by inserting this point.
	 */
	int InsertPoint(int value, int weight, MyVector<int>& alertIDList, int dimLev = 1);

	/*
	 *  Insert a point to this tree.
	 *  Parameter List:
	 *  	coords:		the vector with only one integer
	 *  	weight:		the weight of the given point.
	 *  Return:
	 *  	the number of queries alerted by inserting this point.
	 */
	int InsertPoint(int* coords, int weight, MyVector<int>& alertIDList, int dimLev = 1);

	/*
	 *  Delete the specified point from this tree.
	 *  Parameter List:
	 *  	coords:		the vector with only one integer
	 *  	weight:		the weight of the given point which is > 0.
	 *  Return:
	 *  	the number of queries alerted by deleting this point.
	 */
	int DeletePoint(int* coords, int weight, MyVector<int>& alertIDList, int dimLev = 1);

	/*
	 *  Release the space of this tree.
	 */
	void ReleaseSpace();

	/*
	 *  Delete the specified query from this structure.
	 */
	void DeleteQuery(QueryBase* query);

	int ComputeTreeSize();

	int ComputeTreeSize_SubRoutine(IntervalTreeNode* subroot);

	/*
	 *  Compute the space consumption of this structure.
	 */
	long long ComputeSpaceConsumption();

	long long ComputeSpaceConsumption_SubRoutine(IntervalTreeNode* subroot);

protected:
	/*
	 *  Construct an interval tree for the given list of endpoint elements.
	 *  Parameter List:
	 *  	endPtList:		the given list of endpoint elements.
	 *  Return:
	 *  	the root of the resulted interval tree.
	 */
	IntervalTreeNode* ConstructIntervalTree(vector<EndPtElement>& endPtList,
			int dimLev);

private:
	/*
	 *  Generate the list of endpoint elements for the given query list.
	 *  Parameter List:
	 *  	queryList:		the given list of queries.
	 *  	targetPlace:	the target place to store the endpoint elements.
	 *  	dimLev:			the dimensionality level.
	 */
	void GenerateEndPtElementList(MyVector<QueryBase*>& queryList,
			vector<EndPtElement>& targetPlace, int dimLev);

	/*
	 *  Pick the median value among all the DISTINCT endpoints.
	 *  Parameter List:
	 *  	endPtList:		the list of endpoint elements.
	 *  Return:
	 *  	the median value among all the DISTINCT endpoint values in the given list.
	 */
	int PickKey(vector<EndPtElement>& endPtList);

	/*
	 *  Assign intervals into three parts in the form of endpoint element list:
	 *  	1. Completely to the left of key.
	 *  	2. Stabbed by key.
	 *  	3. Completely to the right of key.
	 *  Parameter List:
	 *  	key:			the key value
	 *  	endPtList:		the list of endpoint elements
	 *  	leftPart:		the target place to store the first part.
	 *  	stabbedSet:		the target place to store the second part.
	 *  	rightPart:		the target place to store the third part.
	 */
	void AssignIntervals(int key, vector<EndPtElement>& endPtList,
			vector<EndPtElement>& leftPart,
			vector<EndPtElement>& stabbedSet,
			vector<EndPtElement>& rightPart, int dimLev);
};

#endif /* INTERVALTREE_H_ */
