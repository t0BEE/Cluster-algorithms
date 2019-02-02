#pragma once

#include "Point.h"
#include "Parameter.h"
#include <vector>
#include <math.h>

class Cluster 
{
private:
	std::vector<Point> cluster;
	Centroid centroid;
	double radius[DIMENSION];
	double diameter[DIMENSION];

public:
	Cluster();
	void addToCluster(Point addPoint);
	void removeFromCluster(Point rmPoint);
	double calcDistance(Point& pEins, Point& pZwei);
	void calcCentroid();
	void calcRadius();
	void clacDiameter();
};