#pragma once

#include "Parameter.h"
#include "Point.h"
#include <vector>

class Cluster
{
private:
	std::vector<Point> cluster;
	double radius;
	double diameter;
	Centroid centroid;
public:
	Cluster(Point initialPoint);
	Cluster();
	void addPoint(Point insert);
	void addCluster(Cluster insert);
	void removePoint(Point remove);
	std::vector<Point> getPoints();

	void update();
	void calcCentroid();
	double calcRadius();
	double calcDiameter();
	double calcDistance(Point pEins, Point pZwei);

	Point getCentroid();
	double getRadius();
	double getDiameter();
};