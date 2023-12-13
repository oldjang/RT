/*
 * ResultFile_IO.cpp
 *
 *  Created on: 16 Oct, 2015
 *      Author: jhgan
 */

#include "ResultFile_IO.h"

/*
 *  Write a result line to the specified file by the given file handler.
 *  Parameter List:
 *  	file:			the given file handler.
 *  	pID:			the id of the point.
 *  	alertIDList:	the id's of the queries alerted due to the insertion of point with pID.
 */
void WriteResultLineToFile(FILE* file, int pID, MyVector<int>& alertIDList) {
	if (file == NULL) {
		ShowError(
				"Error in function WriteResultLineToFile:\nFile handle is NULL!\n");
	}

	// 0 means the start of a new line.
	fprintf(file, "0\t");
	fprintf(file, "%d", pID);

	int idNum = alertIDList.size();

	// sort the id's.
	sort(alertIDList.get_list(), alertIDList.get_list() + idNum);

	for (int i = 0; i < idNum; ++i) {
		fprintf(file, "\t%d", alertIDList[i]);
	}
	fprintf(file, "\n");
}

/*
 *  Read the result file from the file by the given path.
 *  Parameter List:
 *  	filePath:		the path of the specified file.
 *  	targetPlace:	the target place to store the content of this file.
 */
void ReadResultFile(char* filePath, MyVector<int>& targetPlace) {
	FILE* file = fopen(filePath, "rt");
	if (file == NULL) {
		ShowError(
				"Error in function ReadResultFile:\nFile open failed! Please check your file path!\n");
	}

	int tempInt = 0;
	while (fscanf(file, "%d", &tempInt) != EOF) {
		targetPlace.push_back(tempInt);
	}

	fclose(file);
}
