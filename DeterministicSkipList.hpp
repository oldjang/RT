#ifndef DSLRT_DETERMINISTICSKIPLIST_HPP
#define DSLRT_DETERMINISTICSKIPLIST_HPP

#include "SkipListNode.hpp"
#include "global.hpp"

class DeterministicSkipList {
private:
    SkipListNode *head, *tail;
    int maxLevel = 0;
    int nodeCnt = 0;
    int erasedNode = 0;

    static void addRange(int rangeId, SkipListNode *node, int level, int powI){
        Range *range = rangeList[rangeId];
        int idInRange = range->getSize(level);
        int idInNode = node->getSize(level, powI);
        range->insert(level, node, idInNode);
        node->coverWithRange(level, rangeId, idInRange, powI);
    }

    void findCanonicalNode(SkipListNode *LNode, SkipListNode *RNode, Range *range, int rangeId) {
        int level = 0;
        SkipListNode *node = LNode;
        std::vector<std::pair<int, SkipListNode*> > canonicalNodes;
        while (node->forward[level]->key <= range->R) {
            while (level < node->maxLevel && node->forward[level + 1] != nullptr &&
                   range->isCovered(node->key, node->forward[level + 1]->key))
                level++;
            canonicalNodes.emplace_back(level, node);
            node = node->forward[level];
        }
        while (node->key < range->R) {
            while (level > 0 && (node->forward[level] ==
                                 nullptr || !range->isCovered(node->key, node->forward[level]->key)))
                level = level - 1;
            canonicalNodes.emplace_back(level, node);
            node = node->forward[level];
        }
        int length = canonicalNodes.size();
        auto tmp = range->getLogWithLength(length);
        sort(canonicalNodes.begin(), canonicalNodes.end());
        range->setNowI(tmp.first.first);

        range->setSplitLevel(-1);
        for(int i = 0; i < length; i++) {
            SkipListNode *nnode = canonicalNodes[i].second;
            int nlevel = canonicalNodes[i].first;
            if(tmp.first.second > 0 || (i > 0 && nlevel == range->splitLevel)) {
                range->setSplitLevel(nlevel);
                addRange(rangeId, nnode, nlevel, tmp.first.first);
                tmp.first.second --;
            }
            else{
                addRange(rangeId, nnode, nlevel, tmp.second.first);
            }
        }
    }

    void link(SkipListNode *preNode, SkipListNode *nextNode, int level){
        nextNode->forward[level] = preNode->forward[level];
        for(int i = 0;i < preNode->RangeSet[level].listSize(); i++){
            int sz = preNode->RangeSet[level].bucketList[i].size();
            std::vector<std::pair<int,int> >tmp(sz);
            for(int k = 0; k < sz; k++){
                int rangeId = preNode->RangeSet[level].bucketList[i][k]->getQueryIndex();
                int preCnt = preNode->cnt[level] - preNode->RangeSet[level].bucketList[i][k]->getLastCnt();
                preNode->RangeSet[level].bucketList[i][k]->setLastCnt(preNode->cnt[level]);
                tmp[k] = {rangeId,preCnt};
            }
            for(int k = 0; k < sz; k++) {
                int rangeId = tmp[k].first;
                int preCnt = tmp[k].second;
                int flag = sendNotice(rangeList[rangeId], preCnt);
                if(flag!=1) {
                    addRange(rangeId, nextNode, level, rangeList[rangeId]->getPwInLevel(level));
                    fit(rangeList[rangeId], rangeId);
                }
            }
        }
        preNode->forward[level] = nextNode;
    }

    void increaseSkipListLevel() {
        maxLevel++;
        head->forward.push_back(tail);
        head->RangeSet.emplace_back();
        head->lastCnt.emplace_back();
        head->cnt.push_back(0);
    }

    void removeRangeInNode(SkipListNode *node, int level, int powI, int id){
        auto changeId = node->removeRange(level, powI, id);
        if (changeId.first != -1) {
            rangeList[changeId.first]->changId(level, changeId.second, id);
        }
    }

