/*
 * ToolFunctions.cpp
 *
 *  Created on: 14 Sep, 2015
 *      Author: jhgan
 */

#include "ToolFunctions.h"
#include <stdio.h>
using namespace std;

/*
 * Print the usage of the program.
 */
void usage() {
	printf(
			"\n==================================================================\n");
	printf(
			"Threshold Monitor coded by Junhao Gan, The Chinese University of Hong Kong.\n");
	printf(
			"==================================================================\n");
	printf("Options:\n");
	printf("-algo\t{integer}\t the choice of algorithm\n");
	printf("-d\t{integer}\t the dimensionality of points\n");
	printf("-n\t{integer}\t the number of insertions of points\n");
	printf("-m\t{integer}\t the number of queries\n");
	printf("-t\t{integer}\t the value of uniform threshold value\n");
	printf("-w\t{integer}\t the value of uniform weight of points.\n");
	printf("\tIf w = -1, the weight of each point is drawn from N(100, 15^2).\n");
	printf("-tn\t{integer}\t the target number of point insertions\n");
//	printf("-qs\t{string}\t the path of query set file\n");
	printf("-lf\t{string}\t the path of update log folder\n");
	printf("-ls\t{string}\t the path of update log file\n");
	printf("-rf\t{string}\t the path of result output folder\n");
	printf("-rs\t{string}\t the path of result output file\n");
	printf("-gt\t{string}\t the path of ground truth file\n");
//	printf(
//			"-eps\t{float}\t\t the value of epsilon value for approximate version\n");
//	printf(
//			"-del\t{0,1}\t\t the value indicates whether to support point deletions\n");
	printf("\n");

//	printf("0. Generate a update log.\n");
//	printf(
//			"   Parameter list: -algo 0 -gen -d -qsn -pin -qin -qdn -t -w -lf [-u] [-sigma] [-qlen]\n");

	printf("1. Run our DT algorithm.\n");
//	printf("   Parameter list: -algo 1 -d -n -m -t -w -ls -rf\n");
	printf("   Parameter list: -algo 1 -ls -rf -t -w [-tn] \n");

	printf("2. Run the interval tree algorithm.\n");
	printf("   Parameter list: -algo 2 -ls -rf -t -w [-tn]\n");

	printf("3. Run the brute-force naive algorithm.\n");
	printf("   Parameter list: -algo 3 -ls -rf -t -w [-tn]\n");

	printf("4. Run the R-tree algorithm.\n");
	printf("   Parameter list: -algo 4 -ls -rf -t -w [-tn]\n");

	printf("5. Compare a result output file with ground truth.\n");
	printf("   Parameter list: -algo 5 -rs -gt\n");

	printf("\n");

	printf(
			"==================================================================\n");
	printf("\n");
}

/*****************************************************
 *			Tools for Collecting Input
 *****************************************************/

/*
 *  Get the index of next unblank char from a string.
 */
int getNextChar(char *str) {
	int rtn = 0;

	// Jump over all blanks
	while (str[rtn] == ' ') {
		rtn++;
	}

	return rtn;
}

/*
 *  Get next word from a string.
 */
void getNextWord(char *str, char *word) {
	// Jump over all blanks
	while (*str == ' ') {
		str++;
	}

	while (*str != ' ' && *str != '\0') {
		*word = *str;
		str++;
		word++;
	}

	*word = '\0';
}

/*****************************************************
 *			Tools for Correctness Checking
 *****************************************************/
bool CheckResultFile(MyVector<int>& resultFile, MyVector<int>& groundTruth) {
	int num1 = resultFile.size();
	int num2 = groundTruth.size();
	if (num1 != num2) {
		printf(
				"The size of the result file is not the same as that of ground truth!\n");
		printf("resultFile size = %d\t groundTruth size = %d\n", num1, num2);
		printf("\n");
		fflush(stdout);
		//
		return false;
	}
	int curPos = 0;
	for (int i = 0; i < num1; ++i) {
		if (resultFile[i] != groundTruth[i]) {
			curPos = i;
			while (resultFile[curPos] != 0) {
				// 0 means the start of a line.
				curPos--;
				if (curPos < 0) {
					break;
				}
			}
			if (curPos >= 0) {
				curPos++;
				printf("result file list:\n");
				printf("point ID = %d\n", resultFile[curPos++]);
				printf("alert ID list:\n");
				while (curPos <= i) {
					printf("%d\t", resultFile[curPos++]);
				}
				printf("\n");
			}

			curPos = i;
			while (groundTruth[curPos] != 0) {
				// 0 means the start of a line.
				curPos--;
				if (curPos < 0) {
					break;
				}
			}
			if (curPos >= 0) {
				curPos++;
				printf("ground truth list:\n");
				printf("point ID = %d\n", groundTruth[curPos++]);
				printf("alert ID list:\n");
				while (curPos <= i) {
					printf("%d\t", groundTruth[curPos++]);
				}
				printf("\n");
			}
			return false;
		}
	}
	return true;
}

