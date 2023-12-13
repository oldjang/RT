#ifndef DSLRT_SKIPLISTNODE_HPP
#define DSLRT_SKIPLISTNODE_HPP

#include <vector>
#include "Buckets.hpp"
#include "BucketElement.hpp"

class SkipListNode {
public:
    std::vector<SkipListNode*> forward;
//    std::vector<std::vector<std::pair<int, int> > > RangeSet;
    std::vector<Buckets> RangeSet;
    std::vector<std::vector<int> > lastCnt;
    double key;
    int maxLevel;
    std::vector<int> cnt;
    bool erase = false;

    void eraseNode(){erase = true;}

    SkipListNode(double _key, int _level):key(_key), maxLevel(_level), lastCnt(_level + 1){
        forward.resize(_level + 1);
        RangeSet.resize(_level + 1);
        cnt.resize(_level + 1);
    }

    int getSize(int level, int powI){
        if(maxLevel < level) return 0;
        RangeSet[level].extendListSize(powI + 1);
        while(powI >= lastCnt[level].size()){
            lastCnt[level].push_back(cnt[level]);
        }
        return RangeSet[level].bucketList[powI].size();
    }

    void coverWithRange(int level, int rangeId, int id, int pw){
        BucketElement *new_ele = new BucketElement(rangeId, id, cnt[level]);
        RangeSet[level].InsertNewELement(pw, new_ele);
        while(pw >= lastCnt[level].size()){
            lastCnt[level].push_back(cnt[level]);
        }
    }

    std::pair<int,int> removeRange(int level, int powI, int id){
//        while(RangeSet[level].bucketList[level].size() < lastCnt[level].size()) lastCnt[level].pop_back();
        auto ret = RangeSet[level].DeleteElement(powI, id);
        return ret;
    }

    void increaseLevel() {
        maxLevel++;
        forward.push_back(nullptr);
        RangeSet.emplace_back();
        lastCnt.emplace_back();
        cnt.push_back(0);
    }

};


#endif //DSLRT_SKIPLISTNODE_HPP