    int clearRangeInSL(Range *range){
        for(int i = 0; i <= range->maxLevel; i++){
            for(int j = 0; j < range->storedIn[i].size(); j++){
                SkipListNode *node = range->storedIn[i][j].first;
                range->roundCnt += range->storedIn[i][j].first->cnt[i] - range->storedIn[i][j].first->RangeSet[i].bucketList[range->getPwInLevel(i)][range->storedIn[i][j].second]->getLastCnt();
                removeRangeInNode(node, i, range->getPwInLevel(i), range->storedIn[i][j].second);
            }
        }
        if(range->roundCnt >= range->threshold) return 1; else return 2;
    }

    int sendNotice(Range *range, int cntIncrease){
        int flag = range->receiveNotification(cntIncrease);
        if(flag != 0){
            flag = clearRangeInSL(range);
        }
        if(flag == 2){
            range->enterNextRound();
            for(int i = 0; i <= range->maxLevel; i++){
                for(int j = 0; j < range->storedIn[i].size(); j++){
                    SkipListNode *node = range->storedIn[i][j].first;
                    int powI = range->getPwInLevel(i);
                    int idInRange = j;
                    int idInNode = node->getSize(i, powI);
                    node->coverWithRange(i, range->aId, idInRange, powI);
                    range->storedIn[i][j].second = idInNode;
                    range->thresholdSum += 1 << range->getPwInLevel(i);
                }
            }
        }
        return flag;
    }

    void fit(Range *fitRange, int rangeId){
        std::vector<std::pair<std::pair<SkipListNode*, int>, std::pair<std::pair<int,int> ,std::pair<int,int>> > > changeSet;
        fitRange->fit(changeSet);
        int cnt = 0;
        for(int i = 0; i < changeSet.size(); i++){
            SkipListNode* changeNode = changeSet[i].first.first;
            int preId = changeSet[i].first.second;
            int preI = changeSet[i].second.first.first;
            int nxtI = changeSet[i].second.first.second;
            int level = changeSet[i].second.second.first;
            int idInRange = changeSet[i].second.second.second;
            cnt+=changeNode->cnt[level] - changeNode->RangeSet[level].bucketList[preI][preId]->getLastCnt();
            auto changeId = changeNode->removeRange(level, preI, preId);
            if (changeId.first != -1) {
                rangeList[changeId.first]->changId(level, changeId.second, preId);
            }
            int idInNode = changeNode->getSize(level, nxtI);
            changeNode->coverWithRange(level, rangeId, idInRange, nxtI);
            fitRange->changId(level, idInRange, idInNode);
        }
        sendNotice(fitRange,cnt);
    }

    void checkAndPromoteRange(SkipListNode *higherNode, SkipListNode *checkNode, int checkLevel) {
        double higherL = higherNode->key;
        double higherR = higherNode->forward[checkLevel + 1]->key;
        for(int i = 0; i < checkNode->RangeSet[checkLevel].listSize(); i++){
            std::vector<std::pair<int,int> > tmp;
            for(int k = 0; k < checkNode->RangeSet[checkLevel].bucketList[i].size(); k++) {
                Range *checkRange = rangeList[checkNode->RangeSet[checkLevel].bucketList[i][k]->getQueryIndex()];
                if (checkRange->isCovered(higherL, higherR)) {
                    std::vector<std::pair<std::pair<SkipListNode *, int>,int > > eraseSet;
                    int rangeId = checkNode->RangeSet[checkLevel].bucketList[i][k]->getQueryIndex();
                    int cntIncrease = checkRange->getAndDelete(eraseSet, checkLevel, higherL, higherR);
                    addRange(rangeId, higherNode, checkLevel + 1, rangeList[rangeId]->getPwInLevel(checkLevel + 1));
                    for (int j = 0; j < eraseSet.size(); j++) {
                        if (eraseSet[j].first.first == checkNode && eraseSet[j].second == i) k--;
                        auto changeId = eraseSet[j].first.first->removeRange(checkLevel, eraseSet[j].second, eraseSet[j].first.second);
                        if (changeId.first != -1) {
                            rangeList[changeId.first]->changId(checkLevel, changeId.second, eraseSet[j].first.second);
                        }
                    }
                    tmp.push_back({rangeId,cntIncrease});
                }
            }
            for(int k = 0; k< tmp.size();k++){
                int rangeId = tmp[k].first;
                int cntIncrease = tmp[k].second;
                if(sendNotice(rangeList[rangeId], cntIncrease) == 0)
                    fit(rangeList[rangeId], rangeId);
            }
        }
    }


