/*
 * MinHeap.cpp
 *
 *  Created on: 7 Sep, 2015
 *      Author: jhgan
 */

//#include "stdafx.h"
#include "MinHeap.h"

MinHeap::~MinHeap() {
	MHNUM--;
}

//MinHeap::MinHeap(MyVector<unsigned int>& idList,
//		MyVector<HeapElement>& _heapElementList) {
//	this->SetElementList(idList, _heapElementList);
////	this->emitListStart = this->elementList.capacity();
//	MHNUM++;
//}

MinHeap::MinHeap() {
//	this->emitListStart = this->elementList.capacity();
	MHNUM++;
}

/***************************************************************
 *  			Min heap operations.
 ***************************************************************/

//bool MinHeap::HeapElement_Greater(HeapElement& e1, HeapElement& e2) {
//	return e1.getEmitCnt() > e2.getEmitCnt();
//}
void MinHeap::Delete(int index, MyVector<Query>& _queryList,
		MyVector<int>& _upperElementIndexInHeapList) {

//	double start = getCurrentTime();

	int length = elementList.size();

	if (length - 1 < index) {
		// If the index is even larger than the last index of the element list,
		// then return directly.
		return;
	}

	if (length - 1 == index) {
		elementList.pop_back();

		if (elementList.size() < elementList.capacity() * 0.5) {
			elementList.shrink_to_fit();
		}
		return;
	}

//	MyVector<HeapElement>& tempList = _heapElementList;

	int oldKey = elementList[index].getEmitCnt();
	int newKey = elementList[length - 1].getEmitCnt();

//	int temp = elementList[index];
//	elementList[index] = elementList[length - 1];
//	elementList[length - 1] = temp;

	// Swap the two heap elements.
	elementList[index].swap(elementList[length - 1]);

	// reset the index value of the last element after swap.
//	tempList[elementList[index]].heapIndex = index;
//	tempList[elementList[index]].setHeapIndex(index);

//	Query& query = _queryList[elementList[index].getQueryIndex()];
//	Query& query =
//			_queryList[_upperElementIndexInHeapList[elementList[index].getQueryStartIndex()]];

//	if (query.threshold != -1) {
//		// The corresponding query is still alive.
//		query.upperElementIndexList[elementList[index].getIndexInQuery()] =
//				index;
//	}

	int startIndex = elementList[index].getQueryStartIndex();
	int indexInQuery = elementList[index].getIndexInQuery();
	_upperElementIndexInHeapList[startIndex + 1 + indexInQuery] = index;

	elementList.pop_back();

	if (elementList.size() < elementList.capacity() * 0.5) {
		elementList.shrink_to_fit();
	}

//	IniTime_Ours += getCurrentTime() - start;

	if (oldKey != newKey) {
		if (oldKey > newKey) {
			BubbleUp(index, _queryList, _upperElementIndexInHeapList);
		} else {
			BubbleDown(index, _queryList, _upperElementIndexInHeapList);
		}
	}

//	HeapElement* temp = GetTop(_heapElementList);
//	if (temp == NULL)
//		return;
//
//	// Set the emitCnt of the target element to be smaller than the min of this heap.
//	(_heapElementList)[elementList[index]].emitCnt = temp->emitCnt - 1;
//	// Push this element to the top of this heap.
//	BubbleUp(index, _heapElementList);
//	// Just delete the min of this heap.
//	DeleteTop(_heapElementList);
}

void MinHeap::DeleteTop(MyVector<Query>& _queryList,
		MyVector<int>& _upperElementIndexInHeapList) {
	this->Delete(0, _queryList, _upperElementIndexInHeapList);
}

/*
 *  Modify the key value of the element in the specified index.
 *  Parameter List:
 *  	index:		the specified index.
 *  	key:		the new key value.
 */
