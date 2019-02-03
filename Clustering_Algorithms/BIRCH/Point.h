#pragma once
#include "Parameter.h"
#include <vector>

class Point
{
protected:
	double coordinates[DIMENSIONS];
public:
	Point();
	Point(double in[DIMENSIONS] );
	void getCoordinates(double* pointerToDoubleArray);
	double getCoordinate(int dimension);
};

class Centroid : public Point
{
public:
	void setPosition(double newCoordinates[DIMENSIONS]);
};
