/*
 * MyRandom.h
 *
 *  Created on: 5 Oct, 2015
 *      Author: jhgan
 */

#ifndef MYRANDOM_H_
#define MYRANDOM_H_

#include <math.h>
#include <algorithm>
#include <vector>
using namespace std;

/*
 *  The value range of the endpoints of queries.
 */
extern int rangeStart;
extern int rangeEnd;

/*
 *  Generate a random real distributed uniformly in [rangeStart, rangeEnd]. Input must satisfy: rangeStart <= rangeEnd.
 *  The granularity of generated random reals is given by RAND_MAX.
 *  Parameter List:
 *  	rangeStart:		the start value of the range
 *  	rangeEnd:		the end value of the range
 *  Return:
 *  	a random real number in the given range.
 */
double GenUniformRandom(double rangeStart, double rangeEnd);

/*
 *  Generate a random real from normal distribution N(0,1).
 */
double GenGaussianRandom();

/*
 *  Generate a random real from N(u, sqrt(var)).
 *  Parameter List:
 *  	u:			the mean of the Gaussian distribution.
 *  	var:		the variance of the Gaussian distribution.
 */
double GenGaussianRandom(double u, double var);

/*
 *  Generate a random integer from N(u, sqrt(var)).
 *  Parameter List:
 *  	u:			the mean of the Gaussian distribution.
 *  	var:		the variance of the Gaussian distribution.
 */
int GenGaussianRandInt(int u, int var);

/*
 *  Generate a random integer in [rangeS, rangeE].
 *  Parameter List:
 *  	rangeS:		the start of the given range.
 *  	rangeE:		the end of the given range.
 */
int GenRandInt(int rangeS, int rangeE);

/*
 *  Generate a random double value in the given range.
 */
double GenRandDouble(double rangeS, double rangeE);

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
void GenUniformPoint(int dim, int* targetPlace, int rangeS = rangeStart,
		int rangeE = rangeEnd);

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
void GenUniformSquare(int dim, int* targetPlace, int len, int rangeS =
		rangeStart, int rangeE = rangeEnd);

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
void GenGaussianPoint(int dim, int* targetPlace, int u, int sigma, int rangeS =
		rangeStart, int rangeE = rangeEnd);

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
		int rangeS = rangeStart, int rangeE = rangeEnd);

/*
 *  Generate a dim-dimensional integer point from N(u, sigma^2) and lying in [rangeS, rangeE].
 *  Parameter List:
 *  	dim:			dimensionality of point.
 *  	u:				the mean of the Gaussian distribution.
 *  	sigma:			the standard deviation of the Gaussian distribution.
 *  	rangeS:			the start value of the specified range.
 *  	rangeE:			the end value of the specified range.
 */
int* GenGaussianPoint(int dim, int u, int sigma, int rangeS = rangeStart,
		int rangeE = rangeEnd);

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
int* GenGaussianSquare(int dim, int len, int u, int sigma,
		int rangeS = rangeStart, int rangeE = rangeEnd);

/**************************************************
 *			Random Permutation.
 **************************************************/

/*
 *  Randomly permute the given list of integers.
 *  Parameter List:
 *  	targetList:		the given list of integers.
 */
void RandomPermute(vector<int>& targetList);

#endif /* MYRANDOM_H_ */
