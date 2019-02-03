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
	std::vector<Cluster*> childCluster;
	Centroid centroid;
	ClusteringFeature cf;
	double radius;
	double diameter;
	bool isLeafNode;

public:
	Cluster();
	void addToCluster(Point addPoint);
	void removeFromCluster(Point rmPoint);
	double calcDistance(Point& pEins, Point& pZwei);
	void calcCentroid();
	void calcRadius();
	void calcDiameter();
	ClusteringFeature getCF();
	bool getIsLeafNode();	
	void changeLeafNode(bool value);
	void recalculateCF();

};