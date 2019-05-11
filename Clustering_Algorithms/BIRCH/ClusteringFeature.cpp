#include "ClusteringFeature.h"

ClusteringFeature::ClusteringFeature() 
{
	for (int i = 0; i < DIMENSIONS; ++i)
	{
		this->linearSum[i] = 0;
		this->squareSum[i] = 0;
	}
}

ClusteringFeature::ClusteringFeature(int nrPoints, double* LS, double* SS)
{
	for (int i = 0; i < DIMENSIONS; ++i)
	{
		this->linearSum[i] = LS[i];
		this->squareSum[i] = SS[i];
	}
	this->numberOfPoints = nrPoints;
}

// Get the Linear Sum of the Clustering Feature
// Input: pointer to transfer the values
// Output: --
// Effect: the buffer contains the linear sum
void ClusteringFeature::getLS(double* buffer)
{
	for (int i = 0; i < DIMENSIONS; ++i)
	{
		buffer[i] = this->linearSum[i];
	}
}

// Get the Square Sum of the Clustering Feature
// Input: pointer to transfer the values
// Output: --
// Effect: the buffer contains the square sum
void ClusteringFeature::getSS(double* buffer)
{
	for (int i = 0; i < DIMENSIONS; ++i)
	{
		buffer[i] = this->squareSum[i];
	}
}

int ClusteringFeature::getNumberOfPoints()
{
	return this->numberOfPoints;
}

void ClusteringFeature::calcCentroid(double* buffer)
{
	for (int i = 0; i < DIMENSIONS; ++i)
	{
		buffer[i] = this->linearSum[i] / numberOfPoints;
	}
}

double ClusteringFeature::calcRadius(ClusteringFeature cfToAdd)
{
	double radius = 0.0;
	double tmpLS[DIMENSIONS];
	double tmpSS[DIMENSIONS];
	double inputLS[DIMENSIONS];
	double inputSS[DIMENSIONS];
	cfToAdd.getLS(inputLS);
	cfToAdd.getSS(inputSS);
	int totalNrPoints = this->getNumberOfPoints() + cfToAdd.getNumberOfPoints();
	for (int i = 0; i < DIMENSIONS; ++i)
	{
		tmpLS[i] = this->linearSum[i] + inputLS[i];
		tmpSS[i] = this->squareSum[i] + inputSS[i];
		radius += tmpSS[i] - (tmpLS[i] * tmpLS[i] / totalNrPoints);
	}
	radius = sqrt(radius / totalNrPoints);
	return radius;
}

bool ClusteringFeature::absorbCF(ClusteringFeature absorbCF)
{
	if (calcRadius(absorbCF) > threshold_Value)
	{
		return false;
	}
	else
	{
		// Maybe already set in calcRadius
		// update values
		double inputLS[DIMENSIONS], inputSS[DIMENSIONS];
		absorbCF.getLS(inputLS);
		absorbCF.getSS(inputSS);
		for (int i = 0; i < DIMENSIONS; ++i)
		{
			this->linearSum[i] = this->linearSum[i] + inputLS[i];
			this->squareSum[i] = this->squareSum[i] + inputSS[i];
		}
		this->numberOfPoints++;
		return true;
	}
}

void ClusteringFeature::addToLS(double* buffer)
{
	for (int i = 0; i < DIMENSIONS; ++i)
	{
		this->linearSum[i] += buffer[i];
	}
}

void ClusteringFeature::addToSS(double* buffer) 
{
	for (int i = 0; i < DIMENSIONS; ++i)
	{
		this->squareSum[i] += buffer[i];
	}
}

void ClusteringFeature::setNumberOfPoints(int number)
{
	this->numberOfPoints = number;
}
