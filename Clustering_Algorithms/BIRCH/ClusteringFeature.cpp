#include "ClusteringFeature.h"

ClusteringFeature::ClusteringFeature() 
{
	this->leafNode = true;
}

// Recalculate the Linear Sum of the CF
// If the CF is not a leaf node the values can be summed up
// from the child nodes to speed up the process
// Input: --
// Output: --
// Effect: value of LS in a CF change
void ClusteringFeature::calcLinearSum() 
{
	double varLS[DIMENSION];
	double tmpVar[DIMENSION];
	if (this->leafNode)
	{

	}
	else
	{
		for (int j = 0; j < DIMENSION; ++j)
		{
			for (int i = 0; i < this->childClusters.size(); ++i)
			{
				childClusters[i].getCF().getLS(varLS);
				tmpVar[j] += varLS[j];
			}
			this->linearSum[j] = tmpVar[j];
		}
	}
}

// Recalculate the Square Sum of the CF
// If the CF is not a leaf node the values can be summed up
// from the child nodes to speed up the process
// Input: --
// Output: --
// Effect: value of SS in a CF change
void ClusteringFeature::calcSquareSum() 
{
	double varSS[DIMENSION];
	double tmpVar[DIMENSION];
	if (this->leafNode)
	{

	}
	else
	{
		for (int j = 0; j < DIMENSION; ++j)
		{
			for (int i = 0; i < this->childClusters.size(); ++i)
			{
				childClusters[i].getCF().getLS(varSS);
				tmpVar[j] += varSS[j];
			}
			this->linearSum[j] = tmpVar[j];
		}
	}
}


std::vector<Point> ClusteringFeature::getClusterOfThisLevel() 
{

}

// Recalculate the Linear Sum & Square Sum of the CF to refresh
// This function is public
// Input: --
// Output: --
// Effect: value of SS & LS in a CF change
void ClusteringFeature::recalculate()
{
	this->calcLinearSum();
	this->calcSquareSum();
}

// Toggeling the value of leafNode in the cluster feature
// Input: --
// Output: --
// Effect: the value of leafNode changes
void ClusteringFeature::changeLeafNode() 
{
	if (this->leafNode)
	{
		this->leafNode = false;
	}
	else {
		this->leafNode = true;
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