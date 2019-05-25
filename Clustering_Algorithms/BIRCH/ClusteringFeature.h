#pragma once

#include "Parameter.h"


class ClusteringFeature 
{
private:
	int numberOfPoints;
	std::vector<double> linearSum;
	std::vector<double> squareSum;
	double calcRadius(ClusteringFeature cfToAdd);
public:
	ClusteringFeature();
	ClusteringFeature(int nrPoints, double* LS, double* SS);
	void getLS(double* buffer);
	void getSS(double* buffer);
	int getNumberOfPoints();
	void calcCentroid(double* buffer);
	bool absorbCF(ClusteringFeature absorbCF);
	void addToLS(double* buffer);
	void addToSS(double* buffer);
	void setNumberOfPoints(int number);
};