void MinHeap::ModifyKey(int index, int key, MyVector<Query>& _queryList,
		MyVector<int>& _upperElementIndexInHeapList) {

//	double start = getCurrentTime();

	HeapElement& he = elementList[index];
//	int old = he.emitCnt;
//	he.emitCnt = key;

	int old = he.getEmitCnt();
	he.setEmitCnt(key);

//	IniTime_Ours += getCurrentTime() - start;

	if (old != key) {
		if (old < key) {
			BubbleDown(index, _queryList, _upperElementIndexInHeapList);
		} else {
			BubbleUp(index, _queryList, _upperElementIndexInHeapList);
		}
	}
}

void MinHeap::ReleaseSpace() {
	this->elementList.release_space();

//	int num = this->elementList.size();
//	if (num > 0) {
//		HeapElement* temp = NULL;
//		for (int i = 0; i < num; ++i) {
//			temp = this->elementList[i];
//			if (temp != NULL) {
//				temp->query = NULL;
//				// Release the space of this heap element.
//				delete temp;
//				this->elementList[i] = NULL;
//
//				// Decrease the number of HeapElement by 1.
//				HENUM--;
//			}
//		}
//
//		vector<HeapElement*> emptyList;
//		this->elementList.swap(emptyList);
//		emptyList.clear();
//	}
}

int MinHeap::GetSize() {
	return this->elementList.size();
}

void MinHeap::Heapify(MyVector<Query>& _queryList,
		MyVector<int>& _upperElementIndexInHeapList) {
	int length = elementList.size();
	for (int i = (length - 2) / 2; i >= 0; --i) {
		BubbleDown(i, _queryList, _upperElementIndexInHeapList);
	}
//	for (int i = length - 1; i >= 0; --i) {
//		BubbleDown(i, _heapElementList);
//	}
}

void MinHeap::ShrinkToFit() {
//	vector<HeapElement*>(this->elementList).swap(this->elementList);
	this->elementList.shrink_to_fit();
	// Initialize the emitListStart value.
//	this->emitListStart = this->elementList.size();
}

void MinHeap::BubbleDown(int index, MyVector<Query>& _queryList,
		MyVector<int>& _upperElementIndexInHeapList) {
//	double start = getCurrentTime();

	int length = elementList.size();
	int leftChildIndex = 2 * index + 1;
	int rightChildIndex = 2 * index + 2;

	if (leftChildIndex >= length)
		return; //index is a leaf

	int minIndex = index;

//	MyVector<HeapElement>& tempList = *(this->_heapElementList);

//	MyVector<HeapElement>& tempList = _heapElementList;
	HeapElement& parent = elementList[index];
	HeapElement& left = elementList[leftChildIndex];
	HeapElement* min = &parent;

	if (HeapElement_Greater(parent, left)) {
		minIndex = leftChildIndex;
		min = &left;
	}

	if (rightChildIndex < length) {
		HeapElement& right = elementList[rightChildIndex];
		if (HeapElement_Greater(*min, right)) {
			minIndex = rightChildIndex;
			min = &right;
		}
	}

	if (minIndex != index) {
		// need to swap

		// swap the index values in the heap.
//		//		tempList[elementList[index]].heapIndex = minIndex;
//		//		tempList[elementList[minIndex]].heapIndex = index;
//		parent.setHeapIndex(minIndex);
//		min->setHeapIndex(index);
//
//		// swap the index value in the list of all heap elements.
//		int temp = elementList[index];
//		elementList[index] = elementList[minIndex];
//		elementList[minIndex] = temp;

		int startIndex = parent.getQueryStartIndex();
		int indexInQuery = parent.getIndexInQuery();
		_upperElementIndexInHeapList[startIndex + 1 + indexInQuery] = minIndex;

		startIndex = min->getQueryStartIndex();
		indexInQuery = min->getIndexInQuery();
		_upperElementIndexInHeapList[startIndex + 1 + indexInQuery] = index;

//
//		int queryIndex = parent.getQueryIndex();
//		int indexInQuery = 0;
//		// swap the index values in the heap.
//		Query& query = (_queryList[queryIndex]);
//
//		if (query.threshold != -1) {
////			double start = getCurrentTime();
//			// The corresponding query is still alive.
//			indexInQuery = parent.getIndexInQuery();
//
//			query.upperElementIndexList[indexInQuery] = minIndex;
//
////			IniTime_Ours += getCurrentTime() - start;
//		}
//
//		queryIndex = min->getQueryIndex();
//		Query& query2 = (_queryList[queryIndex]);
//		if (query2.threshold != -1) {
//			// The corresponding query is still alive.
//			indexInQuery = min->getIndexInQuery();
//
//			query2.upperElementIndexList[indexInQuery] = index;
//		}

		// Swap the content of the two heap elements.
		parent.swap(*min);

//		IniTime_Ours += getCurrentTime() - start;

		BubbleDown(minIndex, _queryList, _upperElementIndexInHeapList);
	}
}

