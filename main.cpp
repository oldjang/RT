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


#include "DataMaker.hpp"
#include "DeterministicSkipList.hpp"

#include "HighDimSkipList.h"
#include "HighDimSkipListNode.h"

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

int main() {
    DataMaker::DataMaker dataMaker;

    int N = 100000;
    int upper = 10000000;
    int M = 1000000;
    dataMaker.getRandomIntIntervals(N, upper, 10, 1000000);
    std::vector<std::pair<std::pair<double, double>, std::pair<int, int> > > query(N);
    MyVector<QueryBase>queryList;
    for (int i = 0; i < N; i++) {
        query[i] = {{dataMaker.intervals[i].L, dataMaker.intervals[i].R},
                    {i,                        dataMaker.intervals[i].th}};
        QueryBase temp;
        temp.id = i;
        temp.range = new int[2];
        temp.range[0] = dataMaker.intervals[i].L;
        temp.range[1] = dataMaker.intervals[i].R;
        temp.threshold = dataMaker.intervals[i].th;
        queryList.push_back(temp);
    }
    DeterministicSkipList skipList(query);
    MyVector<Query> logList;
    GenQueryListFromRawData(logList, queryList);

    double start = getCurrentTime();
    SegmentTree_Log* strConstructor = new SegmentTree_Log();
    LogarithmicFramework<Query>* our_log = new LogarithmicFramework<Query>(
            strConstructor, logList);

    double end = getCurrentTime();
    double duration = 0;
    duration += end - start;
    double constructionTime = end - start;

    printf("Construction done.\n");
    printf("Construction time = %lf seconds.\n", constructionTime);
    cout<<our_log->GetAliveNum()<<endl;
//		printf("IniTime_Ours = %lf\n", IniTime_Ours);
    fflush(stdout);

    dataMaker.getRandomOp(M,upper,10,1000000,10);
    double s1 = 0,s2 = 0;
    for(int i = 0; i<M;i++){
        if(dataMaker.op[i].first == 0){
            int id = rangeList.size();
            Range *range = new Range(dataMaker.op[i].second.L, dataMaker.op[i].second.R, i, dataMaker.op[i].second.th, id);
            rangeList.push_back(range);

            MyVector<int> range2;
            range2.resize(2,0);
            range2[0] = dataMaker.op[i].second.L;
            range2[1] = dataMaker.op[i].second.R;
            Query q(i,range2, dataMaker.op[i].second.th);
            double t1 = getCurrentTime();
            skipList.cover(dataMaker.op[i].second.L, dataMaker.op[i].second.R, id, dataMaker.op[i].second.th);

            double t2 = getCurrentTime();
            our_log->InsertQuery(q);
            double t3 = getCurrentTime();
            s1 +=t2-t1;
            s2 +=t3-t2;
        }
        else{
            MyVector<int> alertIDList;
            MyVector<int> ptCoords;
            ptCoords.resize(DIM, 0);
            ptCoords[0] = dataMaker.op[i].second.L;
            double t1 = getCurrentTime();
            auto x = skipList.increase(dataMaker.op[i].second.L,dataMaker.op[i].second.th);
            double t2 = getCurrentTime();
            our_log->InsertPoint(ptCoords.get_list(),dataMaker.op[i].second.th,alertIDList);
            double t3 = getCurrentTime();
            s1 +=t2-t1;
            s2 +=t3-t2;
//            for(auto a:x) cout<<a<<' ';puts("");
//            for(int j =0;j<alertIDList.size();j++)cout<<alertIDList[j]<<' ';puts("");
        }
        if(i%10000==0){
            cout<<s1<<' '<<s2<<endl;
            cout<<our_log->GetAliveNum()<<endl;
        }
    }
    cout<<s1<<' '<<s2<<endl;
    return 0;
}
