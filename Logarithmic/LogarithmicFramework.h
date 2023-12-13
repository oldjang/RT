/*
 * LogarithmicFramework.h
 *
 *  Created on: 23 Sep, 2015
 *      Author: jhgan
 */

#ifndef LOGARITHMICFRAMEWORK_H_
#define LOGARITHMICFRAMEWORK_H_

#include "../headers.h"
#include "AbsStructure.h"
#include "../BFNaive.h"

#include "../MyVector/MyVector.h"

#include <vector>
#include <map>
#include <math.h>
using namespace std;

/*
 *  The constant defines small size.
 */
const int SMALL_SIZE = 32;

/*
 *  The constant defines the size level of small size.
 */
const int SMALL_SIZE_LEV = (int) (log2(SMALL_SIZE));

/*
 *  The class of the logarithmic framework.
 */
template<class T>
class LogarithmicFramework {
public:
	LogarithmicFramework();

	/*
	 *  The constructor of LogarithmicFramework.
	 *  Parameter List:
	 *  	strConstructor:		the constructor of specified type of structures.
	 */
	LogarithmicFramework(AbsStructure<T>* strConstructor);

	/*
	 *  The constructor of LogarithmicFramework.
	 *  Parameter List:
	 *  	strConstructor:		the constructor of specified type of structures.
	 *  	queryList:			the given list of queries.
	 */
	LogarithmicFramework(AbsStructure<T>* strConstructor,
			MyVector<T>& queryList);

	~LogarithmicFramework();

	/*
	 *  Insert the given point to the framework.
	 *  Parameter List:
	 *  	coords:		the coordinates of the given point.
	 *  	weight:		the weight of the given point which is > 0.
	 *  	alertList:	the target place to store the ID of the queries alerted due to
	 *  				the insertion of this given point.
	 *  Return:
	 *  	TRUE 	if this structure has global rebuilding due to the deletion of this query.
	 *  	FALSE 	otherwise.
	 */
	bool InsertPoint(int* coords, int weight, MyVector<int>& alertIDList);

	/*
	 *  Insert the given query to the framework.
	 */
	void InsertQuery(T& queryToIns);

	/*
	 *  Insert a list of queries to the framework.
	 */
	void InsertListOfQuery(MyVector<T>& queryListToIns);

	/*
	 *  Delete the query with specified ID from the framework.
	 *  Parameter List:
	 *  	qID:	the id of the query to delete.
	 *  Return:
	 *  	TRUE 	if this structure has global rebuilding due to the deletion of this query.
	 *  	FALSE 	otherwise.
	 */
	bool DeleteQuery(int qID);

	/*
	 *  Return the query corresponding to the given id.
	 */
	T* FindQueryByID(int qID);

	/*
	 *  Construct the structure for the given query list.
	 */
	AbsStructure<T>* ConstructStr(MyVector<T>& queryList);

	/*
	 *  Return the number of alive queries.
	 */
	int GetAliveNum();

	/*
	 *  Global rebuild the whole framework for those queries that have not been deleted.
	 */
	void GlobalRebuilding();

	/*
	 *  Check whether to perform the global rebuilding.
	 *  Return:
	 *  	If (delNum + alertNum) >= (lastQNum + insNum)/2 , return TRUE.
	 *  	Otherwise, return FALSE.
	 */
	bool CheckRebuild();

	/*
	 *  Release the space of this framework.
	 */
	void ReleaseSpace();

	/*
	 *  Release the space of this framework including queries.
	 */
	void ReleaseSpace_All();

	/*
	 *  Compute the space consumption in terms of bytes of this structure.
	 */
	long long ComputeSpaceConsumption();

	/*
	 *  The number of insertions since the last global rebuilding.
	 */
	int insNum;

	/*
	 *  The number of deletions since the last global rebuilding.
	 */
	int delNum;

	/*
	 *  The number of queries alerted since the last global rebuilding.
	 */
	int alertNum;

	/*
	 *  The number of queries when the last global rebuilding is done.
	 */
	int lastQNum;

	/*
	 *  The number of terminated queries that have been removed.
	 */
	int fixNum;

	/*
	 *  The constructor for building specified type of structures.
	 */
	AbsStructure<T>* strConstructor;

