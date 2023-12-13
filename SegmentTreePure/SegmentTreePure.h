/*
 * SegmentTreePure.h
 *
 *  Created on: 15 Sep, 2015
 *      Author: jhgan
 */

#ifndef SEGMENTTREEPURE_H_
#define SEGMENTTREEPURE_H_

#include "../headers.h"
#include "TreeNodePure.h"
#include "../QueryBase.h"
#include "../Logarithmic/AbsStructure.h"

#include "../MyVector/MyVector.h"

#include <vector>
#include <algorithm>
using namespace std;

/*
 *  The class of pure segment tree.
 */
class SegmentTreePure {
public:
	SegmentTreePure();

	/*
	 * 	The constructor of SegmentTree.
	 * 	Parameter List:
	 * 		queryList:		the list of pointers of queries.
	 * 		dimLev:			the level of dimensionality.
	 */
	SegmentTreePure(MyVector<QueryBase*>& queryList, int dimLev = 1);

	~SegmentTreePure();

	/*
	 *  The root of this tree.
	 */
	TreeNodePure* root;

	/*
	 *  The level of dimensionality, namely, which dimension this segment tree is built on.
	 *  dimLev is in the range of [1, DIM].
	 */
	int dimLev;


	/*
	 *  Print out the info of this tree.
	 */
	void ShowInfo();

	/*
	 * 	Insert a query to this segment tree.
	 * 	Parameter List:
	 * 		query:		the pointer of the specified query.
	 */
	void InsertQuery(QueryBase* query);

	/*
	 *  Insert a point to this segment tree.
	 *  Parameter List:
	 *  	coords:		the coordinates of the given point.
	 *  	weight:		the weight of the given point.
	 *  Return:
	 *  	the number of queries alerted by inserting this point.
	 */
	int InsertPoint(int* coords, int weight, MyVector<int>& alertIDList);

	/*
	 *  Delete the specified point from this tree.
	 *  Parameter List:
	 *  	coords:		the coordinates of the given point.
	 *  	weight:		the weight of the given point which is > 0.
	 *  Return:
	 *  	the number of queries alerted by deleting this point.
	 */
	int DeletePoint(int* coords, int weight, MyVector<int>& alertIDList);

	/*
	 *  Release the space of this segment tree.
	 */
	void ReleaseSpace();

	void ReleaseSpace_SubRoutine(TreeNodePure* subroot);


	/*
	 *  Construct the structure for the given query list.
	 *  Parameter List:
	 *  	queryList:		the given query list.
	 *  Return:
	 *  	the pointer of resulted structure.
	 */
	SegmentTreePure* ConstructStr(MyVector<QueryBase*>& queryList);

	/*
	 *  Delete the specified query from this structure.
	 */
	void DeleteQuery(QueryBase& query);

	/*
	 *  Compute the space consumption of this tree.
	 */
	long long ComputeSpaceConsumption();

	/*
	 *  Compute the space consumption of the specified subtree.
	 */
	long long ComputeSpaceConsumption_SubRoutine(TreeNodePure* subroot);

protected:
	/*
	 *  Construct a segment tree by bulk loading the given endpoint list.
	 *  Parameter List:
	 *  	endPtList:		the list of all the endpoints.
	 *  Return:
	 *  	the root of the resulting segment tree.
	 */
	TreeNodePure* ConstructSegmentTree(vector<int>& endPtList);

	/*
	 *  Construct a segment tree by bulk loading the given query list.
	 *  Parameter List:
	 *  	queryList:		the given query list.
	 *  	dimLev:			the level of dimensionality.
	 *  Return:
	 *  	the root of the resulting segment tree.
	 */
	TreeNodePure* ConstructSegmentTree(MyVector<QueryBase*>& queryList,
			int dimLev);

	/*
	 *  Construct a next level segment tree for the each node in the subtree rooted at subroot.
	 *  Parameter List:
	 *		subroot:		the given subtree root.
	 *		nextLev:		the next level of dimensionality.
	 */
	void ConstructNextLevSubTree(TreeNodePure* subroot, int nextLev);

	/*
	 *  Insert the query to subtree.
	 *  Parameter List:
	 *  	subroot:		the pointer of the subtree root.
	 *  	query:			the pointer of the given query.
	 *  	min:			the min of the range of the specified dimension of the query.
	 *  	max:			the max of the range of the specified dimension of the query.
	 */
	void InsertQuery_SubRoutine(TreeNodePure* subroot, QueryBase* query,
			int min, int max);

	/*
	 *  Insert the point to subtree.
	 *  Parameter List:
	 *  	subroot:		the pointer of the subtree root.
	 *  	value:			the coordinate value of the specified dimension.
	 */
	void InsertPoint_SubRoutine(TreeNodePure* subroot, int value);

};

#endif /* SEGMENTTREEPURE_H_ */
