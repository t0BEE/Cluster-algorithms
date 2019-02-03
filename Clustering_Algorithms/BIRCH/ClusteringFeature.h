#pragma once

#include "Cluster.h"
#include "Parameter.h"
#include <vector>


class ClusteringFeature 
{
private:
	unsigned int NumberOfPoints;
	double linearSum[DIMENSION];
	double squareSum[DIMENSION];

public:
	ClusteringFeature();
	void recalculate();
	void getLS(double* buffer);
	void getSS(double* buffer);
	void calcLinearSum(bool isLeaf, std::vector<Point> cluster, std::vector<Cluster*> childCluster);
	void calcSquareSum(bool isLeaf, std::vector<Point> cluster, std::vector<Cluster*> childCluster);
};