	/*
	 *  The list of structures.
	 */
	MyVector<AbsStructure<T>*> strList;

protected:
	/*
	 *  Merge the query lists of the structures that can be merged together.
	 *  Parameter List:
	 *  	preSum:		the reference of the number of queries that need to merged.
	 *  Return:
	 *  	the smallest index *i* in the continuous list, namely, the continuous list is from i to strNum - 1.
	 */
	int MergeContinuousList(int& preSum);

	/*
	 *  Find the mergable structure list.
	 *  Parameter List:
	 *  	listSizeToIns:		the size of the list of queries to insert.
	 *  	preSum:				the reference of the number of queries that need to merged.
	 *  Return:
	 *  	the smallest index *i* in the continuous list, namely, the continuous list is from i to strNum - 1.
	 */
	int FindMergableList(int listSizeToIns, int& preSum);
};

template<class T>
LogarithmicFramework<T>::LogarithmicFramework() {
	// TODO Auto-generated constructor stub
	this->insNum = 0;
	this->delNum = 0;
	this->alertNum = 0;
	this->lastQNum = 0;
	this->fixNum = 0;
	this->strConstructor = NULL;

	// Create a small structure.
	AbsStructure<T>* str = new BFNaive<T>();
	str->sizeLev = SMALL_SIZE_LEV;
	this->strList.push_back(str);
}

template<class T>
LogarithmicFramework<T>::LogarithmicFramework(AbsStructure<T>* strConstructor) {
	this->insNum = 0;
	this->delNum = 0;
	this->alertNum = 0;
	this->lastQNum = 0;
	this->fixNum = 0;
	this->strConstructor = strConstructor;

	// Create a small structure.
	AbsStructure<T>* str = new BFNaive<T>();
	str->sizeLev = SMALL_SIZE_LEV;
	this->strList.push_back(str);
}

template<class T>
LogarithmicFramework<T>::LogarithmicFramework(AbsStructure<T>* strConstructor,
		MyVector<T>& queryList) {
	this->insNum = 0;
	this->delNum = 0;
	this->alertNum = 0;
	this->lastQNum = queryList.size();
	this->fixNum = 0;
	this->strConstructor = strConstructor;
//	this->GlobalRebuilding(queryList);

//	vector<QueryBase*> qList(queryList);

	if (queryList.size() > SMALL_SIZE) {
		AbsStructure<T>* str = this->strConstructor->ConstructStr(queryList);

		str->sizeLev = ceil(log2(this->lastQNum));
		this->strList.push_back(str);
	} else {
		// Create a small structure.
		AbsStructure<T>* str = new BFNaive<T>(queryList);
		str->sizeLev = SMALL_SIZE_LEV;
		this->strList.push_back(str);
	}
}

template<class T>
LogarithmicFramework<T>::~LogarithmicFramework() {
	// TODO Auto-generated destructor stub
}

template<class T>
void LogarithmicFramework<T>::ReleaseSpace() {
	int strNum = this->strList.size();

	delete (this->strConstructor);
	this->strConstructor = NULL;

	for (int i = 0; i < strNum; ++i) {
		this->strList[i]->ReleaseSpace();
		delete (this->strList[i]);
		this->strList[i] = NULL;
	}

//	vector<AbsStructure*>().swap(this->strList);
	this->strList.release_space();
}

template<class T>
void LogarithmicFramework<T>::ReleaseSpace_All() {
	delete (this->strConstructor);
	this->strConstructor = NULL;

	int strNum = this->strList.size();
	for (int i = 0; i < strNum; ++i) {
		this->strList[i]->ReleaseSpace_All();
		delete (this->strList[i]);
		this->strList[i] = NULL;
	}

//	vector<AbsStructure*>().swap(this->strList);
	this->strList.release_space();
}

template<class T>
long long LogarithmicFramework<T>::ComputeSpaceConsumption() {
	long long spaceCons = 0;

	spaceCons += sizeof(LogarithmicFramework);
	spaceCons += sizeof(AbsStructure<T> );

	int strNum = this->strList.size();
	spaceCons += sizeof(void*) * this->strList.capacity();
	for (int i = 0; i < strNum; ++i) {
		spaceCons += this->strList[i]->ComputeSpaceConsumption();
	}

	return spaceCons;
}

