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

int main(int nargs, char **args)
{
    DataMaker::DataMaker dataMaker;
    int N = 1000;
    int upper = 10000;

    dataMaker.getRandomIntIntervals(N,upper, 60);
    auto queryList = dataMaker.intervals;
    DeterministicSkipList skipList(2);
    srand(time(0));
    std::vector<int> v;
    for(int i = 0; i <queryList.size(); i++) v.push_back(rand()%upper);
    std::vector<int> ins;
    for(int i = 0; i <queryList.size(); i++) ins.push_back(rand()%20 + 1);

    std::ofstream fout("test.data");

    for(int i = 0 ;i < N; i++){
        fout<<queryList[i].L<<' '<<queryList[i].R<<' '<<v[i]<<' '<<ins[i]<<'\n';
    }

    fout.close();

    std::ifstream fin("test.data");

    for(int i = 0 ;i < N; i++){
        fin>>queryList[i].L>>queryList[i].R>>v[i]>>ins[i];
    }
    for(int i = 0 ;i <queryList.size(); i++){
        int th = 1000;
        int id = rangeList.size();
        Range *range = new Range(queryList[i].L, queryList[i].R, i, th, id);
        rangeList.push_back(range);

        bruteForce.addQuery(queryList[i].L, queryList[i].R, i, th);
        skipList.cover(queryList[i].L, queryList[i].R, id, th);

//        if(i>=3)printf("%d\n",rangeList[3]->storedIn[0][0].second);
        auto result1 = skipList.increase(v[i], ins[i]);
        auto result2 = bruteForce.addPoint(v[i],ins[i]);
        if(result1.size()!=result2.size()){
            puts("NO");
            if(result1.size()>0){for(auto x:result1)printf("%d ",x);puts("r1");}
            if(result2.size()>0){for(auto x:result2)printf("%d ",x);puts("r2");}
        }
        else {
            for (int j = 0; j < result1.size(); j++) {
                if (result1[j] != result2[j])puts("No");
                break;
            }
        }
    return 0;
}
