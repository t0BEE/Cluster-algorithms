#pragma once
#include "Parameter.h"
#include <vector>

class Point
{
protected:
	double coordinates[DIMENSION];
public:
	Point();
	Point(double in[DIMENSION] );
	void getCoordinates(double* pointerToDoubleArray);
	double getCoordinate(int dimension);
};

class Centroid : public Point
{
public:
	void setPosition(double newCoordinates[DIMENSION]);
};
