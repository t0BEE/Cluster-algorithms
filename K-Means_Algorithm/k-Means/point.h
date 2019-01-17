#pragma once
#include <vector>

class Point
{
	int clusterNumber;
protected:
	double x;
	double y;
public:
	Point();
	Point(double, double);
	void setCluster(int);
	double getX();
	double getY();
	int getCluster();
};

class Centroid: public Point
{
public:
	void setPosition(double, double);
};