    void promoteNode(SkipListNode *higherPreNode, SkipListNode *preNode, SkipListNode *promotedNode, int level){
        promotedNode->increaseLevel();
        link(higherPreNode, promotedNode, level);
        checkAndPromoteRange(higherPreNode, higherPreNode, level - 1);
        checkAndPromoteRange(promotedNode, promotedNode, level - 1);
    }

    SkipListNode *insertNode(double key){
        SkipListNode *newNode = new SkipListNode(key, 0);
        std::vector<SkipListNode* > lastNodeInLevel(maxLevel + 2);
        SkipListNode* preNode = head;
        for(int i = maxLevel; i >= 0; i --){
            while(preNode->forward[i]->key <= key) preNode = preNode->forward[i];
            lastNodeInLevel[i] = preNode;
        }
        lastNodeInLevel[maxLevel + 1] = head;
        if(preNode->key == key) return preNode;
        link(preNode, newNode, 0);
        for(int i = 1; i <= maxLevel; i++){
            if(lastNodeInLevel[i]->forward[i - 1] != tail && lastNodeInLevel[i]->forward[i - 1]->maxLevel == i - 1){
                SkipListNode *nextNode1 = lastNodeInLevel[i]->forward[i - 1];
                if(nextNode1->forward[i - 1] != tail && nextNode1->forward[i - 1]->maxLevel == i - 1){
                    SkipListNode *nextNode2 = nextNode1->forward[i - 1];
                    if(nextNode2->forward[i - 1] != tail && nextNode2->forward[i - 1]->maxLevel == i - 1){
                        promoteNode(lastNodeInLevel[i], nextNode1, nextNode2, i);
                        if(i == maxLevel)  increaseSkipListLevel();
                    } else break;
                } else break;
            } else break;
        }
        return newNode;
    }
public:
    void cover(double L, double R, int intervalId,int threshold) {
        SkipListNode *LNode = insertNode(L);
        SkipListNode *RNode = insertNode(R);
        nodeCnt += 2;
        nodeCnt += 2;
        findCanonicalNode(LNode, RNode, rangeList[intervalId], intervalId);
    }

    std::vector<int> increase(int key, int cntIncrease) {
        SkipListNode *p = head;
        std::vector<int> result(0);
        for (int i = maxLevel; i >= 0; --i) {
            while (p->forward[i]->key <= key) {
                p = p->forward[i];
            }
            if(p==head)continue;
            p->cnt[i] += cntIncrease;
            for(int powI = 0 ; powI < p->RangeSet[i].bucketList.size() ; powI++){
                if(p->cnt[i] - p->lastCnt[i][powI] >= (1 << powI)){
                    p->lastCnt[i][powI] = p->cnt[i];
                    std::vector<std::pair<int,int> > changed;
                    for(int j = 0; j < p->RangeSet[i].bucketList[powI].size(); j++){
                        auto ele = p->RangeSet[i].bucketList[powI][j];
                        int eleIncrease = p->cnt[i] - ele->getLastCnt();
                        ele->setLastCnt(p->cnt[i]);
                        int qId = ele->getQueryIndex();
//                        if(qId == 3)
//                            puts("");
                        changed.push_back({qId,eleIncrease});
                    }
                    for(int j = 0; j < changed.size(); j++){
//                        auto ele = p->RangeSet[i].bucketList[powI][j];
//                        int eleIncrease = p->cnt[i] - ele->getLastCnt();
//                        ele->setLastCnt(p->cnt[i]);
//                        int qId = ele->getQueryIndex();
                        int qId = changed[j].first;
                        int eleIncrease = changed[j].second;
                        int flag = sendNotice(rangeList[qId], eleIncrease);
                        if(flag == 1){
                            result.push_back(rangeList[qId]->id);
                            rangeList[qId]->erase = true;
//                            nodeCnt -= 2;
                            erasedNode += 2;
                        }
                    }
                }
                else break;
            }
        }
        std::sort(result.begin(), result.end());
        if(erasedNode * 2 >= nodeCnt){
            rebuild();
        }
        return result;
    }

