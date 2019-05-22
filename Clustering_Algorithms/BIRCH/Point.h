#pragma once
#include "Parameter.h"

class Point_B
{
protected:
	double coordinates[DIMENSIONS];
public:
	Point_B();
	Point_B(double* position);
	void getCoordinates(double* coordinateBuffer);
	double getCoordinate(int dimension);
	void setCoordinates(double position[DIMENSIONS]);
};