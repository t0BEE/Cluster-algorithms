#pragma once

#include "Cluster.h"
#include "Parameter.h"
#include <vector>


class ClusteringFeature 
{
private:
	std::vector<Cluster> childClusters;
	unsigned int NumberOfPoints;
	double linearSum[DIMENSION];
	double squareSum[DIMENSION];
	bool leafNode;

	void calcLinearSum();
	void calcSquareSum();

public:
	ClusteringFeature();
	std::vector<Point> getClusterOfThisLevel();
	void recalculate();
	void changeLeafNode();
	void getLS(double* buffer);
	void getSS(double* buffer);
};
