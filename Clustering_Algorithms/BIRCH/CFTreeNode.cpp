#include "CFTreeNode.h"

// Constructor of class Cluster
// Input: --
// Output: --
// Effect: New Cluster instance created
CFTreeNode::CFTreeNode() 
{
	this->changeLeafNode(true);
}

CFTreeNode::CFTreeNode(Cluster startingCluster)
{
	this->clustersInLeafNode.push_back(startingCluster);
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
		insertToLeaf(addPoint);
	}
	// go down the tree recursively to find a leaf node to place the point
	else {
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


void CFTreeNode::removeFromNode(int clusterIndex)
{
	this->clustersInLeafNode.erase(clustersInLeafNode.begin() + clusterIndex);
}


CFTreeNode CFTreeNode::insertToLeaf(Point addPoint)
{

	double distance = DBL_MAX, tmpDis;
	int closestIndex = 0;
	for (int i = 0; i < clustersInLeafNode.size(); ++i)
	{
		tmpDis = calcDistance(addPoint, clustersInLeafNode[i].getCentroid());
		if (tmpDis < distance)
		{
			distance = tmpDis;
			closestIndex = i;
		}
	}
	if (clustersInLeafNode.size() == 0)
	clustersInLeafNode.push_back(Cluster(addPoint));
	this->clustersInLeafNode[closestIndex].addPoint(addPoint);
	this->clustersInLeafNode[closestIndex].update();
	// Check if threshold condition is still valid
	if (this->clustersInLeafNode[closestIndex].getRadius() > threshold_Value)
	{
		// If not --> remove point again and place anywhere else
		this->clustersInLeafNode[closestIndex].removePoint(addPoint);
		this->clustersInLeafNode[closestIndex].update();
		// If not all entries are used in node place the point in a new entry
		if (this->clustersInLeafNode.size() < (L_ENTRIES - 1))
		{ 
			clustersInLeafNode.push_back(Cluster(addPoint));
			return;
		}
		// If the Leaf Node is full --> split and create new node
		else
		{	
			// choose farthest pair of entries
			int far1, far2;
			double farDis = 0.0, tmpDis;
			for (int i = 0; i < clustersInLeafNode.size(); ++i)
			{
				for (int j = 0; j < clustersInLeafNode.size(); ++j)
				{
					tmpDis = calcDistance(clustersInLeafNode[i].getCentroid(), clustersInLeafNode[j].getCentroid());
					if (tmpDis > farDis)
					{
						farDis = tmpDis;
						if (i < j)
						{
							far1 = i;
							far2 = j;
						}
						else {
							far1 = j;
							far2 = i;
						}
						// far1 should be the smaller index because it will be removed after far2
					}
				}
			}
			// create new Leaf Node and assign the rest entries to each
			// TDOD
			std::vector<Cluster> tmpClusters;
			CFTreeNode newNode = CFTreeNode(this->clustersInLeafNode[far2]);
			tmpClusters.push_back(this->clustersInLeafNode[far1]);
			this->removeFromNode(far2);
			this->removeFromNode(far1);
			int currentLength = this->clustersInLeafNode.size();
			for (int i = currentLength - 1; i >= 0;  --i)
			{
				tmpClusters.push_back(this->clustersInLeafNode[i]);
				this->clustersInLeafNode.pop_back();
			}
			this->clustersInLeafNode.push_back(tmpClusters.front());
			tmpClusters.erase(tmpClusters.begin());
		// at this point tmpCluster contains all data points except the farthest
		// which are seperated between the 2 CFTreeNodes
			newNode.update();
			this->update();
			currentLength = tmpClusters.size();
			for (int i = currentLength - 1; i >=0; --i)
			{
				if (calcDistance(tmpClusters[i].getCentroid(), newNode.getCentroid()) <= calcDistance(tmpClusters[i].getCentroid(), this->centroid))
				{
					newNode.clustersInLeafNode.push_back(tmpClusters[i]);
					newNode.update();
				}
				else
				{
					this->clustersInLeafNode.push_back(tmpClusters[i]);
					this->update();
				}
				tmpClusters.pop_back();
			}
			return newNode;
		}
	}
	return;
	
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
		for (unsigned int j = 0; j < clustersInLeafNode.size(); ++j)
		{
			newCentroid[i] += clustersInLeafNode[j].getCentroid().getCoordinate(i);
		}
		newCentroid[i] = newCentroid[i] / clustersInLeafNode.size();
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
	for (int i = 0; i < clustersInLeafNode.size(); ++i)
	{
		varRadius += pow(calcDistance(clustersInLeafNode[i].getCentroid(), this->centroid), 2.0);
	}
	this->radius = sqrt(varRadius / clustersInLeafNode.size());
	return this->radius;
}

// This function calculates the average distance between all points in the cluster
// Input: --
// Output: --
// Effect: the diameter variable in cluster is changed
double CFTreeNode::calcDiameter()
{
	double varDiameter = 0.0;
	for (int i = 0; i < clustersInLeafNode.size(); ++i)
	{
		for (int j = 0; j < clustersInLeafNode.size(); ++j)
		{
			varDiameter += pow(calcDistance(clustersInLeafNode[i].getCentroid(), clustersInLeafNode[j].getCentroid), 2.0);
		}
	}
	this->diameter = sqrt(varDiameter / (clustersInLeafNode.size()*(clustersInLeafNode.size() - 1)));
	return this->diameter;
}
