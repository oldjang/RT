/*
 * LogGen_IO.cpp
 *
 *  Created on: 7 Oct, 2015
 *      Author: jhgan
 */

#include "LogGen_IO.h"

/***************************************************************
 *  			Log Generation Related.
 ***************************************************************/
struct DelEntry {
	int time;
	int delID;

	DelEntry() {
		this->time = 0;
		this->delID = 0;
	}

	DelEntry(int time, int delID) {
		this->time = time;
		this->delID = delID;
	}

	bool operator <(const DelEntry& e2) const {
		return this->time < e2.time;
	}
};

/*
 *  The entry structure for operation.
 */
struct OpEntry {
	int time;
	int qID;
	int opType; // op = 2: insert a query; op = 3: delete a query;

	OpEntry() {
		this->time = 0;
		this->qID = 0;
		opType = -1;
	}

	OpEntry(int time, int qID, int opType) {
		this->time = time;
		this->qID = qID;
		this->opType = opType;
	}

	bool operator <(const OpEntry& e2) const {
		if (this->time < e2.time)
			return true;
		else if (this->time == e2.time) {
			return this->opType > e2.opType;
		} else {
			return false;
		}
	}
};

/*
 *  Generate a log of insertions of points and updates of queries to specified file.
 *  Parameter List:
 *  	filePath:		the path of the target file.
 *  	dim:			the dimensionality of space.
 *  	qNumStart:		the number of queries at the beginning.
 *  	ptInsNum:		the number of insertions of points.
 *  	qInsNum:		the number of insertions of queries.
 *		qDelNum:		the number of deletions of queries.
 *		uni_w:			the uniform weight of points.
 *						If uni_w = -1, then we randomly generate weight for each point.
 *		uni_t:			the uniform threshold of queries.
 *						If uni_t = -1, then we randomly generate threshold for each query.
 */
void GenLogToFile_Gaussian(char* filePath, int dim, int qNumStart, int ptInsNum,
		int qInsNum, int qDelNum, int uni_w, int uni_t) {
	FILE* file = fopen(filePath, "wt");
	if (file == NULL) {
		ShowError(
				"Error in function GenLogToFile:\nFile open failed! Please check your file path!\n");
	}

	int* square = new int[2 * dim];
	int* point = new int[dim];

	fprintf(file, "%d\t%d\t%d\t%d\t%d\n", dim, qNumStart, ptInsNum, qInsNum,
			qDelNum);

	for (int i = 0; i < qNumStart; ++i) {
		GenGaussianSquare(dim, square, QLEN, U, SIGMA, rangeStart, rangeEnd);
		QueryBase qb(i + 1, square, uni_t);
		WriteQueryToFile(file, &qb);
		if (qb.range != NULL) {
			delete[] (qb.range);
			qb.range = NULL;
		}
	}

	int num = ptInsNum + qInsNum + qDelNum;

	vector<int> opList;
	opList.resize(num, 0);

	int i = 0;
	for (; i < ptInsNum; ++i) {
		// Insert a point.
		opList[i] = 1;
	}
	for (; i < ptInsNum + qInsNum; ++i) {
		// Insert a query.
		opList[i] = 2;
	}
	for (; i < num; ++i) {
		// Delete a query.
		opList[i] = 3;
	}

	// Randomly permute opList.
	RandomPermute(opList);

	int ptID = 1;
	int qID = qNumStart + 1;

	int w = 0;
	int t = 0;
	int delID = 0;

	for (i = 0; i < num; ++i) {
		if (opList[i] == 1) {
			// Insertion of point.
			GenGaussianPoint(dim, point, U, SIGMA);
//			point[0] = GenRandInt(rangeStart, rangeEnd);

			if (uni_w == -1) {
//				w = GenRandInt(1, 100);
				// Generate a weight from N(100, 15^2);
				w = GenGaussianRandInt(100, 15);
				while (w < 1) {
					w = GenGaussianRandInt(100, 15);
				}
			} else {
				w = uni_w;
			}

			fprintf(file, "1\t%d\t", ptID++);
			for (int j = 0; j < dim; ++j) {
				fprintf(file, "%d\t", point[j]);
			}
			fprintf(file, "%d\n", w);

		} else if (opList[i] == 2) {
			// Insertion of query.
			GenGaussianSquare(dim, square, QLEN, U, SIGMA);
			if (uni_t == -1) {
				t = GenRandInt(1, ptInsNum);
			} else {
				t = uni_t;
			}
			fprintf(file, "2\t");
			QueryBase q(qID, square, t);
			WriteQueryToFile(file, &q, dim);

			if (q.range != NULL) {
				delete[] (q.range);
				q.range = NULL;
			}
			qID++;

		} else if (opList[i] == 3) {
			// Deletion of query.
			delID = GenRandInt(1, qID - 1);
			fprintf(file, "3\t%d\n", delID);
		}
	}

	// -1 means the end of the file.
	fprintf(file, "-1\n");

	if (square != NULL) {
		delete[] square;
		square = NULL;
	}

	if (point != NULL) {
		delete[] point;
		point = NULL;
	}

	fclose(file);
}

/*
 *  Generate a log of insertions of points and updates of queries to specified file.
 *  Parameter List:
 *  	filePath:		the path of the target file.
 *  	dim:			the dimensionality of space.
 *  	qNumStart:		the number of queries at the beginning.
 *  	ptInsNum:		the number of insertions of points.
 *  	qInsNum:		the number of insertions of queries.
 *		qDelNum:		the number of deletions of queries.
 *		uni_t:			the uniform threshold of queries.
 *						If uni_t = -1, then we randomly generate threshold for each query.
 *		uni_w:			the uniform weight of points.
 *						If uni_w = -1, then we randomly generate weight for each point.
 */
