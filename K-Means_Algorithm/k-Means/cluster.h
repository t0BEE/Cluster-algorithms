#pragma once
#include <vector>
#include "point.h"
#include <iostream>

class Cluster
{
	int clusterNumber;
	std::vector<Point> pointList;
	Centroid centroid;
public:
	Cluster(int, Point);
	int getClusternumber();
	void addPoint(Point);
	void removePoint(Point);
	void calcCentroid();
	Point getCentroid();
	void printList();
};
