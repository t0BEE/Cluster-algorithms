#pragma once
#include <vector>
#include "kPoint.h"
#include <iostream>
#include <algorithm>

class Cluster
{
	int clusterNumber;
	std::vector<Point> pointList;
	Centroid centroid;
public:
	Cluster(int, Point);
	int getClusternumber();
	void addPoint(Point);
	void removePoint(const Point&);
	void calcCentroid();
	Point getCentroid();
	void printList();
};
