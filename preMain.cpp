//============================================================================
// Name        : ThesholdMonitor.cpp
// Author      : jhgan
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "GlobalVars.h"
#include "ToolFunctions.h"
#include "./FileIO/QueryGen_IO.h"
#include "./FileIO/LogGen_IO.h"
#include "./FileIO/ResultFile_IO.h"
#include "./MyRandom/MyRandom.h"
#include "./MyVector/MyVector.h"
#include "./MyVector/MyVectorInt.h"

#include "Query.h"
#include "./SegmentTree/SegmentTree.h"
#include "./SegmentTree/SegmentTreeLog.h"
#include "./SegmentTreePure/SegmentTreePure.h"
#include "./SegmentTreePure/SegmentTreePureLog.h"
#include "./IntervalTree/IntervalTree.h"
#include "./IntervalTree/IntervalTreeLog.h"
#include "./Rtree/Rtree.h"
#include "BFNaive.h"

#include "./Logarithmic/LogarithmicFramework.h"
#include "./Logarithmic/AbsStructure.h"

#include <fstream>
#include <vector>
#include <map>
#include <list>
#include <queue>
#include <iostream>
#include <stdio.h>
#include <random>
#include <string.h>
#include <math.h>

using namespace std;

struct DelEntry {
	int time;
	int delID;
};

struct Reverse_Comparison {
	bool operator()(const DelEntry& e1, const DelEntry& e2) {
		return e1.time > e2.time;
	}
};

/*
 *  The number of bytes per mega byte.
 */
const double PER_M = 1024.0 * 1024;

