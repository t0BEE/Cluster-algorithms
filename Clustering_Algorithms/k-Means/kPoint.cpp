#include "kPoint.h"

// Constructor of class Point
// Input: 
// Output: --
// Effect: New Point instance created
Point::Point(){}

// Constructor of class Point
// Input: double array with coordinates
// Output: --
// Effect: New Cluster instance created with position and neutral clusterNumber
Point::Point(double* position)
{
	for (int i = 0; i < DIMENSIONS; ++i)
	{
		this->coordinates[i] = position[i];
	}
	this->clusterNumber = -1;
}

// Set the cluster number of a point
// Input: integer for cluster number
// Output: --
// Effect: variable clusterNumber is changed
void Point::setCluster(int clusterNumber)
{
	this->clusterNumber = clusterNumber;
}

// Get coordinates of a point
// A pointer is necessary because returning an array is not possible
// Input: double pointer for an array to write the coordinates in
// Output: --
// Effect: the input array contains the coordinates
void Point::getCoordinates(double* coordinateBuffer)
{
	for (int i = 0; i < DIMENSIONS; ++i)
	{
		coordinateBuffer[i] = this->coordinates[i];
	}
}

// Get cluster number of a point
// Input: --
// Output: integer for the cluster number
// Effect: --
int Point::getCluster()
{
	return this->clusterNumber;
}

// Get coordinate of only one dimension
// Input: dimension to look for
// Output: double value of dimension coordinate
// Effect: --
double Point::getCoordinate(int dimension)
{
	return this->coordinates[dimension];
}


// Set the position of a centroid
// Input: double array with coordinates
// Output: --
// Effect: the position of a centroid has changed
void Centroid::setPosition(double position[DIMENSIONS])
{
	for (int i = 0; i < DIMENSIONS; ++i)
	{
		this->coordinates[i] = position[i];
	}
}

// Calculate the distance between 2 points
// Manhatten distance
// Input: 2 Points
// Output: double value of distance
// Effect: --
double calcPointDistance(Point one, Point two)
{
	double distance = 0;
	for (int i = 0; i < DIMENSIONS; ++i)
	{
		distance += abs(one.getCoordinate(i) - two.getCoordinate(i));
	}
	return distance;
}
