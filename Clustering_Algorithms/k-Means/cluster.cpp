#include "cluster.h"
#include "kMeans.h"

Cluster::Cluster(int clusterNumber, Point startCentroid)
{
	this->clusterNumber = clusterNumber;
	this->centroid = Centroid();
	this->centroid.setPosition(startCentroid.getX(), startCentroid.getY());
}

int Cluster::getClusternumber()
{
	return this->clusterNumber;
}

void Cluster::addPoint(Point insert)
{
	insert.setCluster(this->clusterNumber);
	pointList.push_back(insert);
}

void Cluster::removePoint(Point remove)
{
	for (unsigned int i = 0; i < pointList.size(); ++i)
	{
		if (abs(calcDistance(remove, pointList[i])) < 0.005) // comparison with 0.0 does not work well
		{
			pointList.erase(pointList.begin() + i);
			break;
		}
	}
}

Point Cluster::getCentroid()
{
	return this->centroid;
}

void Cluster::printList()
{
	std::cout << "--- Cluster " << this->clusterNumber << " --- "<< "Number of points: " << this->pointList.size() << " --  (" << this->centroid.getX() << "|" << this->centroid.getY() << ")" << std::endl;
	std::for_each(pointList.begin(), pointList.end(),[](Point point)
	{
		std::cout << "(" << point.getX() << "|" << point.getY() << ")" << std::endl;
	});
}

void Cluster::calcCentroid()
{
	double x=0, y=0;
	for(auto &point : pointList)
	{
		x += point.getX();
		y += point.getY();
	}
	x = x / pointList.size();
	y = y / pointList.size();
	this->centroid.setPosition(x, y);
}
