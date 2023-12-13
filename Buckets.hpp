#ifndef DSLRT_BUCKETS_HPP
#define DSLRT_BUCKETS_HPP

#include <vector>
#include "BucketElement.hpp"


class Buckets {
private:
    std::vector<BucketElement* > EmptyElement;
public:
    std::vector<std::vector<BucketElement* > > bucketList;
    Buckets(){
    }

    ~Buckets() {
    }

    int listSize(){
        return bucketList.size();
    }
//    int sizeByIndex(int i);

    void extendListSize(int size){
        while(bucketList.size() < size){
            bucketList.push_back(EmptyElement);
        }
    }

//    void shrinkToFit();

    int InsertNewELement(int i, BucketElement* e){
        extendListSize(i + 1);
        bucketList[i].push_back(e);
        return bucketList[i].size();
    }

//    bool CheckEmptyByIndex(int index);
    std::pair<int,int> DeleteElement(int bucketIndex,int elementIndex){
        BucketElement *old_e = bucketList[bucketIndex][elementIndex];
        std::vector<BucketElement* > &bList = this->bucketList[bucketIndex];
        if (bList.size() == elementIndex + 1) {
            delete bList[elementIndex];
            bList.pop_back();
            //this->shrinkToFit();
            return {-1,-1};
        }
        int old_size = bList.size();
        old_e->ValueCopy(bList[old_size - 1]);
        delete bList[old_size - 1];
        bList.pop_back();
        return {old_e->getQueryIndex(),old_e->getIndexInQuery()};
    }
//    void ReleaseSpace();
};


#endif //DSLRT_BUCKETS_HPP
