/*
 * SegmentTreeLog.h
 *
 *  Created on: 9 Oct, 2015
 *      Author: jhgan
 */

#ifndef SEGMENTTREELOG_H_
#define SEGMENTTREELOG_H_

#include "../Logarithmic/AbsStructure.h"
#include "SegmentTree.h"

#include <vector>
#include <map>
using namespace std;

//template<class T>
class SegmentTree_Log: public AbsStructure<Query> {
public:
	SegmentTree_Log();

	SegmentTree_Log(MyVector<Query>& queryList);

	~SegmentTree_Log();

	/*
	 *  The segment tree in this structure.
	 */
	SegmentTree* tree;

	MyVector<TreeNode> _treeNodeList;

	MyVector<MyVector<int>> _tempQueryIndexList;

	MyVector<SegmentTree> _segTreeList;

	MyVector<MinHeap> _minHeapList;

	/*
	 *  The list of all the indexes in heap of heap elements in this structure.
	 *  The format is as follows:
	 *  	queryIndex upperElementIndexInHeapList of that query.
	 */
	MyVector<int> _upperElementIndexInHeapList;

	/*
	 *  Compute the space consumption in terms of bytes of this structure.
	 */
	long long ComputeSpaceConsumption();


	/*
	 *  Delete the query by the given query ID.
	 */
	bool DeleteQuery(int queryID);


	void ObtainExactCntForQuery(MyVector<Query>& targetPlace);


	SegmentTree_Log* ConstructStr(MyVector<Query>& queryList);

	int InsertPoint(int* coords, int weight, MyVector<int>& alertIDList);


	/*
	 *  Release the space of this segment tree.
	 */
	void ReleaseSpace();

	void ReleaseSpace_All();

};

#endif /* SEGMENTTREELOG_H_ */
