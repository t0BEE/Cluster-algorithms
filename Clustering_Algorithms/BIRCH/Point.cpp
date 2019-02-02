#include "Point.h"

// Constructor of class Point
// Input: --
// Output: --
// Effect: New Point instance created
Point::Point() {}

// Constructor of class Point
// Input: double array --> coordinates of the point
// Output: --
// Effect: New Point instance created
Point::Point(double in[DIMENSION])
{
	for (int i = 0; i < DIMENSION; ++i)
	{
		this->coordinates[i] = in[i];
	}
}

// Coordinates of a point will be transfered to an array
// The array has to be given by a pointer of type double
// Because arrays can not be returned
// Input: double * to an array 
// Output: --
// Effect: Change the values in the array which is given by the pointer
void Point::getCoordinates(double* pointerToDoubleArray)
{
	for (int i = 0; i < DIMENSION; ++i)
	{
		pointerToDoubleArray[i] = this->coordinates[i];
	}
}

// Look for the coordinate value of a certain dimension
// Input: integer value of the demanded dimension
// Output: double value of one dimension
// Effect: --
double Point::getCoordinate(int dimension)
{
	return this->coordinates[dimension];
}

void Centroid::setPosition(double newCoordinates[DIMENSION])
{
	for (int i = 0; i < DIMENSION; ++i)
	{
		this->coordinates[i] = newCoordinates[i];
	}
}

