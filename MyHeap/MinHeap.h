/*
 * MinHeap.h
 *
 *  Created on: 7 Sep, 2015
 *      Author: jhgan
 */

#ifndef MINHEAP_H_
#define MINHEAP_H_

#include "../headers.h"
#include "../MyVector/MyVector.h"
#include "../MyVector/MyVectorInt.h"
//#include "MyHeap.h"
#include "../Query.h"

//#include <vector>
//using namespace std;

struct HeapElement {
//	/*
//	 *  Here we use 10 bytes to store all the data member of HeapElement.
//	 *  In the first 3 bytes, we store queryIndex.
//	 *  In the second 3 bytes, we store indexInQuery.
//	 *  In the last 4 bytes, we store emitCnt.
//	 */

	/*
	 *  Here we use 10 bytes to store all the data member of HeapElement.
	 *  In the first 2 bytes, we store indexInQuery.
	 *  In the second 4 bytes, we store queryStartIndex.
	 *  In the last 4 bytes, we store emitCnt.
	 */
	char data[10];

	HeapElement(int indexInQuery, int queryStartIndex) {
		this->setIndexInQuery(indexInQuery);
		this->setQueryStartIndex(queryStartIndex);
		this->setEmitCnt(0);

		// Increase the number of heap elements created so far.
		HENUM++;
	}

	HeapElement() {
		this->setIndexInQuery(0);
		this->setQueryStartIndex(0);
		this->setEmitCnt(0);

		// Increases the number of heap elements created so far.
		HENUM++;
	}

	~HeapElement() {
		HENUM--;
	}

	inline void setIndexInQuery(short indexInQuery) {
//		memcpy(data + 3, (char*) &indexInQuery, 3);
		*((short*) (data)) = indexInQuery;
	}

	inline int getIndexInQuery() {
		return *((short*) (data));
//		int rtn = 0;
//		memcpy((char*) &rtn, data + 3, 3);
//
//		//		char* ptr = (char*) &rtn;
//		//		ptr[0] = this->data[3];
//		//		ptr[1] = this->data[4];
//		//		ptr[2] = this->data[5];
//
//		return rtn;
	}

	inline void setQueryStartIndex(int queryStartIndex) {
//		memcpy(data, (char*) &queryIndex, 3);
		*((int*) (data + 2)) = queryStartIndex;
	}

	inline int getQueryStartIndex() {
		return *((int*) (data + 2));

//		int rtn = 0;
//		memcpy((char*) &rtn, data, 3);
//
////		char* ptr = (char*) &rtn;
////		ptr[0] = this->data[0];
////		ptr[1] = this->data[1];
////		ptr[2] = this->data[2];
//
//		return rtn;
	}

	inline void setEmitCnt(int emitCnt) {
		*((int*) (data + 6)) = emitCnt;
	}

	inline int getEmitCnt() {
		return *((int*) (data + 6));
	}

	/*
	 *  Copy the data of the given heap element to this element.
	 */
	inline void ValueCopy(HeapElement& e) {
		memcpy(data, e.data, 10);
	}

	/*
	 *  Swap the data of this heap element with that of the given heap element.
	 */
	inline void swap(HeapElement& e) {
		char temp[10];
		memcpy(temp, this->data, 10);
		memcpy(this->data, e.data, 10);
		memcpy(e.data, temp, 10);

//		char temp;
//		for (int i = 0; i < 10; ++i) {
//			temp = this->data[i];
//			this->data[i] = e.data[i];
//			e.data[i] = temp;
//		}

//		int temp = this->getEmitCnt();
//		this->setEmitCnt(e.getEmitCnt());
//		e.setEmitCnt(temp);
//
//		temp = this->getQueryIndex();
//		this->setQueryIndex(e.getQueryIndex());
//		e.setQueryIndex(temp);
//
//		temp = this->getIndexInQuery()	;
//		this->setIndexInQuery(e.getIndexInQuery());
//		e.setIndexInQuery(temp);
	}

};