bool CheckCorrectness(MyVector<int>& idList1, MyVector<int>& idList2) {
	int num1 = idList1.size();
	int num2 = idList2.size();
	if (num1 != num2) {
		// For debug
		sort(idList1.get_list(), idList1.get_list() + num1);
		sort(idList2.get_list(), idList2.get_list() + num2);
		printf("list1:\t");
		for (int i = 0; i < num1; ++i) {
			printf("%d ", idList1[i]);
		}
		printf("\n");

		printf("list2:\t");
		for (int i = 0; i < num2; ++i) {
			printf("%d ", idList2[i]);
		}
		printf("\n");
		fflush(stdout);
		//
		return false;
	}

	sort(idList1.get_list(), idList1.get_list() + num1);
	sort(idList2.get_list(), idList2.get_list() + num2);

	for (int i = 0; i < num1; ++i) {
		if (idList1[i] != idList2[i]) {
			printf("id1 = %d id2= %d\n", idList1[i], idList2[i]);
			return false;
		}
	}
	return true;
}

bool CheckCorrectness(vector<int>& idList1, vector<int>& idList2) {
	int num1 = idList1.size();
	int num2 = idList2.size();
	if (num1 != num2) {
		// For debug
		sort(idList1.begin(), idList1.end());
		sort(idList2.begin(), idList2.end());
		printf("list1:\t");
		for (int i = 0; i < num1; ++i) {
			printf("%d ", idList1[i]);
		}
		printf("\n");

		printf("list2:\t");
		for (int i = 0; i < num2; ++i) {
			printf("%d ", idList2[i]);
		}
		printf("\n");
		fflush(stdout);
		//
		return false;
	}

	sort(idList1.begin(), idList1.end());
	sort(idList2.begin(), idList2.end());

	for (int i = 0; i < num1; ++i) {
		if (idList1[i] != idList2[i]) {
			printf("id1 = %d id2= %d\n", idList1[i], idList2[i]);
			return false;
		}
	}
	return true;
}

#ifdef DYNAMIC

bool CheckCorrectness_Dynamic(vector<Query*>& list1,
		vector<QueryBase*>& exactList) {
	int num1 = list1.size();
	int num2 = exactList.size();

	if (num1 != num2) {
		printf("num1 = %d\t num2 = %d\n", num1, num2);
		return false;
	}

	int exactCnt = 0;
	Query* q = NULL;

	for (int i = 0; i < num1; ++i) {
		if (list1[i]->id != exactList[i]->id) {
			printf("id1 = %d\t id2 = %d\n", list1[i]->id, exactList[i]->id);
			return false;
		}
		exactCnt = exactList[i]->GetCounter();

		q = (Query*) list1[i];

		if (q->threshold == -1 && exactList[i]->threshold == -1) {
			continue;
		} else {
			if (q->eps * q->threshold >= 1) {
				if (exactCnt < q->threshold && q->ShowQueryState() != 0) {
					printf(
							"query id = %d: exactCnt = %d evlCnt = %d eps = %f threshold = %d flag = %d\n",
							q->id, exactCnt, q->GetCounter(), q->eps,
							q->threshold, q->ShowQueryState());

					return false;
				}

				if (exactCnt > (1 + q->eps) * q->threshold
						&& q->ShowQueryState() != 1) {
					printf(
							"query id = %d: exactCnt = %d evlCnt = %d eps = %f threshold = %d flag = %d\n",
							q->id, exactCnt, q->GetCounter(), q->eps,
							q->threshold, q->ShowQueryState());
					return false;
				}
			} else {
				if (exactCnt >= q->threshold && q->ShowQueryState() != 1) {
					printf(
							"query id = %d: exactCnt = %d evlCnt = %d eps = %f threshold = %d flag = %d\n",
							q->id, exactCnt, q->GetCounter(), q->eps,
							q->threshold, q->ShowQueryState());
					return false;
				}
			}

		}
	}
	return true;
}