void GenLogToFile_Uniform(char* filePath, int dim, int qNumStart, int ptInsNum,
		int qInsNum, int qDelNum, int uni_t, int uni_w) {
	FILE* file = fopen(filePath, "wt");
	if (file == NULL) {
		ShowError(
				"Error in function GenLogToFile:\nFile open failed! Please check your file path!\n");
	}

	int* square = new int[2 * dim];
	int* point = new int[dim];

	fprintf(file, "%d\t%d\t%d\t%d\t%d\n", dim, qNumStart, ptInsNum, qInsNum,
			qDelNum);

	for (int i = 0; i < qNumStart; ++i) {
		GenUniformSquare(dim, square, QLEN, rangeStart, rangeEnd);
		QueryBase qb(i + 1, square, uni_t);
		WriteQueryToFile(file, &qb);
		if (qb.range != NULL) {
			delete[] (qb.range);
			qb.range = NULL;
		}
	}

	int num = ptInsNum + qInsNum + qDelNum;

	vector<int> opList;
	opList.resize(num, 0);

	int i = 0;
	for (; i < ptInsNum; ++i) {
		// Insert a point.
		opList[i] = 1;
	}
	for (; i < ptInsNum + qInsNum; ++i) {
		// Insert a query.
		opList[i] = 2;
	}
	for (; i < num; ++i) {
		// Delete a query.
		opList[i] = 3;
	}

	// Randomly permute opList.
	RandomPermute(opList);

	int ptID = 1;
	int qID = qNumStart + 1;

	int w = 0;
	int t = 0;
	int delID = 0;

	for (i = 0; i < num; ++i) {
		if (opList[i] == 1) {
			// Insertion of point.
			GenUniformPoint(dim, point, rangeStart, rangeEnd);
//			point[0] = GenRandInt(rangeStart, rangeEnd);

			if (uni_w == -1) {
//				w = GenRandInt(1, 100);
				// Generate a weight from N(100, 15^2);
				w = GenGaussianRandInt(100, 15);
				while (w < 1) {
					w = GenGaussianRandInt(100, 15);
				}
			} else {
				w = uni_w;
			}

			fprintf(file, "1\t%d\t", ptID++);
			for (int j = 0; j < dim; ++j) {
				fprintf(file, "%d\t", point[j]);
			}
			fprintf(file, "%d\n", w);

		} else if (opList[i] == 2) {
			// Insertion of query.
			GenUniformSquare(dim, square, QLEN, rangeStart, rangeEnd);
			if (uni_t == -1) {
				t = GenRandInt(1, ptInsNum);
			} else {
				t = uni_t;
			}
			fprintf(file, "2\t");
			QueryBase q(qID, square, t);
			WriteQueryToFile(file, &q, dim);

			if (q.range != NULL) {
				delete[] (q.range);
				q.range = NULL;
			}
			qID++;

		} else if (opList[i] == 3) {
			// Deletion of query.
			delID = GenRandInt(1, qID - 1);
			fprintf(file, "3\t%d\n", delID);
		}
	}

	// -1 means the end of the file.
	fprintf(file, "-1\n");

	if (square != NULL) {
		delete[] square;
		square = NULL;
	}

	if (point != NULL) {
		delete[] point;
		point = NULL;
	}

	fclose(file);
}

/*
 *  Read the whole log into the specified vector.
 *  Parameter List:
 *  	filePath:			the path of the log file.
 *		queryList:			the target place to store the list of queries registered at the beginning.
 *		rangeValueList:		the target place to store the range values of those queries.
 *  	opList:				the target place to store the list of operation.
 */
void ReadLogFromFile(char* filePath, MyVector<QueryBase>& queryList,
		MyVector<int>& rangeValueList, MyVector<int>& opList) {
	FILE* file = fopen(filePath, "rt");
	if (file == NULL) {
		ShowError(
				"Error in function ReadLogFromFile:\nFile open failed! Please check your file path!\n");
	}

	int dim = 0;
	int qNumStart = 0;
	int ptInsNum = 0; // op = 1 number
	int qInsNum = 0;  // op = 2 number
	int qDelNum = 0;  // op = 3 number

	fscanf(file, "%d\t%d\t%d\t%d\t%d\n", &dim, &qNumStart, &ptInsNum, &qInsNum,
			&qDelNum);

	// Set the DIM value.
	DIM = dim;
	QSN = qNumStart;
	PIN = ptInsNum;
	QIN = qInsNum;
	QDN = qDelNum;

	queryList.clear();
	queryList.reserve(qNumStart);

	rangeValueList.clear();
	rangeValueList.reserve(qNumStart * 2 * dim);

	QueryBase temp;
	int tempInt = 0;

	for (int i = 0; i < qNumStart; ++i) {
		fscanf(file, "%d\t", &temp.id);

		for (int j = 0; j < 2 * dim; ++j) {
			fscanf(file, "%d\t", &tempInt);
			rangeValueList.push_back(tempInt);
		}
		fscanf(file, "%d\n", &temp.threshold);

		temp.range = &(rangeValueList[rangeValueList.size() - 2 * dim]);

		queryList.push_back(temp);
	}

	// op = 1, there are op + pid + dim + w = dim + 3 integers.
	// op = 2, there are op + qid + 2 * dim + t = 2 * dim + 3 integers.
	// op = 3, there are op + delID = 2 integers.
	// EOF, one integer.

	int totalIntNum = ptInsNum * (dim + 3) + qInsNum * (2 * dim + 3)
			+ qDelNum * 2 + 1;

	opList.clear();
	opList.reserve(totalIntNum);

	for (int i = 0; i < totalIntNum; ++i) {
		fscanf(file, "%d", &tempInt);
		opList.push_back(tempInt);
	}

	fclose(file);
}