template<class T>
bool LogarithmicFramework<T>::InsertPoint(int* coords, int weight,
		MyVector<int>& alertIDList) {
	int cnt = 0;

	int strNum = this->strList.size();

	for (int i = 0; i < strNum; ++i) {
		cnt += this->strList[i]->InsertPoint(coords, weight, alertIDList);
	}

	this->alertNum += cnt;

	// Increase the number of operations by 1.
	OPNUM++;

	// Increase the number of operations by the number of maturity.
	OPNUM += cnt;

	// Check whether to rebuild the whole framework.
	if (this->CheckRebuild()) {
		// The framework needs to be rebuilt.
		this->GlobalRebuilding();

		return true;
	}

	return false;
}

template<class T>
void LogarithmicFramework<T>::InsertQuery(T& queryToIns) {

	// The number of insertions increases by one.
	this->insNum++;

	// Increase the number of operations by 1.
	OPNUM++;

	int strNum = this->strList.size();
	BFNaive<T>* smallStr = NULL;
	AbsStructure<T>* strTemp = NULL;
	if (strNum == 0) {
		// If the framework is empty, then construct a small structure.
		smallStr = new BFNaive<T>();
		smallStr->InsertQuery(queryToIns);

		this->strList.push_back(smallStr);
	} else {
		// In this case, this framework has at least one structure.
//		int lastSize = this->strList[strNum - 1]->GetQueryList().size();
		int lastSize = this->strList[strNum - 1]->aliveNum;
		int lastSizeLev = this->strList[strNum - 1]->sizeLev;

		if (lastSizeLev == SMALL_SIZE_LEV) {
			// In this case, the last structures in the list is a BFNaive structure.
			// Insert the given query to the BFNaive structure directly.
			BFNaive<T>* smallStr = (BFNaive<T>*) (this->strList[strNum - 1]);
			smallStr->InsertQuery(queryToIns);

			// The size of the last structure increases by one.
			lastSize++;

			// We need to build the specified structure instead of BFNaive.
			if (lastSize > SMALL_SIZE) {

				int preSum = 0;
				int sizeSum = 0;

				int index = this->MergeContinuousList(preSum);

				MyVector<T> targetPlace;
				targetPlace.reserve(preSum);

				// In this case, some structures can be merged.
				// Release the space of all those mergeable structures.
				for (int j = strNum - 1; j >= index; --j) {
					strTemp = this->strList[j];

					sizeSum += strTemp->GetQueryList().size();

					// Obtain the exact count for each query in this structure.
					strTemp->ObtainExactCntForQuery(targetPlace);

					strTemp->ReleaseSpace();
					delete strTemp;
					strTemp = NULL;
					this->strList[j] = NULL;
					this->strList.pop_back();
				}

				// Decrease the number of finished queries from both insNum and delNum.
				this->fixNum += sizeSum - preSum;

				int listSize = targetPlace.size();

				// Construct the specified structure for queryList.
				AbsStructure<T>* str = this->ConstructStr(targetPlace);

				// Set the size level.
				str->sizeLev = ceil(log2(listSize));

				this->strList.push_back(str);
			}
		} else {
			// In this case, the last structure is not a BFNaive structure.
			// Create a BFNaive structure and then insert the given query to it.
			smallStr = new BFNaive<T>();

			// Set the size level. Note that the size level of small structure is SMALL_SIZE_LEV.
			smallStr->sizeLev = SMALL_SIZE_LEV;

			smallStr->InsertQuery(queryToIns);
			this->strList.push_back(smallStr);
		}
	}
}

