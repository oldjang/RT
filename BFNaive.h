/*
 * BFNaive.h
 *
 *  Created on: 10 Sep, 2015
 *      Author: jhgan
 */

#ifndef BFNAIVE_H_
#define BFNAIVE_H_

#include "headers.h"
#include "QueryBase.h"
#include "Query.h"
#include "./Logarithmic/AbsStructure.h"

#include "./MyVector/MyVector.h"

#include <vector>
using namespace std;

/*
 *  The class of naive brute force algorithm.
 */
template<class T>
class BFNaive: public AbsStructure<T> {
public:
	BFNaive();

	/*
	 *  The constructor of the naive brute force algorithm.
	 */
	BFNaive(MyVector<T>& queryList);

	~BFNaive();

	AbsStructure<T>* ConstructStr(MyVector<T>& queryList);

	void ObtainExactCntForQuery(MyVector<T>& targetPlace);

	/*
	 *  Insert a point with the given coordinate values.
	 *  Parameter List:
	 *  	coords:		the given coordinate values.
	 *  	weight:		the weight of the given point.
	 *  Return:
	 *   	the number of queries alerted by inserting this given point.
	 */
	int InsertPoint(int* coords, int weight, MyVector<int>& alertIDList);

	/*
	 *  Delete a point with the given coordinate values.
	 *  Parameter List:
	 *  	coords:		the given coordinate values.
	 *  	weight:		the weight of the given point which is > 0
	 *  Return:
	 *   	the number of queries alerted by deleting this given point.
	 */
	int DeletePoint(int* coords, int weight, MyVector<int>& alertIDList);

	/*
	 *  Insert the given query to the query list.
	 *  Parameter List:
	 *  	query:		the given query.
	 */
	void InsertQuery(T& query);

	/*
	 *  Delete the given query from the query list.
	 *  Parameter List:
	 *  	query:		the given query.
	 */
	void DeleteQuery(T& query);

	/*
	 *  Delete the query by the query ID.
	 *  Return:
	 *  	Whether the deletion is successful.
	 */
	bool DeleteQuery(int delID);

	/*
	 *  Delete the element at position of *index* from the list.
	 *  Parameter List:
	 *  	index:		the specified index of the element to be deleted.
	 *  Return:
	 *  	*index* - 1 the position of the element right before the element just deleted.
	 */
	int DeleteFromList(int index);

	/*
	 *  Release the space of this algorithm structure but not release the queries.
	 */
	void ReleaseSpace();

	/*
	 *  Release the space of this structure including queries.
	 */
	void ReleaseSpace_All();


	/*
	 *  Check whether to shrink the query list.
	 */
	bool CheckShrink();


	/*
	 *  Return the number of alive queries.
	 */
	int GetAliveNum();

	/*
	 *  Compute the space consumption in term of bytes of this structure.
	 */
	long long ComputeSpaceConsumption();

};

#endif /* BFNAIVE_H_ */
