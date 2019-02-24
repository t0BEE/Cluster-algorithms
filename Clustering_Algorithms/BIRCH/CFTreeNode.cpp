#include "CFTreeNode.h"

/**
 * Constructor of class Cluster
 * Input: --
 * Output: --
 * Effect: New Cluster instance created and set to Leaf Node
 */
CFTreeNode::CFTreeNode() 
{
	this->changeLeafNode(true);
}

/**
 * Constructor of class Cluster
 * Input: first Cluster of Points (Cluster)
 * Output: --
 * Effect: New Cluster instance created with first cluster and set to Leaf Node
*/
CFTreeNode::CFTreeNode(Cluster startingCluster)
{
	this->clustersInLeafNode.push_back(startingCluster);
	this->changeLeafNode(true);
}

/**
 * Return the Centroid
 * Input: --
 * Output: Centroid (Point)
 * Effect: --
 */
Point CFTreeNode::getCentroid()
{
	return this->centroid;
}

/**
 * A point is added to the tree recursivly
 * If it reaches a leaf node, the point is added to a cluster
 * Output has either empty vectors or non empty ones in case of splits
 * Input: Point to insert (Point)
 * Output: New Node, empty if no split occured (CFTreeNode)
 * Effect: cluster variable is changed / increased in size
 */
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
		// GetNumberOfClusterEntries is necessary to identify leafNodes
		if ((newNode.getNumberOfChildEntries() != 0) || (newNode.getNumberOfClusterEntries() != 0)) 
		{
			current_tree_size++;
			if (newNode.getNumberOfChildEntries() != 0)
				newNode.changeLeafNode(false);
			if (this->childCluster.size() <= B_ENTRIES)
			{
				this->childCluster.push_back(newNode);
				// Check if threshold is broken / split than
				if (this->calcRadius() > threshold_Value)
				{
					// If threshold is broken --> split this too
					newNode = this->splitNonLeaf();
				}
				else
				{ // TODO :: else do the merging refinement
					newNode = CFTreeNode();
				}
			}
			else
			{
				// Split because no space is left
				this->childCluster.push_back(newNode);
				newNode = this->splitNonLeaf();
			}
		}
	}
	return newNode;
}

/**
 * A leaf node is split into two 
 * The points are seperated and assigned to them
 * Input: --
 * Output: New Leaf Node (CFTreeNode)
 * Effect: New Node created and Points are torn between both
*/
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
		if (newNode.childCluster.size() == B_ENTRIES)
		{
			this->childCluster.push_back(tmpTreeNode[i]);
			this->update();
		}
		else
		{
			// Compare the distatnces of the centroids and assign clusters to closest
			if ((this->childCluster.size() == B_ENTRIES) || (calcDistance(tmpTreeNode[i].getCentroid(), newNode.getCentroid()) <= calcDistance(tmpTreeNode[i].getCentroid(), this->centroid) && (newNode.childCluster.size() <= B_ENTRIES)))
			{
				newNode.childCluster.push_back(tmpTreeNode[i]);
				newNode.update();
			}
			else
			{
				this->childCluster.push_back(tmpTreeNode[i]);
				this->update();
			}
		}
		tmpTreeNode.pop_back();
	}
	return newNode;
}

/**
 * The distance is calculated by Euclidean Distance
 * Input: 2 Points as References (Point)
 * Output: Distance (double)
 * Effect: --
*/
double CFTreeNode::calcDistance(Point pEins, Point pZwei)
{
	double retValue = 0.0;
	for (int i = 0; i < DIMENSIONS; ++i)
	{
		retValue += pow(pEins.getCoordinate(i) - pZwei.getCoordinate(i), 2.0 );
	}
	return sqrt(retValue);
}

/**
 * Add a child node to a node
 * Because childCluster is not public, this function needs to exist
 * Input: Node to add to childCluster (CFTreeNode)
 * Output: --
 * Effect: Increase size of childCluster
*/
CFTreeNode CFTreeNode::insertNode(CFTreeNode addNode)
{
	if (this->childCluster.size() <= B_ENTRIES)
	{
		this->childCluster.push_back(addNode);
	}
	else
	{
		return this->splitNonLeaf();
	}	
}

/**
 * Remove a child node from a Node
 * If Leaf Node --> remove Cluster
 * If Non Leaf Node --> remove child Node
 * Input: Index to remove (int)
 * Output: --
 * Effect: --
*/
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

