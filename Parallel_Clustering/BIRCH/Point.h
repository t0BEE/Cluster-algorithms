#pragma once
#include "Parameter.h"

class Point_B
{
	std::vector<double> coordinates;

public:
    int cluster;
	Point_B();
	Point_B(double* position);
	void getCoordinates(double* coordinateBuffer);
	double getCoordinate(int dimension);
	void setCoordinates(double *position);
};