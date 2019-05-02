#pragma once

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
	ClusteringFeature(int nrPoints, double* LS, double* SS);
	void getLS(double* buffer);
	void getSS(double* buffer);
	int getNumberOfPoints();
	void calcCentroid(double* buffer);
	bool absorbCF(ClusteringFeature absorbCF);
	void addToLS(double* buffer);
	void addToSS(double* buffer);
	//void calcLinearSum(bool isLeaf, std::vector<Cluster> cluster, std::vector<CFTreeNode> childCluster);
	//void calcSquareSum(bool isLeaf, std::vector<Cluster> cluster, std::vector<CFTreeNode> childCluster);
};

