#pragma once

#include "CFTreeNode.h"
#include "Parameter.h"
#include <vector>


class ClusteringFeature 
{
private:
	unsigned int NumberOfPoints;
	double linearSum[DIMENSIONS];
	double squareSum[DIMENSIONS];

public:
	ClusteringFeature();
	void getLS(double* buffer);
	void getSS(double* buffer);
	void calcLinearSum(bool isLeaf, std::vector<Cluster> cluster, std::vector<CFTreeNode*> childCluster);
	void calcSquareSum(bool isLeaf, std::vector<Cluster> cluster, std::vector<CFTreeNode*> childCluster);
};