int preMain(int nargs, char **args) {

	srand(time(NULL));

	usage();

	// The choice of algorithm.
	int algo = -1;
	// Number of queries.
	int qNum = 0;
	// Number of insertions.
	int insNum = 0;
	// The uniform threshold value.
	int uni_t = -1;
	// The weight of each point.
	int uni_w = -1;

	// The number of queries registered at the beginning.
	int qsn = 0;
	// The number of insertions of points.
	int pin = 0;
	// The number of insertions of queries.
	int qin = 0;
	// The number of deletion of queries.
	int qdn = 0;
	// The number of the spare queries.
	int sqn = 0;
	// The flag that whether to keep the alive number.
	// If keep = 1, keep the alive number. Otherwise, not.
	int keep = 0;

	int targetN = 0;

	// The probability of generating a query insertion at each time stamp.
	double qInsProb = -1;

	// The path of query file.
	char qs[300];
	// The path of the log folder.
	char lf[300];
	// The path of the log file.
	char ls[300];
	// The path of result folder.
	char rf[300];
	// The path of result file.
	char rs[300];
	// The path of ground truth file.
	char gt[300];
	// The path of spare query file.
	char sf[300];
	// The temporal file path.
	char tempFilePath[300];

	// The generation type of log.
	// gen = 1 means generate log from Gausssian distribution.
	// gen = 0 means generate log from Uniform distribution.
	int gen = -1;

	bool failed = false;
	char *arg;
	char para[10];
	int cnt = 1;
	int index = 0;

	printf("The input parameters are:\n\n");

	while (cnt < nargs && !failed) {
		arg = args[cnt++];
		if (cnt == nargs) {
			failed = true;
			break;
		}

		index = getNextChar(arg);
		if (arg[index] != '-') {
			failed = true;
			break;
		}
		getNextWord(arg + index + 1, para);
		printf("%s\t", para);

		arg = args[cnt++];

		if (strcmp(para, "n") == 0) {
			insNum = atoi(arg);
			if (insNum <= 0) {
				failed = true;
				break;
			}
			printf("%d\n", insNum);
		} else if (strcmp(para, "d") == 0) {
			DIM = atoi(arg);
			if (DIM <= 0) {
				printf("d <= 0!\n");
				failed = true;
				break;
			}
			printf("%d\n", DIM);
		} else if (strcmp(para, "m") == 0) {
			qNum = atoi(arg);
			if (qNum <= 0) {
				printf("qNum <= 0!\n");
				failed = true;
				break;
			}
			printf("%d\n", qNum);
		} else if (strcmp(para, "algo") == 0) {
			algo = atoi(arg);
			if (algo < 0 || algo > 6) {
				printf("algo < 0 or algo > 4!\n");
				failed = true;
				break;
			}
			printf("%d\n", algo);
		} else if (strcmp(para, "t") == 0) {
			uni_t = atoi(arg);
			if (uni_t <= 0) {
				printf("uni_t <= 0!\n");
				failed = true;
				break;
			}
			printf("%d\n", uni_t);
		} else if (strcmp(para, "w") == 0) {
			uni_w = atoi(arg);
//			if (uni_w <= 0) {
//				printf("uni_w <= 0!\n");
//				failed = true;
//				break;
//			}
			printf("%d\n", uni_w);
		} else if (strcmp(para, "qsn") == 0) {
			qsn = atoi(arg);
			if (qsn < 0) {
				printf("qsn < 0!\n");
				failed = true;
				break;
			}
			printf("%d\n", qsn);
		} else if (strcmp(para, "pin") == 0) {
			pin = atoi(arg);
			if (pin < 0) {
				printf("pin < 0!\n");
				failed = true;
				break;
			}
			printf("%d\n", pin);
		} else if (strcmp(para, "qin") == 0) {
			qin = atoi(arg);
			if (qin < 0) {
				printf("qin < 0!\n");
				failed = true;
				break;
			}
			printf("%d\n", qin);
		} else if (strcmp(para, "qdn") == 0) {
			qdn = atoi(arg);
			if (qdn < 0) {
				printf("qdn < 0!\n");
				failed = true;
				break;
			}
			printf("%d\n", qdn);
		} else if (strcmp(para, "tn") == 0) {
			targetN = atoi(arg);
			if (targetN < 0) {
				printf("targetN < 0!\n");
				failed = true;
				break;
			}
			printf("%d\n", targetN);
		} else if (strcmp(para, "sqn") == 0) {
			sqn = atoi(arg);
			if (sqn < 0) {
				printf("sqn < 0!\n");
				failed = true;
				break;
			}
			printf("%d\n", sqn);
		} else if (strcmp(para, "keep") == 0) {
			keep = atoi(arg);
			if (keep != 0 && keep != 1) {
				printf("keep != 0 && keep != 1!\n");
				failed = true;
				break;
			}
			printf("%d\n", keep);
		} else if (strcmp(para, "qp") == 0) {
			qInsProb = atof(arg);
			if (qInsProb < 0) {
				printf("qInsProb < 0!\n");
				failed = true;
				break;
			}
			printf("%lf\n", qInsProb);
		} else if (strcmp(para, "u") == 0) {
			U = atoi(arg);
			printf("%d\n", U);
		} else if (strcmp(para, "sigma") == 0) {
			SIGMA = atoi(arg);
			printf("%d\n", SIGMA);
		} else if (strcmp(para, "qlen") == 0) {
			QLEN = atoi(arg);
			printf("%d\n", QLEN);
		} else if (strcmp(para, "gen") == 0) {
			gen = atoi(arg);
			if (gen < 0 || gen > 2) {
				printf("gen < 0 || gen > 2!\n");
				failed = true;
				break;
			}
			printf("%d\n", gen);
		} else if (strcmp(para, "qs") == 0) {
			getNextWord(arg, qs);
			printf("%s\n", qs);
		} else if (strcmp(para, "lf") == 0) {
			getNextWord(arg, lf);
			printf("%s\n", lf);
		} else if (strcmp(para, "ls") == 0) {
			getNextWord(arg, ls);
			printf("%s\n", ls);
		} else if (strcmp(para, "sf") == 0) {
			getNextWord(arg, sf);
			printf("%s\n", sf);
		} else if (strcmp(para, "rf") == 0) {
			getNextWord(arg, rf);
			printf("%s\n", rf);
		} else if (strcmp(para, "rs") == 0) {
			getNextWord(arg, rs);
			printf("%s\n", rs);
		} else if (strcmp(para, "gt") == 0) {
			getNextWord(arg, gt);
			printf("%s\n", gt);
		} else {
			failed = true;
			printf("Unknown option -%s!\n\n", para);
		}

	}

	if (failed) {
		//usage();
		printf(
				"\n==================================================================\n");
		printf("The input parameters are incorrect! Please check them again!");
		printf(
				"\n==================================================================\n");
		return 0;
	}

	printf(
			"\n==================================================================\n");

	fflush(stdout);

////	// Read the log file.
////	MyVector<QueryBase> myqueryList;
////	MyVector<int> myopList;
////	ReadLogFromFile(ls, myqueryList, myopList);
////
////	myqueryList.release_space();
////
////	printf("Log read.\n");
////
//	int m = 1000000;
//	int myTargetN = 2000000;
//	double insProb = 0;
//	int expInsNum = 0;
//	int timeStamp = 0;
//	int opNum = 0;
//	int myStep = 20000;
//	double time = 0;
//
//	int myOpCnt = m;
//	int myptInsNum = 0;
////	int myOpLen = myopList.size();
////	MyVector<int> myOpCntList;
////	myOpCntList.reserve(myTargetN / myStep);
////
////	for (int i = 0; i < myOpLen; i++) {
////		if (myopList[i] == -1) {
////			// The end of the log resultFile.
////			break;
////		} else if (myopList[i] == 1) {
////			// Insertion of a point.
////			i += 1 + DIM + 1;
////			myOpCnt++;
////			myptInsNum++;
////
////			if (myptInsNum % myStep == 0) {
////				myOpCntList.push_back(myOpCnt);
////			}
////
////		} else if (myopList[i] == 2) {
////			// Insertion of a query.
////			i += 1 + 2 * DIM + 1;
////			myOpCnt++;
////		} else if (myopList[i] == 3) {
////			// Deletion of a query.
////			++i;
////			myOpCnt++;
////		} else {
////			printf("opList[%d] = %d\n", i, myopList[i]);
////			ShowError("Error in function main: The opList is incorrect!\n");
////		}
////
////	}
////
////	myopList.release_space();
//
////	char toProcPath[300] = "/home/jhgan/Desktop/test";
//	std::ifstream infile(rs);
//
//	char outputPath[300];
//
//	sprintf(outputPath, "%s_perOp", rs);
//	FILE* file = fopen(outputPath, "wt");
//	if (file == NULL) {
//		ShowError("file open failed.\n");
//	}
//	fprintf(file, "TimeStamp\t AmortizedTime\n");
//
////	FILE* file = fopen(toProcPath, "rt");
////	if(file == NULL){
////		ShowError("File open failed!\n");
////	}
//
////	char tempString[10000];
//	string tempString;
//
////	fscanf(file, "%s\n", tempString);
//
//	size_t pos = 0;
//	int num = 0;
//	char str[1000];
//	int i = 0;
//
//	while (std::getline(infile, tempString) && timeStamp < myTargetN) {
//		pos = tempString.find("aliveNum");
//		if (pos != std::string::npos) {
//			// target found.
////			str = &(tempString[pos + 11]);
//
//			strcpy(str, tempString.c_str());
//			num = atoi(str + pos + 11);
//			printf("aliveNum = %d\t", num);
//		} else {
////			std::getline(infile, tempString);
//			pos = tempString.find("Running time");
//			if (pos != std::string::npos) {
//				// target found.
//				//				str = tempString.c_str();
//
//				timeStamp += myStep;
//				if (timeStamp <= 2000000) {
//					expInsNum = insProb * timeStamp;
//				}
//				opNum = 2 * (m + expInsNum) - num + timeStamp;
//
//
////				opNum = myOpCntList[i++];
//
//				printf("expInsNum = %d\topNum = %d\t", expInsNum, opNum);
//
//				strcpy(str, tempString.c_str());
//
//				time = atof(str + pos + 15);
//				printf("Running Time = %lf\n", time);
//
//				printf("%d\t%.10lf\n", timeStamp, time / opNum);
//
//				fprintf(file, "%d\t%.10lf\n", timeStamp, time / opNum);
//
//			}
//
//		}
//	}
//
//	infile.close();
//	fclose(file);
//
////	for(int i = 0; i< myOpCntList.size(); i++){
////		printf("%d\n", myOpCntList[i]);
////	}
////	myOpCntList.release_space();
//
//
//	return 0;

	/*****************************************************************************/

	double start = 0;
	double end = 0;
	double duration = 0;
	double constructionTime = 0;
	double pinTime = 0;
	double qinTime = 0;
	double qdnTime = 0;

	if (algo == 0) {
		// Generate a update log file.

		if (gen == 1) {
//			// Generate a log from Gaussian distribution.
//			sprintf(ls,
//					"%s/d=%d_qsn=%d_pin=%d_qin=%d_qdn=%d_t=%d_w=%d_U=%d_SIGMA=%d_QLEN=%d_Gaussian.log",
//					lf, DIM, qsn, pin, qin, qdn, uni_t, uni_w, U, SIGMA, QLEN);
////			GenLogToFile_Gaussian(ls, DIM, qsn, pin, qin, qdn, uni_w, uni_t);
//			GenLogToFile_GQuery_UPoint(ls, DIM, qsn, pin, qin, qdn, uni_w,
//					uni_t);

			// Generate a log from Gaussian distribution.
			sprintf(ls,
					"%s/d=%d_qsn=%d_pin=%d_qin=%d_qdn=%d_t=%d_w=%d_U=%d_SIGMA=%d_QLEN=%d_qp=%.2lf_Gaussian.log",
					lf, DIM, qsn, pin, qin, qdn, uni_t, uni_w, U, SIGMA, QLEN,
					qInsProb);
			//			GenLogToFile_Gaussian(ls, DIM, qsn, pin, qin, qdn, uni_w, uni_t);
			GenLogToFile_GQuery_UPoint_InsQ(ls, DIM, qsn, pin, qin, qdn, uni_w,
					uni_t, qInsProb);
		} else if (gen == 2) {
			// Generate a log from Gaussian distribution.
			sprintf(ls,
					"%s/d=%d_qsn=%d_pin=%d_qin=%d_qdn=%d_t=%d_w=%d_U=%d_SIGMA=%d_QLEN=%d_Gaussian_KeepAliveNum.log",
					lf, DIM, qsn, pin, qin, qdn, uni_t, uni_w, U, SIGMA, QLEN);
			sprintf(sf,
					"%s/d=%d_qsn=%d_pin=%d_qin=%d_qdn=%d_t=%d_w=%d_U=%d_SIGMA=%d_QLEN=%d_Gaussian_KeepAliveNum.spare",
					lf, DIM, qsn, pin, qin, qdn, uni_t, uni_w, U, SIGMA, QLEN);

			//			GenLogToFile_Gaussian(ls, DIM, qsn, pin, qin, qdn, uni_w, uni_t);
			GenLogToFile_GQuery_UPoint_KeepAliveNum(ls, DIM, qsn, pin, qin, qdn,
					uni_w, uni_t, sf, sqn);

		} else {
			// Generate a log from Uniform distribution.
			sprintf(ls,
					"%s/d=%d_qsn=%d_pin=%d_qin=%d_qdn=%d_t=%d_w=%d_QLEN=%d_Uniform.log",
					lf, DIM, qsn, pin, qin, qdn, uni_t, uni_w, QLEN);
//			GenLogToFile_Uniform(ls, DIM, qsn, pin, qin, qdn, uni_t, uni_w);
			GenLogToFile_Uniform_TestDel(ls, DIM, qsn, pin, qin, qdn, uni_w,
					uni_t);
		}
		printf("\nLog generation done.\n");
		return 0;
	} else if (algo == 1) {
		// Run our STR algorithm.
		MyVector<int> LogList; // log
		MyVector<Query> logList;

		// Read the log file.
		MyVector<QueryBase> queryList;
		MyVector<int> opList;
		ReadLogFromFile(ls, queryList, opList);

		printf("Log read.\n");

		// Increase the number of operations by the number of queries registered at the beginning.
		OPNUM = queryList.size();

		/************ Generate an output file. **************/

//		MyVector<int> spareList;
//		int spareCurPos = 0;
//		int spareInsCnt = 0;
//
//		// The min heap of the DelEntry with time as key.
//		priority_queue<DelEntry, vector<DelEntry>, Reverse_Comparison> delList;
//		if (keep == 1) {
//			// We keep the alive number.
//			ReadSpareQueryFile(sf, spareList);
//			printf("Spare file read.\n");
//		}
//
//		char outputLog[300];
//		sprintf(outputLog, "%s_Output", ls);
//		FILE* output = fopen(outputLog, "wt");
//
//		fprintf(output, "%d\t%d\t%d\t%d\t%d\n", DIM, QSN, PIN, QIN, QDN);
//		// Output all the queries to file first.
//		for (int i = 0; i < QSN; i++) {
//			WriteQueryToFile(output, &(queryList[i]), DIM);
//		}
		/***************************************************/

		// Compute the result file path for this algorithm.
		sprintf(rs, "%s/Ours_d=%d_m=%d_n=%d_t=%d", rf, DIM, QSN, PIN, uni_t);
		FILE* resultFile = fopen(rs, "wt");
		if (resultFile == NULL) {
			ShowError(
					"Error in function main:\nResult file open failed! Please check your file path!\n");
		}

		// The file to store the space consumption.
		sprintf(tempFilePath, "%s/Ours_d=%d_m=%d_n=%d_t=%d_SpaceCons", rf, DIM,
				QSN, PIN, uni_t);
		FILE* spaceFile = fopen(tempFilePath, "wt");
		if (spaceFile == NULL) {
			ShowError(
					"Error in function main:\nSpaceCons file open failed! Please check your file path!\n");
		}

		fprintf(spaceFile, "TimeStamp\t SpaceCons\n");

		// The file to store the amortized time.
		sprintf(tempFilePath, "%s/Ours_d=%d_m=%d_n=%d_t=%d_AmortizedTime", rf,
				DIM, QSN, PIN, uni_t);
		FILE* aTimeFile = fopen(tempFilePath, "wt");
		if (aTimeFile == NULL) {
			ShowError(
					"Error in function main:\nAmortizedTime file open failed! Please check your resultFile path!\n");
		}
		fprintf(aTimeFile, "TimeStamp\t AmortizedTime\n");

		int opCnt = 0;

		if (targetN == 0) {
			targetN = uni_t / 10;
			if (uni_w == 1) {
				targetN = uni_t * 10;
			}
		}

//		int targetN = PIN;
		printf("targetN = %d\n", targetN);
//		int step = opNum / 10;
		int step = targetN / 100;
		if (step == 0) {
			step = 1;
		}

		printf("Start to generate query list.\n");
		fflush(stdout);

		// Generate the query list we need.
		GenQueryListFromRawData(logList, queryList);

		// Release the raw data list.
		queryList.release_space();

		if (uni_t != -1) {
			// Initialize the threshold of all the queries to uni_t.
			int qNum = logList.size();
			for (int i = 0; i < qNum; ++i) {
				logList[i].threshold = uni_t;
			}
		}

		printf("Start to construct the structure.\n");
		fflush(stdout);

		// Construct the whole structure.
		start = getCurrentTime();
		SegmentTree_Log* strConstructor = new SegmentTree_Log();
		LogarithmicFramework<Query>* our_log = new LogarithmicFramework<Query>(
				strConstructor, logList);

		end = getCurrentTime();
		duration += end - start;
		constructionTime += end - start;

		printf("Construction done.\n");
		printf("Construction time = %lf seconds.\n", constructionTime);
//		printf("IniTime_Ours = %lf\n", IniTime_Ours);
		fflush(stdout);

		long long spaceCons = our_log->ComputeSpaceConsumption();
		long long maxSpaceCons = spaceCons;

		printf("The initial space consumption = %lf MB.\n", spaceCons / PER_M);
		// Output to the space file.
		fprintf(spaceFile, "0\t %lf MB\n", spaceCons / PER_M);

		int pid = 0;
		MyVector<int> ptCoords;
		ptCoords.resize(DIM, 0);
		int ptWeight = 0;
		MyVector<int> alertIDList;

		int qid = 0;
		MyVector<int> range;
		range.resize(2 * DIM, 0);
		int threshold = 0;

		int ptInsNum = 0;
		int maturedNum = 0;
		int qDelNum = 0;

//		double tempTime = 0;
		bool tempState = false;

//		start = getCurrentTime();
		int opLength = opList.size();
		for (int i = 0; i < opLength; ++i) {
			if (opList[i] == -1) {
				// The end of the log resultFile.
				break;
			} else if (opList[i] == 1) {
				// Insertion of a point.
				pid = opList[++i];

				for (int j = 0; j < DIM; ++j) {
					ptCoords[j] = opList[++i];
				}
				ptWeight = opList[++i];

				/******************************************/

//				fprintf(output, "1\t%d\t", pid);
//				for (int j = 0; j < DIM; ++j) {
//					fprintf(output, "%d\t", ptCoords[j]);
//				}
//				fprintf(output, "%d\n", ptWeight);
				/*****************************************/

				if (uni_w != -1) {
					// The user has specified a uniform weight of all the points.
					ptWeight = uni_w;
				}

				start = getCurrentTime();
				tempState = our_log->InsertPoint(ptCoords.get_list(), ptWeight,
						alertIDList);
				end = getCurrentTime();
				duration += end - start;
				pinTime += end - start;

//				if (keep == 1) {
//					// Keep the alive number.
//					// After insert a point, check whether to delete queries.
//					while (delList.empty() == false
//							&& ptInsNum == delList.top().time) {
//
//						/*****************************************/
//
//						fprintf(output, "3\t%d\n", delList.top().delID);
//
//						/*****************************************/
//
//						start = getCurrentTime();
//						tempState = our_log->DeleteQuery(delList.top().delID);
//						end = getCurrentTime();
//						duration += end - start;
//						qdnTime += end - start;
//
////						printf("delTime = %d\n", delList.top().time);
//
//						// Delete the top of the priority queue.
//						delList.pop();
//					}
//
//					// After insert a point, check whether need to insert new queries.
//					if (our_log->GetAliveNum() < QSN) {
//						// We need to insert new queries.
//
//						int delta = QSN - our_log->GetAliveNum();
//						while (delta > 0) {
//							// Insert a query of spareList.
//							qid = spareList[spareCurPos++];
//							if (qid == -1) {
//								ShowError(
//										"Error in main: Not enough spare queries! Please generate a larger spare query file!\n");
//							}
//
//							for (int j = 0; j < 2 * DIM; ++j) {
//								range[j] = spareList[spareCurPos++];
//							}
//							threshold = spareList[spareCurPos++];
//							if (uni_t != -1) {
//								// The user has specified a uniform threshold for all the queries.
//								threshold = uni_t;
//							}
//
//							Query q(qid, range, threshold);
//
//							/*****************************************/
//
//							fprintf(output, "2\t");
//							q.threshold = q.threshold / 100;
//							WriteQueryToFile(output, &q, DIM);
//							q.threshold = uni_t;
//
//							/*****************************************/
//
//							start = getCurrentTime();
//							our_log->InsertQuery(q);
//							end = getCurrentTime();
//							duration += end - start;
//							qinTime += end - start;
//
//							// Insert a new element to the priority queue.
//							DelEntry de;
//							de.time = ptInsNum + spareList[spareCurPos++];
//							de.delID = q.id;
//							delList.push(de);
//
//							spareInsCnt++;
//
//							delta--;
//						}
//					}
//				}

				ptInsNum++;

				if (tempState || ptInsNum % step == 0) {
					// Output the space consumption to space file.
					spaceCons = our_log->ComputeSpaceConsumption();
					if (spaceCons > maxSpaceCons)
						maxSpaceCons = spaceCons;
					printf(
							"TimeStamp = %d\t SpaceCons = %lf MB\t aliveNum = %d\n",
							ptInsNum, spaceCons / PER_M,
							our_log->GetAliveNum());
					fflush(stdout);

					fprintf(spaceFile, "%d\t %lf MB\t aliveNum = %d\n",
							ptInsNum, spaceCons / PER_M,
							our_log->GetAliveNum());
				}

				if (ptInsNum % step == 0) {
//					printf(
//							"%d %%\t point insertions done. Running time = %lf.\t Amortized running time = %lf.\n",
//							ptInsNum / step, duration, duration / ptInsNum);

					printf(
							"%d %%\t point insertions done. OPNUM = %d\t Running time = %lf.\t Amortized running time = %.10lf.\n",
							ptInsNum / step, OPNUM, duration, duration / OPNUM);

					printf("pinTime = %lf\n", pinTime);
					printf("qinTime = %lf\n", qinTime);
					printf("qdnTime = %lf\n", qdnTime);
					//				printf("%d %%\t update operations done.\n", opCnt / step * 10);
					fflush(stdout);

					// Output the amortized time to amortized time file.
//					fprintf(aTimeFile, "%d\t %lf\n", ptInsNum,
//							duration / ptInsNum);
					fprintf(aTimeFile, "%d\t %.10lf\n", ptInsNum,
							duration / OPNUM);

				}

				if (WriteResult && alertIDList.size() > 0) {
					maturedNum += alertIDList.size();

					// Some queries has alerted due to the insertion of the current point.
					WriteResultLineToFile(resultFile, pid, alertIDList);
					alertIDList.clear();
				}

				if (our_log->GetAliveNum() == 0) {
					// All the queries have matured.

					// Output the space consumption to space file.
					spaceCons = our_log->ComputeSpaceConsumption();
					if (spaceCons > maxSpaceCons)
						maxSpaceCons = spaceCons;
					printf(
							"TimeStamp = %d\t SpaceCons = %lf MB\t aliveNum = %d\n",
							ptInsNum, spaceCons / PER_M,
							our_log->GetAliveNum());
					fprintf(spaceFile, "%d\t %lf MB\t aliveNum = %d\n",
							ptInsNum, spaceCons / PER_M,
							our_log->GetAliveNum());

					printf(
							"\nAfter %d point insertions, all the queries have matured.\n",
							pid);
					printf("maturedNum = %d\n\n", maturedNum);
					fflush(stdout);
					break;
				}
			} else if (opList[i] == 2) {
				// Insertion of a query.
				qid = opList[++i];
				for (int j = 0; j < 2 * DIM; ++j) {
					range[j] = opList[++i];
				}
				threshold = opList[++i];
				if (uni_t != -1) {
					// The user has specified a uniform threshold for all the queries.
					threshold = uni_t;
				}

				Query q(qid, range, threshold);

				/*****************************************/

//				fprintf(output, "2\t");
//				q.threshold = q.threshold / 100;
//				WriteQueryToFile(output, &q, DIM);
//				q.threshold = uni_t;
				/*****************************************/

				start = getCurrentTime();
				our_log->InsertQuery(q);
				end = getCurrentTime();
				duration += end - start;
				qinTime += end - start;

			} else if (opList[i] == 3) {
				// Deletion of a query.
				qid = opList[++i];

				/*****************************************/

//				fprintf(output, "3\t%d\n", qid);
				/*****************************************/

				start = getCurrentTime();
				tempState = our_log->DeleteQuery(qid);
				end = getCurrentTime();
				duration += end - start;
				qdnTime += end - start;
				qDelNum++;

//				if (keep == 1) {
//					// Keep the alive number.
//					// After delete a query, check whether need to insert a new query.
//					if (our_log->GetAliveNum() < QSN) {
//						// We need to insert new queries.
//						// Insert a query of spareList.
//						qid = spareList[spareCurPos++];
//						if (qid == -1) {
//							ShowError(
//									"Error in main: Not enough spare queries! Please generate a larger spare query file!\n");
//						}
//
//						for (int j = 0; j < 2 * DIM; ++j) {
//							range[j] = spareList[spareCurPos++];
//						}
//						threshold = spareList[spareCurPos++];
//						if (uni_t != -1) {
//							// The user has specified a uniform threshold for all the queries.
//							threshold = uni_t;
//						}
//
//						Query q(qid, range, threshold);
//
//						/*****************************************/
//
//						fprintf(output, "2\t");
//						q.threshold = q.threshold / 100;
//						WriteQueryToFile(output, &q, DIM);
//						q.threshold = uni_t;
//
//						/*****************************************/
//
//						start = getCurrentTime();
//						our_log->InsertQuery(q);
//						end = getCurrentTime();
//						duration += end - start;
//						qinTime += end - start;
//
//						// Insert a new element to the priority queue.
//						DelEntry de;
//						de.time = ptInsNum + spareList[spareCurPos++];
//						de.delID = q.id;
//						delList.push(de);
//
//						spareInsCnt++;
//					}
//				}

				if (tempState) {
					// The structure has global rebuilt.
					// Output the space consumption to space file.
					spaceCons = our_log->ComputeSpaceConsumption();
					if (spaceCons > maxSpaceCons)
						maxSpaceCons = spaceCons;
					printf(
							"TimeStamp = %d\t SpaceCons = %lf MB\t aliveNum = %d\n",
							ptInsNum, spaceCons / PER_M,
							our_log->GetAliveNum());
					fprintf(spaceFile, "%d\t %lf MB\t aliveNum = %d\n",
							ptInsNum, spaceCons / PER_M,
							our_log->GetAliveNum());
					fflush(stdout);
				}

			} else {
				printf("opList[%d] = %d\n", i, opList[i]);
				ShowError("Error in function main: The opList is incorrect!\n");
			}

			opCnt++;

		}

		/*****************************************/

//		fprintf(output, "-1\n");
//		fclose (output);
		/*****************************************/

		ptCoords.release_space();
		alertIDList.release_space();

		fclose(resultFile);
		fclose(spaceFile);
		fclose(aTimeFile);

		printf("\nAliveNum = %d\n", our_log->GetAliveNum());
		int aliveNum = 0;
		// Release the space of the logarithmic framework.
		if (our_log != NULL) {
			int strNum = our_log->strList.size();
			printf("Number of structures in Log = %d\n", strNum);

			for (int i = 0; i < strNum; ++i) {
				printf(
						"str[%d]: list size = %d\t aliveNum = %d\t sizeLev = %d\n",
						i, (int) our_log->strList[i]->_queryList.size(),
						our_log->strList[i]->aliveNum,
						our_log->strList[i]->sizeLev);

				aliveNum += our_log->strList[i]->aliveNum;
			}

			our_log->ReleaseSpace_All();
			delete our_log;
			our_log = NULL;
		}

		logList.release_space();
		Query::nodeIndexList.release_space();

		printf(
				"\n==================================================================\n");
		printf("Our algorithm:\n");
		printf(
				"Total running time = %lf seconds.\t Construction time = %lf seconds.\n\n",
				duration, constructionTime);
		printf("pinTime = %lf\n", pinTime);
		printf("qinTime = %lf\n", qinTime);
		printf("qdnTime = %lf\n", qdnTime);
		printf("maturedNum = %d\n", maturedNum);
		printf("maxSpaceCons = %lf MB\n", maxSpaceCons / PER_M);

		printf(
				"\n==================================================================\n");

		fflush(stdout);

	} else if (algo == 2) {
		// Run the interval tree algorithm.

		// Read the log file.
		MyVector<QueryBase> queryList;
		MyVector<int> opList;
		ReadLogFromFile(ls, queryList, opList);

		printf("Log read.\n");

		// Increase the number of operations by the number of queries registered at the beginning.
		OPNUM = queryList.size();

		// Compute the result file path for this algorithm.
		sprintf(rs, "%s/Inv_d=%d_m=%d_n=%d_t=%d", rf, DIM, QSN, PIN, uni_t);
		FILE* resultFile = fopen(rs, "wt");
		if (resultFile == NULL) {
			ShowError(
					"Error in function main:\nResult file open failed! Please check your file path!\n");
		}

		// The file to store the space consumption.
		sprintf(tempFilePath, "%s/Inv_d=%d_m=%d_n=%d_t=%d_SpaceCons", rf, DIM,
				QSN, PIN, uni_t);
		FILE* spaceFile = fopen(tempFilePath, "wt");
		if (spaceFile == NULL) {
			ShowError(
					"Error in function main:\nSpaceCons file open failed! Please check your file path!\n");
		}

		fprintf(spaceFile, "TimeStamp\t SpaceCons\n");

		// The file to store the amortized time.
		sprintf(tempFilePath, "%s/Inv_d=%d_m=%d_n=%d_t=%d_AmortizedTime", rf,
				DIM, QSN, PIN, uni_t);
		FILE* aTimeFile = fopen(tempFilePath, "wt");
		if (aTimeFile == NULL) {
			ShowError(
					"Error in function main:\nAmortizedTime file open failed! Please check your resultFile path!\n");
		}
		fprintf(aTimeFile, "TimeStamp\t AmortizedTime\n");

		int opCnt = 0;

		if (targetN == 0) {
			targetN = uni_t / 10;
			if (uni_w == 1) {
				targetN = uni_t * 10;
			}
		}

		printf("targetN = %d\n", targetN);

		int step = targetN / 100;
		if (step == 0) {
			step = 1;
		}

		if (uni_t != -1) {
			// Initialize the threshold of all the queries to uni_t.
			int qNum = queryList.size();
			for (int i = 0; i < qNum; ++i) {
				queryList[i].threshold = uni_t;
			}
		}

		// Construct the whole structure.
		start = getCurrentTime();
		AbsStructure<QueryBase>* strConstructor = NULL;
		if (DIM == 1) {
			// Use interval tree.
			strConstructor = new IntervalTree_Log();
		} else {
			strConstructor = new SegmentTreePure_Log();
		}
		LogarithmicFramework<QueryBase>* inv = new LogarithmicFramework<
				QueryBase>(strConstructor, queryList);
		end = getCurrentTime();
		duration += end - start;
		constructionTime += end - start;

		printf("TNUM_SEG = %d\n", TNUM_SEG);
		printf("TNNUM_SEG = %d\n", TNNUM_SEG);
		printf("TNUM_INV = %d\n", TNUM_INV);
		printf("TNNUM_INV = %d\n", TNNUM_INV);

		printf("Construction time = %lf seconds.\n", constructionTime);

		long long spaceCons = inv->ComputeSpaceConsumption();
		long long maxSpaceCons = spaceCons;
		printf("The initial space consumption = %lf MB.\n", spaceCons / PER_M);
		// Output to the space file.
		fprintf(spaceFile, "0\t %lf MB\n", spaceCons / PER_M);

		printf("Construction done.\n");
		fflush(stdout);

		int pid = 0;
		MyVector<int> ptCoords;
		ptCoords.resize(DIM, 0);
		int ptWeight = 0;
		MyVector<int> alertIDList;

		int qid = 0;
		MyVector<int> range;
		range.resize(2 * DIM, 0);
		int threshold = 0;

		int opLength = opList.size();
//		start = getCurrentTime();

		int qDelNum = 0;

		int ptInsNum = 0;
		int maturedNum = 0;
		bool tempState = false;

		for (int i = 0; i < opLength; ++i) {
			if (opList[i] == -1) {
				// The end of the log file.
				break;
			} else if (opList[i] == 1) {
				// Insertion of a point.
				pid = opList[++i];

				for (int j = 0; j < DIM; ++j) {
					ptCoords[j] = opList[++i];
				}
				ptWeight = opList[++i];

				if (uni_w != -1) {
					// The user has specified a uniform weight of all the points.
					ptWeight = uni_w;
				}

				start = getCurrentTime();
				tempState = inv->InsertPoint(ptCoords.get_list(), ptWeight,
						alertIDList);
				end = getCurrentTime();
				duration += end - start;
				pinTime += end - start;

				ptInsNum++;

				if (tempState || ptInsNum % step == 0) {
					// Output the space consumption to space file.
					spaceCons = inv->ComputeSpaceConsumption();
					if (spaceCons > maxSpaceCons)
						maxSpaceCons = spaceCons;
					printf(
							"TimeStamp = %d\t SpaceCons = %lf MB\t aliveNum = %d\n",
							ptInsNum, spaceCons / PER_M, inv->GetAliveNum());
					fprintf(spaceFile, "%d\t %lf MB\t aliveNum = %d\n",
							ptInsNum, spaceCons / PER_M, inv->GetAliveNum());
				}

				if (ptInsNum % step == 0) {
//					printf(
//							"%d %%\t point insertions done. Running time = %lf.\t Amortized running time = %lf.\n",
//							ptInsNum / step, duration, duration / ptInsNum);

					printf(
							"%d %%\t point insertions done. OPNUM = %d\t Running time = %lf.\t Amortized running time = %.10lf.\n",
							ptInsNum / step, OPNUM, duration, duration / OPNUM);

					printf("pinTime = %lf\n", pinTime);
					printf("qinTime = %lf\n", qinTime);
					printf("qdnTime = %lf\n", qdnTime);
					//				printf("%d %%\t update operations done.\n", opCnt / step * 10);
					fflush(stdout);

					// Output the amortized time to amortized time file.
//					fprintf(aTimeFile, "%d\t %lf\n", ptInsNum,
//							duration / ptInsNum);
					fprintf(aTimeFile, "%d\t %.10lf\n", ptInsNum,
							duration / OPNUM);
				}

				if (WriteResult && alertIDList.size() > 0) {
					maturedNum += alertIDList.size();

					// Some queries has alerted due to the insertion of the current point.
					WriteResultLineToFile(resultFile, pid, alertIDList);
					alertIDList.clear();
				}

				if (inv->GetAliveNum() == 0) {
					// All the queries have matured.

					// Output the space consumption to space file.
					spaceCons = inv->ComputeSpaceConsumption();
					if (spaceCons > maxSpaceCons)
						maxSpaceCons = spaceCons;
					printf(
							"TimeStamp = %d\t SpaceCons = %lf MB\t aliveNum = %d\n",
							ptInsNum, spaceCons / PER_M, inv->GetAliveNum());
					fprintf(spaceFile, "%d\t %lf MB\t aliveNum = %d\n",
							ptInsNum, spaceCons / PER_M, inv->GetAliveNum());

					printf(
							"\nAfter %d point insertions, all the queries have matured.\n",
							pid);
					printf("maturedNum = %d\n", maturedNum);
					break;
				}

			} else if (opList[i] == 2) {
				// Insertion of a query.
				qid = opList[++i];
				for (int j = 0; j < 2 * DIM; ++j) {
					range[j] = opList[++i];
				}
				threshold = opList[++i];
				if (uni_t != -1) {
					// The user has specified a uniform threshold for all the queries.
					threshold = uni_t;
				}

				QueryBase q(qid, range, threshold);
				start = getCurrentTime();
				inv->InsertQuery(q);
				end = getCurrentTime();
				duration += end - start;
				qinTime += end - start;

			} else if (opList[i] == 3) {

				// Deletion of a query.
				qid = opList[++i];
				start = getCurrentTime();
				tempState = inv->DeleteQuery(qid);
				end = getCurrentTime();
				duration += end - start;
				qdnTime += end - start;

				qDelNum++;

				if (tempState) {
					// Output the space consumption to space file.
					spaceCons = inv->ComputeSpaceConsumption();
					if (spaceCons > maxSpaceCons)
						maxSpaceCons = spaceCons;
					printf(
							"TimeStamp = %d\t SpaceCons = %lf MB\t aliveNum = %d\n",
							ptInsNum, spaceCons / PER_M, inv->GetAliveNum());
					fprintf(spaceFile, "%d\t %lf MB\t aliveNum = %d\n",
							ptInsNum, spaceCons / PER_M, inv->GetAliveNum());
				}
			} else {
				printf("opList[%d] = %d\n", i, opList[i]);
				ShowError("Error in function main: The opList is incorrect!\n");
			}

			opCnt++;

		}

		ptCoords.release_space();
		alertIDList.release_space();
		fclose(resultFile);
		fclose(spaceFile);
		fclose(aTimeFile);

		printf("\nAliveNum = %d\n", inv->GetAliveNum());
		int aliveNum = 0;
		// Release the space of the logarithmic framework.
		if (inv != NULL) {
			int strNum = inv->strList.size();
			printf("Number of structures in Log = %d\n", strNum);

			for (int i = 0; i < strNum; ++i) {
				printf(
						"str[%d]: list size = %d\t aliveNum = %d\t sizeLev = %d\n",
						i, (int) inv->strList[i]->_queryList.size(),
						inv->strList[i]->aliveNum, inv->strList[i]->sizeLev);

				aliveNum += inv->strList[i]->aliveNum;
			}

			inv->ReleaseSpace_All();
			delete inv;
			inv = NULL;
		}

		queryList.release_space();
		printf(
				"\n==================================================================\n");
		printf("TNUM_INV = %d\t TNNUM_INV = %d\n", TNUM_INV, TNNUM_INV);
		printf("TNUM_SEG = %d\t TNNUM_SEG = %d\n", TNUM_SEG, TNNUM_SEG);

		printf(
				"\n==================================================================\n");
		printf("Inv algorithm:\n");
		printf(
				"Total running time = %lf seconds.\t Construction time = %lf seconds.\n\n",
				duration, constructionTime);

		printf("RebuildTime = %lf\t MaxRebuildTime = %lf\n\n", RebuildTime,
				MaxRebuildTime);

		printf("pinTime = %lf\n", pinTime);
		printf("qinTime = %lf\n", qinTime);
		printf("qdnTime = %lf\n", qdnTime);

		printf("maturedNum = %d\n", maturedNum);
		printf("maxSpaceCons = %lf MB\n", maxSpaceCons / PER_M);

		printf("\nAverage stabbing number = %lf\n", StabNum * 1.0 / pid);

		printf(
				"\n==================================================================\n");
		fflush(stdout);

	} else if (algo == 3) {
		// Run the brute force naive algorithm.

		// Read the log file.
		MyVector<QueryBase> queryList;
		MyVector<int> opList;

		ReadLogFromFile(ls, queryList, opList);

		printf("Log read.\n");
		printf("queryList size = %d\n", queryList.size());

		// Increase the number of operations by the number of queries registered at the beginning.
		OPNUM = queryList.size();

		// Compute the result file path for this algorithm.
		sprintf(rs, "%s/Bf_d=%d_m=%d_n=%d_t=%d", rf, DIM, QSN, PIN, uni_t);
		FILE* resultFile = fopen(rs, "wt");
		if (resultFile == NULL) {
			ShowError(
					"Error in function main:\nResult file open failed! Please check your file path!\n");
		}

		// The file to store the space consumption.
		sprintf(tempFilePath, "%s/Bf_d=%d_m=%d_n=%d_t=%d_SpaceCons", rf, DIM,
				QSN, PIN, uni_t);
		FILE* spaceFile = fopen(tempFilePath, "wt");
		if (spaceFile == NULL) {
			ShowError(
					"Error in function main:\nSpaceCons file open failed! Please check your file path!\n");
		}

		fprintf(spaceFile, "TimeStamp\t SpaceCons\n");

		// The file to store the amortized time.
		sprintf(tempFilePath, "%s/Bf_d=%d_m=%d_n=%d_t=%d_AmortizedTime", rf,
				DIM, QSN, PIN, uni_t);
		FILE* aTimeFile = fopen(tempFilePath, "wt");
		if (aTimeFile == NULL) {
			ShowError(
					"Error in function main:\nAmortizedTime file open failed! Please check your resultFile path!\n");
		}
		fprintf(aTimeFile, "TimeStamp\t AmortizedTime\n");

		int opCnt = 0;
		int opNum = PIN + QIN + QDN;

		printf("opNum = %d\n", opNum);

		if (targetN == 0) {
			targetN = uni_t / 10;
			if (uni_w == 1) {
				targetN = uni_t * 10;
			}
		}
		printf("targetN = %d\n", targetN);

		int step = targetN / 100;
		if (step == 0) {
			step = 1;
		}

		if (uni_t != -1) {
			// Initialize the threshold of all the queries to uni_t.
			int qNum = queryList.size();
			for (int i = 0; i < qNum; ++i) {
				queryList[i].threshold = uni_t;
			}
		}

		// Construct the whole structure.
		start = getCurrentTime();
		BFNaive<QueryBase>* bf = new BFNaive<QueryBase>(queryList);
		end = getCurrentTime();
		duration += end - start;
		constructionTime += end - start;

		printf("Construction time = %lf.\n", constructionTime);

		long long spaceCons = bf->ComputeSpaceConsumption();
		long long maxSpaceCons = spaceCons;
		printf("The initial space consumption = %lf MB.\n", spaceCons / PER_M);

		// Output to the space file.
		fprintf(spaceFile, "0\t %lf MB\n", spaceCons / PER_M);

		printf("Construction done.\n");
		fflush(stdout);

		int pid = 0;
		MyVector<int> ptCoords;
		ptCoords.resize(DIM, 0);
		int ptWeight = 0;
		MyVector<int> alertIDList;

		int qid = 0;
		MyVector<int> range;
		range.resize(2 * DIM, 0);
		int threshold = 0;

		int opLength = opList.size();

		int qDelNum = 0;
		int ptInsNum = 0;
		int maturedNum = 0;

		bool tempState = false;

//		start = getCurrentTime();
		for (int i = 0; i < opLength; ++i) {
			if (opList[i] == -1) {
				// The end of the log file.
				break;
			} else if (opList[i] == 1) {
				// Insertion of a point.
				pid = opList[++i];

				for (int j = 0; j < DIM; ++j) {
					ptCoords[j] = opList[++i];
				}
				ptWeight = opList[++i];

				if (uni_w != -1) {
					// The user has specified a uniform weight of all the points.
					ptWeight = uni_w;
				}

				start = getCurrentTime();
				bf->InsertPoint(ptCoords.get_list(), ptWeight, alertIDList);
				end = getCurrentTime();
				duration += end - start;
				pinTime += end - start;

				ptInsNum++;

				// Increase the number of operations.
				OPNUM++;
				OPNUM += alertIDList.size();

				if (ptInsNum % step == 0) {

//					printf(
//							"%d %%\t point insertions done. Running time = %lf.\t Amortized running time = %lf.\n",
//							ptInsNum / step, duration, duration / ptInsNum);

					printf(
							"%d %%\t point insertions done. OPNUM = %d\t Running time = %lf.\t Amortized running time = %.10lf.\n",
							ptInsNum / step, OPNUM, duration, duration / OPNUM);

					printf("pinTime = %lf\n", pinTime);
					printf("qinTime = %lf\n", qinTime);
					printf("qdnTime = %lf\n", qdnTime);

					fflush(stdout);

					// Output the amortized time to amortized time file.
//					fprintf(aTimeFile, "%d\t %lf\n", ptInsNum,
//							duration / ptInsNum);
					fprintf(aTimeFile, "%d\t %.10lf\n", ptInsNum,
							duration / OPNUM);

					// Output the space consumption to space file.
					spaceCons = bf->ComputeSpaceConsumption();
					if (spaceCons > maxSpaceCons)
						maxSpaceCons = spaceCons;
					printf(
							"TimeStamp = %d\t SpaceCons = %lf MB\t aliveNum = %d\n",
							ptInsNum, spaceCons / PER_M, bf->GetAliveNum());
					fprintf(spaceFile, "%d\t %lf MB\t\n", ptInsNum,
							spaceCons / PER_M);

				}

				if (WriteResult && alertIDList.size() > 0) {
					// Some queries has alerted due to the insertion of the current point.
					maturedNum += alertIDList.size();

					WriteResultLineToFile(resultFile, pid, alertIDList);
					alertIDList.clear();
				}

				if (bf->GetAliveNum() == 0) {
					// All the queries have matured.

					// Output the space consumption to space file.
					spaceCons = bf->ComputeSpaceConsumption();
					if (spaceCons > maxSpaceCons)
						maxSpaceCons = spaceCons;
					printf(
							"TimeStamp = %d\t SpaceCons = %lf MB\t aliveNum = %d\n",
							ptInsNum, spaceCons / PER_M, bf->GetAliveNum());
					fprintf(spaceFile, "%d\t %lf MB\n", ptInsNum,
							spaceCons / PER_M);

					printf(
							"\nAfter %d point insertions, all the queries have matured.\n",
							pid);
					printf("maturedNum = %d\n", maturedNum);
					break;
				}

			} else if (opList[i] == 2) {
				// Insertion of a query.
				qid = opList[++i];
				for (int j = 0; j < 2 * DIM; ++j) {
					range[j] = opList[++i];
				}
				threshold = opList[++i];
				if (uni_t != -1) {
					// The user has specified a uniform threshold for all the queries.
					threshold = uni_t;
				}

				QueryBase q(qid, range, threshold);
				start = getCurrentTime();
				bf->InsertQuery(q);
				end = getCurrentTime();
				duration += end - start;
				qinTime += end - start;

				// Increase the number of operations.
				OPNUM++;

			} else if (opList[i] == 3) {
				// Deletion of a query.
				qid = opList[++i];
				start = getCurrentTime();
				tempState = bf->DeleteQuery(qid);
				end = getCurrentTime();
				duration += end - start;
				qdnTime += end - start;

				qDelNum++;
				if (tempState) {
					// Increase the number of operations.
					OPNUM++;
				}

				if (bf->GetAliveNum() == 0) {
					// All the queries have matured.

					// Output the space consumption to space file.
					spaceCons = bf->ComputeSpaceConsumption();
					if (spaceCons > maxSpaceCons)
						maxSpaceCons = spaceCons;
					printf(
							"TimeStamp = %d\t SpaceCons = %lf MB\t aliveNum = %d\n",
							ptInsNum, spaceCons / PER_M, bf->GetAliveNum());
					fprintf(spaceFile, "%d\t %lf MB\n", ptInsNum,
							spaceCons / PER_M);

					printf(
							"\nAfter %d point insertions, all the queries have matured.\n",
							pid);
					printf("maturedNum = %d\n", maturedNum);
					break;
				}

			} else {
				printf("opList[%d] = %d\n", i, opList[i]);
				ShowError("Error in function main: The opList is incorrect!\n");
			}

			opCnt++;

		}

		ptCoords.release_space();
		alertIDList.release_space();

		fclose(resultFile);
		fclose(spaceFile);
		fclose(aTimeFile);

		printf("\nAliveNum = %d\n", bf->GetAliveNum());

		// Release the space of the logarithmic framework.
		if (bf != NULL) {
			bf->ReleaseSpace_All();
			delete bf;
			bf = NULL;
		}

		queryList.release_space();

		printf(
				"\n==================================================================\n");
		printf("Bf algorithm:\n");
		printf(
				"Total running time = %lf seconds.\t Construction time = %lf seconds.\n\n",
				duration, constructionTime);

		printf("RebuildTime = %lf\t MaxRebuildTime = %lf\n\n", RebuildTime,
				MaxRebuildTime);
		printf("pinTime = %lf\n", pinTime);
		printf("qinTime = %lf\n", qinTime);
		printf("qdnTime = %lf\n", qdnTime);

		printf("maturedNum = %d\n", maturedNum);
		printf("maxSpaceCons = %lf MB\n", maxSpaceCons / PER_M);

		printf("qDelNum = %d\n", qDelNum);
		printf("\nAverage stabbing number = %lf\n", StabNum * 1.0 / pid);

		printf(
				"\n==================================================================\n");

		fflush(stdout);

	} else if (algo == 4) {
		// Run the rtree algorithm.

		// Read the log file.
		MyVector<QueryBase> queryList;
		MyVector<int> opList;

		ReadLogFromFile(ls, queryList, opList);

		printf("Log read.\n");

		// Increase the number of operations by the number of queries registered at the beginning.
		OPNUM = queryList.size();

		// Compute the result file path for this algorithm.
		sprintf(rs, "%s/Rt_d=%d_m=%d_n=%d_t=%d", rf, DIM, QSN, PIN, uni_t);
		FILE* resultFile = fopen(rs, "wt");
		if (resultFile == NULL) {
			ShowError(
					"Error in function main:\nResult file open failed! Please check your file path!\n");
		}

		// The file to store the space consumption.
		sprintf(tempFilePath, "%s/Rt_d=%d_m=%d_n=%d_t=%d_SpaceCons", rf, DIM,
				QSN, PIN, uni_t);
		FILE* spaceFile = fopen(tempFilePath, "wt");
		if (spaceFile == NULL) {
			ShowError(
					"Error in function main:\nSpaceCons file open failed! Please check your file path!\n");
		}

		fprintf(spaceFile, "TimeStamp\t SpaceCons\n");

		// The file to store the amortized time.
		sprintf(tempFilePath, "%s/Rt_d=%d_m=%d_n=%d_t=%d_AmortizedTime", rf,
				DIM, QSN, PIN, uni_t);
		FILE* aTimeFile = fopen(tempFilePath, "wt");
		if (aTimeFile == NULL) {
			ShowError(
					"Error in function main:\nAmortizedTime file open failed! Please check your resultFile path!\n");
		}
		fprintf(aTimeFile, "TimeStamp\t AmortizedTime\n");

		int opCnt = 0;

		if (targetN == 0) {
			targetN = uni_t / 10;
			if (uni_w == 1) {
				targetN = uni_t * 10;
			}
		}
		printf("targetN = %d\n", targetN);
//		int step = opNum / 10;
		int step = targetN / 100;
		if (step == 0) {
			step = 1;
		}

		if (uni_t != -1) {
			// Initialize the threshold of all the queries to uni_t.
			int qNum = queryList.size();
			for (int i = 0; i < qNum; ++i) {
				queryList[i].threshold = uni_t;
			}
		}

		int qNum = queryList.size();
		vector<QueryBase*> queryPtrList;
		QueryBase* qTemp = NULL;
		for (int i = 0; i < qNum; ++i) {
			qTemp = new QueryBase();
			qTemp->ValueCopy(queryList[i]);
			queryPtrList.push_back(qTemp);
		}
		queryList.release_space();

		// Construct the whole structure.
		start = getCurrentTime();
//		BFNaive<QueryBase>* bf = new BFNaive<QueryBase>(queryList);
		Rtree* rtree = new Rtree(queryPtrList);

		end = getCurrentTime();
		duration += end - start;
		constructionTime += end - start;

		printf("Construction time = %lf.\n", constructionTime);

		long long spaceCons = rtree->ComputeSpaceConsumption();
		long long maxSpaceCons = spaceCons;
		printf("The initial space consumption = %lf MB.\n", spaceCons / PER_M);
		// Output to the space file.
		fprintf(spaceFile, "0\t %lf MB\n", spaceCons / PER_M);

		printf("Construction done.\n");
		fflush(stdout);

		int pid = 0;
		MyVector<int> ptCoords;
		ptCoords.resize(DIM, 0);
		int ptWeight = 0;
		MyVector<int> alertIDList;

		int qid = 0;
		MyVector<int> range;
		range.resize(2 * DIM, 0);
		int threshold = 0;

		int opLength = opList.size();

		int ptInsNum = 0;
		int maturedNum = 0;

		//		start = getCurrentTime();

		vector<int> coords;
		QueryBase* qPtr = NULL;

		for (int i = 0; i < opLength; ++i) {
			if (opList[i] == -1) {
				// The end of the log file.
				break;
			} else if (opList[i] == 1) {
				coords.clear();

				// Insertion of a point.
				pid = opList[++i];

				for (int j = 0; j < DIM; ++j) {
					coords.push_back(opList[++i]);
				}
				ptWeight = opList[++i];

				if (uni_w != -1) {
					// The user has specified a uniform weight of all the points.
					ptWeight = uni_w;
				}

				start = getCurrentTime();
				rtree->InsertPoint(coords, ptWeight, alertIDList);
				end = getCurrentTime();
				duration += end - start;
				pinTime += end - start;

				ptInsNum++;

				if (ptInsNum % step == 0) {
//					printf(
//							"%d %%\t point insertions done. Running time = %lf.\t Amortized running time = %lf.\n",
//							ptInsNum / step, duration, duration / ptInsNum);

					printf(
							"%d %%\t point insertions done. OPNUM = %d\t Running time = %lf.\t Amortized running time = %.10lf.\n",
							ptInsNum / step, OPNUM, duration, duration / OPNUM);

					printf("pinTime = %lf\n", pinTime);
					printf("qinTime = %lf\n", qinTime);
					printf("qdnTime = %lf\n", qdnTime);

					fflush(stdout);

					// Output the amortized time to amortized time file.
//					fprintf(aTimeFile, "%d\t %lf\n", ptInsNum,
//							duration / ptInsNum);
					fprintf(aTimeFile, "%d\t %.10lf\n", ptInsNum,
							duration / OPNUM);

					// Output the space consumption to space file.
					spaceCons = rtree->ComputeSpaceConsumption();
					if (spaceCons > maxSpaceCons)
						maxSpaceCons = spaceCons;
					printf(
							"TimeStamp = %d\t SpaceCons = %lf MB\t aliveNum = %d\n",
							ptInsNum, spaceCons / PER_M, rtree->GetAliveNum());
					fprintf(spaceFile, "%d\t %lf MB\n", ptInsNum,
							spaceCons / PER_M);
				}

				if (WriteResult && alertIDList.size() > 0) {
					// Some queries has alerted due to the insertion of the current point.
					maturedNum += alertIDList.size();
					WriteResultLineToFile(resultFile, pid, alertIDList);
					alertIDList.clear();
				}

				if (rtree->GetAliveNum() == 0) {
					// All the queries have matured.

					// Output the space consumption to space file.
					spaceCons = rtree->ComputeSpaceConsumption();
					if (spaceCons > maxSpaceCons)
						maxSpaceCons = spaceCons;
					printf(
							"TimeStamp = %d\t SpaceCons = %lf MB\t aliveNum = %d\n",
							ptInsNum, spaceCons / PER_M, rtree->GetAliveNum());
					fprintf(spaceFile, "%d\t %lf MB\n", ptInsNum,
							spaceCons / PER_M);

					printf(
							"\nAfter %d point insertions, all the queries have matured.\n",
							pid);
					printf("maturedNum = %d\n", maturedNum);
					break;
				}

			} else if (opList[i] == 2) {
				// Insertion of a query.
				qid = opList[++i];
				for (int j = 0; j < 2 * DIM; ++j) {
					range[j] = opList[++i];
				}
				threshold = opList[++i];
				if (uni_t != -1) {
					// The user has specified a uniform threshold for all the queries.
					threshold = uni_t;
				}

//				QueryBase q(qid, range, threshold);
				qPtr = new QueryBase(qid, range, threshold);
				start = getCurrentTime();
//				bf->InsertQuery(q);
				rtree->InsertQuery(qPtr);
				end = getCurrentTime();
				duration += end - start;
				qinTime += end - start;

			} else if (opList[i] == 3) {
				// Deletion of a query.
				qid = opList[++i];
				start = getCurrentTime();
				rtree->DeleteQuery(qid);
				end = getCurrentTime();
				duration += end - start;
				qdnTime += end - start;

				if (rtree->GetAliveNum() == 0) {
					// All the queries have matured.

					// Output the space consumption to space file.
					spaceCons = rtree->ComputeSpaceConsumption();
					if (spaceCons > maxSpaceCons)
						maxSpaceCons = spaceCons;
					printf(
							"TimeStamp = %d\t SpaceCons = %lf MB\t aliveNum = %d\n",
							ptInsNum, spaceCons / PER_M, rtree->GetAliveNum());
					fprintf(spaceFile, "%d\t %lf MB\n", ptInsNum,
							spaceCons / PER_M);

					printf(
							"\nAfter %d point insertions, all the queries have matured.\n",
							pid);
					printf("maturedNum = %d\n", maturedNum);
					break;
				}

			} else {
				printf("opList[%d] = %d\n", i, opList[i]);
				ShowError("Error in function main: The opList is incorrect!\n");
			}

			opCnt++;
		}

		ptCoords.release_space();
		alertIDList.release_space();
		fclose(resultFile);

		printf("\nAliveNum = %d\n", rtree->GetAliveNum());

		// Release the space of the logarithmic framework.
		if (rtree != NULL) {
			rtree->ReleaseSpace_All();
			delete rtree;
			rtree = NULL;
		}

		printf(
				"\n==================================================================\n");
		printf("Rtree algorithm:\n");
		printf(
				"Total running time = %lf seconds.\t Construction time = %lf seconds.\n\n",
				duration, constructionTime);

		printf("pinTime = %lf\n", pinTime);
		printf("qinTime = %lf\n", qinTime);
		printf("qdnTime = %lf\n", qdnTime);

		printf("maturedNum = %d\n", maturedNum);
		printf("maxSpaceCons = %lf MB\n", maxSpaceCons / PER_M);

		printf(
				"\n==================================================================\n");

		fflush(stdout);

	} else if (algo == 5) {
		// Compare the two result files.
		MyVector<int> resultList;
		MyVector<int> gtList;

		ReadResultFile(rs, resultList);
		ReadResultFile(gt, gtList);

		if (CheckResultFile(resultList, gtList)) {
			printf("\nThe results of the two algorithms are the same.\n");
		} else {
			printf("\nThe two result files are not the same! Please check!\n");
		}
	} else {
		ShowError("Error in main: The algo value is invalid!\n");
	}

	printf("\nThe program exits normally.\n");
	return 0;
}
