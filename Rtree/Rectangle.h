/*
 * Rectangle.h
 *
 *  Created on: Sep 17, 2014
 *      Author: jhgan
 */

#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include "../headers.h"

#include <vector>
using namespace std;

/*
 *  The class of rectangle.
 */
class Rectangle {

public:
	/*
	 *  The list of lower coordinates.
	 */
	int* minValues;

	/*
	 *  The list of higher coordinates.
	 */
	int* maxValues;

	Rectangle();

	/*
	 *  The constructor with parameters.
	 */
	Rectangle(int* minValues, int* maxValues);

	~Rectangle();

	/*
	 *  Detect whether this rectangle intersects with the given rectangle *rec*.
	 *  Return:
	 *  	If YES, return TRUE. Otherwise, return FALSE.
	 */
	bool intersect(const Rectangle& rec) const;

	/*
	 *  Detect whether this rectangle is fully inside the given rectangle *rec*.
	 *  Return:
	 *  	If YES, return TRUE. Otherwise, return FALSE.
	 */
	bool inside(const Rectangle& rec) const;

	/*
	 *  Detect whether the given rectangle *rec* is fully contained in this rectangle.
	 *  Return:
	 *  	If YES, return TRUE. Otherwise, return FALSE.
	 */
	bool contain(const Rectangle& rec) const;

	/*
	 *  Compute the increasement of the perimeter if the given rectangle is merged with this rectangle.
	 *  Parameter List:
	 *  	rec:		the given rectangle.
	 *  Return:
	 *  	the increasement of the perimeter.
	 */
	long long perimeterIncreasement(const Rectangle& rec) const;

	/*
	 *  Compute the perimeter of this rectangle.
	 *  Return:
	 *  	the perimeter of this rectangle.
	 */
	long long computePerimeter() const;

	/*
	 *  Find the minimum bounding rectangle of this and the given rectangles.
	 *  Parameter List:
	 *  	rec:			the given rectangle
	 *  Return:
	 *  	the resulting MBR
	 */
	Rectangle& enlarge(const Rectangle& rec);

	/*
	 *  Set this rectangle by a given rectangle.
	 */
	void setByRectangle(const Rectangle& rec);

	/*
	 *  Set the list of lower values.
	 */
	void setMinCoords(const int* minCoords);

	/*
	 *  Set the list of higher values.
	 */
	void setMaxCoords(const int* maxCoords);

	/*
	 *  Check whether this rectangle is stabbed by the given point.
	 *  Parameter List:
	 *  	coords:		the coordinates of the given point.
	 *  Return:
	 *  	If Yes, return TRUE. Otherwise, return FALSE.
	 */
	bool CheckStab(vector<int>& coords);

	/*
	 *  Show the coordinates of this rectangle.
	 */
	void showRectangle();

	/*
	 *  Release the space of this rectangle.
	 */
	void ReleaseSpace();

	/*
	 *  Compute the space consumption in terms of bytes of this structure.
	 */
	long long ComputeSpaceConsumption();

};

#endif /* RECTANGLE_H_ */