/*
 *  Read the whole log into the specified vector.
 *  Parameter List:
 *  	filePath:			the path of the log file.
 *		queryList:			the target place to store the list of queries registered at the beginning.
 *  	opList:				the target place to store the list of operation.
 */
void ReadLogFromFile(char* filePath, MyVector<QueryBase>& queryList,
		MyVector<int>& opList) {
	FILE* file = fopen(filePath, "rt");
	if (file == NULL) {
		ShowError(
				"Error in function ReadLogFromFile:\nFile open failed! Please check your file path!\n");
	}

	int dim = 0;
	int qNumStart = 0;
	int ptInsNum = 0; // op = 1 number
	int qInsNum = 0;  // op = 2 number
	int qDelNum = 0;  // op = 3 number

	fscanf(file, "%d\t%d\t%d\t%d\t%d\n", &dim, &qNumStart, &ptInsNum, &qInsNum,
			&qDelNum);

	// Set the DIM value.
	DIM = dim;
	QSN = qNumStart;
	PIN = ptInsNum;
	QIN = qInsNum;
	QDN = qDelNum;

	queryList.clear();
	queryList.reserve(qNumStart);

	QueryBase temp;
	int tempInt = 0;

	for (int i = 0; i < qNumStart; ++i) {
		fscanf(file, "%d\t", &temp.id);

		temp.range = new int[2 * dim];

		for (int j = 0; j < 2 * dim; ++j) {
			fscanf(file, "%d\t", &tempInt);
			temp.range[j] = tempInt;
		}
		fscanf(file, "%d\n", &temp.threshold);

		queryList.push_back(temp);
	}

	// op = 1, there are op + pid + dim + w = dim + 3 integers.
	// op = 2, there are op + qid + 2 * dim + t = 2 * dim + 3 integers.
	// op = 3, there are op + delID = 2 integers.
	// EOF, one integer.

	int totalIntNum = ptInsNum * (dim + 3) + qInsNum * (2 * dim + 3)
			+ qDelNum * 2 + 1;

	opList.clear();
	opList.reserve(totalIntNum);

	tempInt = 0;

	while (tempInt != -1) {
		fscanf(file, "%d", &tempInt);
		opList.push_back(tempInt);
	}

	opList.shrink_to_fit();

//	for (int i = 0; i < totalIntNum; ++i) {
//		fscanf(file, "%d", &tempInt);
//		opList.push_back(tempInt);
//	}

	fclose(file);
}

void testSort() {
	int num = 10;
	MyVector<DelEntry> delList;
	delList.reserve(num);
	for (int i = 0; i < num; ++i) {
		DelEntry entry(10 - i, i);
		delList.push_back(entry);
	}

	sort(delList.get_list(), delList.get_list() + num);

	for (int i = 0; i < num; ++i) {
		printf("%d: time = %d\t delID = %d\n", i, delList[i].time,
				delList[i].delID);
	}

	delList.release_space();
}

/*
 *  Generate a log of insertions of points and determine the deletion time for each query.
 *  Parameter List:
 *  	filePath:		the path of the target file.
 *  	dim:			the dimensionality of space.
 *  	qNumStart:		the number of queries at the beginning.
 *  	ptInsNum:		the number of insertions of points.
 *  	qInsNum:		the number of insertions of queries.
 *		qDelNum:		the number of deletions of queries.
 *		uni_w:			the uniform weight of points.
 *						If uni_w = -1, then we randomly generate weight for each point.
 *		uni_t:			the uniform threshold of queries.
 *						If uni_t = -1, then we randomly generate threshold for each query.
 */
