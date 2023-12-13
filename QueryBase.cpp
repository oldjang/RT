/*
 * QueryBase.cpp
 *
 *  Created on: 10 Sep, 2015
 *      Author: jhgan
 */

#include "QueryBase.h"

QueryBase::QueryBase() {
	// TODO Auto-generated constructor stub
	this->id = -1;
	this->range = NULL;
	this->threshold = -1;
	this->evlCnt = 0;
//	this->lastCnt = 0;
}

QueryBase::QueryBase(int id, int* range, int threshold) {
	this->id = id;
//	this->range = range;
	this->range = new int[2 * DIM];
	for (int i = 0; i < 2 * DIM; ++i) {
		this->range[i] = range[i];
	}
	this->threshold = threshold;
	this->evlCnt = 0;
//	this->lastCnt = 0;
}

QueryBase::QueryBase(int id, MyVector<int>& list, int threshold) {
	this->id = id;
	int num = list.size();
	this->range = new int[num];
	for (int i = 0; i < num; ++i) {
		range[i] = list[i];
	}
	this->threshold = threshold;
	this->evlCnt = 0;
}

QueryBase::QueryBase(const QueryBase* q) {
	this->id = q->id;
	this->range = new int[2 * DIM];
	for (int i = 0; i < 2 * DIM; ++i) {
		this->range[i] = q->range[i];
	}
	this->threshold = q->threshold;
	this->evlCnt = q->evlCnt;
}

QueryBase::QueryBase(const QueryBase & q) {
	this->id = q.id;
	this->range = new int[2 * DIM];
	for (int i = 0; i < 2 * DIM; ++i) {
		this->range[i] = q.range[i];
	}
	this->threshold = q.threshold;
	this->evlCnt = q.evlCnt;
//	this->lastCnt = q.lastCnt;
}

QueryBase::~QueryBase() {
	// TODO Auto-generated destructor stub
//	this->ReleaseSpace();
//	printf("Call.\n");

}

void QueryBase::SetBy(const QueryBase& q) {
	this->id = q.id;
	this->range = new int[2 * DIM];
	for (int i = 0; i < 2 * DIM; ++i) {
		this->range[i] = q.range[i];
	}
	this->threshold = q.threshold;
	this->evlCnt = 0;
//	this->lastCnt = 0;
}

void QueryBase::ValueCopy(const QueryBase& q) {
	this->id = q.id;
	this->range = q.range;
	this->threshold = q.threshold;
	this->evlCnt = q.evlCnt;
//	this->lastCnt = q.lastCnt;
}

void QueryBase::ReleaseSpace() {
	if (this->range != NULL) {
		delete[] (this->range);
		this->range = NULL;
	}
	this->threshold = -1;
}

void QueryBase::RecordExactCounter() {
}


void QueryBase::ShowRange() {
	int d = DIM;
	for (int i = 0; i < d - 1; ++i) {
		printf("[%d, %d]*", this->range[2 * i], this->range[2 * i + 1]);
	}
	printf("[%d, %d]", this->range[2 * (d - 1)], this->range[2 * (d - 1) + 1]);
}

void QueryBase::ShowInfo() {
	printf("Query ID:\t %d\n", this->id);
	printf("Range:\t ");
	this->ShowRange();
	printf("\n");
	printf("Threshold:\t %d\n", this->threshold);
}

void QueryBase::Alert() {
#ifdef SHOWALERT
	printf("Query ID = %d: ", this->id);
	this->ShowRange();
	printf(" has reached its threshold %d\n", this->threshold);
	fflush(stdout);
#endif

}

int QueryBase::ShowQueryState() {
	int rtn = 0;
	if (this->evlCnt >= this->threshold) {
		rtn = 1;
	}

	return rtn;
}

