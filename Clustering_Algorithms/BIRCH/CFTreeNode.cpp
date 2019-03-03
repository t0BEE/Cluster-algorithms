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
 * Return the Centroid
 * Input: --
 * Output: Centroid (Point)
 * Effect: --
 */
void CFTreeNode::getCentroid(double* buffer)
{
	double coordinates[DIMENSIONS], tmp[DIMENSIONS];
	int totalNrPoints=0;
	for (int i = 0; i < this->childNodes.size(); ++i)
	{
		this->childCF[i].getLS(tmp);
		addTwoPoints(coordinates, tmp);
		totalNrPoints += childCF[i].getNumberOfPoints();
	}
	for (int i = 0; i < DIMENSIONS; ++i)
	{
		buffer[i] = coordinates[i] / totalNrPoints;
	}
}

bool CFTreeNode::isLeafNode()
{
	if (this->childNodes.size() != 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

/**
 * A point is added to the tree recursivly
 * If it reaches a leaf node, the point is added to a cluster
 * Output has either empty vectors or non empty ones in case of splits
 * Input: Point to insert (Point)
 * Output: New Node, empty if no split occured (CFTreeNode)
 * Effect: cluster variable is changed / increased in size
 */
CFTreeNode* CFTreeNode::insert(ClusteringFeature addCF)
{
	CFTreeNode* newNode;
	// for leaf nodes
	if (this->isLeafNode())
	{	
		// if a split occurs a new CFTreeNode is returned
		return insertToLeaf(addCF);
	}
	// go down the tree recursively to find a leaf node to place the point
	else {
		double distance = DBL_MAX, tmpDis;
		double tmpInsertCentroid[DIMENSIONS], tmpCentroid[DIMENSIONS];
		int closestIndex;
		addCF.calcCentroid(tmpInsertCentroid);
		for (int i = 0; i < this->childNodes.size(); ++i)
		{
			this->childNodes[i]->getCentroid(tmpCentroid);

			tmpDis = calcDistance(tmpInsertCentroid, tmpCentroid);
			if (tmpDis < distance)
			{
				distance = tmpDis;
				closestIndex = i;
			}
		}
		// recursive call for the tree structure
		newNode = this->childNodes[closestIndex]->insert(addCF);
		// update the path of the inserted node
		addCF.getLS(tmpInsertCentroid);
		addCF.getSS(tmpCentroid);
		this->childCF[closestIndex].addToLS(tmpInsertCentroid);
		this->childCF[closestIndex].addToSS(tmpCentroid);

		// if a split occurs a new CFTreeNode is returned with pointer otherwise nullptr
		// TODO:: what about the CF values??
		if (newNode != nullptr)
		{ // a split occured
			current_tree_size++;
			// try to insert new node here
			if (this->childNodes.size() < B_ENTRIES)
			{ // some space is left

				
				this->childNodes.push_back(newNode);
				this->childCF.push_back();
				return nullptr;
				// TODO Is it necessary to check for the threshold ???
			}
			else
			{ // split the non leaf because no space is left
				newNode = this->splitNonLeaf(this, newNode);
			}

		}


				/*
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
				*/
	}
	return newNode;
}


/**
 * Insert a point to a leaf cluster
 * Input: point to add (Point)
 * Output: Node which is non empty if the leaf is full and a split occurs (CFTreeNode)
 * Effect: --
*/
CFTreeNode* CFTreeNode::insertToLeaf(ClusteringFeature addCF)
{
	double distance = DBL_MAX, tmpDis;
	int closestIndex = 0;
	double tmpInsert[DIMENSIONS], tmp[DIMENSIONS];
	// If there is no cluster in leaf node, create a new one
	if (childCF.size() == 0)
	{
		childCF.push_back(addCF);
	}
	else {
		// Get index of closest cluster ( meassured by centroid of cf to insert - other centroids )
		for (int i = 0; i < childCF.size(); ++i)
		{
			tmpDis = calcDistance(addCF.calcCentroid(tmpInsert), childCF[i].calcCentroid(tmp));
			if (tmpDis < distance)
			{
				distance = tmpDis;
				closestIndex = i;
			}
		}
		if (this->childCF[closestIndex].absorbCF(addCF))
		{ // absorbation successfully, the threshold condition is not broken 

		}
		else
		{ // addCF could not be absorbed
			// check if there is space left in the leaf node to create a new entry
			if (this->childCF.size() < L_ENTRIES)
			{
				this->childCF.push_back(addCF);
			}
			else
			{	// split the leaf and create new node
				CFTreeNode* newNode = new CFTreeNode();
				// before splitting push_back the new CF to oldNode
				this->childCF.push_back(addCF);
				splitLeaf(this, newNode);
				this->next = newNode;
				newNode->prev = this;
				return newNode;
			}
		}
	}
	return nullptr;
}

/**
 * A leaf node is split into two 
 * The points are seperated and assigned to them
 * Input: --
 * Output: New Leaf Node (CFTreeNode)
 * Effect: New Node created and Points are torn between both
*/
CFTreeNode* CFTreeNode::splitNonLeaf(CFTreeNode* oldNode, CFTreeNode* newNode)
{
	// choose farthest pair of entries
	int far1, far2;
	double farDis = 0.0, tmpDis;
	double tmpCentroid1[DIMENSIONS], tmpCentroid2[DIMENSIONS], tmpCentroidInsert[DIMENSIONS];
	for (int i = 0; i < oldNode->childCF.size(); ++i)
	{
		for (int j = 0; j < oldNode->childCF.size(); ++j)
		{
			oldNode->childCF[i].calcCentroid(tmpCentroid1);
			oldNode->childCF[j].calcCentroid(tmpCentroid2);
			tmpDis = calcDistance(tmpCentroid1, tmpCentroid2);
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
				// otherwise the bigger index will be changed when erase the smaller one
			}
		}
	}



	// TODO
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
 * Creates a new Leaf Node and assignes the farthest Clusters to new and old
 * The rest will be assigned to the closest of both
 * Input: --
 * Output: New Leaf Node (CFTreeNode)
 * Effect: New leaf node created
*/
void CFTreeNode::splitLeaf(CFTreeNode* oldNode, CFTreeNode* newNode)
{
	// choose farthest pair of entries
	int far1, far2;
	double farDis = 0.0, tmpDis;
	double tmpCentroid1[DIMENSIONS], tmpCentroid2[DIMENSIONS], tmpCentroidInsert[DIMENSIONS];
	for (int i = 0; i < oldNode->childCF.size(); ++i)
	{
		for (int j = 0; j < oldNode->childCF.size(); ++j)
		{
			oldNode->childCF[i].calcCentroid(tmpCentroid1);
			oldNode->childCF[j].calcCentroid(tmpCentroid2);
			tmpDis = calcDistance(tmpCentroid1, tmpCentroid2);
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
				// otherwise the bigger index will be changed when erase the smaller one
			}
		}
	}
	// create temporary vector of CFs and assign the rest of CFs to it
	std::vector<ClusteringFeature> tmpCFs;
	newNode->childCF.push_back(oldNode->childCF[far2]);
	// move far1 to the beginning of tmpCFs to access it in a later moment
	// and copy all other CFs to tmpCFs
	tmpCFs.push_back(oldNode->childCF[far1]);
	oldNode->childCF.erase(childCF.begin() + far2);
	oldNode->childCF.erase(childCF.begin() + far1);
	int currentLength = oldNode->childCF.size();
	for (int i = currentLength - 1; i >= 0; --i)
	{
		tmpCFs.push_back(oldNode->childCF[i]);
		oldNode->childCF.pop_back();
	}
	// move the first element in tmpCFs (far1) to the oldNode again
	oldNode->childCF.push_back(tmpCFs.front());
	tmpCFs.erase(tmpCFs.begin());
	// at this point tmpCFs contains all CF except of the farthest
	// now assign!
	currentLength = tmpCFs.size();
	oldNode->childCF[0].calcCentroid(tmpCentroid1);
	newNode->childCF[0].calcCentroid(tmpCentroid2);
	for (int i = currentLength - 1; i >= 0; --i)
	{	// Compare distances of the centroids and assign clusters to the closest
		tmpCFs[i].calcCentroid(tmpCentroidInsert);
		if (calcDistance(tmpCentroid1, tmpCentroidInsert) <= calcDistance(tmpCentroid2, tmpCentroidInsert))
		{
			oldNode->childCF.push_back(tmpCFs[i]);
		}
		else
		{
			newNode->childCF.push_back(tmpCFs[i]);
		}
		tmpCFs.pop_back();
	}
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