void MinHeap::BubbleUp(int index, MyVector<Query>& _queryList,
		MyVector<int>& _upperElementIndexInHeapList) {
	if (index == 0)
		return;

//	double start = getCurrentTime();

	int parentIndex = (index - 1) / 2;

//	MyVector<HeapElement>& tempList = _heapElementList;
	HeapElement& parent = elementList[parentIndex];
	HeapElement& cur = elementList[index];

	if (HeapElement_Greater(parent, cur)) {
		// need to swap

		// swap the index values in the heap.
//		//		tempList[elementList[index]].heapIndex = parentIndex;
//		//		tempList[elementList[parentIndex]].heapIndex = index;
//		cur.setHeapIndex(parentIndex);
//		parent.setHeapIndex(index);
//		// swap the index value in the list of all heap elements.
//		int temp = elementList[parentIndex];
//		elementList[parentIndex] = elementList[index];
//		elementList[index] = temp;

		int startIndex = parent.getQueryStartIndex();
		int indexInQuery = parent.getIndexInQuery();
		_upperElementIndexInHeapList[startIndex + 1 + indexInQuery] = index;

		startIndex = cur.getQueryStartIndex();
		indexInQuery = cur.getIndexInQuery();
		_upperElementIndexInHeapList[startIndex + 1 + indexInQuery] =
				parentIndex;

//		// swap the index values in the heap.
//		Query& query = (_queryList[parent.getQueryIndex()]);
//		if (query.threshold != -1) {
//			// The corresponding query is still alive.
//			query.upperElementIndexList[parent.getIndexInQuery()] = index;
//		}
//
//		Query& query2 = (_queryList[cur.getQueryIndex()]);
//		if (query2.threshold != -1) {
//			query2.upperElementIndexList[cur.getIndexInQuery()] = parentIndex;
//		}

		// Swap the content of the two heap elements.
		parent.swap(cur);

//		IniTime_Ours += getCurrentTime() - start;

		BubbleUp(parentIndex, _queryList, _upperElementIndexInHeapList);
	}
}

void MinHeap::Insert(HeapElement& newElement, MyVector<Query>& _queryList,
		MyVector<int>& _upperElementIndexInHeapList) {
//	int length = elementList.size();
////	newElement.heapIndex = length;
//	newElement.setHeapIndex(length);

//	MyVector<HeapElement>& tempList = *(this->_heapElementList);
//	MyVector<HeapElement>& tempList = _heapElementList;
//	int index = tempList.size();
//	tempList.push_back(newElement);
//	elementList.push_back(index);

	elementList.push_back(newElement);
	BubbleUp(elementList.size() - 1, _queryList, _upperElementIndexInHeapList);
}

void MinHeap::Add(HeapElement& newElement) {
//	int length = elementList.size();
////	newElement.heapIndex = length;
//	newElement.setHeapIndex(length);
//
////	MyVector<HeapElement>& tempList = *(this->_heapElementList);
//	MyVector<HeapElement>& tempList = _heapElementList;
//	int index = tempList.size();
//	tempList.push_back(newElement);
//	elementList.push_back(index);

	elementList.push_back(newElement);

// In this function, we do not need to maintain the heap.
//	BubbleUp(length);
}

HeapElement* MinHeap::GetTop() {
	if (this->elementList.size() > 0)
		return &(this->elementList[0]);
	else {
		return NULL;
	}
}

