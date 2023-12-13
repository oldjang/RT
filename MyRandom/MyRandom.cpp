/*
 * MyRandom.cpp
 *
 *  Created on: 5 Oct, 2015
 *      Author: jhgan
 */

#include "MyRandom.h"
#include "../headers.h"

/*
 *  The value range of the endpoints of queries.
 */
int rangeStart = 1;
int rangeEnd = 100000;

/*
 *  Generate a random real distributed uniformly in [rangeStart,rangeEnd].
 *  Input must satisfy: rangeStart <= rangeEnd.
 *  The granularity of generated random reals is given by RAND_MAX.
 */
double GenUniformRandom(double rangeStart, double rangeEnd) {
	if (rangeStart > rangeEnd) {
		ShowError(
				"Error in genUniformRandom: the rangeEnd is < the rangeStart!\n");
	}
	double r;
	r = rangeStart
			+ ((rangeEnd - rangeStart) * (double) random() / (double) RAND_MAX);
	if (r < rangeStart || r > rangeEnd) {
		ShowError(
				"Error in genUniformRandom: the number generated is not in the given range!\n");
	}
	return r;
}

/*
 *  Generate a random real from normal distribution N(0,1).
 */
double GenGaussianRandom() {
	// Use Box-Muller transform to generate a point from normal
	// distribution.
	double x1, x2;
	do {
		x1 = GenUniformRandom(0.0, 1.0);
	} while (x1 == 0); // cannot take log of 0.
	x2 = GenUniformRandom(0.0, 1.0);
	double z;
	z = sqrt(-2.0 * log(x1)) * cos(2.0 * M_PI * x2);
	return z;
}

/*
 *  Generate a random real from N(u, sigma^2).
 *  Parameter List:
 *  	u:			the mean of the Gaussian distribution.
 *  	sigma:		the standard deviation of the Gaussian distribution.
 */
double GenGaussianRandom(double u, double sigma) {
	// Generate a real from N(0, 1).
	double x = GenGaussianRandom();
	x *= sigma;
	x += u;
	return x;
}

/*
 *  Generate a random integer from N(u, sigma^2).
 *  Parameter List:
 *  	u:			the mean of the Gaussian distribution.
 *  	sigma:		the standard deviation of the Gaussian distribution.
 */
int GenGaussianRandInt(int u, int sigma) {
	double x = GenGaussianRandom(u, sigma);
	return (int) (x + 0.5);
}

/*
 *  Generate a random integer in the given range.
 */
int GenRandInt(int rangeS, int rangeE) {
	return (int) (rangeS + ((double) rand() / RAND_MAX) * (rangeE - rangeS));
}

/*
 *  Generate a random double value in the given range.
 */
double GenRandDouble(double rangeS, double rangeE) {
	return GenUniformRandom(rangeS, rangeE);
}

/**************************************************
 *			Geometry Objects Generation.
 **************************************************/
/*
 *  Generate a dim-dimensional integer point from Uniform distribution and lying in [rangeS, rangeE]^dim.
 *  Parameter List:
 *  	dim:			dimensionality of point.s
 *  	rangeS:			the start value of the specified range.
 *  	rangeE:			the end value of the specified range.
 */
void GenUniformPoint(int dim, int* targetPlace, int rangeS, int rangeE) {
//	int* targetPlace = new int[dim];

	for (int i = 0; i < dim; ++i) {
		targetPlace[i] = GenRandInt(rangeS, rangeE);
		// To make sure the center and the query range lie in [rangeStart, rangeEnd].
		while (targetPlace[i] < rangeS || targetPlace[i] > rangeE) {
			targetPlace[i] = GenRandInt(rangeS, rangeE);
		}
	}

//	return targetPlace;
}

/*
 *  Generate a dim-dimensional square whose center is an integer point from Uniform distribution.
 *  and this square lies in the cube of [rangeS, rangeE]^dim.
 *  Parameter List:
 *  	dim:			dimensionality of point.
 *  	targetPlace:	the targe place to store the square generated.
 *  	len:			the length of the square.
 *  	rangeS:			the start value of the specified range.
 *  	rangeE:			the end value of the specified range.
 */
void GenUniformSquare(int dim, int* targetPlace, int len, int rangeS,
		int rangeE) {

//	int* targetPlace = new int[2 * dim];

//	targetPlace.clear();
//	targetPlace.resize(2 * dim, 0);

	int coords[dim];
	int qLen_Half = len / 2;
	for (int j = 0; j < dim; ++j) {

		coords[j] = GenRandInt(rangeStart, rangeEnd);
		// To make sure the square range lie in [rangeStart, rangeEnd].
		while (coords[j] - qLen_Half < rangeStart
				|| coords[j] + qLen_Half > rangeEnd) {
			coords[j] = GenRandInt(rangeStart, rangeEnd);
		}

//		// The good version for Gaussian.
//		coords[j] = GenGaussianRandInt(u, var);
//		// To make sure the square range lie in [rangeStart, rangeEnd].
//		while (coords[j] - qLen_Half < rangeStart
//				|| coords[j] + qLen_Half > rangeEnd) {
//			coords[j] = GenGaussianRandInt(u, var);
//		}

		targetPlace[2 * j] = coords[j] - qLen_Half;
		targetPlace[2 * j + 1] = coords[j] + qLen_Half;
	}

//	return targetPlace;
}

