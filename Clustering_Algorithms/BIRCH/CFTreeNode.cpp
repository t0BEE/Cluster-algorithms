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
CFTreeNode CFTreeNode::insertPoint(Point addPoint)
{
	CFTreeNode newNode;
	// for leaf nodes
	if (this->isLeafNode)
	{	
		// if a split occurs a new CFTreeNode is returned
		newNode = insertToLeaf(addPoint);
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
		// if a split occurs a new CFTreeNode is returned
		newNode = childCluster[closestIndex].insertPoint(addPoint);
		this->update();
		// It is not possible to retun NULL, so if no split occurs, the insertPoint function returns a node without clusters
		// Second part is necessary for leaf nodes
		if ((newNode.getNumberOfEntries() != 0) || (newNode.getNumberOfClusterEntries() != 0)) 
		{
			current_tree_size++;
			if (newNode.getNumberOfEntries() != 0)
				newNode.changeLeafNode(false);

			// Check if threshold is broken /  split than
			this->childCluster.push_back(newNode);
			if (this->childCluster[closestIndex].calcRadius() > threshold_Value)
			{
				// If threshold is broken --> split this too
				newNode = childCluster[closestIndex].splitNonLeaf();
			}
			// TODO :: else do the merging refinement
		}
	}
	// If the returned new Node is not empty --> add it to the node

	// recycle newNode variable
	return newNode;
}

