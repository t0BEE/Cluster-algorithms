#include "ClusteringFeature.h"

ClusteringFeature::ClusteringFeature() 
{
	for (int i = 0; i < dimensions; ++i)
	{
		this->linearSum.push_back(0.0);
		this->squareSum.push_back(0.0);
	}
}

ClusteringFeature::ClusteringFeature(int nrPoints, double* LS, long double* SS)
{
	for (int i = 0; i < dimensions; ++i)
	{
		this->linearSum.push_back(LS[i]);
		this->squareSum.push_back(SS[i]);
	}
	this->numberOfPoints = nrPoints;
}

// Get the Linear Sum of the Clustering Feature
// Input: pointer to transfer the values
// Output: --
// Effect: the buffer contains the linear sum
void ClusteringFeature::getLS(double* buffer)
{
	for (int i = 0; i < dimensions; ++i)
	{
		buffer[i] = this->linearSum[i];
	}
}


// Get the Square Sum of the Clustering Feature
// Input: pointer to transfer the values
// Output: --
// Effect: the buffer contains the square sum
void ClusteringFeature::getSS(long double* buffer)
{
	for (int i = 0; i < dimensions; ++i)
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
	for (int i = 0; i < dimensions; ++i)
	{
		buffer[i] = this->linearSum[i] / numberOfPoints;
	}
}

double ClusteringFeature::calcRadius(ClusteringFeature cfToAdd)
{
	double radius = 0.0;
	double tmpLS[dimensions];
	long double tmpSS[dimensions];
	double inputLS[dimensions];
	long double inputSS[dimensions];
	cfToAdd.getLS(inputLS);
	cfToAdd.getSS(inputSS);
	int totalNrPoints = this->getNumberOfPoints() + cfToAdd.getNumberOfPoints();
	for (int i = 0; i < dimensions; ++i)
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
		// update values
		for (int i = 0; i < dimensions; ++i)
		{
			this->linearSum[i] = this->linearSum[i] + absorbCF.getDimLS(i);
			this->squareSum[i] = this->squareSum[i] + absorbCF.getDimSS(i);
		}
		this->numberOfPoints = this->numberOfPoints + absorbCF.getNumberOfPoints();
		return true;
	}
}

void ClusteringFeature::addToLS(double* buffer)
{
	for (int i = 0; i < dimensions; ++i)
	{
		this->linearSum[i] += buffer[i];
	}
}

void ClusteringFeature::addToSS(long double* buffer)
{
	for (int i = 0; i < dimensions; ++i)
	{
		this->squareSum[i] += buffer[i];
	}
}

void ClusteringFeature::setNumberOfPoints(int number)
{
	this->numberOfPoints = number;
}

double ClusteringFeature::getDimLS(int dimension)
{
	return this->linearSum[dimension];
}

long double ClusteringFeature::getDimSS(int dimension)
{
return this->squareSum[dimension];
}