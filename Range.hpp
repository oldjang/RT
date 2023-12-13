#ifndef DSLRT_RANGE_HPP
#define DSLRT_RANGE_HPP

#include <vector>
#include <math.h>

#include "SkipListNode.hpp"

class Range {
public:
    typedef std::pair<int,int> intPair;
    double L, R;
    int id;
    int length = 0;
    int threshold;
    int thresholdOnSite;
    int halfTS;
    int roundCnt;
    int nowI;
    int splitLevel;
    int thresholdSum;
    bool erase = false;
    int aId;


    std::vector<std::vector<std::pair<SkipListNode* ,int> > > storedIn;

    std::pair<intPair , intPair> getLogWithLength(int l){
        int upperTSOnSite= std::max(threshold / (2 * l), 1);
        int i = log(upperTSOnSite) / log(2);
        if(i == 0) return {{i, l - (halfTS - l)}, {i + 1, halfTS - l}};
        int hNumber = halfTS/(1 << i) - l;
        int lNumber = l - hNumber;
        return {{i, lNumber}, {i + 1, hNumber}};
    }

    void setSplitLevel(int level){
        splitLevel = level;
    }

    void setNowI(int i){
        nowI = i;
        thresholdOnSite = 1<<nowI;
    }

    int receiveNotification(int cntIncrease){
        roundCnt += cntIncrease;
        if(roundCnt >= threshold){
            return 1;
        }
        if((thresholdOnSite > 1||(thresholdOnSite == 1&&splitLevel!=maxLevel)) && roundCnt >= halfTS){
//            enterNextRound();
            return 2;
        }
        return 0;
    }

    void enterNextRound(){
        threshold = threshold - roundCnt;
        halfTS = threshold / 2;
        thresholdSum = 0;
        roundCnt = 0;
        auto tmp = getLogWithLength(length);
        splitLevel = -1;
        thresholdOnSite = (1<<(tmp.first.first));
        nowI = tmp.first.first;
        for(int i = 0; i <= maxLevel; i++){
            if(tmp.first.second > 0){
                splitLevel = i;
                tmp.first.second -= storedIn[i].size();
            }
        }
    }

    int getPwInLevel(int level){
        if(level <= splitLevel) return nowI;
        else return nowI + 1;
    }

   void fit(std::vector<std::pair<std::pair<SkipListNode*, int>, std::pair<intPair ,intPair> > > &changeSet){
        while((thresholdSum > halfTS) && nowI != 0){
            if(splitLevel == maxLevel){
                splitLevel = -1;
                nowI --;
                thresholdOnSite = 1 << nowI;
            }
            for(int i = 0 ; i < storedIn[splitLevel + 1].size(); i++){
                changeSet.push_back({storedIn[splitLevel + 1][i], {{nowI + 1,nowI}, {splitLevel + 1, i}}});
                thresholdSum -= thresholdOnSite;
            }
            splitLevel++;
        }

        while(thresholdSum + thresholdOnSite * storedIn[splitLevel].size() <= halfTS){
            for(int i = 0 ; i < storedIn[splitLevel].size(); i++){
                changeSet.push_back({storedIn[splitLevel][i], {{nowI,nowI + 1}, {splitLevel, i}}});
                thresholdSum += thresholdOnSite;
            }
            splitLevel--;
            if(splitLevel == -1){
                splitLevel = maxLevel;
                nowI ++;
                thresholdOnSite = 1 << nowI;
            }
        }

    }
    bool isCovered(double l, double r) const {
        return L <= l && r <= R;
    }

    int getSize(int level) {
        if(maxLevel < level) return 0;
        return storedIn[level].size();
    }

    void insert(int level, SkipListNode *node, int id) {
        while(maxLevel < level) {
            storedIn.emplace_back();
            maxLevel ++;
        }
        storedIn[level].push_back({node, id});
        thresholdSum += 1 << getPwInLevel(level);
        length ++;
    }

    int getAndDelete(std::vector<std::pair<std::pair<SkipListNode* ,int> ,int> > &eraseSet, int level, double L, double R){
        short j = 0;
        int cntIncrease = 0;
        int endId = storedIn[level].size() - 1;
        for(int i = 0; i <= endId; i++){
            SkipListNode *node = storedIn[level][i].first;
            if(L <= node->key && node->forward[level]->key <= R){
                eraseSet.push_back({storedIn[level][i], getPwInLevel(level)});
                thresholdSum -= 1<<getPwInLevel(level);
                cntIncrease += storedIn[level][i].first->cnt[level] - storedIn[level][i].first->RangeSet[level].bucketList[getPwInLevel(level)][storedIn[level][i].second]->getLastCnt();
                storedIn[level][i].first->RangeSet[level].bucketList[getPwInLevel(level)][storedIn[level][i].second]->setLastCnt(storedIn[level][i].first->cnt[level]);
            }
            else{
                if(j != i){
                    storedIn[level][j] = storedIn[level][i];
                    storedIn[level][j].first->RangeSet[level].bucketList[getPwInLevel(level)][storedIn[level][j].second]->setIndexInQuery(j);
                }
                j++;
            }
        }
        while(storedIn[level].size() > j) {
            storedIn[level].pop_back();
            length --;
        }
        return cntIncrease;
    }

    void changId(int level, int id, int newIdInNode){
        storedIn[level][id].second = newIdInNode;
    }

    Range(double l, double r, int Id, int _threshold,int _aId):L(l), R(r), id(Id), storedIn(1), threshold(_threshold){
        thresholdSum = 0;
        halfTS = threshold / 2;
        roundCnt = 0;
        aId = _aId;
    }

    int maxLevel = 0;
};


#endif //DSLRT_RANGE_HPP