/*
 *  Generate a dim-dimensional integer point from N(u, sigma^2) and lying in [rangeS, rangeE].
 *  Parameter List:
 *  	dim:			dimensionality of point.
 *  	targetPlace:	the target place to store the point.
 *  	u:				the mean of the Gaussian distribution.
 *  	sigma:			the standard deviation of the Gaussian distribution.
 *  	rangeS:			the start value of the specified range.
 *  	rangeE:			the end value of the specified range.
 */
void GenGaussianPoint(int dim, int* targetPlace, int u, int sigma, int rangeS,
		int rangeE) {
//	int* targetPlace = new int[dim];

	for (int i = 0; i < dim; ++i) {
		targetPlace[i] = GenGaussianRandInt(u, sigma);
		// To make sure the center and the query range lie in [rangeStart, rangeEnd].
		while (targetPlace[i] < rangeS || targetPlace[i] > rangeE) {
			targetPlace[i] = GenGaussianRandInt(u, sigma);
		}
	}

//	return targetPlace;
}

/*
 *  Generate a dim-dimensional square whose center is an integer point from N(u, sigma^2)
 *  and this square lies in the cube of [rangeS, rangeE]^dim.
 *  Parameter List:
 *  	dim:			dimensionality of point.
 *  	targetPlace:	the target place to store the square generated.
 *  	len:			the length of the square.
 *  	u:				the mean of the Gaussian distribution.
 *  	sigma:			the standard deviation of the Gaussian distribution.
 *  	rangeS:			the start value of the specified range.
 *  	rangeE:			the end value of the specified range.
 */
void GenGaussianSquare(int dim, int* targetPlace, int len, int u, int sigma,
		int rangeS, int rangeE) {

//	int* targetPlace = new int[2 * dim];

//	targetPlace.clear();
//	targetPlace.resize(2 * dim, 0);

	int coords[dim];
	int qLen_Half = len / 2;
	for (int j = 0; j < dim; ++j) {

//		// Just for test the uniform intervals.
//		coords[j] = GenRandInt(rangeStart, rangeEnd);
//		// To make sure the square range lie in [rangeStart, rangeEnd].
//		while (coords[j] - qLen_Half < rangeStart
//				|| coords[j] + qLen_Half > rangeEnd) {
//			coords[j] = GenRandInt(rangeStart, rangeEnd);
//		}

		// The good version for Gaussian.
		coords[j] = GenGaussianRandInt(u, sigma);
		// To make sure the square range lie in [rangeStart, rangeEnd].
		while (coords[j] - qLen_Half < rangeStart
				|| coords[j] + qLen_Half > rangeEnd) {
			coords[j] = GenGaussianRandInt(u, sigma);
		}

		targetPlace[2 * j] = coords[j] - qLen_Half;
		targetPlace[2 * j + 1] = coords[j] + qLen_Half;
	}

//	return targetPlace;
}

/*
 *  Generate a dim-dimensional integer point from N(u, sigma^2) and lying in [rangeS, rangeE].
 *  Parameter List:
 *  	dim:			dimensionality of point.
 *  	u:				the mean of the Gaussian distribution.
 *  	sigma:			the standard deviation of the Gaussian distribution.
 *  	rangeS:			the start value of the specified range.
 *  	rangeE:			the end value of the specified range.
 */
int* GenGaussianPoint(int dim, int u, int sigma, int rangeS, int rangeE) {
	int* targetPlace = new int[dim];

	for (int i = 0; i < dim; ++i) {
		targetPlace[i] = GenGaussianRandInt(u, sigma);
		// To make sure the center and the query range lie in [rangeStart, rangeEnd].
		while (targetPlace[i] < rangeS || targetPlace[i] > rangeE) {
			targetPlace[i] = GenGaussianRandInt(u, sigma);
		}
	}

	return targetPlace;
}


/*
 *  Generate a dim-dimensional square whose center is an integer point from N(u, sigma^2)
 *  and this square lies in the cube of [rangeS, rangeE]^dim.
 *  Parameter List:
 *  	dim:			dimensionality of point.
 *  	targetPlace:	the targe place to store the square generated.
 *  	len:			the length of the square.
 *  	u:				the mean of the Gaussian distribution.
 *  	sigma:			the standard deviation of the Gaussian distribution.
 *  	rangeS:			the start value of the specified range.
 *  	rangeE:			the end value of the specified range.
 */
int* GenGaussianSquare(int dim, int len, int u, int sigma, int rangeS,
		int rangeE) {

	int* targetPlace = new int[2 * dim];

//	targetPlace.clear();
//	targetPlace.resize(2 * dim, 0);

	int coords[dim];
	int qLen_Half = len / 2;
	for (int j = 0; j < dim; ++j) {
		coords[j] = GenGaussianRandInt(u, sigma);
		// To make sure the square range lie in [rangeStart, rangeEnd].
		while (coords[j] - qLen_Half < rangeStart
				|| coords[j] + qLen_Half > rangeEnd) {
			coords[j] = GenGaussianRandInt(u, sigma);
		}

		targetPlace[2 * j] = coords[j] - qLen_Half;
		targetPlace[2 * j + 1] = coords[j] + qLen_Half;
	}

	return targetPlace;
}

/**************************************************
 *			Random Permutation.
 **************************************************/

/*
 *  Randomly permute the given list of integers.
 *  Parameter List:
 *  	targetList:		the given list of integers.
 */
void RandomPermute(vector<int>& targetList) {
//	vector<int> A(targetList);
	int length = targetList.size();
	int rid = 0;
	int temp = 0;
	for (int i = 0; i < length; ++i) {
		rid = GenRandInt(0, i);
		if (rid != i) {
			temp = targetList[i];
			targetList[i] = targetList[rid];
			targetList[rid] = temp;
		}
	}
}
