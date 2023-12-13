/*
 * QueryGen_IO.h
 *
 *  Created on: 5 Oct, 2015
 *      Author: jhgan
 */

#ifndef QUERYGEN_IO_H_
#define QUERYGEN_IO_H_

#include "../headers.h"
#include "../ToolFunctions.h"
#include "../MyRandom/MyRandom.h"
#include "../QueryBase.h"

#include <fstream>
#include <vector>
using namespace std;

/*
 *  Randomly generate a query and output to file by the given handler.
 *  Parameter List:
 *  	file:			the file handler.
 *  	dim:			the dimensionality fo queries.
 * 		uni_t:			the uniform threshold value. If uni_t = 0, then randomly generate a threshold for each query.
 * 		uni_eps:		the uniform epsilon value. If uni_eps < 0, then randomly generate an eps for each query.
 */
void GenQueryToFile_SubRoutine(FILE* file, int qid, int dim, int uni_t,
		float uni_eps = -1);

/*
 * 	Randomly generate queries to file.
 * 	Parameter List:
 * 		filePath:		the file path of the target file to output.
 * 		qNum:			the number of queries.
 * 		dim:			the dimensionality of queries.
 * 		uni_t:			the uniform threshold value. If uni_t = 0, then randomly generate a threshold for each query.
 * 		uni_eps:		the uniform epsilon value. If uni_eps < 0, then randomly generate an eps for each query.
 */
void GenQueryToFile(char* filePath, int qNum, int dim, int uni_t,
		float uni_eps);

/*
 *  Write the given query to file by the given file handler.
 */
void WriteQueryToFile(FILE* file, QueryBase* query, int dim = DIM);

/*
 *  Write the given query list to the specified file.
 */
void WriteQueryListToFile(char* filePath, vector<QueryBase*>& queryList, int dim = DIM);

/*
 *  Read an object of QueryBase by the given file handler.
 *  Parameter List:
 *  	file:			the file handler.
 *  	dim:			the dimensionality of query.
 *  Return:
 *  	the pointer of the object read from the file.
 */
QueryBase* ReadQueryFromFile_SubRoutine(FILE* file, int dim = DIM);

/*
 *  Read the query list from the given file.
 *  Parameter List:
 *  	filePath:		the file path of the target file to read.
 *  	targetPlace:	the target place to store the queries.
 */
void ReadQueryFromFile(char* filePath, vector<QueryBase*>& targetPlace);

/*
 *  Read the query list from the given file.
 *  Return:
 *  	the pointer of the array of queries.
 */
QueryBase** ReadQueryFromFile(char* filePath);

void ReadQueryFromFile(char* filePath, MyVector<QueryBase>& targetPlace, int* rangeValueList);

void ReadQueryFromFile_SubRoutine(FILE* file, QueryBase& temp,
		MyVector<QueryBase>& targetPlace, int* rangePosition, int dim = DIM);

/*
 *  Generate Gnuplot script for the given query.
 */
void GenQuery_PlotScript_SubRoutine(FILE* file, QueryBase* q, int dim = DIM);

/*
 *  Generate Gnuplot script for the given query list.
 */
void GenQuery_PlotScript(char* filePath, vector<QueryBase*>& queryList);

/*
 *  Write the given point to file by the given file handler.
 *  Parameter List:
 *  	file:			the file handler.
 *  	coords:			the coordinates of the given point.
 */
void WritePointToFile(FILE* file, vector<int>& coords);

#endif /* QUERYGEN_IO_H_ */
