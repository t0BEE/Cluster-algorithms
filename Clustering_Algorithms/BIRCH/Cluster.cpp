#include "Cluster.h"

Cluster::Cluster(Point initialPoint)
{
	cluster.push_back(initialPoint);
}

Cluster::Cluster() {}

void Cluster::addPoint(Point insert)
{
	this->cluster.push_back(insert);
}
void Cluster::addCluster(Cluster insert)
{
	for (int i = 0; i < insert.cluster.size(); ++i)
	{
		this->cluster.push_back(insert.cluster[i]);
	}
}

void Cluster::removePoint(Point remove)
{
	for (int i = 0; i < cluster.size(); ++i)
	{
		if (calcDistance(cluster[i], remove) < 0.005)
		{
			cluster.erase(cluster.begin() + i);
			break;
		}
	}
}

std::vector<Point> Cluster::getPoints()
{
	return this->cluster;
}

Point Cluster::getCentroid()
{
	return this->centroid;
}

double Cluster::getRadius()
{
	return this->radius;
}

double Cluster::getDiameter()
{
	return this->diameter;
}

void Cluster::update()
{
	calcCentroid();
	calcDiameter();
	calcRadius();
}

// Centroid is calculated by defining the average value for each dimension first
// Than the new Position is set for the centroid of the cluster
// Input: --
// Output: --
// Effect: the centroid variable in cluster is changed
void Cluster::calcCentroid()
{
	double newCentroid[DIMENSIONS];
	for (unsigned int i = 0; i < DIMENSIONS; ++i)
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
double Cluster::calcRadius()
{
	this->calcCentroid();
	double varRadius = 0.0;
	for (int i = 0; i < cluster.size(); ++i)
	{
		varRadius += pow(calcDistance(cluster[i], this->centroid), 2.0);
	}
	this->radius = sqrt(varRadius / cluster.size());
	return this->radius;
}

// This function calculates the average distance between all points in the cluster
// Input: --
// Output: --
// Effect: the diameter variable in cluster is changed
double Cluster::calcDiameter()
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
	return this->diameter;
}

// The distance is calculated by Euclidean Distance
// Input: 2 Points as References
// Output: double value --> distance
// Effect: --
double Cluster::calcDistance(Point pEins, Point pZwei)
{
	double retValue = 0.0;
	for (int i = 0; i < DIMENSIONS; ++i)
	{
		retValue += pow(pEins.getCoordinate(i) - pZwei.getCoordinate(i), 2.0);
	}
	return sqrt(retValue);
}
