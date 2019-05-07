#pragma once
#include <vector>
#include "kParameter.h"

class Point
{
	int clusterNumber;
protected:
	double coordinates[DIMENSIONS];
public:
	Point();
	Point(double* position);
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

double calcPointDistance(Point one, Point two);