void GenLogToFile_Uniform_TestDel(char* filePath, int dim, int qNumStart,
		int ptInsNum, int qInsNum, int qDelNum, int uni_w, int uni_t) {
	FILE* file = fopen(filePath, "wt");
	if (file == NULL) {
		ShowError(
				"Error in function GenLogToFile:\nFile open failed! Please check your file path!\n");
	}

	int* square = new int[2 * dim];
	int* point = new int[dim];

	fprintf(file, "%d\t%d\t%d\t%d\t%d\n", dim, qNumStart, ptInsNum, qInsNum,
			qDelNum);

	MyVector<DelEntry> delList;
	delList.reserve(qNumStart);

	double p = 10.0 / uni_t;

//	printf("p = %lf\n", p);

	// Seed with a real random value, if available
	std::random_device rd;
	std::default_random_engine generator(rd());
	std::geometric_distribution<int> distribution(p);

	int delTimeStamp = 0;

	for (int i = 0; i < qNumStart; ++i) {
		GenUniformSquare(dim, square, QLEN, rangeStart, rangeEnd);
		QueryBase qb(i + 1, square, uni_t);
		WriteQueryToFile(file, &qb);
		if (qb.range != NULL) {
			delete[] (qb.range);
			qb.range = NULL;
		}

		delTimeStamp = distribution(generator);
		DelEntry entry(delTimeStamp, i + 1);
		delList.push_back(entry);

//		printf("delTimeStap = %d\n", delTimeStamp);
	}

	sort(delList.get_list(), delList.get_list() + qNumStart);

	int num = ptInsNum;

	vector<int> opList;
	opList.resize(num, 0);

	int i = 0;
	for (; i < ptInsNum; ++i) {
		// Insert a point.
		opList[i] = 1;
	}
//	for (; i < ptInsNum + qInsNum; ++i) {
//		// Insert a query.
//		opList[i] = 2;
//	}
//	for (; i < num; ++i) {
//		// Delete a query.
//		opList[i] = 3;
//	}
//
//	// Randomly permute opList.
//	RandomPermute(opList);

	int ptID = 1;
	int qID = qNumStart + 1;

	int w = 0;
	int t = 0;
	int delID = 0;
	int curPos = 0;

	for (i = 0; i < num; ++i) {
		if (opList[i] == 1) {
			// Insertion of point.
			GenUniformPoint(dim, point, rangeStart, rangeEnd);
			//			point[0] = GenRandInt(rangeStart, rangeEnd);

			if (uni_w == -1) {
				//				w = GenRandInt(1, 100);
				// Generate a weight from N(100, 15^2);
				w = GenGaussianRandInt(100, 15);
				while (w < 1) {
					w = GenGaussianRandInt(100, 15);
				}
			} else {
				w = uni_w;
			}

			fprintf(file, "1\t%d\t", ptID++);
			for (int j = 0; j < dim; ++j) {
				fprintf(file, "%d\t", point[j]);
			}
			fprintf(file, "%d\n", w);

			while (curPos < delList.size() && i == delList[curPos].time) {
				// Deletion of query.
				fprintf(file, "3\t%d\n", delList[curPos].delID);
				curPos++;
			}
		}

//		else if (opList[i] == 2) {
//			// Insertion of query.
//			GenUniformSquare(dim, square, QLEN, rangeStart, rangeEnd);
//			if (uni_t == -1) {
//				t = GenRandInt(1, ptInsNum);
//			} else {
//				t = uni_t;
//			}
//			fprintf(file, "2\t");
//			QueryBase q(qID, square, t);
//			WriteQueryToFile(file, &q, dim);
//
//			if (q.range != NULL) {
//				delete[] (q.range);
//				q.range = NULL;
//			}
//			qID++;
//
//		} else if (opList[i] == 3) {
//			// Deletion of query.
//			delID = GenRandInt(1, qID - 1);
//			fprintf(file, "3\t%d\n", delID);
//		}
	}

	// -1 means the end of the file.
	fprintf(file, "-1\n");

	if (square != NULL) {
		delete[] square;
		square = NULL;
	}

	if (point != NULL) {
		delete[] point;
		point = NULL;
	}

	delList.release_space();

	fclose(file);
}

/*
 *  Generate a log of insertions of points and determine the deletion time for each query,
 *  where each query is generated by Gaussian distribution and each point is generated by uniform distribution.
 *  Parameter List:
 *  	filePath:		the path of the target file.
 *  	dim:			the dimensionality of space.
 *  	qNumStart:		the number of queries at the beginning.
 *  	ptInsNum:		the number of insertions of points.
 *  	qInsNum:		the number of insertions of queries.
 *		qDelNum:		the number of deletions of queries.
 *		uni_w:			the uniform weight of points.
 *						If uni_w = -1, then we randomly generate weight for each point.
 *		uni_t:			the uniform threshold of queries.
 *						If uni_t = -1, then we randomly generate threshold for each query.
 */
