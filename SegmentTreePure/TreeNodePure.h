/*
 * TreeNodePure.h
 *
 *  Created on: 15 Sep, 2015
 *      Author: jhgan
 */

#ifndef TREENODEPURE_H_
#define TREENODEPURE_H_

#include "../headers.h"
#include "../MyVector/MyVector.h"
#include "../QueryBase.h"

#include <vector>
using namespace std;

/*
 *  The class of tree node of pure segment tree.
 */
class TreeNodePure {
public:
	TreeNodePure();

	/*
	 *  The constructor of internal node.
	 */
	TreeNodePure(TreeNodePure* left, TreeNodePure* right);

	/*
	 *  The constructor of leaf.
	 */
	TreeNodePure(int minEndpt, int maxEndpt);

	/*
	 *  The constructor of tree node.
	 */
//	TreeNodePure(int minEndpt, int maxEndpt, int leftIndex = -1,
//			int rightIndex = -1);

//	TreeNodePure(int minEndpt, int maxEndpt, TreeNodePure* left, TreeNodePure* right);

	~TreeNodePure();

	/*
	 *  The type of the structurePtr in this tree node.
	 *  If ptrType = 1, structurePtr is a pointer of vector<void*> which is the list of queries stored in this node.
	 *  If ptrType = 2, structurePtr is a pointer of SegmentTree which is the segment tree at the next level.
	 *  If ptrType = 3, structurePtr is a pointer of IntervalTree which is the interval tree at the last dimensionality level.
	 */
	int ptrType;


	/*
	 *  The left endpoint of the range corresponds to this node.
	 */
	int minEndpt;

	/*
	 *  The right endpoint of the range corresponds to this node.
	 */
	int maxEndpt;

	/*
	 *  The pointer of left child.
	 */
	TreeNodePure* left;

	/*
	 *  The pointer of right child.
	 */
	TreeNodePure* right;

	/*
	 *  The pointer of the structure stored in this node.
	 */
	void* structurePtr;

	/***************************************************************
	 *  			Tree structure related.
	 ***************************************************************/

	/*
	 *  Construct the next level segment tree on the list of queries covering this node.
	 *  Parameter List:
	 *  	nextLev:		the next level of dimensionality.
	 */
	void ConstructNextLevelTree(int nextLev);

	/*
	 *	Check whether the given range covers the range of this node.
	 *	Parameter List:
	 *		min:	the min endpoint of the given range.
	 *		max:	the max endpoint of the given range.
	 *	Return:
	 *		If the given range covers the range of this node, then return TRUE. Otherwise, return FALSE.
	 */
	bool CheckCover(int min, int max);

	/*
	 *  Check whether the given range intersects with the range of this node.
	 *  Parameter List:
	 *  	min:   the min endpoint of the given range.
	 *  	max:   the max endpoint of the given range.
	 *  Return:
	 *  	If the given range intersects with the range of this node, then return TRUE. Otherwise, return FALSE.
	 */
	bool CheckIntersect(int min, int max);

	/*
	 *  Check whether the given value stabs the range of this node.
	 *  Parameter List:
	 *  	value:	the given value.
	 *  Return:
	 *  	If Yes, return TRUE. Otherwise, return FALSE.
	 */
	bool CheckStab(int value);

	/*
	 *  Print out the info of this node.
	 */
	void ShowInfo();

	/*
	 *  Print out the range of this node.
	 */
	void ShowRange();

	/*
	 *  Release the space of this tree node.
	 */
	void ReleaseSpace();

	/*
	 *  Insert a point to this node virtually.
	 *  Parameter List:
	 *  	coords:				the coordinates of the given point.
	 *  	dimLev:				the level of dimensionality of this node.
	 *  	weight:				the weight of the given point.
	 *  Return:
	 *  	the number of queries alerted by inserting this point.
	 */
	int InsertPoint(int* coords, int dimLev, int weight,
			MyVector<int>& alertIDList);

	/*
	 *  Insert a point to this node virtually.
	 *  Return:
	 *  	the number of queries alerted by inserting this point.
	 */
	int InsertPoint(int weight, MyVector<int>& alertIDList);

	/*
	 *  Insert a query associating this node.
	 *  Parameter List:
	 *  	query:		the pointer of the given query.
	 *  	dimLev:		the level of the dimensionality of this node.
	 */
	void InsertQuery(QueryBase* query, int dimLev);

	/*
	 *  Delete the element at position of *index* from the list.
	 *  Parameter List:
	 *  	index:		the specified index of the element to be deleted.
	 *  Return:
	 *  	*index* - 1 the position of the element right before the element just deleted.
	 */
	int DeleteFromList(int index);

	/*
	 *  Compute the space consumption of this tree node.
	 */
	long long ComputeSpaceConsumption();
};

#endif /* TREENODEPURE_H_ */
