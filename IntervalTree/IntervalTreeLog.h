/*
 * IntervalTreeLog.h
 *
 *  Created on: 15 Oct, 2015
 *      Author: jhgan
 */

#ifndef INTERVALTREELOG_H_
#define INTERVALTREELOG_H_

#include "../Logarithmic/AbsStructure.h"
#include "../QueryBase.h"
#include "IntervalTree.h"

class IntervalTree_Log: public AbsStructure<QueryBase> {
public:
	IntervalTree_Log();
	IntervalTree_Log(MyVector<QueryBase>& queryList);


	~IntervalTree_Log();

	IntervalTree* tree;

	/*
	 *  Construct the structure for the given query list.
	 *  Parameter List:
	 *  	queryList:		the given query list.
	 *  Return:
	 *  	the pointer of resulted structure.
	 */
	IntervalTree_Log* ConstructStr(MyVector<QueryBase>& queryList);

	/*
	 *  Obtain the exact count for each query in this structure.
	 */
	void ObtainExactCntForQuery(MyVector<QueryBase>& targetPlace);

	/*
	 *  Delete the specified query from this structure by the given query ID.
	 */
	bool DeleteQuery(int delID);

	/*
	 *  Release the space of this structure but not the queries.
	 */
	void ReleaseSpace();

	/*
	 *  Release the space of this structure including queries.
	 */
	void ReleaseSpace_All();

	/*
	 *  Insert a point to the structure.
	 *  Parameter List:
	 *  	coords:			the coordinates of the given point.
	 *  	weight:			the weight of the given point which is > 0.
	 *  	alertIDList:	the list of ids of the queries alerted by inserting this point.
	 *  Return:
	 *  	the number of queries alerted by inserting this given point.
	 */
	int InsertPoint(int* coords, int weight, MyVector<int>& alertIDList);

	/*
	 *  Compute the space consumption in terms of bytes of this structure.
	 */
	long long ComputeSpaceConsumption();

};

#endif /* INTERVALTREELOG_H_ */
