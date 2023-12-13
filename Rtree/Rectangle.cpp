/*
 * Rectangle.cpp
 *
 *  Created on: Sep 17, 2014
 *      Author: jhgan
 */

#include "Rectangle.h"

Rectangle::Rectangle() {
	this->minValues = new int[DIM];
	this->maxValues = new int[DIM];
}

Rectangle::Rectangle(int* minValues, int* maxValues) {
	this->minValues = minValues;
	this->maxValues = maxValues;
}

Rectangle::~Rectangle() {
}

bool Rectangle::inside(const Rectangle& rec) const {
	for (int i = 0; i < DIM; i++) {
		if (this->minValues[i] < rec.minValues[i]) {
			return false;
		}
		if (this->maxValues[i] > rec.maxValues[i]) {
			return false;
		}
	}
	return true;
}

bool Rectangle::intersect(const Rectangle& rec) const {
	for (int i = 0; i < DIM; i++) {
		if (this->minValues[i] > rec.maxValues[i]) {
			return false;
		}
		if (this->maxValues[i] < rec.minValues[i]) {
			return false;
		}
	}
	return true;
}

bool Rectangle::contain(const Rectangle& rec) const {
	for (int i = 0; i < DIM; ++i) {
		if (rec.minValues[i] < this->minValues[i]) {
			return false;
		}
		if (rec.maxValues[i] > this->maxValues[i]) {
			return false;
		}
	}
	return true;
}

long long Rectangle::computePerimeter() const {
	long long rtn = 0;
	for (int i = 0; i < DIM; ++i) {
		rtn += (this->maxValues[i] - this->minValues[i]);
	}
	rtn *= 2;
	return rtn;
}

long long Rectangle::perimeterIncreasement(const Rectangle& rec) const {
	long long rtn = 0;
	for (int i = 0; i < DIM; ++i) {
		rtn += (max(this->maxValues[i], rec.maxValues[i])
				- min(this->minValues[i], rec.minValues[i]));
	}
	rtn *= 2;
	return rtn - (this->computePerimeter());
}

Rectangle& Rectangle::enlarge(const Rectangle& rec) {
	for (int i = 0; i < DIM; i++) {
		this->minValues[i] = min(this->minValues[i], rec.minValues[i]);
		this->maxValues[i] = max(this->maxValues[i], rec.maxValues[i]);
	}
	return *(this);
}

void Rectangle::setMinCoords(const int* minCoords) {
	if (this->minValues == NULL) {
		this->minValues = new int[DIM];
	}
	for (int i = 0; i < DIM; i++) {
		this->minValues[i] = minCoords[i];
	}

}

void Rectangle::setMaxCoords(const int* maxCoords) {
	if (this->maxValues == NULL) {
		this->maxValues = new int[DIM];
	}
	for (int i = 0; i < DIM; i++) {
		this->maxValues[i] = maxCoords[i];
	}
}

void Rectangle::setByRectangle(const Rectangle& rec) {
	this->setMinCoords(rec.minValues);
	this->setMaxCoords(rec.maxValues);
}

bool Rectangle::CheckStab(vector<int>& coords) {
	for (int i = 0; i < DIM; ++i) {
		if (coords[i] < this->minValues[i] || coords[i] > this->maxValues[i])
			return false;
	}
	return true;
}

void Rectangle::showRectangle() {
	// print the range
	printf("The rectangle is bounded by [%d, %d]", this->minValues[0],
			this->maxValues[0]);
	for (int j = 1; j < DIM; ++j) {
		printf(" * [%d, %d]", this->minValues[j], this->maxValues[j]);
	}
	printf("\n");

// // Old version.
//	int dim = DIM;
//	printf("The rectangle is bounded by: ");
//	printf("(");
//	for (int i = 0; i < dim - 1; i++) {
//		printf("%d, ", this->minValues[i]);
//	}
//	printf("%d)\t", this->minValues[dim - 1]);
//
//	printf("(");
//	for (int i = 0; i < dim - 1; i++) {
//		printf("%d, ", this->maxValues[i]);
//	}
//	printf("%d)\n", this->maxValues[dim - 1]);
}

void Rectangle::ReleaseSpace() {
	if (this->minValues != NULL) {
		delete[] (this->minValues);
		this->minValues = NULL;
	}

	if (this->maxValues != NULL) {
		delete[] (this->maxValues);
		this->maxValues = NULL;
	}
}

long long Rectangle::ComputeSpaceConsumption() {
	long long spaceCons = 0;
	spaceCons += sizeof(Rectangle);
	if (this->minValues != NULL) {
		spaceCons += sizeof(int) * DIM;
	}

	if (this->maxValues != NULL) {
		spaceCons += sizeof(int) * DIM;
	}
	return spaceCons;
}