void GenLogToFile_GQuery_UPoint(char* filePath, int dim, int qNumStart,
		int ptInsNum, int qInsNum, int qDelNum, int uni_w, int uni_t) {
	FILE* file = fopen(filePath, "wt");
	if (file == NULL) {
		ShowError(
				"Error in function GenLogToFile:\nFile open failed! Please check your file path!\n");
	}

	int* square = new int[2 * dim];
	int* point = new int[dim];

	fprintf(file, "%d\t%d\t%d\t%d\t%d\n", dim, qNumStart, ptInsNum, qInsNum,
			qDelNum);

	MyVector<DelEntry> delList;
	delList.reserve(qNumStart);

	double p = 1.0 / (uni_t * 4.34);
//	printf("p = %lf\n", p);

	// Seed with a real random value, if available
	std::random_device rd;
	std::default_random_engine generator(rd());
	std::geometric_distribution<int> distribution(p);

	int delTimeStamp = 0;

	for (int i = 0; i < qNumStart; ++i) {
//		GenUniformSquare(dim, square, QLEN, rangeStart, rangeEnd);
		GenGaussianSquare(dim, square, QLEN, U, SIGMA, rangeStart, rangeEnd);
		QueryBase qb(i + 1, square, uni_t);
		WriteQueryToFile(file, &qb);
		if (qb.range != NULL) {
			delete[] (qb.range);
			qb.range = NULL;
		}

		delTimeStamp = distribution(generator);
		DelEntry entry(delTimeStamp, i + 1);
		delList.push_back(entry);

//		printf("delTimeStap = %d\n", delTimeStamp);
	}

	sort(delList.get_list(), delList.get_list() + qNumStart);
//	for (int i = 0; i < qNumStart; ++i) {
//		printf("%d:\t time = %d\t delID = %d\n", i, delList[i].time,
//				delList[i].delID);
//	}

	int num = ptInsNum;

	vector<int> opList;
	opList.resize(num, 0);

	int i = 0;
	for (; i < ptInsNum; ++i) {
		// Insert a point.
		opList[i] = 1;
	}
	//	for (; i < ptInsNum + qInsNum; ++i) {
	//		// Insert a query.
	//		opList[i] = 2;
	//	}
	//	for (; i < num; ++i) {
	//		// Delete a query.
	//		opList[i] = 3;
	//	}
	//
	//	// Randomly permute opList.
	//	RandomPermute(opList);

	int ptID = 1;
	int qID = qNumStart + 1;

	int w = 0;
	int t = 0;
	int delID = 0;
	unsigned int curPos = 0;

	for (i = 0; i < num; ++i) {
		if (opList[i] == 1) {
			// Insertion of point.
			GenUniformPoint(dim, point, rangeStart, rangeEnd);
//			GenGaussianPoint(dim, point, U, SIGMA, rangeStart, rangeEnd);
			//			point[0] = GenRandInt(rangeStart, rangeEnd);

			if (uni_w == -1) {
				//				w = GenRandInt(1, 100);
				// Generate a weight from N(100, 15^2);
				w = GenGaussianRandInt(100, 15);
				while (w < 1) {
					w = GenGaussianRandInt(100, 15);
				}
			} else {
				w = uni_w;
			}

			fprintf(file, "1\t%d\t", ptID++);
			for (int j = 0; j < dim; ++j) {
				fprintf(file, "%d\t", point[j]);
			}
			fprintf(file, "%d\n", w);

			while (curPos < delList.size() && i == delList[curPos].time) {
				// Deletion of query.
				fprintf(file, "3\t%d\n", delList[curPos].delID);
				curPos++;
			}

		}
//		else if (opList[i] == 2) {
//			// Insertion of query.
//			GenUniformSquare(dim, square, QLEN, rangeStart, rangeEnd);
//			if (uni_t == -1) {
//				t = GenRandInt(1, ptInsNum);
//			} else {
//				t = uni_t;
//			}
//			fprintf(file, "2\t");
//			QueryBase q(qID, square, t);
//			WriteQueryToFile(file, &q, dim);
//
//			if (q.range != NULL) {
//				delete[] (q.range);
//				q.range = NULL;
//			}
//			qID++;
//
//		} else if (opList[i] == 3) {
//			// Deletion of query.
//			delID = GenRandInt(1, qID - 1);
//			fprintf(file, "3\t%d\n", delID);
//		}
	}

	while (curPos < delList.size()) {
		// Deletion of query.
		fprintf(file, "3\t%d\n", delList[curPos].delID);
		curPos++;
	}

	// -1 means the end of the file.
	fprintf(file, "-1\n");

	if (square != NULL) {
		delete[] square;
		square = NULL;
	}

	if (point != NULL) {
		delete[] point;
		point = NULL;
	}

	delList.release_space();

	fclose(file);
}

/*
 *  Generate a log of insertions of points and determine the deletion time for each query.
 *  Parameter List:
 *  	filePath:		the path of the target file.
 *  	dim:			the dimensionality of space.
 *  	qNumStart:		the number of queries at the beginning.
 *  	ptInsNum:		the number of insertions of points.
 *  	qInsNum:		the number of insertions of queries.
 *		qDelNum:		the number of deletions of queries.
 *		uni_w:			the uniform weight of points.
 *						If uni_w = -1, then we randomly generate weight for each point.
 *		uni_t:			the uniform threshold of queries. (unweighted version)
 *						If uni_t = -1, then we randomly generate threshold for each query.
 *		qInsProb:		the probability of generating a query insertion at each time stamp.
 */
