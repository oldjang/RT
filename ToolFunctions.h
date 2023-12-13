/*
 * ToolFunctions.h
 *
 *  Created on: 14 Sep, 2015
 *      Author: jhgan
 */

#ifndef TOOLFUNCTIONS_H_
#define TOOLFUNCTIONS_H_

#include "QueryBase.h"
#include "Query.h"
#include "./MyRandom/MyRandom.h"

#include <vector>
#include <algorithm>
using namespace std;

/*
 *  Print the usage manual.
 */
void usage();

/*****************************************************
 *			Tools for Collecting Input
 *****************************************************/

/*
 *  Get the index of next unblank char from a string.
 */
int getNextChar(char *str);

/*
 *  Get next word from a string.
 */
void getNextWord(char *str, char *word);

/*****************************************************
 *			Tools for Correctness Checking
 *****************************************************/

bool CheckSame(vector<QueryBase*>& list, vector<QueryBase*>& exactList);

bool CheckCorrectness_Dynamic(vector<Query*>& list1,
		vector<QueryBase*>& exactList);

bool CheckCorrectness(vector<int>& idList1, vector<int>& idList2);

bool CheckCorrectness(MyVector<int>& idList1, MyVector<int>& idList2);

bool CheckResultFile(MyVector<int>& resultFile, MyVector<int>& groundTruth);

/*****************************************************
 *			Tools for Query Generation
 *****************************************************/

/*
 *  Generate a list of queries from the raw data read.
 */
//void GenQueryListFromRawData(MyVector<Query>& targetPlace,
//		MyVector<int>& rangeList, MyVector<QueryBase> const& rawData);

void GenQueryListFromRawData(MyVector<Query>& targetPlace,
		MyVector<QueryBase> const& rawData);

/*
 *  Generate a list of queries from the raw data read.
 */
//void GenQueryListFromRawData(MyVector<QueryBase>& targetPlace,
//		MyVector<int>& rangeList, MyVector<QueryBase> const& rawData);

void GenQueryListFromRawData(MyVector<QueryBase>& targetPlace,
		MyVector<QueryBase> const& rawData);

/*
 *  Generate a query range by the last range generated.
 */
void GenQueryRange(vector<int>& lastRange, vector<int>& targetRange);

/*
 *  Generate a query range in the default range.
 */
void GenQueryRange(vector<int>& targetRange, int dim = DIM);

/*
 *  Generate a list of query ranges.
 *  Parameter List:
 *  	qNum: 	the number of target queries.
 *  	s:		the lower bound of the stabbing number of each value.
 *  	rs:		the value of range start.
 *  	re:		the value of range end.
 */
void GenQueryRangeList(int qNum, vector<vector<int>>& targetPlace, int s,
		int rs, int re);

/*****************************************************
 *			Tools for memory tracking.
 *****************************************************/

//template<class TrackType>
//size_t* mem_used() {
//	static size_t s = 0;
//	return &s;
//}
//
//template<class T, class TrackType, class BaseAllocator = std::allocator<T> >
//class TrackerAllocator: public BaseAllocator {
//public:
//	typedef typename BaseAllocator::pointer pointer;
//	typedef typename BaseAllocator::size_type size_type;
//
//	TrackerAllocator() throw () :
//			BaseAllocator() {
//	}
//	TrackerAllocator(const TrackerAllocator& b) throw () :
//			BaseAllocator(b) {
//	}
//	TrackerAllocator(TrackerAllocator&& b) throw () :
//			BaseAllocator(b) {
//	}
//
//	template<class U> struct rebind {
//		typedef TrackerAllocator<U, TrackType,
//				typename BaseAllocator::template rebind<U>::other> other;
//	};
//
//	template<class U> TrackerAllocator(
//			const typename TrackerAllocator::template rebind<U>::other& b) throw () :
//			BaseAllocator(b) {
//	}
//	~TrackerAllocator() {
//	}
//
//	pointer allocate(size_type n) {
//		pointer r = BaseAllocator::allocate(n);
//		*mem_used<TrackType>() += n;
//		return r;
//	}
//	pointer allocate(size_type n, pointer h) {
//		pointer r = BaseAllocator::allocate(n, h);
//		*mem_used<TrackType>() += n;
//		return r;
//	}
//	void deallocate(pointer p, size_type n) throw () {
//		BaseAllocator::deallocate(p, n);
//		*mem_used<TrackType>() -= n;
//	}
//};
#endif /* TOOLFUNCTIONS_H_ */