bool CheckCorrectness_RangeCnt(vector<QueryRangeCnt*>& list1,
		vector<QueryBase*>& exactList) {
	int num1 = list1.size();
	int num2 = exactList.size();

	if (num1 != num2) {
		printf("num1 = %d\t num2 = %d\n", num1, num2);
		return false;
	}

	int exactCnt = 0;
	QueryRangeCnt* q = NULL;
	int error = 0;

	for (int i = 0; i < num1; ++i) {
		if (list1[i]->id != exactList[i]->id) {
			printf("id1 = %d\t id2 = %d\n", list1[i]->id, exactList[i]->id);
			return false;
		}
		exactCnt = exactList[i]->GetCounter();

		q = (QueryRangeCnt*) list1[i];

		if (q->threshold == -1 && exactList[i]->threshold == -1) {
			continue;
		} else {
			error = floor(q->eps * q->threshold);
			if (exactCnt + error < q->GetCounter()
					|| exactCnt - error > q->GetCounter()) {
				printf(
						"query id = %d: exactCnt = %d evlCnt = %d eps = %f threshold = %d flag = %d\n",
						q->id, exactCnt, q->GetCounter(), q->eps, q->threshold,
						q->ShowQueryState());
				return false;
			}

//			if (q->eps * q->threshold >= 1) {
//				if (exactCnt < q->threshold && q->ShowQueryState() != 0) {
//					printf(
//							"query id = %d: exactCnt = %d evlCnt = %d eps = %f threshold = %d flag = %d\n",
//							q->id, exactCnt, q->GetCounter(), q->eps,
//							q->threshold, q->ShowQueryState());
//
//					return false;
//				}
//
//				if (exactCnt > (1 + q->eps) * q->threshold
//						&& q->ShowQueryState() != 1) {
//					printf(
//							"query id = %d: exactCnt = %d evlCnt = %d eps = %f threshold = %d flag = %d\n",
//							q->id, exactCnt, q->GetCounter(), q->eps,
//							q->threshold, q->ShowQueryState());
//					return false;
//				}
//			} else {
//				if (exactCnt >= q->threshold && q->ShowQueryState() != 1) {
//					printf(
//							"query id = %d: exactCnt = %d evlCnt = %d eps = %f threshold = %d flag = %d\n",
//							q->id, exactCnt, q->GetCounter(), q->eps,
//							q->threshold, q->ShowQueryState());
//					return false;
//				}
//			}
		}
	}
	return true;
}

bool CheckSame(vector<QueryBase*>& list, vector<QueryBase*>& exactList) {
	int num1 = list.size();
	int num2 = exactList.size();

	if (num1 != num2) {
		printf("num1 = %d\t num2 = %d\n", num1, num2);
		return false;
	}

	int exactCnt = 0;
	QueryBase* q = NULL;

	for (int i = 0; i < num1; ++i) {
		if (list[i]->id != exactList[i]->id) {
			printf("id1 = %d\t id2 = %d\n", list[i]->id, exactList[i]->id);
			return false;
		}

		exactCnt = exactList[i]->GetCounter();
		q = list[i];

		if (q->GetCounter() != exactCnt) {
			printf(
					"query id = %d: exactCnt = %d evlCnt = %d eps = %f threshold = %d flag = %d\n",
					list[i]->id, exactCnt, q->GetCounter(), q->eps,
					q->threshold, q->ShowQueryState());
			return false;
		}
	}

	return true;
}
#endif

/*****************************************************
 *			Tools for Query Generation
 *****************************************************/

/*
 *  Generate a list of queries from the raw data read.
 */
void GenQueryListFromRawData(MyVector<Query>& targetPlace,
		MyVector<int>& rangeList, MyVector<QueryBase> const& rawData) {
	int qNum = rawData.size();

	targetPlace.clear();
	targetPlace.reserve(qNum);

	rangeList.clear();
	rangeList.reserve(qNum * 2 * DIM);

//	int* rangePos = NULL;
	Query temp;

	for (int i = 0; i < qNum; ++i) {

		temp.ValueCopy(rawData[i]);
//		temp.range = rangePos;
		for (int j = 0; j < 2 * DIM; ++j) {
			rangeList.push_back(rawData[i].range[j]);
		}

		temp.range = &(rangeList[rangeList.size() - 2 * DIM]);

		targetPlace.push_back(temp);
//		rangePos += 2 * DIM;
	}
}