/**
 * Insert a point to a leaf cluster
 * Input: point to add (Point)
 * Output: Node which is non empty if the leaf is full and a split occurs (CFTreeNode)
 * Effect: --
*/
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
	// If there is no cluster in leaf node, create a new one
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
			CFTreeNode newNode = this->splitLeaf();
			this->next = &newNode;
			newNode.prev = this;
			return newNode;
		}
	}
	return CFTreeNode();	
}


/**
 * Insert a cluster to a leaf node
 * Input: cluster to add (cluster)
 * Output: Node which is non empty if the leaf is full and a split occurs (CFTreeNode)
 * Effect: --
*/
CFTreeNode CFTreeNode::insertToLeaf(Cluster addCluster)
{
	double distance = DBL_MAX, tmpDis;
	int closestIndex = 0;
	Cluster thresholdCluster = Cluster();
	// Get index of closest cluster ( meassured by point - centroid )
	for (int i = 0; i < clustersInLeafNode.size(); ++i)
	{
		tmpDis = calcDistance(addCluster.getCentroid(), clustersInLeafNode[i].getCentroid());
		if (tmpDis < distance)
		{
			distance = tmpDis;
			closestIndex = i;
		}
	}
	// If there is no cluster in leaf node, create a new one
	if (clustersInLeafNode.size() == 0)
	{
		clustersInLeafNode.push_back(addCluster);
	}
	else {
		thresholdCluster.addCluster(this->clustersInLeafNode[closestIndex]);
		thresholdCluster.addCluster(addCluster);
		
	}
	
	thresholdCluster.update();
	// Check if threshold condition is still valid
	if (thresholdCluster.getRadius() > threshold_Value)
	{
		// If not all entries are used in node, place the point in a new entry
		if (this->clustersInLeafNode.size() < (L_ENTRIES - 1))
		{
			clustersInLeafNode.push_back(addCluster);
			return;
		}
		// If the Leaf Node is full --> split and create new node
		else
		{
			CFTreeNode newNode = this->splitLeaf();
			this->next = &newNode;
			newNode.prev = this;
			return newNode;
		}
	}
	else
	{
		// If the radius is smaller than the threshold value
		this->clustersInLeafNode[closestIndex].addCluster(addCluster);
		this->clustersInLeafNode[closestIndex].update();
	}
	return CFTreeNode();
}

/**
 * Creates a new Leaf Node and assignes the farthest Clusters to new and old
 * The rest will be assigned to the closest of both
 * Input: --
 * Output: New Leaf Node (CFTreeNode)
 * Effect: New leaf node created
*/
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

/**
 * Insert a cluster to the tree
 * It is very similar to adding points
 * In case of split the retunr value is a non empty tree node
 * Input: Cluster to add (Cluster)
 * Output: New Leaf Node (CFTreeNode)
 * Effect: --
*/
CFTreeNode CFTreeNode::insertCluster(Cluster addCluster)
{
	CFTreeNode newNode;
	// for leaf nodes
	if (this->isLeafNode)
	{
		// if a split occurs a new CFTreeNode is returned
		newNode = insertToLeaf(addCluster);
	}
	// go down the tree recursively to find a leaf node to place the point
	else {
		double distance = DBL_MAX, tmpDis;
		int closestIndex;
		for (int i = 0; i < childCluster.size(); ++i)
		{
			tmpDis = calcDistance(addCluster.getCentroid(), childCluster[i].getCentroid());
			if (tmpDis < distance)
			{
				distance = tmpDis;
				closestIndex = i;
			}
		}
		// recursive call for the tree structure
		// if a split occurs a new CFTreeNode is returned
		newNode = childCluster[closestIndex].insertCluster(addCluster);
		this->update();
		// It is not possible to retun NULL, so if no split occurs, the insertPoint function returns a node without clusters
		// GetNumberOfClusterEntries is necessary to identify leafNodes
		if ((newNode.getNumberOfChildEntries() != 0) || (newNode.getNumberOfClusterEntries() != 0))
		{
			current_tree_size++;
			if (newNode.getNumberOfChildEntries() != 0)
				newNode.changeLeafNode(false);
			if (this->childCluster.size() <= B_ENTRIES)
			{
				this->childCluster.push_back(newNode);
				// Check if threshold is broken / split than
				if (this->calcRadius() > threshold_Value)
				{
					// If threshold is broken --> split this too
					newNode = this->splitNonLeaf();
				}
				else
				{ // TODO :: else do the merging refinement
					newNode = CFTreeNode();
				}
			}
			else
			{
				// Split because no space is left
				this->childCluster.push_back(newNode);
				newNode = this->splitNonLeaf();
			}
		}
	}
	this->update();
	return newNode;
}


