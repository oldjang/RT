/*
 * GlobalVars.h
 *
 *  Created on: 8 Sep, 2015
 *      Author: jhgan
 */

#ifndef GLOBALVARS_H_
#define GLOBALVARS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <sys/time.h>
using namespace std;

// #define SHOWALERT

/*
 *  The largest integer.
 */
#define MYINFT 2147483647

/******************************************************************************
 *  This file defines all the global variables.
 ******************************************************************************/

/*
 *  The dimensionality of all the query ranges.
 */
extern int DIM;

/*
 *  The number of queries registered at the beginning.
 */
extern int QSN;

/*
 *  The number of point insertions.
 */
extern int PIN;

/*
 *  The number of query insertions.
 */
extern int QIN;

/*
 *  The number of query deletions.
 */
extern int QDN;

/*
 *  Whether to write the alert ID list to file.
 */
extern bool WriteResult;

/*
 *  The mean of Gaussian distribution.
 */
extern int U;

/*
 *  The standard deviation of Gaussian distribution.
 */
extern int SIGMA;

/*
 *  The side length of query squares.
 */
extern int QLEN;

/*
 *  The number of operations:
 *  	point insertions;
 *  	query insertions;
 *  	query deletions;
 *  	query maturities;
 */
extern int OPNUM;

/*
 *  The number of query objects created.
 */
extern int QNUM;

/*
 *  The number of segment trees created.
 */
extern int TNUM;

/*
 *  The number of tree nodes created.
 */
extern int TNNUM;

/*
 *  The number of heap elements created.
 */
extern int HENUM;

/*
 *  The number of min heap objects created.
 */
extern int MHNUM;

/*
 *  The number of the interval trees created.
 */
extern int TNUM_INV;

/*
 *  The number of the interval tree nodes created.
 */
extern int TNNUM_INV;

/*
 *  The number of the pure segment trees created.
 */
extern int TNUM_SEG;

/*
 *  The number of the pure segment tree nodes created.
 */
extern int TNNUM_SEG;

/*
 *  The number of non-empty tree nodes.
 */
extern int NonEmptyNum;

/*
 *  The number of query ranges that stabbed.
 */
extern long long StabNum;

/*
 *  The total time of global rebuilding.
 */
extern double RebuildTime;

/*
 *  The maximum global rebuilding time.
 */
extern double MaxRebuildTime;

/*****************************************************************************
 *  The following functions are for error processing.
 *****************************************************************************/

/*
 *  Show the error message.
 *  Parameter List:
 *   	msg:	the error info
 */
void ShowError(string msg);

/*****************************************************************************
 *  The following functions and variables are for running time evaluation.
 *****************************************************************************/

double getCurrentTime();

#endif /* GLOBALVARS_H_ */
