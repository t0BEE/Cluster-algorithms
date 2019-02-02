#pragma once

#include "Point.h"
#include "Parameter.h"
#include "ClusteringFeature.h"
#include <vector>
#include <math.h>

class Cluster 
{
private:
	std::vector<Point> cluster;
	Centroid centroid;
	ClusteringFeature cf;
	double radius;
	double diameter;

public:
	Cluster();
	void addToCluster(Point addPoint);
	void removeFromCluster(Point rmPoint);
	double calcDistance(Point& pEins, Point& pZwei);
	void calcCentroid();
	void calcRadius();
	void calcDiameter();
	ClusteringFeature getCF();
};