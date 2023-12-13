///*
// * LogarithmicFramework.cpp
// *
// *  Created on: 23 Sep, 2015
// *      Author: jhgan
// */
//
//#include "LogarithmicFramework.h"
//
//template<class T>
//LogarithmicFramework<T>::LogarithmicFramework() {
//	// TODO Auto-generated constructor stub
//	this->insNum = 0;
//	this->delNum = 0;
//	this->alertNum = 0;
//	this->lastQNum = 0;
//	this->strConstructor = NULL;
//}
//
//template<class T>
//LogarithmicFramework<T>::LogarithmicFramework(AbsStructure<T>* strConstructor) {
//	this->insNum = 0;
//	this->delNum = 0;
//	this->alertNum = 0;
//	this->lastQNum = 0;
//	this->strConstructor = strConstructor;
//}
//
//template<class T>
//LogarithmicFramework<T>::LogarithmicFramework(AbsStructure<T>* strConstructor,
//		MyVector<T>& queryList) {
//	this->insNum = 0;
//	this->delNum = 0;
//	this->alertNum = 0;
//	this->lastQNum = queryList.size();
//	this->strConstructor = strConstructor;
////	this->GlobalRebuilding(queryList);
//
//
////	vector<QueryBase*> qList(queryList);
//	AbsStructure<T>* str = this->strConstructor->ConstructStr(queryList);
//
//	str->sizeLev = ceil(log2(this->lastQNum));
//	this->strList.push_back(str);
//}
//
//template<class T>
//LogarithmicFramework<T>::~LogarithmicFramework() {
//	// TODO Auto-generated destructor stub
//}
//
////void LogarithmicFramework::InsertPoint(vector<int>& coords, int weight) {
////	int strNum = this->strList.size();
////
////	for (int i = 0; i < strNum; ++i) {
////		this->alertNum += this->strList[i]->InsertPoint(coords, weight);
////	}
////}
//
//template<class T>
//void LogarithmicFramework<T>::ReleaseSpace() {
//	int strNum = this->strList.size();
//
//	delete (this->strConstructor);
//	this->strConstructor = NULL;
//
//	for (int i = 0; i < strNum; ++i) {
//		this->strList[i]->ReleaseSpace();
//		delete (this->strList[i]);
//		this->strList[i] = NULL;
//	}
//
////	vector<AbsStructure*>().swap(this->strList);
//	this->strList.release_space();
//}
//
//template<class T>
//void LogarithmicFramework<T>::ReleaseSpace_All() {
//	delete (this->strConstructor);
//	this->strConstructor = NULL;
//
//	int strNum = this->strList.size();
//	for (int i = 0; i < strNum; ++i) {
//		this->strList[i]->ReleaseSpace_All();
//		delete (this->strList[i]);
//		this->strList[i] = NULL;
//	}
//
////	vector<AbsStructure*>().swap(this->strList);
//	this->strList.release_space();
//}
//
//template<class T>
//int LogarithmicFramework<T>::InsertPoint(int* coords, int weight,
//		MyVector<int>& alertIDList) {
//	int cnt = 0;
//
//	int strNum = this->strList.size();
//
//	for (int i = 0; i < strNum; ++i) {
//		cnt += this->strList[i]->InsertPoint(coords, weight, alertIDList);
//	}
//
//	this->alertNum += cnt;
//
//	// Check whether to rebuild the whole framework.
//	if (this->CheckRebuild()) {
//		// The framework needs to be rebuilt.
//		this->GlobalRebuilding();
//	}
//
//	return cnt;
//}
//
//template<class T>
//int LogarithmicFramework<T>::DeletePoint(int* coords, int weight,
//		MyVector<int>& alertIDList) {
//	int cnt = 0;
//
//	int strNum = this->strList.size();
//
//	for (int i = 0; i < strNum; ++i) {
//		cnt += this->strList[i]->DeletePoint(coords, weight, alertIDList);
//	}
//
//	this->alertNum += cnt;
//
//	// In fact, in the on-shoot problem, deletion will not lead to global rebuilding.
//
//	// Check whether to rebuild the whole framework.
//	if (this->CheckRebuild()) {
//		// The framework needs to be rebuilt.
//		this->GlobalRebuilding();
//	}
//
//	return cnt;
//}
//
//template<class T>
//void LogarithmicFramework<T>::InsertQuery(T* queryToIns) {
//
//	// Insert the query to the id-pointer map.
////	this->id_ptr_map[queryToIns->id] = queryToIns;
//
//	double start = 0;
//	double end = 0;
//
//	start = getCurrentTime();
//
//	// The number of insertions increases by one.
//	this->insNum++;
//
//	int strNum = this->strList.size();
//	BFNaive<T>* smallStr = NULL;
//	AbsStructure<T>* strTemp = NULL;
//	if (strNum == 0) {
//		// If the framework is empty, then construct a small structure.
//		smallStr = new BFNaive<T>();
//		smallStr->InsertQuery(queryToIns);
//
//		this->strList.push_back(smallStr);
//	} else {
//		// In this case, this framework has at least one structure.
////		int lastSize = this->strList[strNum - 1]->GetQueryList().size();
//		int lastSize = this->strList[strNum - 1]->aliveNum;
//		int lastSizeLev = this->strList[strNum - 1]->sizeLev;
//
//		if (lastSizeLev == SMALL_SIZE_LEV) {
//			// In this case, the last structures in the list is a BFNaive structure.
//			// Insert the given query to the BFNaive structure directly.
//			BFNaive<T>* smallStr = (BFNaive<T>*) (this->strList[strNum - 1]);
//			smallStr->InsertQuery(queryToIns);
//
//			// The size of the last structure increases by one.
//			lastSize++;
//
//			// We need to build the specified structure instead of BFNaive.
//			if (lastSize > SMALL_SIZE) {
//				MyVector<T> targetPlace;
//				int index = this->MergeContinuousList(targetPlace);
//
////				int sizeLev = this->strList[index]->sizeLev;
//
////				if (index == strNum - 1) {
////					// Only the small structure can be merged.
////					sizeLev = this->strList[index]->sizeLev;
////				} else {
////					sizeLev = this->strList[index]->sizeLev + 1;
////				}
//
//				// In this case, some structures can be merged.
//				// Release the space of all those mergeable structures.
//				for (int j = strNum - 1; j >= index; --j) {
//					strTemp = this->strList[j];
//
//					// Obtain the exact count for each query in this structure.
//					strTemp->ObtainExactCntForQuery();
//
//					strTemp->ReleaseSpace();
//					delete strTemp;
//					strTemp = NULL;
//
//					this->strList.pop_back();
//				}
//
//				int listSize = targetPlace.size();
//
//				// Construct the specified structure for queryList.
//				AbsStructure<T>* str = this->ConstructStr(targetPlace);
//
//				// Set the size level.
//				str->sizeLev = ceil(log2(listSize));
//
//				this->strList.push_back(str);
//			}
//		} else {
//			// In this case, the last structure is not a BFNaive structure.
//			// Create a BFNaive structure and then insert the given query to it.
//			smallStr = new BFNaive<T>();
//
//			// Set the size level. Note that the size level of small structure is SMALL_SIZE_LEV.
//			smallStr->sizeLev = SMALL_SIZE_LEV;
//
//			smallStr->InsertQuery(queryToIns);
//			this->strList.push_back(smallStr);
//		}
//	}
//
//	end = getCurrentTime();
//	InsQTime_Ours += end - start;
//}
//
////template<class T>
////void LogarithmicFramework<T>::DeleteQuery(T* queryToDel) {
////	if (queryToDel != NULL) {
////		if (queryToDel->threshold != -1) {
////			// The query to be deleted is alive.
////
////			int strNum = this->strList.size();
////
////			if (strNum > 0) {
////
////				this->delNum++;
////
////				this->strList[0]->DeleteQuery(queryToDel);
////
////				if (strNum > 1) {
////					if (this->strList[strNum - 1]->sizeLev == SMALL_SIZE_LEV) {
////						// To ensure the query also can be deleted from the small structure.
////						this->strList[strNum - 1]->DeleteQuery(queryToDel);
////					}
////				}
////
////				// Check whether to rebuild the whole framework.
////				if (this->CheckRebuild()) {
////					// The framework needs to be rebuilt.
////					this->GlobalRebuilding();
////				}
////			}
////		}
////	}
////}
//
//template<class T>
//void LogarithmicFramework<T>::DeleteQuery(int qID) {
//	int strNum = this->strList.size();
//
//	for (int i = 0; i < strNum; ++i) {
//		this->strList[i]->DeleteQuery(qID);
//	}
//}
//
//template<class T>
//AbsStructure<T>* LogarithmicFramework<T>::ConstructStr(MyVector<T>& queryList) {
//	return this->strConstructor->ConstructStr(queryList);
//}
//
//template<class T>
//void LogarithmicFramework<T>::GlobalRebuilding() {
//	double start = 0;
//	double end = 0;
//	double start2 = 0;
//	double end2 = 0;
//
//	start = getCurrentTime();
//
//	// The number of alive queries.
//	int aliveNum = lastQNum + insNum - delNum - alertNum;
//	MyVector<T> globalList;
//	globalList.reserve(aliveNum);
//
//	printf("\nStart global rebuilding:\n");
//	printf("lastQNum = %d\t insNum = %d\t delNum = %d\t alertNum = %d\n\n",
//			lastQNum, insNum, delNum, alertNum);
//	fflush(stdout);
//
//	int strNum = this->strList.size();
//	int length = 0;
//
//	start2 = getCurrentTime();
//	// Obtain the global alive query list.
//	for (int i = 0; i < strNum; ++i) {
//		MyVector<T>& qlist = this->strList[i]->GetQueryList();
//		length = qlist.size();
//		for (int j = 0; j < length; ++j) {
//			if (qlist[j]->threshold != -1) {
//				// We only keep the alive queries.
//				globalList.push_back(qlist[j]);
//			}
//		}
//	}
//	end2 = getCurrentTime();
//	printf("obtain global list time = %lf\n", end2 - start2);
//
//	if (aliveNum != (int) globalList.size()) {
//		printf("aliveNum = %d\t globalList = %d\n", aliveNum,
//				(int) globalList.size());
//		ShowError(
//				"Error in LogarithmicFramework::GlobalRebuilding: aliveNum is incorrect!\n");
//	}
//
//	double start3 = 0, end3 = 0;
//	double collectTime = 0;
//	double clearTime = 0;
//
//	// Release space of the all the current structures.
//	for (int i = 0; i < strNum; ++i) {
//		start3 = getCurrentTime();
//		// Obtain the exact counter for each query in this structure.
//		this->strList[i]->ObtainExactCntForQuery();
//		end3 = getCurrentTime();
//		collectTime += end3 - start3;
//
//		start3 = getCurrentTime();
//		this->strList[i]->ReleaseSpace();
//		delete (this->strList[i]);
//		this->strList[i] = NULL;
//		end3 = getCurrentTime();
//		clearTime += end3 - start3;
//	}
//	this->strList.clear();
//	printf("collect time = %lf\n", collectTime);
//	printf("clear strList time = %lf\n", clearTime);
//
//	// Start to rebuild the whole framework.
////	// This is a stupid version on 2015-10-08.
////	this->GlobalRebuilding(globalList);
//
//	// Start to rebuild the whole framework.
//	this->insNum = 0;
//	this->delNum = 0;
//	this->alertNum = 0;
//	this->lastQNum = aliveNum;
//
//	start2 = getCurrentTime();
//	// We just rebuild one structure together instead of O(log m) ones.
//	if (aliveNum <= SMALL_SIZE) {
//		BFNaive<T>* smallStr = new BFNaive<T>(globalList);
//
//		// Set the size level. Note that the size level of small structure is SMALL_SIZE_LEV.
//		smallStr->sizeLev = SMALL_SIZE_LEV;
//
//		this->strList.push_back(smallStr);
//	} else {
//		AbsStructure<T>* str = this->strConstructor->ConstructStr(globalList);
//
//		str->sizeLev = ceil(log2(aliveNum));
//
//		this->strList.push_back(str);
//	}
//	end2 = getCurrentTime();
//	printf("rebuilding time = %lf\n", end2 - start2);
//
//	end = getCurrentTime();
//	RebuildTime_Ours += end - start;
//	if (end - start > MaxRebuildTime_Ours) {
//		MaxRebuildTime_Ours = end - start;
//	}
//	printf("total global rebuilding time = %lf\n", end - start);
//
//}
//
//template<class T>
//int LogarithmicFramework<T>::MergeContinuousList(MyVector<T>& targetPlace) {
//	int strNum = this->strList.size();
//
//	if (strNum <= 0) {
//		ShowError(
//				"Error in LogarithmicFramework::MergeContinuousList: the strList is empty!\n");
//		return 0;
//	}
//
//	int preSum = 0;
//	int sizeLev = 0;
//	int preSizeLev = this->strList[strNum - 1]->sizeLev - 1;
//	int i = strNum - 1;
//
//	for (; i >= 0; --i) {
//		sizeLev = this->strList[i]->sizeLev;
//		if (sizeLev == preSizeLev + 1) {
//			// This is a continuous level.
////			preSum += this->strList[i]->GetQueryList().size();
//			preSum += this->strList[i]->aliveNum;
//			if (preSum <= (1 << sizeLev)) {
//				break;
//			} else {
//				preSizeLev = sizeLev;
//			}
//		} else {
//			// The index of the previous structure.
//			++i;
//			break;
//		}
//	}
//
//	if (i < 0) {
//		// In this case, all the structures should be merged together.
//		i = 0;
//	}
//
//	// The structures at position from i to strNum - 1 are continuous.
//	targetPlace.clear();
//	targetPlace.reserve(preSum);
//
//	int finishNum = 0;
//	int num = 0;
//	for (int j = i; j < strNum; ++j) {
//		MyVector<T>& list = this->strList[j]->GetQueryList();
//		num = list.size();
//		for (int k = 0; k < num; ++k) {
//			if (list[k]->threshold != -1) {
//				// This query is alive. We only keep the alive queries.
//				targetPlace.push_back(list[k]);
//			} else {
//				// Record the total number of queries that have finished or been deleted.
//				finishNum++;
//			}
//		}
//	}
//
//	// Update the global information so that to decrease the number of global rebuildings.
//	this->insNum -= finishNum;
//	this->delNum -= finishNum;
//
//	return i;
//}
//
//template<class T>
//bool LogarithmicFramework<T>::CheckRebuild() {
//	bool rtn = (this->delNum + this->alertNum)
//			>= ((this->lastQNum + this->insNum) / 2);
//
//	rtn = rtn && ((this->lastQNum + this->insNum) >= SMALL_SIZE);
//
//	return rtn;
//}
//
////	// Good version on 2015-10-08
////int LogarithmicFramework::MergeContinuousList(vector<QueryBase*>& targetPlace) {
//////	printf("Starting merging continuous structures.\n");
////
////	int strNum = this->strList.size();
////
////	if (strNum <= 0) {
////		ShowError(
////				"Error in LogarithmicFramework::MergeContinuousList: the strList is empty!\n");
////		return 0;
////	}
////
//////	if (strNum == 1) {
//////		// There is at most one structure in the framework.
//////		return 0;
//////	}
////
////	int preSum = this->strList[strNum - 1]->GetQueryList().size();
////
////	int i = strNum - 1;
////	int listSize = 0;
////
////	int preLev = (1 << (this->strList[strNum - 1]->sizeLev));
////	int sizeLev = 0;
////	for (; i > 0; --i) {
////		vector<QueryBase*>& list = this->strList[i - 1]->GetQueryList();
////		listSize = list.size();
////
////		sizeLev = (1 << (this->strList[i - 1]->sizeLev));
////
////		if (sizeLev == preLev) {
//////			printf("listSize = %d\n", listSize);
////			preSum += listSize;
////			preLev += sizeLev;
////		} else {
////			break;
////		}
////	}
////
////	// The structures at position from i to strNum - 1 are continuous.
////	targetPlace.clear();
////	targetPlace.reserve(preSum);
////
////	int num = 0;
////	for (int j = i; j < strNum; ++j) {
////		vector<QueryBase*>& list = this->strList[j]->GetQueryList();
////		num = list.size();
////		for (int k = 0; k < num; ++k) {
////			if (list[k]->threshold != -1) {
////				// This query is alive. We only keep the alive queries.
////				targetPlace.push_back(list[k]);
////			}
////		}
////	}
////
////	// Good version on 2015-09-29 4PM
//////	for (; i > 0; --i) {
//////		vector<QueryBase*>& list = this->strList[i - 1]->GetQueryList();
//////		listSize = list.size();
//////		if (listSize == preSum) {
////////			printf("listSize = %d\n", listSize);
//////			preSum += list.size();
//////		} else {
//////			break;
//////		}
//////	}
//////	// The structures at position from i to strNum - 1 are continuous.
//////	targetPlace.clear();
//////	targetPlace.reserve(preSum);
//////
//////	int num = 0;
//////	for (int j = i; j < strNum; ++j) {
//////		vector<QueryBase*>& list = this->strList[j]->GetQueryList();
//////		num = list.size();
//////		for (int k = 0; k < num; ++k) {
//////			targetPlace.push_back(list[k]);
//////		}
//////	}
////
////	return i;
////}
//
////AbsStructure* LogarithmicFramework::MergeStructure(AbsStructure* s1,
////		AbsStructure* s2) {
////	vector<QueryBase*> list1 = s1->GetQueryList();
////	vector<QueryBase*> list2 = s2->GetQueryList();
////
////	int num1 = list1.size();
////	int num2 = list2.size();
////
////	vector<QueryBase*> queryList;
////	queryList.reserve(num1 + num2);
////
////	for (int i = 0; i < num1; ++i) {
////		if (list1[i]->threshold != -1) {
////			// This query has not been deleted.
////			queryList.push_back(list1[i]);
////		}
////	}
////
////	for (int i = 0; i < num2; ++i) {
////		if (list2[i]->threshold != -1) {
////			// This query has not been deleted.
////			queryList.push_back(list2[i]);
////		}
////	}
////
////	s1->ReleaseSpace();
////	delete s1;
////	s1 = NULL;
////
////	s2->ReleaseSpace();
////	delete s2;
////	s2 = NULL;
////
////	return this->strConstructor->ConstructStr(queryList);
////}
//