template<class T>
void LogarithmicFramework<T>::InsertListOfQuery(MyVector<T>& queryListToIns) {

	int listSize = queryListToIns.size();

	// Increase the number of insertions.
	this->insNum += listSize;

	int strNum = this->strList.size();
	AbsStructure<T> *strTemp = NULL;
	if (strNum == 0) {
		// If the framework is empty, then construct a structure to store queryListToIns.

		if (listSize > SMALL_SIZE) {
			AbsStructure<T> *str = this->strConstructor->ConstructStr(
					queryListToIns);

			str->sizeLev = ceil(log2(listSize));
			this->strList.push_back(str);
		} else {
			// Create a small structure.
			AbsStructure<T> *str = new BFNaive<T>(queryListToIns);
			str->sizeLev = SMALL_SIZE_LEV;
			this->strList.push_back(str);
		}

	} else {
		// In this case, this framework has at least one structure.

		int lastSize = this->strList[strNum - 1]->aliveNum;
		int lastSizeLev = this->strList[strNum - 1]->sizeLev;

		if (listSize > SMALL_SIZE
				|| (lastSizeLev == SMALL_SIZE_LEV
						&& lastSize + listSize > SMALL_SIZE)) {
			// In this case, we have to find a mergable list of structure to merge.

			int preSum = 0;
			int sizeSum = 0;

			int index = this->FindMergableList(listSize, preSum);

			//				printf("preSum = %d\n", preSum);

			MyVector<T> targetPlace;
			targetPlace.reserve(preSum);

			for (int i = 0; i < listSize; ++i) {

				queryListToIns[i].RecordExactCounter();
				targetPlace.push_back(queryListToIns[i]);
			}

			// In this case, some structures can be merged.
			// Release the space of all those mergeable structures.
			for (int j = strNum - 1; j >= index; --j) {
				strTemp = this->strList[j];

				sizeSum += strTemp->GetQueryList().size();

				// Obtain the exact count for each query in this structure.
				strTemp->ObtainExactCntForQuery(targetPlace);

				strTemp->ReleaseSpace();
				delete strTemp;
				strTemp = NULL;
				this->strList[j] = NULL;
				this->strList.pop_back();
			}

			// Decrease the number of finished queries from both insNum and delNum.
			this->fixNum += sizeSum - preSum + listSize;

			int targetSize = targetPlace.size();

//			start2 = getCurrentTime();

			// Construct the specified structure for queryList.
			AbsStructure<T> *str = this->ConstructStr(targetPlace);

//			end2 = getCurrentTime();
//			CopyQueryTime += end2 - start2;

			// Set the size level.
			str->sizeLev = ceil(log2(targetSize));

			this->strList.push_back(str);

		} else if (lastSizeLev == SMALL_SIZE_LEV
				&& lastSize + listSize <= SMALL_SIZE) {
			// Add the list of queries directly to the small structure.
			BFNaive<T> *smallStr = (BFNaive<T>*) (this->strList[strNum - 1]);
			for (int i = 0; i < listSize; ++i) {
				smallStr->InsertQuery(queryListToIns[i]);
			}
		} else if (lastSizeLev > SMALL_SIZE_LEV) {
			// In this case, listSize must <= SMALL_SIZE.

			// Create a small structure.
			AbsStructure<T> *str = new BFNaive<T>(queryListToIns);
			str->sizeLev = SMALL_SIZE_LEV;
			this->strList.push_back(str);
		}
	}
}

template<class T>
bool LogarithmicFramework<T>::DeleteQuery(int qID) {
	int strNum = this->strList.size();

	for (int i = 0; i < strNum; ++i) {
		if (this->strList[i]->DeleteQuery(qID)) {
			// If this deletion is successful, then increase the delNum by one.
			this->delNum++;

			// Increase the number of operations by 1.
			OPNUM++;

//			DelNum_Ours++;

			// Check whether to rebuild the whole framework.
			if (this->CheckRebuild()) {
				// The framework needs to be rebuilt.
				this->GlobalRebuilding();
				return true;
			}
		}
	}
	return false;
}

template<class T>
T* LogarithmicFramework<T>::FindQueryByID(int qID) {
	int strNum = this->strList.size();

	T* temp = NULL;

	for (int i = 0; i < strNum; ++i) {
		temp = this->strList[i]->FindQueryByID(qID);
		if (temp != NULL) {
			return temp;
		}
	}

	return NULL;
}

template<class T>
AbsStructure<T>* LogarithmicFramework<T>::ConstructStr(MyVector<T>& queryList) {
	return this->strConstructor->ConstructStr(queryList);
}

template<class T>
int LogarithmicFramework<T>::GetAliveNum() {
	return lastQNum + insNum - delNum - alertNum;
}

