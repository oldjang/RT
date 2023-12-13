/*
 * QueryGen_IO.cpp
 *
 *  Created on: 5 Oct, 2015
 *      Author: jhgan
 */

#include "QueryGen_IO.h"

/*
 *  A buffer for reading some useless char.
 */
char sBuffer[100];

/*
 *  Randomly generate a query and output to file by the given handler.
 *  Parameter List:
 *  	file:			the file handler.
 *  	dim:			the dimensionality fo queries.
 * 		uni_t:			the uniform threshold value. If uni_t = 0, then randomly generate a threshold for each query.
 * 		uni_eps:		the uniform epsilon value. If uni_eps < 0, then randomly generate an eps for each query.
 */
void GenQueryToFile_SubRountine(FILE* file, int qid, int dim, int uni_t,
		float uni_eps) {
	if (file == NULL) {
		ShowError(
				"Error in function GenQueryToFile_SubRountine:\nFile handle is NULL!\n");
	}

	// Output qid.
	fprintf(file, "%d", qid);

	vector<int> range;
	// Generate a random range.
	GenQueryRange(range, dim);

	// Output the query range.
	for (int i = 0; i < 2 * dim; i++) {
		fprintf(file, "\t%d", range[i]);
	}

	// Output threshold.
	if (uni_t == 0) {
		uni_t = GenRandInt(rangeStart, rangeEnd);
	}
	fprintf(file, "\t%d", uni_t);

#ifdef DYNAMIC
	// Output eps.
	if (uni_eps < 0) {
		uni_eps = GenRandDouble(0, 1.0);
	}
	fprintf(file, "\t%.f", uni_eps);
#endif

	fprintf(file, "\n");
}

/*
 * 	Randomly generate queries to file.
 * 	Parameter List:
 * 		filePath:		the file path of the target file to output.
 * 		qNum:			the number of queries.
 * 		uni_t:			the uniform threshold value. If uni_t = 0, then randomly generate a threshold for each query.
 * 		uni_eps:		the uniform epsilon value. If uni_eps < 0, then randomly generate an eps for each query.
 */
void GenQueryToFile(char* filePath, int qNum, int dim, int uni_t,
		float uni_eps) {
	FILE* file = fopen(filePath, "wt");
	if (file == NULL) {
		ShowError(
				"Error in function GenQueryToFile:\nFile open failed! Please check your file path!\n");
	}

	fprintf(file, "%d\t %d\n", qNum, dim);

	for (int i = 0; i < qNum; ++i) {
		// Generate a query.
		GenQueryToFile_SubRountine(file, i + 1, dim, uni_t, uni_eps);
	}

	fclose(file);
}

void WriteQueryToFile(FILE* file, QueryBase* query, int dim) {
	if (file == NULL) {
		ShowError(
				"Error in function WriteQueryToFile:\nFile handle is NULL!\n");
	}

	// Output qid.
	fprintf(file, "%d", query->id);

	// Output the query range.
	for (int i = 0; i < 2 * dim; i++) {
		fprintf(file, "\t%d", query->range[i]);
	}

	// Output threshold.
	fprintf(file, "\t%d", query->threshold);

#ifdef DYNAMIC
	// Output eps.
	fprintf(file, "\t%.f", query->eps);
#endif

	fprintf(file, "\n");
}

/*
 *  Write the given query list to the specified file.
 */
void WriteQueryListToFile(char* filePath, vector<QueryBase*>& queryList,
		int dim) {
	FILE* file = fopen(filePath, "wt");
	if (file == NULL) {
		ShowError(
				"Error in function WriteQueryListToFile:\nFile open failed! Please check your file path!\n");
	}

	int qNum = queryList.size();
	fprintf(file, "%d\t %d\n", qNum, dim);

	for (int i = 0; i < qNum; ++i) {
		WriteQueryToFile(file, queryList[i], dim);
	}

	fclose(file);
}

/*
 *  Read an object of QueryBase by the given file handler.
 *  Parameter List:
 *  	file:			the file handler.
 *  	dim:			the dimensionality of query.
 *  Return:
 *  	the pointer of the object read from the file.
 */
QueryBase* ReadQueryFromFile_SubRoutine(FILE* file, int dim) {
	if (file == NULL) {
		ShowError(
				"Error in function ReadQueryFromFile_SubRoutine:\nFile handle is NULL!\n");
	}
	int id = 0;
	int* coords = new int[2 * dim];
	int t = 0;
	float eps = 0.0;

	fscanf(file, "%d", &id);
	for (int i = 0; i < 2 * dim; i++) {
		fscanf(file, "%d", &(coords[i]));
	}
	fscanf(file, "%d", &t);

#ifdef DYNAMIC
	fscanf(file, "%.f", &eps);
#endif
	// eat the useless char
	fscanf(file, "%[^\n]", sBuffer);

	QueryBase* q = new QueryBase(id, coords, t);
	if (q == NULL) {
		ShowError(
				"Error in ReadQueryFromFile_SubRoutine: Fail to create a pointer of QueryBase!\n");
	}

	delete[] coords;
	return q;
}

