/*
 * LogGen_IO.h
 *
 *  Created on: 7 Oct, 2015
 *      Author: jhgan
 */

#ifndef LOGGEN_IO_H_
#define LOGGEN_IO_H_

#include "QueryGen_IO.h"
#include "../MyRandom/MyRandom.h"

#include <random>
using namespace std;

/***************************************************************
 *  			Log Generation Related.
 ***************************************************************/

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
		int qInsNum, int qDelNum, int uni_w, int uni_t);

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
void GenLogToFile_Uniform(char* filePath, int dim, int qNumStart, int ptInsNum,
		int qInsNum, int qDelNum, int uni_w, int uni_t);

/*
 *  Read the whole log into the specified vector.
 *  Parameter List:
 *  	filePath:			the path of the log file.
 *		queryList:			the target place to store the list of queries registered at the beginning.
 *		rangeValueList:		the target place to store the range values of those queries.
 *  	opList:				the target place to store the list of operation.
 */
void ReadLogFromFile(char* filePath, MyVector<QueryBase>& queryList,
		MyVector<int>& rangeValueList, MyVector<int>& opList);

/*
 *  Read the whole log into the specified vector.
 *  Parameter List:
 *  	filePath:			the path of the log file.
 *		queryList:			the target place to store the list of queries registered at the beginning.
 *  	opList:				the target place to store the list of operation.
 */
void ReadLogFromFile(char* filePath, MyVector<QueryBase>& queryList,
		MyVector<int>& opList);

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
		int ptInsNum, int qInsNum, int qDelNum, int uni_w, int uni_t);


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
void GenLogToFile_GQuery_UPoint(char* filePath, int dim, int qNumStart,
		int ptInsNum, int qInsNum, int qDelNum, int uni_w, int uni_t);

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
		int ptInsNum, int qInsNum, int qDelNum, int uni_w, int uni_t, double qInsProb);


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
void GenLogToFile_GQuery_UPoint_KeepAliveNum(char* filePath, int dim, int qNumStart,
		int ptInsNum, int qInsNum, int qDelNum, int uni_w, int uni_t);


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
void GenLogToFile_GQuery_UPoint_KeepAliveNum(char* filePath, int dim, int qNumStart,
		int ptInsNum, int qInsNum, int qDelNum, int uni_w, int uni_t, char* spareQueryPath, int spareQNum);


/*
 * 	Read the spare query file.
 * 	Parameter List:
 * 		spareQUeryPath:		the file path of the spare query file.
 * 		spareList:			the list of the content of the file.
 */
void ReadSpareQueryFile(char* spareQueryPath, MyVector<int>& spareList);


void testSort();

#endif /* LOGGEN_IO_H_ */
