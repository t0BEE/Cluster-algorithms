#include "ClusteringFeature.h"

ClusteringFeature::ClusteringFeature() {}

// Recalculate the Linear Sum of the CF
// If the CF is not a leaf node the values can be summed up
// from the child nodes to speed up the process
// Input: --
// Output: --
// Effect: value of LS in a CF change
void ClusteringFeature::calcLinearSum(bool isLeaf, std::vector<Cluster> cluster, std::vector<CFTreeNode*> childCluster)
{
	double varLS[DIMENSIONS];
	double tmpVar[DIMENSIONS];
	if (isLeaf)
	{
		for (int j = 0; j < DIMENSIONS; ++j)
		{
			varLS[j] = 0.0;
			for (int i = 0; i < cluster.size(); ++i)
			{
				std::vector<Point> vaCluster = cluster[i].getPoints();
				for (int k = 0; k < vaCluster.size(); ++k)
				{
				varLS[j] += vaCluster[k].getCoordinate(j);
				}
			}
			this->linearSum[j] = varLS[j];
		}
	}
	else
	{
	for (int j = 0; j < DIMENSIONS; ++j)
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
void ClusteringFeature::calcSquareSum(bool isLeaf, std::vector<Cluster> cluster, std::vector<CFTreeNode*> childCluster)
{
	double varSS[DIMENSIONS];
	double tmpVar[DIMENSIONS];
	if (isLeaf)
	{
		for (int j = 0; j < DIMENSIONS; ++j)
		{
			varSS[j] = 0.0;
			for (int i = 0; i < cluster.size(); ++i)
			{
				std::vector<Point> vaCluster = cluster[i].getPoints();
				for (int k = 0; k < vaCluster.size(); ++k)
				{
					varSS[j] += pow(vaCluster[k].getCoordinate(j), 2.0);
				}
			}
			this->linearSum[j] = varSS[j];
		}
	}
	else
	{
		for (int j = 0; j < DIMENSIONS; ++j)
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
	for (int i = 0; i < DIMENSIONS; ++i)
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
	for (int i = 0; i < DIMENSIONS; ++i)
	{
		buffer[i] = this->squareSum[i];
	}
}