#ifndef DSLRT_BUCKETELEMENT_HPP
#define DSLRT_BUCKETELEMENT_HPP

#include <cstdlib>
#include <cstring>
#include <cstdio>
class BucketElement {
    /*
	 *  Here we use 14 bytes to store all the data member of HeapElement.
	 *  In the first 2 bytes, we store indexInQuery.
	 *  In the second 8 bytes, we store queryStartIndex.
	 *  In the last 4 bytes, we store emitCnt.
	 */
    char data[14];
    short indexInq;
    long long queryInd;
    int lastCnt;

public:

    inline void setIndexInQuery(short indexInQuery) {
        indexInq = indexInQuery;
        *((short*) (data)) = indexInQuery;
    }

    inline int getIndexInQuery() {
        return *((short*) (data));

    }

    inline void setQueryIndex(long long queryIndex) {
        queryInd = queryIndex;
        *((long long*) (data + 2)) = queryIndex;
    }

    inline long long getQueryIndex() {
        return *((long long *) (data + 2));
    }

    inline void setLastCnt(int emitCnt) {
        lastCnt = emitCnt;
        *((int*) (data + 10)) = emitCnt;
    }

    inline int getLastCnt() {
        return *((int*) (data + 10));
    }

    inline void swap(BucketElement* e) {
        char temp[14];
        memcpy(temp, this->data, 14);
        memcpy(this->data, e->data, 14);
        memcpy(e->data, temp, 14);


    }
    inline void ValueCopy(BucketElement *e) {
        indexInq = e->indexInq;
        queryInd =e->queryInd;
        lastCnt = e->lastCnt;
        memcpy(data, e->data, 14);
    }

    BucketElement(int QueryIndex, int IndexInQuery, int lastCnt){
        setIndexInQuery(IndexInQuery);
        setQueryIndex(QueryIndex);
        setLastCnt(lastCnt);
    }
};


#endif //DSLRT_BUCKETELEMENT_HPP
