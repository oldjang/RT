/*
 * BFNaive.cpp
 *
 *  Created on: 10 Sep, 2015
 *      Author: jhgan
 */

#include "BFNaive.h"
#include "./Logarithmic/LogarithmicFramework.h"

template<class T>
BFNaive<T>::BFNaive() {
	// TODO Auto-generated constructor stub
	this->aliveNum = 0;
}

template<class T>
BFNaive<T>::BFNaive(MyVector<T>& queryList) {
	this->_queryList.swap(queryList);

	this->aliveNum = this->_queryList.size();

#ifndef REMOVE_MAP
	// Initialize the <id, index> map.
	for (int i = 0; i < this->aliveNum; ++i) {
		this->id_index_map[this->_queryList[i].id] = i;
	}
#endif
}

template<class T>
BFNaive<T>::~BFNaive() {
	// TODO Auto-generated destructor stub
}

template<class T>
AbsStructure<T>* BFNaive<T>::ConstructStr(MyVector<T>& queryList) {
	return new BFNaive(queryList);
}

template<class T>
void BFNaive<T>::ObtainExactCntForQuery(MyVector<T>& targetPlace) {
	// Update the threshold of each query by its current counter.

	int qNum = this->_queryList.size();

	for (int i = 0; i < qNum; ++i) {
		if (this->_queryList[i].threshold != -1) {
			// q is alive.
			this->_queryList[i].RecordExactCounter();
			targetPlace.push_back(this->_queryList[i]);
		}
	}
}

template<class T>
void BFNaive<T>::ReleaseSpace() {
	this->_queryList.release_space();
	this->id_index_map.clear();
//	this->_queryRangeList.release_space();
}

template<class T>
void BFNaive<T>::ReleaseSpace_All() {

	int qNum = this->_queryList.size();
	for (int i = 0; i < qNum; ++i) {
		this->_queryList[i].ReleaseSpace();
	}

	this->_queryList.release_space();
	this->id_index_map.clear();
}

template<class T>
int BFNaive<T>::InsertPoint(int* coords, int weight,
		MyVector<int>& alertIDList) {

	int alertCnt = 0;

	T* q = NULL;

	int qNum = this->_queryList.size();
//	OverallAliveNum += qNum;

	for (int i = 0; i < qNum; i++) {
		q = &(this->_queryList[i]);
		if (q->threshold != -1) {
			if (q->IsInRange(coords)) {

				// Increase the StabNum by one.
				StabNum++;
				///*
				if (q->InsertPoint(weight)) {
					// This query alerts.
					alertCnt++;
					alertIDList.push_back(q->id);
					// Now we will release the query in function DeleteFromList(int).
//					q->ReleaseSpace();
					i = this->DeleteFromList(i);
					qNum--;
				}
				//*/
			}
		}
	}

	this->aliveNum -= alertCnt;

	if (this->CheckShrink()) {
		this->_queryList.shrink_to_fit();
	}

	return alertCnt;
}

template<class T>
int BFNaive<T>::DeletePoint(int* coords, int weight,
		MyVector<int>& alertIDList) {
	return this->InsertPoint(coords, -weight, alertIDList);
}

template<class T>
int BFNaive<T>::DeleteFromList(int index) {

	int qNum = this->_queryList.size();

	if (index >= qNum || index < 0) {
		ShowError(
				"Error in BFNaive::DeleteFromList: the index exceeds the length of the list!\n");
		return -1;
	}

#ifndef REMOVE_MAP
	// Update the id_index map.
	this->id_index_map[this->_queryList[qNum - 1].id] = index;
	this->id_index_map.erase(this->_queryList[index].id);
#endif

	// Move the last element to index i.
	T q;
	q.ValueCopy(this->_queryList[index]);
	this->_queryList[index].ValueCopy(this->_queryList[qNum - 1]);
	this->_queryList[qNum - 1].ValueCopy(q);

	// Release the space of the last element.
	this->_queryList[qNum - 1].ReleaseSpace();

	// Delete the last element.
	this->_queryList.pop_back();

	return index - 1;
}

template<class T>
void BFNaive<T>::InsertQuery(T& query) {
	this->id_index_map[query.id] = this->_queryList.size();
	this->_queryList.push_back(query);
	this->aliveNum++;
}

template<class T>
void BFNaive<T>::DeleteQuery(T& query) {

// Set the threshold of the given query to -1 so that this query can be
// deleted at the time of next point insertion.
	query.ReleaseSpace();
	query.threshold = -1;
	this->aliveNum--;
}

template<class T>
bool BFNaive<T>::DeleteQuery(int delID) {
	map<int, int>::iterator it = this->id_index_map.find(delID);
	if (it != this->id_index_map.end()) {
		int index = it->second;
		// Now we will release the query in function DeleteFromList(int).
//		this->_queryList[index].ReleaseSpace();
		// Delete from list.
		this->DeleteFromList(index);
		this->aliveNum--;

		if (this->CheckShrink()) {
			this->_queryList.shrink_to_fit();
		}
		return true;

	}
	return false;
}


template<class T>
bool BFNaive<T>::CheckShrink() {
	return this->_queryList.size() < this->_queryList.capacity() * 0.5;
}

template<class T>
int BFNaive<T>::GetAliveNum() {
	return this->aliveNum;
}

template<class T>
long long BFNaive<T>::ComputeSpaceConsumption() {
	long long spaceCons = 0;

	MyVector<T>& scQueryList = this->_queryList;
	spaceCons += (sizeof(T)) * scQueryList.capacity();
	spaceCons += sizeof(int) * 2 * DIM * this->aliveNum;
	spaceCons += 48 * this->id_index_map.size();
	return spaceCons;
}

template class BFNaive<Query> ;
template class BFNaive<QueryBase> ;