    void rebuild(){
        std::vector<int> inc(rangeList.size());
        SkipListNode *preP = nullptr;
        for(auto p = head; p!=tail;p = p->forward[0]){
            if(preP !=nullptr) delete preP;
            for(int i = 0; i <= p->maxLevel; i++){
                for(int j = 0; j < p->RangeSet[i].listSize(); j++){
                    for(int k = 0; k < p->RangeSet[i].bucketList[j].size(); k++){
                        inc[p->RangeSet[i].bucketList[j][k]->getQueryIndex()]+=p->cnt[i]-p->RangeSet[i].bucketList[j][k]->getLastCnt();
                        delete p->RangeSet[i].bucketList[j][k];
                    }
                }
            }
            preP = p;
        }
        std::vector<std::pair<std::pair<double,double>, std::pair<int,int>> > querys;
        for(int i = 0 ;i < rangeList.size(); i++){
            if(!rangeList[i]->erase){
                int th = rangeList[i]->threshold - rangeList[i]->roundCnt - inc[i];
                querys.push_back({{rangeList[i]->L,rangeList[i]->R},{rangeList[i]->id,th}});
            }
            delete(rangeList[i]);
        }
        rangeList.clear();
        buildThroughIntervals(querys);
    }
    void buildThroughIntervals(std::vector<std::pair<std::pair<double,double>, std::pair<int,int> > > querys){
        std::vector<std::pair<double,int> >keyList;
        for(int i = 0; i < querys.size(); i++){
            keyList.push_back({querys[i].first.first,i*2});
            keyList.push_back({querys[i].first.second,i*2+1});
        }
        sort(keyList.begin(),keyList.end());
        int n = keyList.size();
        int level = 2;
        while(n>=(1<<level)) level++;
        head = new SkipListNode(-2e9, level);
        tail = new SkipListNode(2e9, level);
        std::vector<SkipListNode*>preNode(level + 1,head);
        std::vector<SkipListNode*>actualNode(n);
        nodeCnt = n;
        erasedNode = 0;
        for(int i = 0; i < n; i++){
            int nlevel = 0;
            while((i+1)%(1<<(nlevel+1))==0)
                nlevel++;
            SkipListNode *newNode = new SkipListNode(keyList[i].first, nlevel);
            for(int j = 0; j <= nlevel;j ++){
                preNode[j]->forward[j] = newNode;
                preNode[j] = newNode;
            }
            actualNode[keyList[i].second] = newNode;
        }

        for(int i = 0;i <=level;i++) preNode[i]->forward[i] = tail;

        for(int i = 0 ; i < querys.size(); i++){
            Range *range = new Range(querys[i].first.first, querys[i].first.second, querys[i].second.first, querys[i].second.second, i);
            rangeList.push_back(range);
            findCanonicalNode(actualNode[i*2],actualNode[i*2+1],range,i);
        }
        maxLevel = level;
    }

    DeterministicSkipList(int level):maxLevel(level){
        head = new SkipListNode(-2e9, level);
        tail = new SkipListNode(2e9, level);
        for(int i = 0; i <= level; i++) head->forward[i] = tail;
    }
};


#endif //DSLRT_DETERMINISTICSKIPLIST_HPP
