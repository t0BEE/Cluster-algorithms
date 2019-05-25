#include "kCluster.h"

// Constructor of class Cluster
// Input: Integer for cluster number, a point as starting centroid
// Output: --
// Effect: New Cluster instance created with number and centroid set
Cluster::Cluster(int clusterNumber, Point startCentroid)
{
	double startingPosition[DIMENSIONS];
	startCentroid.getCoordinates(startingPosition);
	this->clusterNumber = clusterNumber;
	this->centroid = Centroid();
	this->centroid.setPosition(startingPosition);
}

// Get cluster number
// Input: --
// Output: integer cluster number
// Effect: --
int Cluster::getClusternumber()
{
	return this->clusterNumber;
}

// Add a point to the cluster
// Input: point to add
// Output: --
// Effect: pointList in the cluster is increased in size
void Cluster::addPoint(Point insert)
{
    #pragma omp critical (changeCluster)
    {
        insert.setCluster(this->clusterNumber);
        pointList.push_back(insert);
    }
}

// Remove a point from the cluster
// Comparison is based on the distance and calculating distances
// with doubles may lead to problems
// Therefore find a really close point and remove it
// Input: point to remove
// Output: --
// Effect: pointList in the cluster is decreased in size
void Cluster::removePoint(const Point& remove)
{
    #pragma omp critical (changeCluster)
	for (unsigned int i = 0; i < pointList.size(); ++i)
	{
		if (std::abs(calcPointDistance(remove, pointList[i])) < 0.005)
		{
			pointList.erase(pointList.begin() + i);
			break;
		}
	}
}

// Get the cluster's centroid
// Input: --
// Output: centroid of the cluster as point type
// Effect: --
Point Cluster::getCentroid()
{
	return this->centroid;
}

// Print cluster information (number, size and centroid coordinates)
// Input: --
// Output: --
// Effect: Information is printed in console
void Cluster::printList()
{
	std::cout << "--- Cluster " << this->clusterNumber << " --- " << "Number of points: " << this->pointList.size() << " --  (";
	for (int j = 0; j < DIMENSIONS; ++j)
	{
		std::cout << centroid.getCoordinate(j);
		if (j < (DIMENSIONS - 1)) std::cout << "|";
	}
	std::cout << ")" << std::endl;
	std::for_each(pointList.begin(), pointList.end(),[](Point point)
	{
		std::cout << "(";
		for (int j = 0; j < DIMENSIONS; ++j)
		{
			std::cout << point.getCoordinate(j);
			if (j < (DIMENSIONS - 1)) std::cout << "|";
		}	
		std::cout << ")" << std::endl;
	});
}

// Calculate the centroid for a cluster
// By calculating each average dimension value
// Input: --
// Output: --
// Effect: coordinates of centroid of cluster have changed
void Cluster::calcCentroid()
{
	double coordinates[DIMENSIONS];
	for (int i = 0; i < DIMENSIONS; ++i)
	{
		coordinates[i] = 0.0;
		for (int j = 0; j < pointList.size(); ++j)
		{
			coordinates[i] += pointList[i].getCoordinate(i);
		}
		coordinates[i] = coordinates[i] / pointList.size();
	} 
	
	this->centroid.setPosition(coordinates);
}
