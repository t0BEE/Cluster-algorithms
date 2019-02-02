#include "point.h"

Point::Point(){}

Point::Point(double x, double y)
{
	this->x = x;
	this->y = y;
	this->clusterNumber = -1;
}

void Point::setCluster(int clusterNumber)
{
	this->clusterNumber = clusterNumber;
}

double Point::getX()
{
	return this->x;
}

double Point::getY()
{
	return this->y;
}

int Point::getCluster()
{
	return this->clusterNumber;
}

void Centroid::setPosition(double x, double y)
{
	this->x = x;
	this->y = y;
}

