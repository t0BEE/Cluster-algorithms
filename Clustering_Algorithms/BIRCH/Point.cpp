#include "Point.h"

// Constructor of class Point_B
// Input: 
// Output: --
// Effect: New Point_B instance created
Point_B::Point_B() {}

// Constructor of class Point_B
// Input: double array with coordinates
// Output: --
// Effect: New Cluster instance created with position and neutral clusterNumber
Point_B::Point_B(double* position)
{
	for (int i = 0; i < DIMENSIONS; ++i)
	{
		this->coordinates[i] = position[i];
	}
}

// Get coordinates of a Point_B
// A Point_Ber is necessary because returning an array is not possible
// Input: double Point_Ber for an array to write the coordinates in
// Output: --
// Effect: the input array contains the coordinates
void Point_B::getCoordinates(double* coordinateBuffer)
{
	for (int i = 0; i < DIMENSIONS; ++i)
	{
		coordinateBuffer[i] = this->coordinates[i];
	}
}

// Get coordinate of only one dimension
// Input: dimension to look for
// Output: double value of dimension coordinate
// Effect: --
double Point_B::getCoordinate(int dimension)
{
	return this->coordinates[dimension];
}

void Point_B::setCoordinates(double position[DIMENSIONS])
{
	for (int i = 0; i < DIMENSIONS; ++i)
	{
		this->coordinates[i] = position[i];
	}
}