/*
 *  The implementation of min heap.
 */
class MinHeap {

public:

//	/*
//	 *  Constructor.
//	 *  Parameter List:
//	 *  	idList:				the list of indices of the heap elements.
//	 *  	heapElementList:	the list of all heap elements.
//	 */
//	MinHeap(MyVector<unsigned int>& idList,
//			MyVector<HeapElement>& _heapElementList);

//	MinHeap(MyVector<HeapElement>& _heapElementList);

	MinHeap();

	~MinHeap();

//	/*
//	 *  The start index of the list of queries which should emit for every point.
//	 */
//	int emitListStart;
//
//	/*
//	 *  The list of indexes of the heap elements.
//	 */
//	MyVectorInt<unsigned int> elementList;

//	MyVector<unsigned int> elementList;

	MyVector<HeapElement> elementList;

	/***************************************************************
	 *  			Min heap operations.
	 ***************************************************************/

	/*
	 *  Maintain the heap by push down the element of the given index to correct place.
	 *  Parameter List:
	 *  	index:	the index of the element in the list.
	 */
	void BubbleDown(int index, MyVector<Query>& _queryList,
			MyVector<int>& _upperElementIndexInHeapList);

	/*
	 *  Maintain the heap by push up the element of the given index to correct place.
	 *  Parameter List:
	 *  	index: 	the index of the element in the list.
	 */
	void BubbleUp(int index, MyVector<Query>& _queryList,
			MyVector<int>& _upperElementIndexInHeapList);

	/*
	 *  Make the list of elements a heap.
	 */
	void Heapify(MyVector<Query>& _queryList,
			MyVector<int>& _upperElementIndexInHeapList);

	/*
	 *  Shrink the element list to fit the size.
	 */
	void ShrinkToFit();

	/*
	 *  Return the size of this heap.
	 */
	int GetSize();

//	/*
//	 *  Set the element list by a given list.
//	 *  Parameter List:
//	 *   	list:	the given element list.
//	 */
////	void SetElementList(vector<HeapElement*>& list);
//	void SetElementList(MyVector<unsigned int>& idList,
//			MyVector<HeapElement>& _heapElementList);

	/*
	 *  Insert a new element into heap.
	 *  Parameter List:
	 *  	newElement:	 the given new element.
	 */
	void Insert(HeapElement& newElement, MyVector<Query>& _queryList,
			MyVector<int>& _upperElementIndexInHeapList);

	/*
	 *  Add a new element to the element list but do not maintain the heap.
	 *  Parameter List:
	 *  	newElement:	 the given new element.
	 */
	void Add(HeapElement& newElement);

	/*
	 *  Return the top value of the heap.
	 */
	HeapElement* GetTop();

	/*
	 *  Delete the top value from the heap.
	 */
	void DeleteTop(MyVector<Query>& _queryList,
			MyVector<int>& _upperElementIndexInHeapList);

	/*
	 *  Release space of this heap.
	 */
	void ReleaseSpace();

	/*
	 *  Delete the element with the given index from the heap.
	 *  Parameter List:
	 *  	index:		the given index of the target element
	 */
	void Delete(int index, MyVector<Query>& _queryList,
			MyVector<int>& _upperElementIndexInHeapList);

	/*
	 *  Modify the key value of the element in the specified index.
	 *  Parameter List:
	 *  	index:		the specified index.
	 *  	key:		the new key value.
	 */
	void ModifyKey(int index, int key, MyVector<Query>& _queryList,
			MyVector<int>& _upperElementIndexInHeapList);

private:
//	bool HeapElement_NoGreater(HeapElement& e1, HeapElement& e2);

	inline bool HeapElement_Greater(HeapElement& e1, HeapElement& e2) {
		return e1.getEmitCnt() > e2.getEmitCnt();
	}
};

#endif /* MINHEAP_H_ */