CFTreeNode CFTreeNode::splitNonLeaf()
{
	// choose farthest pair of entries
	int far1, far2;
	double farDis = 0.0, tmpDis;
	for (int i = 0; i < childCluster.size(); ++i)
	{
		for (int j = 0; j < childCluster.size(); ++j)
		{
			tmpDis = calcDistance(childCluster[i].getCentroid(), childCluster[j].getCentroid());
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
	// create new Node and assign the rest entries to each
	std::vector<CFTreeNode> tmpTreeNode;
	CFTreeNode newNode = CFTreeNode();
	newNode.insertNode(this->childCluster[far2]);
	tmpTreeNode.push_back(this->childCluster[far1]);
	this->removeFromNode(far2);
	this->removeFromNode(far1);
	int currentLength = this->childCluster.size();
	for (int i = currentLength - 1; i >= 0; --i)
	{
		tmpTreeNode.push_back(this->childCluster[i]);
		this->childCluster.pop_back();
	}
	this->childCluster.push_back(tmpTreeNode.front());
	tmpTreeNode.erase(tmpTreeNode.begin());
	// at this point tmpCluster contains all data points except the farthest
	// which are seperated between the 2 CFTreeNodes
	newNode.update();
	this->update();
	currentLength = tmpTreeNode.size();
	for (int i = currentLength - 1; i >= 0; --i)
	{
		// Compare the distatnces of the centroids and assign clusters to closest
		if (calcDistance(tmpTreeNode[i].getCentroid(), newNode.getCentroid()) <= calcDistance(tmpTreeNode[i].getCentroid(), this->centroid))
		{
			newNode.childCluster.push_back(tmpTreeNode[i]);
			newNode.update();
		}
		else
		{
			this->childCluster.push_back(tmpTreeNode[i]);
			this->update();
		}
		tmpTreeNode.pop_back();
	}
	return newNode;
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

// because childCluster is not public, this function needs to exist
CFTreeNode CFTreeNode::insertNode(CFTreeNode addNode)
{
	this->childCluster.push_back(addNode);
}


void CFTreeNode::removeFromNode(int clusterNodeIndex)
{
	if (this->isLeafNode)
	{
		this->clustersInLeafNode.erase(clustersInLeafNode.begin() + clusterNodeIndex);
	}
	else
	{
		this->childCluster.erase(childCluster.begin() + clusterNodeIndex);
	}
}

CFTreeNode CFTreeNode::insertToLeaf(Point addPoint)
{
	double distance = DBL_MAX, tmpDis;
	int closestIndex = 0;
	// Get index of closest cluster ( meassured by point - centroid )
	for (int i = 0; i < clustersInLeafNode.size(); ++i)
	{
		tmpDis = calcDistance(addPoint, clustersInLeafNode[i].getCentroid());
		if (tmpDis < distance)
		{
			distance = tmpDis;
			closestIndex = i;
		}
	}
	// If no cluster in leaf create new one
	if (clustersInLeafNode.size() == 0)
	{ 
		clustersInLeafNode.push_back(Cluster(addPoint));
	}
	else {
	this->clustersInLeafNode[closestIndex].addPoint(addPoint);
	}
	this->clustersInLeafNode[closestIndex].update();
	// Check if threshold condition is still valid
	if (this->clustersInLeafNode[closestIndex].getRadius() > threshold_Value)
	{
		// If not --> remove point again and place anywhere else
		this->clustersInLeafNode[closestIndex].removePoint(addPoint);
		this->clustersInLeafNode[closestIndex].update();
		// If not all entries are used in node, place the point in a new entry
		if (this->clustersInLeafNode.size() < (L_ENTRIES - 1))
		{ 
			clustersInLeafNode.push_back(Cluster(addPoint));
			return;
		}
		// If the Leaf Node is full --> split and create new node
		else
		{	
			CFTreeNode newNode = splitLeaf();
			this->next = &newNode;
			newNode.prev = this;
			return newNode;
		}
	}
	return CFTreeNode();
	
}

CFTreeNode CFTreeNode::splitLeaf()
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
				// otherwise the bigger number will be changed when erase the smaller one
			}
		}
	}
	// create new Leaf Node and assign the rest entries to each
	std::vector<Cluster> tmpClusters; // this is a temporary vector to help sorting
	CFTreeNode newNode = CFTreeNode(this->clustersInLeafNode[far2]);
	tmpClusters.push_back(this->clustersInLeafNode[far1]);
	this->removeFromNode(far2);
	this->removeFromNode(far1);
	int currentLength = this->clustersInLeafNode.size();
	for (int i = currentLength - 1; i >= 0; --i)
	{
		tmpClusters.push_back(this->clustersInLeafNode[i]);
		this->clustersInLeafNode.pop_back();
	}
	// Put far1 to the clustersInLeafNode of this node
	this->clustersInLeafNode.push_back(tmpClusters.front());
	tmpClusters.erase(tmpClusters.begin());
	// at this point tmpCluster contains all data points except the farthest
	// which are seperated between the 2 CFTreeNodes
	newNode.update();
	this->update();
	currentLength = tmpClusters.size();
	for (int i = currentLength - 1; i >= 0; --i)
	{
		// Compare the distatnces of the centroids and assign clusters to closest
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
	this->cf.calcLinearSum(this->isLeafNode, this->clustersInLeafNode, this->childCluster);
	this->cf.calcSquareSum(this->isLeafNode, this->clustersInLeafNode, this->childCluster);
}

void CFTreeNode::update()
{
	calcCentroid();
	calcDiameter();
	calcRadius();
	recalculateCF();
}

// Centroid is calculated by defining the average value for each dimension first
// Than the new Position is set for the centroid of the cluster
// Input: --
// Output: --
// Effect: the centroid variable in cluster is changed
void CFTreeNode::calcCentroid()
{
	if ( this->isLeafNode)
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
	else
	{
		double newCentroid[DIMENSIONS];
		for (unsigned int i = 0; i < DIMENSIONS; ++i)
		{
			newCentroid[i] = 0.0;
			for (unsigned int j = 0; j < childCluster.size(); ++j)
			{
				newCentroid[i] += childCluster[j].getCentroid().getCoordinate(i);
			}
			newCentroid[i] = newCentroid[i] / childCluster.size();
		}
		this->centroid.setPosition(newCentroid);
	}
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

int CFTreeNode::getNumberOfEntries()
{
	return this->childCluster.size();
}

int CFTreeNode::getNumberOfClusterEntries()
{
	return this->clustersInLeafNode.size();
}