template<class T>
void LogarithmicFramework<T>::GlobalRebuilding() {
	double start = 0;
	double end = 0;
	double start2 = 0;
	double end2 = 0;

	double start3 = 0, end3 = 0;
	double collectTime = 0;
	double clearTime = 0;

	start = getCurrentTime();

	// The number of alive queries.
	int aliveNum = lastQNum + insNum - delNum - alertNum;
	MyVector<T> globalList;
	globalList.reserve(aliveNum);

	printf("\nStart global rebuilding:\n");
	printf(
			"lastQNum = %d\t insNum = %d\t delNum = %d\t alertNum = %d\t fixNum = %d\n",
			lastQNum, insNum, delNum, alertNum, fixNum);
	fflush(stdout);

	int strNum = this->strList.size();

	// Obtain the global alive query list.
	for (int i = 0; i < strNum; ++i) {

		// Obtain the exact counter for each query in this structure.
		this->strList[i]->ObtainExactCntForQuery(globalList);

		// Release the space of structures.
		this->strList[i]->ReleaseSpace();
		delete (this->strList[i]);
		this->strList[i] = NULL;
	}
	this->strList.release_space();

	if (aliveNum != (int) globalList.size()) {
		printf("aliveNum = %d\t globalList = %d\n", aliveNum,
				(int) globalList.size());
		ShowError(
				"Error in LogarithmicFramework::GlobalRebuilding: aliveNum is incorrect!\n");
	}

	// Start to rebuild the whole framework.
	this->insNum = 0;
	this->delNum = 0;
	this->alertNum = 0;
	this->lastQNum = aliveNum;
	this->fixNum = 0;

	// We just rebuild one structure together instead of O(log m) ones.
	if (aliveNum <= SMALL_SIZE) {
		BFNaive<T>* smallStr = new BFNaive<T>(globalList);

		// Set the size level. Note that the size level of small structure is SMALL_SIZE_LEV.
		smallStr->sizeLev = SMALL_SIZE_LEV;

		this->strList.push_back(smallStr);
	} else {
		AbsStructure<T>* str = this->strConstructor->ConstructStr(globalList);

		str->sizeLev = ceil(log2(aliveNum));

		this->strList.push_back(str);
	}

	end = getCurrentTime();
	RebuildTime += end - start;
	if (end - start > MaxRebuildTime) {
		MaxRebuildTime = end - start;
	}
	printf("total global rebuilding time = %lf. aliveNum = %d\n\n", end - start,
			aliveNum);

}

template<class T>
int LogarithmicFramework<T>::MergeContinuousList(int& preSum) {
	int strNum = this->strList.size();

	if (strNum <= 0) {
		ShowError(
				"Error in LogarithmicFramework::MergeContinuousList: the strList is empty!\n");
		return 0;
	}

//	int preSum = 0;
	preSum = 0;
	int sizeLev = 0;
	int preSizeLev = this->strList[strNum - 1]->sizeLev - 1;
	int i = strNum - 1;

	for (; i >= 0; --i) {
		sizeLev = this->strList[i]->sizeLev;
		if (sizeLev == preSizeLev + 1) {
			// This is a continuous level.
//			preSum += this->strList[i]->GetQueryList().size();
			preSum += this->strList[i]->aliveNum;
			if (preSum <= (1 << sizeLev)) {
				break;
			} else {
				preSizeLev = sizeLev;
			}
		} else {
			// The index of the previous structure.
			++i;
			break;
		}
	}

	if (i < 0) {
		// In this case, all the structures should be merged together.
		i = 0;
	}

	return i;
}

template<class T>
int LogarithmicFramework<T>::FindMergableList(int listSizeToIns, int& preSum) {
	int strNum = this->strList.size();

	if (strNum <= 0) {
		ShowError(
				"Error in LogarithmicFramework::FindMergableList: the strList is empty!\n");
		return 0;
	}

	preSum = listSizeToIns + this->strList[strNum - 1]->aliveNum;
	int sizeLev = 0;
	int i = strNum - 2;

	for (; i >= 0; --i) {
		sizeLev = this->strList[i]->sizeLev;
		if (preSum <= (1 << (sizeLev - 1))) {
			// In this case, we should not merge the strList[i].
			++i;
			break;
		}
		preSum += this->strList[i]->aliveNum;
	}

	if (i < 0) {
		// In this case, all the structures should be merged together.
		i = 0;
	}

	return i;
}

template<class T>
bool LogarithmicFramework<T>::CheckRebuild() {
	bool rtn = (this->delNum + this->alertNum - this->fixNum)
			>= ((this->lastQNum + this->insNum - this->fixNum) / 2);

	rtn = rtn && ((this->lastQNum + this->insNum - this->fixNum) >= SMALL_SIZE);

	return rtn;
}

#endif /* LOGARITHMICFRAMEWORK_H_ */
