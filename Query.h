/*
 * Query.h
 *
 *  Created on: 7 Sep, 2015
 *      Author: jhgan
 */

#ifndef QUERY_H_
#define QUERY_H_

#include "QueryBase.h"
//#include "SegmentTree.h"
//#include "./SegmentTree/TreeNode.h"
#include "./MyVector/MyVector.h"
//#include "./MyHeap/MinHeap.h"

#include <math.h>
#include <vector>
using namespace std;

/*
 *  The class of query.
 */
class Query: public QueryBase {
public:

	Query();

	/*
	 *  The constructor of Query.
	 *  Parameter List:
	 *  	id:			the unique id of the query.
	 *  	range:		the given query DIM-dimensional range.
	 *  	threshold:	the specified threshold.
	 *  	eps:		the epsilon value of this query.
	 *  				When eps == -1.0, it runs the exact algorithm.
	 *  				When eps > 0, it runs the eps-version of algorithm.
	 */
	Query(int id, int* range, int threshold);

	/*
	 *  The constructor of Query.
	 */
	Query(int id, MyVector<int>& list, int threshold);

	Query(const QueryBase* q);

	Query(const QueryBase& q);

	~Query();

	/*
	 *  Alert the user that the current number of points in this range has reached to the threshold.
	 */
	void Alert();

	/*
	 *  Release the space of this query.
	 */
	void ReleaseSpace();

	/*
	 *  Show the current state of the query.
	 *  Return:
	 *  	1		the query should alert.
	 *  	0		otherwise.
	 */
	int ShowQueryState();

	/*
	 *  The number of messages received from tree nodes in this round.
	 */
	int msgCnt;

	/*
	 *  The value of current threshold on each site (TreeNode).
	 */
	int thresholdOnSite;

	/*
	 *  The list of the indexes of heap elements in their corresponding min heap.
	 */
//	MyVector<unsigned int> upperElementIndexList;

	/*
	 *  The start index of the upper element indexes in the global list.
	 */
	int startIndex;

	/*
	 *  The number of sites.
	 */
	int siteNum;


	static MyVector<int> nodeIndexList;

	/***************************************************************
	 *  			Space Efficient version related.
	 ***************************************************************/
public:
//	/*
//	 *  Initialize the threshold on each site.
//	 *  Parameter List:
//	 *  	segTree:		the pointer of the segment tree where this query lies.
//	 *  	segTree_Log:	the pointer of the SegmentTree_Log object.
//	 */
//	void InitializeSites_Space(void* segTree, void* segTree_Log);

	/*
	 *  Delete the query itself from the segment tree.
	 *  Parameter List:
	 *  	segTree:			the pointer of the segment tree containing this query.
	 *  	_treeNodeList:		the pointer of the list of all tree nodes.
	 *  	_segTreeList:		the pointer of the list of all segment trees.
	 *  	_heapElementList:	the reference of the list of all heap elements.
	 *  	_minHeapList:		the reference of the list of all min heaps.
	 */
	void DeleteMyself_Space(void* segTree_Log);

	/*
	 *  Clean the tree node info of this query.
	 */
	void CleanMySelf_Space();

	/*
	 *  Receive the notification from sites.
	 *  Return:
	 *  	When this query alerts, return 1.
	 *  	When this notification has started a new round, return 2.
	 *  	Otherwise, return 0.
	 */
	int ReceiveNotification_Space(void* segTree_Log, int ptCntDelta);

	/*
	 *  Enter to the next round.
	 */
	void EnterNextRound_Space(void* segTree_Log);

	/*
	 *  Initialize the threshold on each site.
	 */
	void InitializeSites_Space();

	/*
	 *  Collect the number of points in each site.
	 *  Parameter List:
	 *  	segTree:		the pointer of the segment tree containing this query.
	 *  	_treeNodeList:	the pointer of the list of all tree nodes.
	 *  	_segTreeList:	the pointer of the list of all segment trees.
	 *  Return:
	 *  	the exact total number of points in the range of this query.
	 */
	int CollectInfoFromSites_Space(void* segTree_Log);

	/*
	 *  Compute the value of *thresholdOnSite* in current round.
	 */
	void ComputeThresholdOnSite_Space();

	/*
	 *  Broadcast the new current thresholdOnSite to each site (TreeNode).
	 */
	void BroadCastThresholdOnSite_Space(void* segTree_Log);

	/*
	 *  Record the exact counter to lastCnt.
	 *  Parameter List:
	 *  	segTree:		the pointer of the segment tree containing this query.
	 *  	_treeNodeList:	the pointer of the list of all tree nodes.
	 *  	_segTreeList:	the pointer of the list of all segment trees.
	 */
	void RecordExactCounter_Space(void* segTree_Log);

	void RecordExactCounter();

	/*
	 *  Release the space of this query.
	 */
	void ReleaseSpace_Space(void* segTree_Log);

	/*
	 *  Compute the space consumption of this query.
	 */
	long long ComputeSpaceConsumption_Space();

private:
	/*
	 *  The action of exact insertion only threshold monitoring algorithm.
	 *  Return:
	 *  	0	Nothing.
	 *  	1	The query has alerted.
	 *  	2	The algorithm has started a new round.
	 */
	int ThresholdMonitoring_Action_Space(void* segTree_Log, int ptCntDelta);
};

#endif /* QUERY_H_ */