/*
 *  Generate a list of queries from the raw data read.
 */
void GenQueryListFromRawData(MyVector<Query>& targetPlace,
		MyVector<QueryBase> const& rawData) {
	int qNum = rawData.size();

	targetPlace.clear();
	targetPlace.reserve(qNum);

//	int* rangePos = NULL;
	Query temp;

	for (int i = 0; i < qNum; ++i) {

		temp.ValueCopy(rawData[i]);
		targetPlace.push_back(temp);
	}
}

/*
 *  Generate a list of queries from the raw data read.
 */
void GenQueryListFromRawData(MyVector<QueryBase>& targetPlace,
		MyVector<int>& rangeList, MyVector<QueryBase> const& rawData) {
	int qNum = rawData.size();

	targetPlace.clear();
	targetPlace.reserve(qNum);

	rangeList.clear();
	rangeList.reserve(qNum * 2 * DIM);

//	int* rangePos = rangeList;
	QueryBase temp;

	for (int i = 0; i < qNum; ++i) {
		temp.ValueCopy(rawData[i]);
//		temp.range = rangePos;
		for (int j = 0; j < 2 * DIM; ++j) {
//			temp.range[j] = rawData[i].range[j];
			rangeList.push_back(rawData[i].range[j]);
		}

		temp.range = &(rangeList[rangeList.size() - 2 * DIM]);

		targetPlace.push_back(temp);

//		rangePos += 2 * DIM;
	}
}

void GenQueryListFromRawData(MyVector<QueryBase>& targetPlace,
		MyVector<QueryBase> const& rawData) {
	int qNum = rawData.size();

	targetPlace.clear();
	targetPlace.reserve(qNum);

	QueryBase temp;

	for (int i = 0; i < qNum; ++i) {
		temp.ValueCopy(rawData[i]);
		targetPlace.push_back(temp);
	}
}

/*
 *  Generate a query range by the last range generated.
 */
void GenQueryRange(vector<int>& lastRange, vector<int>& targetRange) {
	int num = lastRange.size();
	if (num != DIM * 2) {
		ShowError(
				"Error in GenQueryRange: The size of lastRange is not 2*DIM!\n");
	}

	targetRange.clear();
	targetRange.resize(num);

	int a = 0;
	int b = 0;
	for (int i = 0; i < DIM; ++i) {
		a = GenRandInt((int) (lastRange[2 * i]), (int) (lastRange[2 * i + 1]));
		b = GenRandInt(rangeStart, rangeEnd);
		if (a < b) {
			targetRange[2 * i] = a;
			targetRange[2 * i + 1] = b;
		} else {
			targetRange[2 * i] = b;
			targetRange[2 * i + 1] = a;
		}
	}
}

/*
 *  Generate a query range in the default range.
 */
void GenQueryRange(vector<int>& targetRange, int dim) {

	targetRange.clear();
	targetRange.resize(2 * dim);

	int a = 0;
	int b = 0;
	for (int i = 0; i < dim; ++i) {
		a = GenRandInt(rangeStart, rangeEnd);
		b = GenRandInt(rangeStart, rangeEnd);
		if (a < b) {
			targetRange[2 * i] = a;
			targetRange[2 * i + 1] = b;
		} else {
			targetRange[2 * i] = b;
			targetRange[2 * i + 1] = a;
		}
	}
}

void GenQueryRangeList(int qNum, vector<vector<int> >& targetPlace, int s,
		int rs, int re) {
	targetPlace.clear();
	targetPlace.resize(qNum);

	for (int i = 0; i < qNum; ++i) {
		targetPlace[i].resize(2);
		targetPlace[i][0] = re;
		targetPlace[i][1] = rs;
	}

	int tempID = 0;

	for (int i = rs; i <= re; ++i) {
		for (int j = 0; j < s; ++j) {
			tempID = GenRandInt(1, qNum);
			if (i < targetPlace[tempID - 1][0]) {
				targetPlace[tempID - 1][0] = i;
			}
			if (i > targetPlace[tempID - 1][1]) {
				targetPlace[tempID - 1][1] = i;
			}
		}
	}
}

