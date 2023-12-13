/*
 * SegmentTreePureLog.cpp
 *
 *  Created on: 15 Oct, 2015
 *      Author: jhgan
 */

#include "SegmentTreePureLog.h"

SegmentTreePure_Log::SegmentTreePure_Log() {
	// TODO Auto-generated constructor stub
	this->tree = NULL;
}

SegmentTreePure_Log::SegmentTreePure_Log(MyVector<QueryBase>& queryList) {

	int qNum = queryList.size();

	this->_queryList.swap(queryList);

	this->aliveNum = qNum;

	MyVector<QueryBase*> tempList;
	tempList.reserve(qNum);

	for (int i = 0; i < qNum; ++i) {
		tempList.push_back(&(_queryList[i]));
		this->id_index_map[_queryList[i].id] = i;
	}

	this->tree = new SegmentTreePure(tempList);

	tempList.release_space();
}

SegmentTreePure_Log::~SegmentTreePure_Log() {
	// TODO Auto-generated destructor stub
}

/*
 *  Construct the structure for the given query list.
 *  Parameter List:
 *  	queryList:		the given query list.
 *  Return:
 *  	the pointer of resulted structure.
 */
SegmentTreePure_Log* SegmentTreePure_Log::ConstructStr(
		MyVector<QueryBase>& queryList) {
	return new SegmentTreePure_Log(queryList);
}

/*
 *  Obtain the exact count for each query in this structure.
 */
void SegmentTreePure_Log::ObtainExactCntForQuery(
		MyVector<QueryBase>& targetPlace) {
	int qNum = this->_queryList.size();

//	int oldSize = this->_queryRangeList.size() / (2 * DIM);

	for (int i = 0; i < qNum; ++i) {
		if (_queryList[i].threshold != -1) {
			// q is alive.
			_queryList[i].RecordExactCounter();
			targetPlace.push_back(_queryList[i]);
		}
	}
}

/*
 *  Delete the specified query from this structure by the given query ID.
 */
bool SegmentTreePure_Log::DeleteQuery(int delID) {
	map<int, int>::iterator it = this->id_index_map.find(delID);
	if (it != this->id_index_map.end()) {
		int index = it->second;
		if (_queryList[index].threshold != -1) {
			_queryList[index].ReleaseSpace();

			this->id_index_map.erase(it);
			this->aliveNum--;
			return true;
		}
		this->id_index_map.erase(it);
		return false;
	}
	return false;
}

/*
 *  Release the space of this structure but not the queries.
 */
void SegmentTreePure_Log::ReleaseSpace() {
	int num = this->_queryList.size();
	for (int i = 0; i < num; ++i) {
		if (_queryList[i].threshold == -1) {
			_queryList[i].ReleaseSpace();
		}
	}

	if (this->tree != NULL) {
		this->tree->ReleaseSpace();
		delete (this->tree);
		this->tree = NULL;
	}

	_queryList.release_space();
}

/*
 *  Release the space of this structure including alive queries.
 */
void SegmentTreePure_Log::ReleaseSpace_All() {
	int num = this->_queryList.size();
	for (int i = 0; i < num; ++i) {
		_queryList[i].ReleaseSpace();
	}

	if (this->tree != NULL) {
		this->tree->ReleaseSpace();
		delete (this->tree);
		this->tree = NULL;
	}

	_queryList.release_space();
}

/*
 *  Insert a point to the structure.
 *  Parameter List:
 *  	coords:			the coordinates of the given point.
 *  	weight:			the weight of the given point which is > 0.
 *  	alertIDList:	the list of ids of the queries alerted by inserting this point.
 *  Return:
 *  	the number of queries alerted by inserting this given point.
 */
int SegmentTreePure_Log::InsertPoint(int* coords, int weight,
		MyVector<int>& alertIDList) {
	int alertNum = 0;
	if (this->tree != NULL) {
		alertNum = this->tree->InsertPoint(coords, weight, alertIDList);
		this->aliveNum -= alertNum;
	}
	return alertNum;
}

long long SegmentTreePure_Log::ComputeSpaceConsumption() {
	long long rtn = 0;
	rtn += sizeof(SegmentTreePure_Log);
	rtn += 48 * this->id_index_map.size();
	rtn += (sizeof(QueryBase)) * this->_queryList.capacity();
	rtn += 2 * DIM * sizeof(int) * this->aliveNum;

	if (this->tree != NULL) {
		rtn += this->tree->ComputeSpaceConsumption();
	}
	return rtn;
}

