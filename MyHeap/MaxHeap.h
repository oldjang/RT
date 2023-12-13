///*
// * MaxHeap.h
// *
// *  Created on: 21 Sep, 2015
// *      Author: jhgan
// */
//
//#ifndef MAXHEAP_H_
//#define MAXHEAP_H_
//
//#include "MyHeap.h"
//
//class MaxHeap: public MyHeap {
//public:
//	/*
//	 *  Constructor.
//	 *  Parameter List:
//	 *  	idList:				the list of indices of the heap elements.
//	 *  	heapElementList:	the list of all heap elements.
//	 */
//	MaxHeap(MyVector<int>& idList, MyVector<HeapElement>& _heapElementList);
//
//	MaxHeap(MyVector<HeapElement>& _heapElementList);
//
//	~MaxHeap();
//
//	/*
//	 *  Delete the element with the given index from the heap.
//	 *  Parameter List:
//	 *  	index:		the given index of the target element
//	 */
//	void Delete(int index);
//
//	/*
//	 *  Modify the key value of the element in the specified index.
//	 *  Parameter List:
//	 *  	index:		the specified index.
//	 *  	key:		the new key value.
//	 */
//	void ModifyKey(int index, int key);
//
//	/*
//	 *  The implementation of the virtual function in class MyHeap.
//	 */
//	bool HeapElementCompare(HeapElement& e1, HeapElement& e2);
//
//private:
//	bool HeapElement_Greater(HeapElement& e1, HeapElement& e2);
//};
//
//#endif /* MAXHEAP_H_ */