/**
 * Get the CF
 * Input: --
 * Output: CF of the cluster (ClusteringFeature)
 * Effect: --
*/
ClusteringFeature CFTreeNode::getCF()
{
	return this->cf;
}

/**
 * Is the cluster a leaf node ?
 * Input: --
 * Output: leaf node ? (boolean)
 * Effect: --
*/
bool CFTreeNode::getIsLeafNode()
{
	return this->isLeafNode;
}

/**
 * Toggeling the value of leafNode in the cluster feature
 * Input: boolean of new value (boolean)
 * Output: --
 * Effect: the value of leafNode changes
*/
void CFTreeNode::changeLeafNode(bool value)
{
		this->isLeafNode = value;
}

/**
 * Recalculate the Linear Sum & Square Sum of the CF to refresh
 * This function is public
 * Input: --
 * Output: --
 * Effect: value of SS & LS in a CF change
*/
void CFTreeNode::recalculateCF()
{
	this->cf.calcLinearSum(this->isLeafNode, this->clustersInLeafNode, this->childCluster);
	this->cf.calcSquareSum(this->isLeafNode, this->clustersInLeafNode, this->childCluster);
}

/**
 * Update the Node
 * Input: --
 * Output: --
 * Effect: update Centroid, Diameter, Radius and CF
*/
void CFTreeNode::update()
{
	calcCentroid();
	calcDiameter();
	calcRadius();
	recalculateCF();
}

/**
 * Centroid is calculated by defining the average value for each dimension first
 * Than the new Position is set for the centroid of the cluster
 * Input: --
 * Output: --
 * Effect: the centroid variable in cluster is changed
*/
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

/**
 * This function first recalculates the centroid and uses it to
 * measure the average distance between it and all points in the cluster
 * Input: --
 * Output: radius (double)
 * Effect: the radius variable in cluster is changed
*/
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

/**
 * This function calculates the average distance between all points in the cluster
 * Input: --
 * Output: diameter (double)
 * Effect: the diameter variable in cluster is changed
*/
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

/**
 * Get number of child nodes for non leaf nodes
 * Input: --
 * Output: amount of child nodes (int)
 * Effect: --
*/
int CFTreeNode::getNumberOfChildEntries()
{
	return this->childCluster.size();
}

/**
 * Get number of child nodes for non leaf nodes
 * Input: --
 * Output: amount of child nodes (int)
 * Effect: --
*/
int CFTreeNode::getNumberOfClusterEntries()
{
	return this->clustersInLeafNode.size();
}

/**
 * Get the child node at index i
 * Input: Index (int)
 * Output: Node at index (CFTreeNode)
 * Effect: --
*/
CFTreeNode CFTreeNode::getElement(int i)
{
	return this->childCluster[i];
}

/**
 * Get the cluster at index i
 * Input: Index (int)
 * Output: CLuster at index (Cluster)
 * Effect: --
*/
Cluster CFTreeNode::getClusterElement(int i)
{
	return this->clustersInLeafNode[i];
}

/**
 * Get the closest distance of Cluster centroids in a leaf node 
 * Input: --
 * Output: closest distance of Clusters (double)
 * Effect: --
*/
double CFTreeNode::getclosestDistanceOfEntries()
{
	// find nearest pair of entries
	double distanceHelp = DBL_MAX, dblHelp;
	for (int i = 0; i < this->getNumberOfClusterEntries(); ++i)
	{
		for (int j = 0; j < this->getNumberOfClusterEntries(); ++j)
		{
			if (i != j)
			{
				dblHelp = calcDistance(this->clustersInLeafNode[i].getCentroid(), this->clustersInLeafNode[i].getCentroid());
					if (dblHelp < distanceHelp)
						distanceHelp = dblHelp;
			}
		}
	}
	return distanceHelp;
}

