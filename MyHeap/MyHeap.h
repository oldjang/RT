///*
// * MyHeap.h
// *
// *  Created on: 21 Sep, 2015
// *      Author: jhgan
// */
//
//#ifndef MYHEAP_H_
//#define MYHEAP_H_
//
//#include "../headers.h"
//#include "../MyVector/MyVector.h"
//
//#include <vector>
//using namespace std;
//
///*
// *  The structure of the element in the min heap.
// */
//struct HeapElement {
//	/*
//	 *  The index in the heap of this element.
//	 */
//	int heapIndex;
//
//	/*
//	 *  The index of the query in the list of all queries.
//	 */
//	int queryIndex;
//
//	/*
//	 *  The next emit count of points.
//	 */
//	int emitCnt;
//
//	HeapElement(int queryIndex) {
//		this->queryIndex = queryIndex;
//		this->emitCnt = 0;
//		this->heapIndex = 0;
//
//		// Increase the number of heap elements created so far.
//		HENUM++;
//	}
//
//	HeapElement() {
//		this->queryIndex = 0;
//		this->emitCnt = 0;
//		this->heapIndex = 0;
//		// Increases the number of heap elements created so far.
//		HENUM++;
//	}
//
//	~HeapElement() {
//		HENUM--;
//	}
//
//};
//
///*
// *  The implementation of my heap.
// */
//class MyHeap {
//
//public:
//
//	/*
//	 *  Constructor.
//	 *  Parameter List:
//	 *  	idList:				the list of indices of the heap elements.
//	 *  	heapElementList:	the list of all heap elements.
//	 */
//	MyHeap(MyVector<int>& idList, MyVector<HeapElement>& _heapElementList);
//
////	MyHeap(MyVector<HeapElement>& _heapElementList);
//
//	MyHeap();
//
//	virtual ~MyHeap();
//
//	/*
//	 *  The list of heap elements.
//	 */
////	vector<HeapElement*> elementList;
//	/*
//	 *  The list of indices of the heap elements.
//	 */
//	MyVector<int> elementList;
//
//	/*
//	 *  The pointer of the list of all heap elements.
//	 */
////	MyVector<HeapElement>* _heapElementList;
//	/*
//	 *  Maintain the heap by push down the element of the given index to correct place.
//	 *  Parameter List:
//	 *  	index:	the index of the element in the list.
//	 */
//	void BubbleDown(int index, MyVector<HeapElement>& _heapElementList);
//
//	/*
//	 *  Maintain the heap by push up the element of the given index to correct place.
//	 *  Parameter List:
//	 *  	index: 	the index of the element in the list.
//	 */
//	void BubbleUp(int index, MyVector<HeapElement>& _heapElementList);
//
//	/*
//	 *  Make the list of elements a heap.
//	 */
//	void Heapify(MyVector<HeapElement>& _heapElementList);
//
//	/*
//	 *  Shrink the element list to fit the size.
//	 */
//	void ShrinkToFit();
//
//	/*
//	 *  Return the size of this heap.
//	 */
//	int GetSize();
//
//	/*
//	 *  Set the element list by a given list.
//	 *  Parameter List:
//	 *   	list:	the given element list.
//	 */
////	void SetElementList(vector<HeapElement*>& list);
//	void SetElementList(MyVector<int>& idList,
//			MyVector<HeapElement>& _heapElementList);
//
//	/*
//	 *  Insert a new element into heap.
//	 *  Parameter List:
//	 *  	newElement:	 the given new element.
//	 */
//	void Insert(HeapElement& newElement,
//			MyVector<HeapElement>& _heapElementList);
//
//	/*
//	 *  Add a new element to the element list but do not maintain the heap.
//	 *  Parameter List:
//	 *  	newElement:	 the given new element.
//	 */
//	void Add(HeapElement& newElement, MyVector<HeapElement>& _heapElementList);
//
//	/*
//	 *  Return the top value of the heap.
//	 */
//	HeapElement* GetTop(MyVector<HeapElement>& _heapElementList);
//
//	/*
//	 *  Delete the top value from the heap.
//	 */
//	void DeleteTop(MyVector<HeapElement>& _heapElementList);
//
//	/*
//	 *  Release space of this heap.
//	 */
//	void ReleaseSpace();
//
//	/*
//	 *  Modify the key value of the element in the specified index.
//	 *  Parameter List:
//	 *  	index:		the specified index.
//	 *  	key:		the new key value.
//	 */
//	virtual void ModifyKey(int index, int key,
//			MyVector<HeapElement>& _heapElementList) = 0;
//
//	/*
//	 *  Delete the element with the given index from the heap.
//	 *  Parameter List:
//	 *  	index:		the given index of the target element
//	 */
//	virtual void Delete(int index, MyVector<HeapElement>& _heapElementList) = 0;
//
//	/*
//	 *  The compare function for the heap elements.
//	 */
//	virtual bool HeapElementCompare(HeapElement& e1, HeapElement& e2) = 0;
//};
//
//#endif /* MYHEAP_H_ */
