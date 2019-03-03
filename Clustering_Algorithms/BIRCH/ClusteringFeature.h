#pragma once

#include "CFTreeNode.h"
#include "Parameter.h"
#include <vector>


class ClusteringFeature 
{
private:
	unsigned int numberOfPoints;
	double linearSum[DIMENSIONS];
	double squareSum[DIMENSIONS];
	double calcRadius(ClusteringFeature cfToAdd);
public:
	ClusteringFeature();
	void getLS(double* buffer);
	void getSS(double* buffer);
	int getNumberOfPoints();
	void calcCentroid(double* buffer);
	bool absorbCF(ClusteringFeature absorbCF);

	//void calcLinearSum(bool isLeaf, std::vector<Cluster> cluster, std::vector<CFTreeNode> childCluster);
	//void calcSquareSum(bool isLeaf, std::vector<Cluster> cluster, std::vector<CFTreeNode> childCluster);
};

