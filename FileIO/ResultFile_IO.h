/*
 * ResultFile_IO.h
 *
 *  Created on: 16 Oct, 2015
 *      Author: jhgan
 */

#ifndef RESULTFILE_IO_H_
#define RESULTFILE_IO_H_

#include "../headers.h"
#include "../MyVector/MyVector.h"


#include <fstream>
#include <algorithm>
using namespace std;


/*
 *  Write a result line to the specified file by the given file handler.
 *  Parameter List:
 *  	file:			the given file handler.
 *  	pID:			the id of the point.
 *  	alertIDList:	the id's of the queries alerted due to the insertion of point with pID.
 */
void WriteResultLineToFile(FILE* file, int pID, MyVector<int>& alertIDList);

/*
 *  Read the result file from the file by the given path.
 *  Parameter List:
 *  	filePath:		the path of the specified file.
 *  	targetPlace:	the target place to store the content of this file.
 */
void ReadResultFile(char* filePath, MyVector<int>& targetPlace);



#endif /* RESULTFILE_IO_H_ */
