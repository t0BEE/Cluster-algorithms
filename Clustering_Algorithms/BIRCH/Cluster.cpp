#include "Cluster.h"

// Constructor of class Cluster
// Input: --
// Output: --
// Effect: New Cluster instance created
Cluster::Cluster() {}

// A point is added to the cluster
// Input: Points
// Output: --
// Effect: cluster variable is changed / increased in size
void Cluster::addToCluster(Point addPoint)
{
	this->cluster.push_back(addPoint);
}

// The distance is calculated by Euclidean Distance
// Input: 2 Points as References
// Output: double value --> distance
// Effect: --
double Cluster::calcDistance(Point& pEins, Point& pZwei)
{
	double retValue = 0.0;
	for (int i = 0; i < DIMENSION; ++i)
	{
		retValue += pow(pEins.getCoordinate(i) - pZwei.getCoordinate(i), 2.0 );
	}
	return sqrt(retValue);
}

// The Point is searched by finding a Point in the cluster which is "close" enough
// This point will be removed from the cluster
// Input: Point to be removed
// Output: --
// Effect: the cluster variable is changed
void Cluster::removeFromCluster(Point rmPoint)
{
	for (unsigned int i = 0; i < cluster.size(); ++i)
	{
		// Difference comparison with 0.0 for doubles leads to errors
		if (abs(calcDistance(rmPoint, cluster[i])) < 0.005) 
		{
			cluster.erase(cluster.begin() + i);
			break;
		}
	}
}

// Centroid is calculated by defining the average value for each dimension first
// Than the new Position is set for the centroid of the cluster
// Input: --
// Output: --
// Effect: the centroid variable in cluster is changed
void Cluster::calcCentroid()
{
	double newCentroid[DIMENSION];
	for (unsigned int i = 0; i < DIMENSION; ++i)
	{
		newCentroid[i] = 0.0;
		for (unsigned int j = 0; j < cluster.size(); ++j)
		{
			newCentroid[i] += cluster[j].getCoordinate(i);
		}
		newCentroid[i] = newCentroid[i] / cluster.size();
	}
	this->centroid.setPosition(newCentroid);
}

// This function first recalculates the centroid and uses it to
// measure the average distance between it and all points in the cluster
// Input: --
// Output: --
// Effect: the radius variable in cluster is changed
void Cluster::calcRadius()
{
	this->calcCentroid();
	double varRadius = 0.0;
	for (int i = 0; i < cluster.size(); ++i)
	{
		varRadius += pow(calcDistance(cluster[i], this->centroid), 2.0);
	}
	this->radius = sqrt(varRadius / cluster.size());
}

// This function calculates the average distance between all points in the cluster
// Input: --
// Output: --
// Effect: the diameter variable in cluster is changed
void Cluster::calcDiameter()
{
	double varDiameter = 0.0;
	for (int i = 0; i < cluster.size(); ++i)
	{
		for (int j = 0; j < cluster.size(); ++j)
		{
			varDiameter += pow(calcDistance(cluster[i], cluster[j]), 2.0);
		}
	}
	this->diameter = sqrt(varDiameter / (cluster.size()*(cluster.size() - 1)));
}

// Get the CF
// Input: --
// Output: CF of the cluster
// Effect: --
ClusteringFeature Cluster::getCF()
{
	return this->cf;
}

// Is the cluster a leaf node ?
// Input: --
// Output: boolean
// Effect: --
bool Cluster::getIsLeafNode()
{
	return this->isLeafNode;
}

// Toggeling the value of leafNode in the cluster feature
// Input: boolean of new value
// Output: --
// Effect: the value of leafNode changes
void Cluster::changeLeafNode(bool value)
{
		this->isLeafNode = value;
}

// Recalculate the Linear Sum & Square Sum of the CF to refresh
// This function is public
// Input: --
// Output: --
// Effect: value of SS & LS in a CF change
void Cluster::recalculateCF()
{
	this->cf.calcLinearSum(this->isLeafNode, this->cluster, this->childCluster);
	this->cf.calcSquareSum(this->isLeafNode, this->cluster, this->childCluster);
}