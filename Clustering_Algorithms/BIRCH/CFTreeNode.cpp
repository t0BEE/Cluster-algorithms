#include "CFTreeNode.h"

// Constructor of class Cluster
// Input: --
// Output: --
// Effect: New Cluster instance created
CFTreeNode::CFTreeNode() 
{
	this->changeLeafNode(true);
}

Point CFTreeNode::getCentroid()
{
	return this->centroid;
}

// TODO: -- Coment adjustment 
// A point is added to the cluster
// Input: Points
// Output: --
// Effect: cluster variable is changed / increased in size
void CFTreeNode::insertPoint(Point addPoint)
{
	// for leaf nodes
	if (this->isLeafNode)
	{	
		// If not all entries are used place the point in a new entry
		if (this->clustersOfPoints.size() < (B_ENTRIES - 1))
			clustersOfPoints.push_back(Cluster(addPoint));
		// if all entries used place the point within the closest cluster entry
		else
		{	
			double distance = DBL_MAX, tmpDis;
			int closestIndex;
			for (int i = 0; i < B_ENTRIES; ++i)
			{
				// clustersOfPoints[i].calcCentroid();			// don't know if necessary
				tmpDis = calcDistance(addPoint, clustersOfPoints[i].getCentroid());
				if (tmpDis < distance)
				{
					distance = tmpDis;
					closestIndex = i;
				}
			}
			this->clustersOfPoints[closestIndex].addPoint(addPoint);
			this->clustersOfPoints[closestIndex].update();
		}
	}
	else {
		// go down the tree to find a leaf node to place the point
		double distance = DBL_MAX, tmpDis;
		int closestIndex;
		for (int i = 0; i < childCluster.size(); ++i)
		{
			tmpDis = calcDistance(addPoint, childCluster[i].getCentroid());
			if (tmpDis < distance)
			{
				distance = tmpDis;
				closestIndex = i;
			}
		}
		// recursive call for the tree structure
		childCluster[closestIndex].insertPoint(addPoint);
		this->update();

		/* add splits
		|
		|
		|
		|
		|
		|
		|
		|
		*/
	}
}

// The distance is calculated by Euclidean Distance
// Input: 2 Points as References
// Output: double value --> distance
// Effect: --
double CFTreeNode::calcDistance(Point pEins, Point pZwei)
{
	double retValue = 0.0;
	for (int i = 0; i < DIMENSIONS; ++i)
	{
		retValue += pow(pEins.getCoordinate(i) - pZwei.getCoordinate(i), 2.0 );
	}
	return sqrt(retValue);
}

// The Point is searched by finding a Point in the cluster which is "close" enough
// This point will be removed from the cluster
// Input: Point to be removed
// Output: --
// Effect: the cluster variable is changed
void CFTreeNode::removeFromCluster(Point rmPoint)
{
	/*
|
|
|
|
|
|
|
|
*/
	for (unsigned int i = 0; i < cluster.size(); ++i)
	{
		// Difference comparison with 0.0 for doubles leads to errors
		if (abs(calcDistance(rmPoint, cluster[i])) < 0.005) 
		{
			cluster.erase(cluster.begin() + i);
			break;
		}
	}
}

// Get the CF
// Input: --
// Output: CF of the cluster
// Effect: --
ClusteringFeature CFTreeNode::getCF()
{
	return this->cf;
}

// Is the cluster a leaf node ?
// Input: --
// Output: boolean
// Effect: --
bool CFTreeNode::getIsLeafNode()
{
	return this->isLeafNode;
}

// Toggeling the value of leafNode in the cluster feature
// Input: boolean of new value
// Output: --
// Effect: the value of leafNode changes
void CFTreeNode::changeLeafNode(bool value)
{
		this->isLeafNode = value;
}

// Recalculate the Linear Sum & Square Sum of the CF to refresh
// This function is public
// Input: --
// Output: --
// Effect: value of SS & LS in a CF change
void CFTreeNode::recalculateCF()
{
	this->cf.calcLinearSum(this->isLeafNode, this->cluster, this->childCluster);
	this->cf.calcSquareSum(this->isLeafNode, this->cluster, this->childCluster);
}

void CFTreeNode::update()
{
	calcCentroid();
	calcDiameter();
	calcRadius();
}

// Centroid is calculated by defining the average value for each dimension first
// Than the new Position is set for the centroid of the cluster
// Input: --
// Output: --
// Effect: the centroid variable in cluster is changed
void CFTreeNode::calcCentroid()
{
	double newCentroid[DIMENSIONS];
	for (unsigned int i = 0; i < DIMENSIONS; ++i)
	{
		newCentroid[i] = 0.0;
		for (unsigned int j = 0; j < clustersOfPoints.size(); ++j)
		{
			newCentroid[i] += clustersOfPoints[j].getCentroid().getCoordinate(i);
		}
		newCentroid[i] = newCentroid[i] / clustersOfPoints.size();
	}
	this->centroid.setPosition(newCentroid);
}

// This function first recalculates the centroid and uses it to
// measure the average distance between it and all points in the cluster
// Input: --
// Output: --
// Effect: the radius variable in cluster is changed
double CFTreeNode::calcRadius()
{
	this->calcCentroid();
	double varRadius = 0.0;
	for (int i = 0; i < clustersOfPoints.size(); ++i)
	{
		varRadius += pow(calcDistance(clustersOfPoints[i].getCentroid(), this->centroid), 2.0);
	}
	this->radius = sqrt(varRadius / clustersOfPoints.size());
	return this->radius;
}

// This function calculates the average distance between all points in the cluster
// Input: --
// Output: --
// Effect: the diameter variable in cluster is changed
double CFTreeNode::calcDiameter()
{
	double varDiameter = 0.0;
	for (int i = 0; i < clustersOfPoints.size(); ++i)
	{
		for (int j = 0; j < clustersOfPoints.size(); ++j)
		{
			varDiameter += pow(calcDistance(clustersOfPoints[i].getCentroid(), clustersOfPoints[j].getCentroid), 2.0);
		}
	}
	this->diameter = sqrt(varDiameter / (clustersOfPoints.size()*(clustersOfPoints.size() - 1)));
	return this->diameter;
}
