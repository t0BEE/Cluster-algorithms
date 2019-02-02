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

void Cluster::calcRadius()
{
	this->calcCentroid();
	this->radius = 
}
void Cluster::clacDiameter();
