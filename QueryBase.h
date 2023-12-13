/*
 * QueryBase.h
 *
 *  Created on: 10 Sep, 2015
 *      Author: jhgan
 */

#ifndef QUERYBASE_H_
#define QUERYBASE_H_

#include "headers.h"
#include "./MyVector/MyVector.h"

#include <vector>
using namespace std;

/*
 *  The base class of query.
 */
class QueryBase {
public:
	QueryBase();

	/*
	 *  The constructor of Query.
	 *  Parameter List:
	 *  	id:			the unique id of this query.
	 *  	range:		the given query DIM-dimensional range.
	 *  	threshold:	the specified threshold.
	 *  	eps:		the epsilon value of this query.
	 *  				When eps == -1.0, it runs the threshold monitoring algorithm.
	 *  				When eps > 0, it runs the value tracking algorithm
	 */
	QueryBase(int id, int* range, int threshold);

	/*
	 *  The constructor of Query.
	 */
	QueryBase(int id, MyVector<int>& list, int threshold);

	QueryBase(const QueryBase* q);

	QueryBase(const QueryBase& q);

	~QueryBase();

	/*
	 *  The unique id of this query.
	 */
	int id;

	/*
	 *  The threshold of this query.
	 */
	int threshold;

	/*
	 *  The evaluation of the real count.
	 */
	int evlCnt;

	/*
	 *  The list of endpoints of the range of this query.
	 *  The length of this list is 2 * DIM, where the left-endpoint and right-endpoint of the i-th dimension
	 *  are stored in range[i-1] and range[i], respectively.
	 */
	int* range;

	/*
	 *  Set all the data member by the given query object.
	 */
	void SetBy(const QueryBase& q);

	void ValueCopy(const QueryBase& q);

	/*
	 *  Print out the info of this query.
	 */
	void ShowInfo();

	/*
	 *  Print out the range info of this query.
	 */
	void ShowRange();

	/*
	 *  Alert the user that the current number of points in this range has reached to the threshold.
	 */
	void Alert();

	/*
	 *  Show the current state of the query.
	 *  Return:
	 *  	1		the current evlCnt >= threshold.
	 *  	0		the current evlCnt < threshold.
	 */
	int ShowQueryState();

	/*
	 *  Check whether a point with given coordinates is inside the range of this query.
	 *  Parameter List:
	 *  	coords:		the given coordinate values.
	 *  Return:
	 *  	If Yes, return TRUE. Otherwise, return FALSE.
	 */
	inline bool IsInRange(int* coords) {

//		bool ret = true;
		int * ptr = this->range;
		for (int i = 0; i < DIM; i++) {
			if ((*ptr) > coords[i]) {
				return false;
			}
			ptr++;
			if ((*ptr) < coords[i]) {
				return false;
			}
			ptr++;
		}
		return true;
	}

	/*
	 *  Virtually insert a point to this query range.
	 *  Parameter List:
	 *  	weight:      the weight of the given point. In default, weight = 1.
	 *  Return:
	 *  	When this query alerts, return TRUE. Otherwise, return FALSE.
	 */
	inline bool InsertPoint(int weight) {
		this->evlCnt += weight;
		// Threshold monitoring.
		if (this->evlCnt >= this->threshold) {
			//		this->Alert();
			return true;
		}
		return false;
	}

	/*
	 *  Record the exact counter to lastCnt.
	 */
	void RecordExactCounter();

	/*
	 * Release the space of this query.
	 */
	void ReleaseSpace();

};

#endif /* QUERYBASE_H_ */