void ReadQueryFromFile(char* filePath, vector<QueryBase*>& targetPlace) {
	FILE* file = fopen(filePath, "rt");
	if (file == NULL) {
		ShowError(
				"Error in function ReadQueryFromFile:\nFile open failed! Please check your file path!\n");
	}

	int qNum = 0;
	int dim = 0;
	fscanf(file, "%d\t %d\n", &qNum, &dim);

	targetPlace.reserve(qNum);

	for (int i = 0; i < qNum; ++i) {
		targetPlace.push_back(ReadQueryFromFile_SubRoutine(file, dim));
	}

	fclose(file);
}

QueryBase** ReadQueryFromFile(char* filePath) {
	FILE* file = fopen(filePath, "rt");
	if (file == NULL) {
		ShowError(
				"Error in function ReadQueryFromFile:\nFile open failed! Please check your file path!\n");
	}

	int qNum = 0;
	int dim = 0;
	fscanf(file, "%d\t %d\n", &qNum, &dim);

	QueryBase** queryList = new QueryBase*[qNum];

	for (int i = 0; i < qNum; ++i) {
		queryList[i] = ReadQueryFromFile_SubRoutine(file, dim);
	}

	fclose(file);

	return queryList;
}

void ReadQueryFromFile_SubRoutine(FILE* file, QueryBase& temp,
		MyVector<QueryBase>& targetPlace, int* rangePosition, int dim) {
	if (file == NULL) {
		ShowError(
				"Error in function ReadQueryFromFile_SubRoutine:\nFile handle is NULL!\n");
	}
//	int id = 0;
//	int* coords = temp.range;
//	int t = 0;
//	float eps = 0.0;

//	temp.range = new int[2 * dim];
	temp.range = rangePosition;

	fscanf(file, "%d", &(temp.id));
	for (int i = 0; i < 2 * dim; i++) {
		fscanf(file, "%d", &(temp.range[i]));
	}
	fscanf(file, "%d", &(temp.threshold));

#ifdef DYNAMIC
	fscanf(file, "%.f", &eps);
#endif
	// eat the useless char
	fscanf(file, "%[^\n]", sBuffer);

//	QueryBase q(id, coords, t, eps);
	targetPlace.push_back(temp);

//	q.ReleaseSpace();
//	delete[] coords;
}

void ReadQueryFromFile(char* filePath, MyVector<QueryBase>& targetPlace,
		int* rangeValueList) {
	FILE* file = fopen(filePath, "rt");
	if (file == NULL) {
		ShowError(
				"Error in function ReadQueryFromFile:\nFile open failed! Please check your file path!\n");
	}

	int qNum = 0;
	int dim = 0;
	fscanf(file, "%d\t %d\n", &qNum, &dim);

	targetPlace.clear();
	targetPlace.reserve(qNum);

	rangeValueList = new int[2 * dim * qNum];

	int* rangePosition = rangeValueList;

//	QueryBase** queryList = new QueryBase*[qNum];

	QueryBase temp;
//	temp.range = new int[dim * 2];

	for (int i = 0; i < qNum; ++i) {
		ReadQueryFromFile_SubRoutine(file, temp, targetPlace, rangePosition,
				dim);
		rangePosition += 2 * dim;
	}

//	temp.ReleaseSpace();

	fclose(file);
}

/*
 *  Generate Gnuplot script for the given query.
 */
void GenQuery_PlotScript_SubRoutine(FILE* file, QueryBase* q, int dim) {
	if (file == NULL) {
		ShowError(
				"Error in GenQuery_PlotScript_SubRoutine:\n File handle is NULL!\n");
	}
	int id = q->id;
	int* range = q->range;

	if (dim == 2) {
		// Print the gnuplot script.
		fprintf(file, "set object %d rect from ", id);
		fprintf(file, "%d", range[0]);
		for (int i = 1; i < dim; ++i) {
			fprintf(file, ", %d", range[2 * i]);
		}
		fprintf(file, " to %d", range[1]);
		for (int i = 1; i < dim; ++i) {
			fprintf(file, ", %d", range[2 * i + 1]);
		}
		fprintf(file, " fs empty border lc rgb \'red\'\n");
	} else {
		ShowError(
				"Error in GenQuery_PlotScript_SubRoutine: the given dim != 2!\n");
	}
}

/*
 *  Generate Gnuplot script for the given query list.
 */
void GenQuery_PlotScript(char* filePath, vector<QueryBase*>& queryList) {
	FILE* file = fopen(filePath, "wt");
	if (file == NULL) {
		ShowError(
				"Error in function GenQuery_PlotScript:\nFile open failed! Please check your file path!\n");
	}

	int qNum = queryList.size();
	for (int i = 0; i < qNum; ++i) {
		GenQuery_PlotScript_SubRoutine(file, queryList[i]);
	}

	fclose(file);
}

/*
 *  Write the given point to file by the given file handler.
 *  Parameter List:
 *  	file:			the file handler.
 *  	coords:			the coordinates of the given point.
 */
void WritePointToFile(FILE* file, vector<int>& coords) {
	if (file == NULL) {
		ShowError(
				"Error in function WritePointToFile:\nFile open failed! Please check your file path!\n");
	}

	int dim = coords.size();
	fprintf(file, "%d", coords[0]);
	for (int i = 1; i < dim; ++i) {
		fprintf(file, "\t%d", coords[i]);
	}
	fprintf(file, "\n");
}

