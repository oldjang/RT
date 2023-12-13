/*
 * AbsStructure.h
 *
 *  Created on: 22 Sep, 2015
 *      Author: jhgan
 */

#ifndef ABSSTRUCTURE_H_
#define ABSSTRUCTURE_H_

#include "../headers.h"

#include "../QueryBase.h"
#include "../MyVector/MyVector.h"

#include <map>
using namespace std;

/*
 *  The abstract class of the structure in logarithmic framework.
 */
template<class T>
class AbsStructure {
public:
	AbsStructure();
	virtual ~AbsStructure();

	/*
	 *  Construct the structure for the given query list.
	 *  Parameter List:
	 *  	queryList:		the given query list.
	 *  Return:
	 *  	the pointer of resulted structure.
	 */
	virtual AbsStructure<T>* ConstructStr(MyVector<T>& queryList) = 0;

	/*
	 *  Obtain the exact count for each query in this structure.
	 */
	virtual void ObtainExactCntForQuery(MyVector<T>& targetPlace) = 0;

	/*
	 *  Delete the specified query from this structure by the given query ID.
	 */
	virtual bool DeleteQuery(int delID) = 0;

	/*
	 *  Release the space of this structure but not the queries.
	 */
	virtual void ReleaseSpace() = 0;

	/*
	 *  Release the space of this structure including queries.
	 */
	virtual void ReleaseSpace_All() = 0;

	/*
	 *  Compute the space consumption in terms of bytes of this structure.
	 */
	virtual long long ComputeSpaceConsumption() = 0;

	/*
	 *  Insert a point to the structure.
	 *  Parameter List:
	 *  	coords:			the coordinates of the given point.
	 *  	weight:			the weight of the given point which is > 0.
	 *  	alertIDList:	the list of ids of the queries alerted by inserting this point.
	 *  Return:
	 *  	the number of queries alerted by inserting this given point.
	 */
	virtual int InsertPoint(int* coords, int weight,
			MyVector<int>& alertIDList) = 0;

	/*
	 * 	Return the list of queries in this structure.
	 */
	MyVector<T>& GetQueryList();

	/*
	 *  Return the query corresponds to the given id.
	 */
	T* FindQueryByID(int id);

	/*
	 *  The level of size in terms of exponent of 2.
	 */
	int sizeLev;

	/*
	 *  The number of alive elements in this structure.
	 */
	int aliveNum;

	/*
	 *  The list of queries in this structure.
	 */
	MyVector<T> _queryList;

	/*
	 *  The map for finding the query index by the given query ID.
	 */
	map<int, int> id_index_map;

};

template<class T>
AbsStructure<T>::AbsStructure() {
	// TODO Auto-generated constructor stub
	this->sizeLev = 0;
	this->aliveNum = 0;
}

template<class T>
AbsStructure<T>::~AbsStructure() {
	// TODO Auto-generated destructor stub
}

template<class T>
MyVector<T>& AbsStructure<T>::GetQueryList() {
	return this->_queryList;
}

template<class T>
T* AbsStructure<T>::FindQueryByID(int id) {
	map<int, int>::iterator it = this->id_index_map.find(id);
	if (it != this->id_index_map.end()) {
		return &(this->_queryList[it->second]);
	} else {
		return NULL;
	}
}

#endif /* ABSSTRUCTURE_H_ */
