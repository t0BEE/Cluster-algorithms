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
	double varLS = 0.0;
	if (this->leafNode)
	{

	}
	else
	{
		for (int i = 0; i < this->childClusters.size(); ++i)
		{
			varLS += childClusters[i].getCF().getLS();
		}
	}
	this->linearSum = varLS;
}

// Recalculate the Square Sum of the CF
// If the CF is not a leaf node the values can be summed up
// from the child nodes to speed up the process
// Input: --
// Output: --
// Effect: value of SS in a CF change
void ClusteringFeature::calcSquareSum() 
{
	double varSS = 0.0;
	if (this->leafNode)
	{

	}
	else
	{
		for (int i = 0; i < this->childClusters.size(); ++i)
		{
			varSS += childClusters[i].getCF().getSS();
		}
	}
	this->squareSum = varSS;
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
// Input: --
// Output: LS of the CF
// Effect: --
double ClusteringFeature::getLS()
{
	return this->linearSum;
}

// Get the Square Sum of the Clustering Feature
// Input: --
// Output: SS of the CF
// Effect: --
double ClusteringFeature::getSS()
{
	return this->squareSum;
}