void GenLogToFile_GQuery_UPoint_InsQ(char* filePath, int dim, int qNumStart,
		int ptInsNum, int qInsNum, int qDelNum, int uni_w, int uni_t,
		double qInsProb) {
	FILE* file = fopen(filePath, "wt");
	if (file == NULL) {
		ShowError(
				"Error in function GenLogToFile:\nFile open failed! Please check your file path!\n");
	}

	int* square = new int[2 * dim];
	int* point = new int[dim];

	fprintf(file, "%d\t%d\t%d\t%d\t%d\n", dim, qNumStart, ptInsNum, qInsNum,
			qDelNum);

	MyVector<DelEntry> delList;
	delList.reserve(qNumStart);

	double p = 1.0 / (uni_t * 4.34);
//	printf("p = %lf\n", p);

	// Seed with a real random value, if available
	std::random_device rd;
	std::default_random_engine generator(rd());
	std::geometric_distribution<int> distribution(p);

	int delTimeStamp = 0;

	QueryBase qb;
	qb.range = square;

	for (int i = 0; i < qNumStart; ++i) {
//		GenUniformSquare(dim, square, QLEN, rangeStart, rangeEnd);
		GenGaussianSquare(dim, square, QLEN, U, SIGMA, rangeStart, rangeEnd);
//		QueryBase qb(i + 1, square, uni_t);
		qb.id = i + 1;
		qb.threshold = uni_t;
		WriteQueryToFile(file, &qb);
//		if (qb.range != NULL) {
//			delete[] (qb.range);
//			qb.range = NULL;
//		}

		delTimeStamp = distribution(generator) + 1;
		DelEntry entry(delTimeStamp, i + 1);
		delList.push_back(entry);

//		printf("delTimeStap = %d\n", delTimeStamp);
	}

	int num = ptInsNum + qInsNum;

	int ptID = 1;
	int qID = qNumStart + 1;

	vector<int> opList;
	opList.reserve(num);

	int i = 0;

	// Only the first $targetNum$ time stamps possibly have query insertions.
	int targetNum = 10 * uni_t;

	for (; i < ptInsNum; ++i) {
		// Insert a point.
		opList.push_back(1);
		if (i < targetNum && GenUniformRandom(0, 1) <= qInsProb) {
			// Generate a query insertion.
			opList.push_back(2);
			delTimeStamp = distribution(generator) + i + 1;
			DelEntry entry(delTimeStamp, qID);
			delList.push_back(entry);
			qID++;
		}
	}

	sort(delList.get_list(), delList.get_list() + delList.size());
//	for (int i = 0; i < delList.size(); ++i) {
//		printf("%d:\t time = %d\t delID = %d\n", i, delList[i].time,
//				delList[i].delID);
//	}

	int w = 0;
	int t = 0;
	unsigned int curPos = 0;

	// Reset the qID to qNumStart + 1.
	qID = qNumStart + 1;
	num = opList.size();
	for (i = 0; i < num; ++i) {
		if (opList[i] == 1) {
			// Insertion of point.
			GenUniformPoint(dim, point, rangeStart, rangeEnd);
//			GenGaussianPoint(dim, point, U, SIGMA, rangeStart, rangeEnd);
			//			point[0] = GenRandInt(rangeStart, rangeEnd);

			if (uni_w == -1) {
				//				w = GenRandInt(1, 100);
				// Generate a weight from N(100, 15^2);
				w = GenGaussianRandInt(100, 15);
				while (w < 1) {
					w = GenGaussianRandInt(100, 15);
				}
			} else {
				w = uni_w;
			}

			fprintf(file, "1\t%d\t", ptID);
			for (int j = 0; j < dim; ++j) {
				fprintf(file, "%d\t", point[j]);
			}
			fprintf(file, "%d\n", w);

			while (curPos < delList.size() && ptID == delList[curPos].time) {
				// Deletion of query.
				fprintf(file, "3\t%d\n", delList[curPos].delID);
				curPos++;
			}

			ptID++;

		} else if (opList[i] == 2) {
			// Insertion of query.

//			GenUniformSquare(dim, square, QLEN, rangeStart, rangeEnd);
			GenGaussianSquare(dim, square, QLEN, U, SIGMA, rangeStart,
					rangeEnd);
			if (uni_t == -1) {
				t = GenRandInt(1, ptInsNum);
			} else {
				t = uni_t;
			}
			fprintf(file, "2\t");
//			QueryBase q(qID, square, t);

			qb.id = qID;
			qb.threshold = t;

			WriteQueryToFile(file, &qb, dim);
//
//			if (q.range != NULL) {
//				delete[] (q.range);
//				q.range = NULL;
//			}
			qID++;

		}
//		else if (opList[i] == 3) {
//			// Deletion of query.
//			delID = GenRandInt(1, qID - 1);
//			fprintf(file, "3\t%d\n", delID);
//		}
	}

	// We just ignore the rest deletion when all the points are inserted.
//	while (curPos < delList.size()) {
//		// Deletion of query.
//		fprintf(file, "3\t%d\n", delList[curPos].delID);
//		curPos++;
//	}

	// -1 means the end of the file.
	fprintf(file, "-1\n");

	if (square != NULL) {
		delete[] square;
		square = NULL;
	}

	if (point != NULL) {
		delete[] point;
		point = NULL;
	}

	delList.release_space();

	fclose(file);
}

/*
 *  Generate a log of insertions of points, determine the deletion time for each query and keep the number of alive queries.
 *  Parameter List:
 *  	filePath:		the path of the target file.
 *  	dim:			the dimensionality of space.
 *  	qNumStart:		the number of queries at the beginning.
 *  	ptInsNum:		the number of insertions of points.
 *  	qInsNum:		the number of insertions of queries.
 *		qDelNum:		the number of deletions of queries.
 *		uni_w:			the uniform weight of points.
 *						If uni_w = -1, then we randomly generate weight for each point.
 *		uni_t:			the uniform threshold of queries. (unweighted version)
 *						If uni_t = -1, then we randomly generate threshold for each query.
 */
