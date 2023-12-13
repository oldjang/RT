///*
// * MaxHeap.cpp
// *
// *  Created on: 21 Sep, 2015
// *      Author: jhgan
// */
//
//#include "MaxHeap.h"
//
//MaxHeap::MaxHeap(MyVector<int>& idList, MyVector<HeapElement>& _heapElementList) :
//		MyHeap(idList, _heapElementList) {
//
//}
//
//MaxHeap::MaxHeap(MyVector<HeapElement>& _heapElementList) :
//		MyHeap(_heapElementList) {
//}
//
//MaxHeap::~MaxHeap() {
//	// TODO Auto-generated destructor stub
//}
//
//bool MaxHeap::HeapElementCompare(HeapElement& e1, HeapElement& e2) {
//	return this->HeapElement_Greater(e1, e2);
//}
//
//bool MaxHeap::HeapElement_Greater(HeapElement& e1, HeapElement& e2) {
//	return e1.emitCnt >= e2.emitCnt;
//}
//
//void MaxHeap::Delete(int index) {
//	int length = elementList.size();
//
//	if (length - 1 < index) {
//		// If the index is even larger than the last index of the element list,
//		// then return directly.
//		return;
//	}
//
//	HeapElement* temp = GetTop();
//	if (temp == NULL)
//		return;
//
//	// Set the emitCnt of the target element to be larger than the max of this heap.
//	(*(this->_heapElementList))[elementList[index]].emitCnt = temp->emitCnt + 1;
//	// Push this element to the top of this heap.
//	BubbleUp(index);
//	// Just delete the min of this heap.
//	DeleteTop();
//
////	HeapElement* temp = GetTop();
////	if (temp == NULL)
////		return;
////
////	// Set the emitCnt of the target element to be larger than the max of this heap.
////	elementList[index]->emitCnt = temp->emitCnt + 1;
////	// Push this element to the top of this heap.
////	BubbleUp(index);
////	// Just delete the max of this heap.
////	DeleteTop();
//}
//
///*
// *  Modify the key value of the element in the specified index.
// *  Parameter List:
// *  	index:		the specified index.
// *  	key:		the new key value.
// */
//void MaxHeap::ModifyKey(int index, int key) {
//	HeapElement& he = (*(this->_heapElementList))[elementList[index]];
//	int old = he.emitCnt;
//	he.emitCnt = key;
//
//	if (old != key) {
//		if (old < key) {
//			BubbleUp(index);
//		} else {
//			BubbleDown(index);
//		}
//	}
//}
