#pragma once

#include "Cluster.h"
#include "Parameter.h"
#include <vector>


class ClusteringFeature 
{
private:
	std::vector <Cluster> clusters;
	unsigned int NumberOfClusters;
	std::vector <double> LinearSum;
	std::vector <double> SquareSum;

public:
	void calcLinearSum();
	void calcSquareSum();
};
