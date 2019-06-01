#pragma once

#include "Parameter.h"


class ClusteringFeature 
{
private:
	int numberOfPoints;
	std::vector<double> linearSum;
	std::vector<long double> squareSum;
	double calcRadius(ClusteringFeature cfToAdd);
public:
	ClusteringFeature();
	ClusteringFeature(int nrPoints, double* LS, long double* SS);
	void getLS(double* buffer);
	void getSS(long double* buffer);
	double getDimLS(int dimension);
	long double getDimSS(int dimension);
	int getNumberOfPoints();
	void calcCentroid(double* buffer);
	bool absorbCF(ClusteringFeature absorbCF, int treeID);
	void addToLS(double* buffer);
	void addToSS(long double* buffer);
	void setNumberOfPoints(int number);
};

