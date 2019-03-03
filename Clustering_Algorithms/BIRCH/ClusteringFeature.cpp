#include "ClusteringFeature.h"

ClusteringFeature::ClusteringFeature() {}

// Recalculate the Linear Sum of the CF
// If the CF is not a leaf node the values can be summed up
// from the child nodes to speed up the process
// Input: --
// Output: --
// Effect: value of LS in a CF change
void ClusteringFeature::calcLinearSum(bool isLeaf, std::vector<Cluster> cluster, std::vector<CFTreeNode> childCluster)
{
	double varLS[DIMENSIONS];
	double tmpVar[DIMENSIONS]; // use as buffer to store the solutions inside
	if (isLeaf)
	{
		for (int j = 0; j < DIMENSIONS; ++j)
		{
			varLS[j] = 0.0;
			for (int i = 0; i < cluster.size(); ++i)
			{
				std::vector<Point> vaCluster = cluster[i].getPoints();
				for (int k = 0; k < vaCluster.size(); ++k)
				{
				varLS[j] += vaCluster[k].getCoordinate(j);
				}
			}
			this->linearSum[j] = varLS[j];
		}
	}
	else
	{
	for (int j = 0; j < DIMENSIONS; ++j)
	{
		varLS[j] = 0.0;
		for (int i = 0; i < childCluster.size(); ++i)
		{
			childCluster[i].getCF().getLS(tmpVar);
			varLS[j] += tmpVar[j];
		}
		this->linearSum[j] = varLS[j];
	}
	}
}

// Recalculate the Square Sum of the CF
// If the CF is not a leaf node the values can be summed up
// from the child nodes to speed up the process
// Input: --
// Output: --
// Effect: value of SS in a CF change
void ClusteringFeature::calcSquareSum(bool isLeaf, std::vector<Cluster> cluster, std::vector<CFTreeNode> childCluster)
{
	double varSS[DIMENSIONS];
	double tmpVar[DIMENSIONS]; // use as buffer to store the solutions inside
	if (isLeaf)
	{
		for (int j = 0; j < DIMENSIONS; ++j)
		{
			varSS[j] = 0.0;
			for (int i = 0; i < cluster.size(); ++i)
			{
				std::vector<Point> vaCluster = cluster[i].getPoints();
				for (int k = 0; k < vaCluster.size(); ++k)
				{
					varSS[j] += pow(vaCluster[k].getCoordinate(j), 2.0);
				}
			}
			this->linearSum[j] = varSS[j];
		}
	}
	else
	{
		for (int j = 0; j < DIMENSIONS; ++j)
		{
			varSS[j] = 0.0;
			for (int i = 0; i < childCluster.size(); ++i)
			{
				childCluster[i].getCF().getSS(tmpVar);
				varSS[j] += tmpVar[j];
			}
			this->squareSum[j] = varSS[j];
		}
	}
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
	double tmpLS[DIMENSIONS], tmpSS[DIMENSIONS];
	double inputLS[DIMENSIONS], inputSS[DIMENSIONS];
	cfToAdd.getLS(inputLS);
	cfToAdd.getSS(inputSS);
	int totalNrPoints = this->getNumberOfPoints() + cfToAdd.getNumberOfPoints();
	for (int i = 0; i < DIMENSIONS; ++i)
	{
		tmpLS[i] = this->linearSum[i] + inputLS[i];
		tmpSS[i] = this->squareSum[i] + inputSS[i];
	}
	for (int i = 0; i < DIMENSIONS; ++i)
	{
		radius += tmpSS[i] - (2 * tmpLS[i] * tmpLS[i] / totalNrPoints) + (tmpLS[i]);
		radius = sqrt(radius / totalNrPoints);
	}
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
		double inputLS[DIMENSIONS], inputSS[DIMENSIONS];
		absorbCF.getLS(inputLS);
		absorbCF.getSS(inputSS);
		for (int i = 0; i < DIMENSIONS; ++i)
		{
			linearSum[i] = this->linearSum[i] + inputLS[i];
			squareSum[i] = this->squareSum[i] + inputSS[i];
		}
		return true;
	}
}