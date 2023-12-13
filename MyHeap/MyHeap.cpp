///*
// * MyHeap.cpp
// *
// *  Created on: 21 Sep, 2015
// *      Author: jhgan
// */
//
//#include "MyHeap.h"
//
//MyHeap::~MyHeap() {
//	// TODO Auto-generated destructor stub
//}
//
////MyHeap::MyHeap(vector<HeapElement*>& vector) {
////	this->SetElementList(vector);
////	Heapify();
////}
//
//MyHeap::MyHeap(MyVector<int>& idList, MyVector<HeapElement>& _heapElementList) {
////	this->_heapElementList = &(_heapElementList);
//	this->SetElementList(idList, _heapElementList);
//}
//
////MyHeap::MyHeap(MyVector<HeapElement>& _heapElementList) {
//////	this->_heapElementList = &(_heapElementList);
////}
//
//MyHeap::MyHeap() {
////	this->_heapElementList = NULL;
//}
//
//void MyHeap::ReleaseSpace() {
//	this->elementList.release_space();
//
////	int num = this->elementList.size();
////	if (num > 0) {
////		HeapElement* temp = NULL;
////		for (int i = 0; i < num; ++i) {
////			temp = this->elementList[i];
////			if (temp != NULL) {
////				temp->query = NULL;
////				// Release the space of this heap element.
////				delete temp;
////				this->elementList[i] = NULL;
////
////				// Decrease the number of HeapElement by 1.
////				HENUM--;
////			}
////		}
////
////		vector<HeapElement*> emptyList;
////		this->elementList.swap(emptyList);
////		emptyList.clear();
////	}
//}
//
//int MyHeap::GetSize() {
//	return this->elementList.size();
//}
//
////void MyHeap::SetElementList(vector<HeapElement*>& list) {
////	this->elementList.swap(list);
////	int length = elementList.size();
////	for (int i = 0; i < length; ++i) {
////		this->elementList[i]->heapIndex = i;
////	}
////}
//
//void MyHeap::SetElementList(MyVector<int>& idList,
//		MyVector<HeapElement>& _heapElementList) {
//	this->elementList.swap(idList);
//	int length = elementList.size();
////	MyVector<HeapElement>& tempList = *(this->_heapElementList);
//	for (int i = 0; i < length; ++i) {
//		_heapElementList[this->elementList[i]].heapIndex = i;
//	}
//}
//
//void MyHeap::Heapify(MyVector<HeapElement>& _heapElementList) {
//	int length = elementList.size();
//	for (int i = length - 1; i >= 0; --i) {
//		BubbleDown(i, _heapElementList);
//	}
//}
//
//void MyHeap::ShrinkToFit() {
////	vector<HeapElement*>(this->elementList).swap(this->elementList);
//	this->elementList.shrink_to_fit();
//}
//
//void MyHeap::BubbleDown(int index, MyVector<HeapElement>& _heapElementList) {
//	int length = elementList.size();
//	int leftChildIndex = 2 * index + 1;
//	int rightChildIndex = 2 * index + 2;
//
//	if (leftChildIndex >= length)
//		return; //index is a leaf
//
//	int minIndex = index;
//
////	MyVector<HeapElement>& tempList = *(this->_heapElementList);
//
//	MyVector<HeapElement>& tempList = _heapElementList;
//
//	if (!HeapElementCompare(tempList[elementList[index]],
//			tempList[elementList[leftChildIndex]])) {
//		minIndex = leftChildIndex;
//	}
//
//	if ((rightChildIndex < length)
//			&& (!HeapElementCompare(tempList[elementList[minIndex]],
//					tempList[elementList[rightChildIndex]]))) {
//		minIndex = rightChildIndex;
//	}
//
//	if (minIndex != index) {
//		// need to swap
//
//		// swap the index values in the heap.
//		tempList[elementList[index]].heapIndex = minIndex;
//		tempList[elementList[minIndex]].heapIndex = index;
//
//		// swap the index value in the list of all heap elements.
//		int temp = elementList[index];
//		elementList[index] = elementList[minIndex];
//		elementList[minIndex] = temp;
//		BubbleDown(minIndex, _heapElementList);
//	}
//}
//
//void MyHeap::BubbleUp(int index, MyVector<HeapElement>& _heapElementList) {
//	if (index == 0)
//		return;
//
//	int parentIndex = (index - 1) / 2;
//
////	MyVector<HeapElement>& tempList = *(this->_heapElementList);
//
//	MyVector<HeapElement>& tempList = _heapElementList;
//
//	if (!HeapElementCompare(tempList[elementList[parentIndex]],
//			tempList[elementList[index]])) {
//		// need to swap
//
//		// swap the index values in the heap.
//		tempList[elementList[index]].heapIndex = parentIndex;
//		tempList[elementList[parentIndex]].heapIndex = index;
//
//		// swap the index value in the list of all heap elements.
//		int temp = elementList[parentIndex];
//		elementList[parentIndex] = elementList[index];
//		elementList[index] = temp;
//		BubbleUp(parentIndex, tempList);
//	}
//}
//
//void MyHeap::Insert(HeapElement& newElement,
//		MyVector<HeapElement>& _heapElementList) {
//	int length = elementList.size();
//	newElement.heapIndex = length;
//
////	MyVector<HeapElement>& tempList = *(this->_heapElementList);
//	MyVector<HeapElement>& tempList = _heapElementList;
//	int index = tempList.size();
//	tempList.push_back(newElement);
//	elementList.push_back(index);
//
//	BubbleUp(length, tempList);
//}
//
//void MyHeap::Add(HeapElement& newElement,
//		MyVector<HeapElement>& _heapElementList) {
//	int length = elementList.size();
//	newElement.heapIndex = length;
//
////	MyVector<HeapElement>& tempList = *(this->_heapElementList);
//	MyVector<HeapElement>& tempList = _heapElementList;
//	int index = tempList.size();
//	tempList.push_back(newElement);
//	elementList.push_back(index);
//
//	// In this function, we do not need to maintain the heap.
////	BubbleUp(length);
//}
//
//HeapElement* MyHeap::GetTop(MyVector<HeapElement>& _heapElementList) {
//	if (this->elementList.size() > 0)
//		return &(_heapElementList[elementList[0]]);
//	else {
//		return NULL;
//	}
//}
//
//void MyHeap::DeleteTop(MyVector<HeapElement>& _heapElementList) {
////	this->Delete(0);
//
//	int length = elementList.size();
//
//	if (length == 0) {
//		return;
//	}
//
////	MyVector<HeapElement>& tempList = *(this->_heapElementList);
//	MyVector<HeapElement>& tempList = _heapElementList;
//
//	int temp = elementList[0];
//	elementList[0] = elementList[length - 1];
//	elementList[length - 1] = temp;
//
//// reset the index value of the last element after swap.
//	tempList[elementList[0]].heapIndex = 0;
//
////	// release the space
////	delete (elementList[length - 1]);
////	elementList[length - 1] = NULL;
//
//// If we want to release the space of the deleted heap element,
//// we need to do more things.
///// TODO
//
//// Decrease the number of Heap Element by 1.
////	HENUM--;
//
//	elementList.pop_back();
//	if (elementList.size() < elementList.capacity() * 0.5) {
//		elementList.shrink_to_fit();
//	}
//
//	BubbleDown(0, tempList);
//}