void GenLogToFile_GQuery_UPoint_KeepAliveNum(char* filePath, int dim,
		int qNumStart, int ptInsNum, int qInsNum, int qDelNum, int uni_w,
		int uni_t) {
	FILE* file = fopen(filePath, "wt");
	if (file == NULL) {
		ShowError(
				"Error in function GenLogToFile:\nFile open failed! Please check your file path!\n");
	}

	int* square = new int[2 * dim];
	int* point = new int[dim];

	fprintf(file, "%d\t%d\t%d\t%d\t%d\n", dim, qNumStart, ptInsNum, qInsNum,
			qDelNum);

	MyVector<OpEntry> entryList;
	entryList.reserve(qNumStart);

	double p = 1.0 / (uni_t * 4.34);
	//	printf("p = %lf\n", p);

	// Seed with a real random value, if available
	std::random_device rd;
	std::default_random_engine generator(rd());
	std::geometric_distribution<int> distribution(p);

	int delTimeStamp = 0;

	QueryBase qb;
	qb.range = square;

	for (int i = 0; i < qNumStart; ++i) {
		//		GenUniformSquare(dim, square, QLEN, rangeStart, rangeEnd);
		GenGaussianSquare(dim, square, QLEN, U, SIGMA, rangeStart, rangeEnd);
		//		QueryBase qb(i + 1, square, uni_t);
		qb.id = i + 1;
		qb.threshold = uni_t;
		WriteQueryToFile(file, &qb);

		delTimeStamp = distribution(generator) + 1;
		OpEntry entry(delTimeStamp, i + 1, 3);
		entryList.push_back(entry);
	}

	int ptID = 1;
	int qID = qNumStart + 1;

	vector<int> opList;
	opList.reserve(ptInsNum);

	int i = 0;

	for (; i < ptInsNum; ++i) {
		// Insert a point.
		opList.push_back(1);
	}

	sort(entryList.get_list(), entryList.get_list() + entryList.size());
	// The add operations to keep the alive number.
	int delTime = 0;
	for (int i = 0; i < qNumStart; ++i) {
		delTime = entryList[i].time;
		while (delTime <= ptInsNum) {
			// Generate an insertion of a query.
			OpEntry entry(delTime, qID, 2);
			entryList.push_back(entry);
			// Compute the dead time for this query.
			delTime += distribution(generator) + 1;
			OpEntry delEntry(delTime, entry.qID, 3);
			entryList.push_back(delEntry);
			qID++;
		}
	}

	// sort the entryList again.
	sort(entryList.get_list(), entryList.get_list() + entryList.size());

	int w = 0;
	int t = 0;
	unsigned int curPos = 0;

	for (i = 0; i < ptInsNum; ++i) {
		if (opList[i] == 1) {
			// Insertion of point.
			GenUniformPoint(dim, point, rangeStart, rangeEnd);
			//			GenGaussianPoint(dim, point, U, SIGMA, rangeStart, rangeEnd);
			//			point[0] = GenRandInt(rangeStart, rangeEnd);

			if (uni_w == -1) {
				//				w = GenRandInt(1, 100);
				// Generate a weight from N(100, 15^2);
				w = GenGaussianRandInt(100, 15);
				while (w < 1) {
					w = GenGaussianRandInt(100, 15);
				}
			} else {
				w = uni_w;
			}

			fprintf(file, "1\t%d\t", ptID);
			for (int j = 0; j < dim; ++j) {
				fprintf(file, "%d\t", point[j]);
			}
			fprintf(file, "%d\n", w);

			while (curPos < entryList.size() && ptID == entryList[curPos].time) {

				if (entryList[curPos].opType == 3) {
					// Deletion of a query.
					fprintf(file, "3\t%d\n", entryList[curPos].qID);
				} else if (entryList[curPos].opType == 2) {
					// Insertion of a query.
					GenGaussianSquare(dim, square, QLEN, U, SIGMA, rangeStart,
							rangeEnd);
					if (uni_t == -1) {
						t = GenRandInt(1, ptInsNum);
					} else {
						t = uni_t;
					}
					fprintf(file, "2\t");
					//			QueryBase q(qID, square, t);

					qb.id = entryList[curPos].qID;
					qb.threshold = t;

					WriteQueryToFile(file, &qb, dim);
				}
				curPos++;
			}
			ptID++;
		}
	}

	// -1 means the end of the file.
	fprintf(file, "-1\n");

	if (square != NULL) {
		delete[] square;
		square = NULL;
	}

	if (point != NULL) {
		delete[] point;
		point = NULL;
	}

	entryList.release_space();

	fclose(file);
}

/*
 *  Generate a log of insertions of points, determine the deletion time for each query and keep the number of alive queries.
 *  Parameter List:
 *  	filePath:		the path of the target file.
 *  	dim:			the dimensionality of space.
 *  	qNumStart:		the number of queries at the beginning.
 *  	ptInsNum:		the number of insertions of points.
 *  	qInsNum:		the number of insertions of queries.
 *		qDelNum:		the number of deletions of queries.
 *		uni_w:			the uniform weight of points.
 *						If uni_w = -1, then we randomly generate weight for each point.
 *		uni_t:			the uniform threshold of queries. (unweighted version)
 *						If uni_t = -1, then we randomly generate threshold for each query.
 *  	spareQueryPath:	the path of the file of all the spare queries.
 *  	spareQNum:		the number of spare queries.
 */
