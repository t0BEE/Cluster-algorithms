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
Point::Point(double in[DIMENSIONS])
{
	for (int i = 0; i < DIMENSIONS; ++i)
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
	for (int i = 0; i < DIMENSIONS; ++i)
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

// Set the centroids position
// Input: array of type double
// Output: --
// Effect: centroid variable is changed
void Centroid::setPosition(double newCoordinates[DIMENSIONS])
{
	for (int i = 0; i < DIMENSIONS; ++i)
	{
		this->coordinates[i] = newCoordinates[i];
	}
}

