/*
 * GlobalVars.cpp
 *
 *  Created on: 7 Sep, 2015
 *      Author: jhgan
 */

#include "GlobalVars.h"

/*
 *  The dimensionality of all the query ranges.
 */
int DIM = 1;

/*
 *  The number of queries registered at the beginning.
 */
int QSN = 0;

/*
 *  The number of point insertions.
 */
int PIN = 0;

/*
 *  The number of query insertions.
 */
int QIN = 0;

/*
 *  The number of query deletions.
 */
int QDN = 0;

/*
 *  Whether to write the alert ID list to file.
 */
bool WriteResult = true;


/*
 *  The mean of Gaussian distribution.
 */
int U = 50000;

/*
 *  The standard deviation of Gaussian distribution.
 */
int SIGMA = 7500;

/*
 *  The side length of query squares.
 */
int QLEN = 10000;

/*
 *  The number of operations:
 *  	point insertions;
 *  	query insertions;
 *  	query deletions;
 *  	query maturities;
 */
int OPNUM = 0;

/*
 *  The number of query objects created.
 */
int QNUM = 0;

/*
 *  The number of segment trees created.
 */
int TNUM = 0;

/*
 *  The number of tree nodes created.
 */
int TNNUM = 0;

/*
 *  The number of heap elements created.
 */
int HENUM = 0;

/*
 *  The number of min heap objects created.
 */
int MHNUM = 0;

/*
 *  The number of the interval trees created.
 */
int TNUM_INV = 0;

/*
 *  The number of the interval tree nodes created.
 */
int TNNUM_INV = 0;

/*
 *  The number of the pure segment trees created.
 */
int TNUM_SEG = 0;

/*
 *  The number of the pure segment tree nodes created.
 */
int TNNUM_SEG = 0;

/*
 *  The number of non-empty tree nodes.
 */
int NonEmptyNum = 0;

/*
 *  The number of query ranges that stabbed.
 */
long long StabNum = 0;

/*
 *  The total time of global rebuilding.
 */
double RebuildTime = 0;

/*
 *  The maximum global rebuilding time.
 */
double MaxRebuildTime = 0;

///*
// *  The total time of merging continuous list.
// */
//double InsQTime_Ours = 0;
//
///*
// *  The total time of calling DeleteMyself() function.
// */
//double DeleteTime_Ours = 0;
//
///*
// *  The total time of calling ThresholdMonitoring_Action() function.
// */
//double MonitorTime_Ours = 0;
//
///*
// *  The total time of finding canonical nodes during monitoring.
// */
//double FindCanonicalTime_Ours = 0;
//
///*
// *  The total time of processing stabbing queries.
// */
//double StabTime_Ours = 0;
//
///*
// *  The start time of a stab query.
// */
//double StabStart_Ours = 0;
//
///*
// *  The total time of scanning the emitList.
// */
//double ScanTime_Ours = 0;
//
///*
// *  The total time of constructing the emitList.
// */
//double ConsListTime_Ours = 0;
//
///*
// *  The total time of notifying queries.
// */
//double NotificationTime_Ours = 0;
//
///*
// *  The total time of maintaining the heap.
// */
//double HeapMaintinanceTime_Ours = 0;
//
///*
// *  The total time of checking whether to emit.
// */
//double CheckTime_Ours = 0;
//
///*
// *  The total time of query movement.
// */
//double MovementTime = 0;
//
//double CopyQueryTime = 0;
//
//double InsQueryTime = 0;
//
//double EndPointTreeTime = 0;
//
//double CanonicalTime = 0;
//
///*
// *  The total time of segTreeLog structure.
// */
//double SegTreeLogTime = 0;
//
///*
// *  The total time of emitLog structure.
// */
//double EmitLogTime = 0;
//
///*
// *  The total construction time of BF_SEG.
// */
//double CONSTIME_SEG = 0;
//
///*
// *  The total time of inserting query to the segment tree.
// */
//double InsQTime_Seg = 0;
//
///*
// *  The total time of broadcast during the construction.
// */
//double BroadCastTime_Seg = 0;
//
///*
// *  The total query time of BF_SEG.
// */
//double SEG_TIME = 0;
//
///*
// *  The total construction time of BF_INV.
// */
//double CONSTIME_INV = 0;
//
///*
// *  The total query time of BF_INV.
// */
//double INV_TIME = 0;

/*****************************************************************************
 *  		The following functions are for error processing.
 *****************************************************************************/

void ShowError(string msg) {
	printf("%s\n", msg.c_str());
	exit(0);
}

/*****************************************************************************
 *  The following functions are for running time evaluation.
 *****************************************************************************/

/*
 *  The data structure for getting time.
 */
static struct timeval tim;

double getCurrentTime() {
	gettimeofday(&tim, NULL);
	return tim.tv_sec + (tim.tv_usec / (double) CLOCKS_PER_SEC);
}
