#pragma once
#include <vector>
#include "Parameter.h"

class Point
{
	int clusterNumber;
protected:
	double coordinates[DIMENSIONS];
public:
	Point();
	Point(double position[DIMENSIONS]);
	void setCluster(int);
	void getCoordinates(double* coordinateBuffer);
	int getCluster();
	double getCoordinate(int dimension);
};

class Centroid: public Point
{
public:
	void setPosition(double position[DIMENSIONS]);
};
