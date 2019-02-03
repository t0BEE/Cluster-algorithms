#include "ClusteringFeature.h"

ClusteringFeature::ClusteringFeature() {}

// Recalculate the Linear Sum of the CF
// If the CF is not a leaf node the values can be summed up
// from the child nodes to speed up the process
// Input: --
// Output: --
// Effect: value of LS in a CF change
void ClusteringFeature::calcLinearSum(bool isLeaf, std::vector<Point> cluster, std::vector<Cluster*> childCluster)
{
	double varLS[DIMENSION];
	double tmpVar[DIMENSION];
	if (isLeaf)
	{
		for (int j = 0; j < DIMENSION; ++j)
		{
			varLS[j] = 0.0;
			for (int i = 0; i < cluster.size(); ++i)
			{
				varLS[j] += cluster[i].getCoordinate(j);
			}
			this->linearSum[j] = varLS[j];
		}
	}
	else
	{
	for (int j = 0; j < DIMENSION; ++j)
	{
		varLS[j] = 0.0;
		for (int i = 0; i < childCluster.size(); ++i)
		{
			childCluster[i]->getCF().getLS(tmpVar);
			varLS[j] += tmpVar[j];
		}
		this->linearSum[j] = varLS[j];
	}
	}
}

// Recalculate the Square Sum of the CF
// If the CF is not a leaf node the values can be summed up
// from the child nodes to speed up the process
// Input: --
// Output: --
// Effect: value of SS in a CF change
void ClusteringFeature::calcSquareSum(bool isLeaf, std::vector<Point> cluster, std::vector<Cluster*> childCluster)
{
	double varSS[DIMENSION];
	double tmpVar[DIMENSION];
	if (isLeaf)
	{
		for (int j = 0; j < DIMENSION; ++j)
		{
			varSS[j] = 0.0;
			for (int i = 0; i < cluster.size(); ++i)
			{
				varSS[j] += pow(cluster[i].getCoordinate(j), 2.0);
			}
			this->squareSum[j] = varSS[j];
		}
	}
	else
	{
		for (int j = 0; j < DIMENSION; ++j)
		{
			varSS[j] = 0.0;
			for (int i = 0; i < childCluster.size(); ++i)
			{
				childCluster[i]->getCF().getSS(tmpVar);
				varSS[j] += tmpVar[j];
			}
			this->squareSum[j] = varSS[j];
		}
	}
}


// Get the Linear Sum of the Clustering Feature
// Input: pointer to transfer the values
// Output: --
// Effect: the buffer contains the linear sum
void ClusteringFeature::getLS(double* buffer)
{
	for (int i = 0; i < DIMENSION; ++i)
	{
		buffer[i] = this->linearSum[i];
	}
}

// Get the Square Sum of the Clustering Feature
// Input: pointer to transfer the values
// Output: --
// Effect: the buffer contains the square sum
void ClusteringFeature::getSS(double* buffer)
{
	for (int i = 0; i < DIMENSION; ++i)
	{
		buffer[i] = this->squareSum[i];
	}
}