void GenLogToFile_GQuery_UPoint_KeepAliveNum(char* filePath, int dim,
		int qNumStart, int ptInsNum, int qInsNum, int qDelNum, int uni_w,
		int uni_t, char* spareQueryPath, int spareQNum) {
	FILE* file = fopen(filePath, "wt");
	if (file == NULL) {
		ShowError(
				"Error in function GenLogToFile:\nFile open failed! Please check your file path!\n");
	}

	int* square = new int[2 * dim];
	int* point = new int[dim];

	fprintf(file, "%d\t%d\t%d\t%d\t%d\n", dim, qNumStart, ptInsNum, qInsNum,
			qDelNum);

	MyVector<OpEntry> entryList;
	entryList.reserve(qNumStart);

	double p = 1.0 / (uni_t * 4.34);
	//	printf("p = %lf\n", p);

	// Seed with a real random value, if available
	std::random_device rd;
	std::default_random_engine generator(rd());
	std::geometric_distribution<int> distribution(p);

	int delTimeStamp = 0;

	QueryBase qb;
	qb.range = square;

	for (int i = 0; i < qNumStart; ++i) {
		//		GenUniformSquare(dim, square, QLEN, rangeStart, rangeEnd);
		GenGaussianSquare(dim, square, QLEN, U, SIGMA, rangeStart, rangeEnd);
		//		QueryBase qb(i + 1, square, uni_t);
		qb.id = i + 1;
		qb.threshold = uni_t;
		WriteQueryToFile(file, &qb);

		delTimeStamp = distribution(generator) + 1;
		OpEntry entry(delTimeStamp, i + 1, 3);
		entryList.push_back(entry);
	}

	int ptID = 1;
	int qID = qNumStart + 1;

	vector<int> opList;
	opList.reserve(ptInsNum);

	int i = 0;

	for (; i < ptInsNum; ++i) {
		// Insert a point.
		opList.push_back(1);
	}

	sort(entryList.get_list(), entryList.get_list() + entryList.size());
//	// The add operations to keep the alive number.
//	int delTime = 0;
//	for (int i = 0; i < qNumStart; ++i) {
//		delTime = entryList[i].time;
//		while (delTime <= ptInsNum) {
//			// Generate an insertion of a query.
//			OpEntry entry(delTime, qID, 2);
//			entryList.push_back(entry);
//			// Compute the dead time for this query.
//			delTime += distribution(generator) + 1;
//			OpEntry delEntry(delTime, entry.qID, 3);
//			entryList.push_back(delEntry);
//			qID++;
//		}
//	}
//
//	// sort the entryList again.
//	sort(entryList.get_list(), entryList.get_list() + entryList.size());

	int w = 0;
	int t = 0;
	unsigned int curPos = 0;

	for (i = 0; i < ptInsNum; ++i) {
		if (opList[i] == 1) {
			// Insertion of point.
			GenUniformPoint(dim, point, rangeStart, rangeEnd);
			//			GenGaussianPoint(dim, point, U, SIGMA, rangeStart, rangeEnd);
			//			point[0] = GenRandInt(rangeStart, rangeEnd);

			if (uni_w == -1) {
				//				w = GenRandInt(1, 100);
				// Generate a weight from N(100, 15^2);
				w = GenGaussianRandInt(100, 15);
				while (w < 1) {
					w = GenGaussianRandInt(100, 15);
				}
			} else {
				w = uni_w;
			}

			fprintf(file, "1\t%d\t", ptID);
			for (int j = 0; j < dim; ++j) {
				fprintf(file, "%d\t", point[j]);
			}
			fprintf(file, "%d\n", w);

			while (curPos < entryList.size() && ptID == entryList[curPos].time) {

				if (entryList[curPos].opType == 3) {
					// Deletion of a query.
					fprintf(file, "3\t%d\n", entryList[curPos].qID);
				} else if (entryList[curPos].opType == 2) {
					// Insertion of a query.
					GenGaussianSquare(dim, square, QLEN, U, SIGMA, rangeStart,
							rangeEnd);
					if (uni_t == -1) {
						t = GenRandInt(1, ptInsNum);
					} else {
						t = uni_t;
					}
					fprintf(file, "2\t");
					//			QueryBase q(qID, square, t);

					qb.id = entryList[curPos].qID;
					qb.threshold = t;

					WriteQueryToFile(file, &qb, dim);
				}
				curPos++;
			}
			ptID++;
		}
	}

	// -1 means the end of the file.
	fprintf(file, "-1\n");

	entryList.release_space();

	fclose(file);

	// Write the spare query file.
	file = fopen(spareQueryPath, "wt");
	if (file == NULL) {
		ShowError(
				"Error in function GenLogToFile:\nFile open failed! Please check your spare query file path!\n");
	}

	fprintf(file, "%d\t%d\n", dim, spareQNum);

	for (int i = 0; i < spareQNum; ++i) {
		//		GenUniformSquare(dim, square, QLEN, rangeStart, rangeEnd);
		GenGaussianSquare(dim, square, QLEN, U, SIGMA, rangeStart, rangeEnd);
		//		QueryBase qb(i + 1, square, uni_t);
		qb.id = i + 1 + qNumStart;
		qb.threshold = uni_t;
		WriteQueryToFile(file, &qb);

		delTimeStamp = distribution(generator) + 1;
//		OpEntry entry(delTimeStamp, i + 1, 3);
//		entryList.push_back(entry);

		fprintf(file, "%d\n", delTimeStamp);
	}

	fprintf(file, "-1\n");

	fclose(file);

	if (square != NULL) {
		delete[] square;
		square = NULL;
	}

	if (point != NULL) {
		delete[] point;
		point = NULL;
	}
}

/*
 * 	Read the spare query file.
 * 	Parameter List:
 * 		spareQUeryPath:		the file path of the spare query file.
 * 		spareList:			the list of the content of the file.
 */
void ReadSpareQueryFile(char* spareQueryPath, MyVector<int>& spareList) {
	FILE* file = fopen(spareQueryPath, "rt");
	if (file == NULL) {
		ShowError(
				"Error in function ReadSpareQueryFile:\nSpare query file open failed! Please check your file path!\n");
	}

	int dim = 0;
	int spareQNum = 0;

	fscanf(file, "%d\t%d\n", &dim, &spareQNum);
	printf("dim = %d\t spareQNum = %d\n", dim, spareQNum);

	// The length of the spareList.
	// (id + range + threshold + delTime) * spareQNum + EOF.
	int length = (1 + 2 * dim + 1 + 1) * spareQNum + 1;

	spareList.reserve(length);

	int tempInt = 0;
	for (int i = 0; i < length; ++i) {
		fscanf(file, "%d", &tempInt);
		spareList.push_back(tempInt);
	}

	fclose(file);
}

