#include "CFTreeNode.h"

/**
 * Constructor of class Cluster
 * Input: --
 * Output: --
 * Effect: New Cluster instance created and set to Leaf Node
 */
CFTreeNode::CFTreeNode() {}

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

		// if a split occurs a new CFTreeNode is returned by pointer otherwise nullptr
		if (newNode != nullptr)
		{ // a split occured
			current_tree_size++;
			// try to insert new node here
			if (this->childNodes.size() < B_ENTRIES)
			{ // some space is left		
				this->childNodes.push_back(newNode);
				this->childCF.push_back(newNode->getCF());
				return nullptr;
			}
			else
			{ // split the non leaf because no space is left
				newNode = this->splitNonLeaf(this, newNode);
			}

		}

		// update the path
		this->childCF.erase(childCF.begin() + closestIndex);
		this->childCF.insert(childCF.begin() + closestIndex, this->childNodes[closestIndex]->getCF());
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
			addCF.calcCentroid(tmpInsert);
			childCF[i].calcCentroid(tmp);
			tmpDis = calcDistance(tmpInsert, tmp);
			if (tmpDis < distance)
			{
				distance = tmpDis;
				closestIndex = i;
			}
		}
		if (this->childCF[closestIndex].absorbCF(addCF))
		{ }// absorbation successfully, the threshold condition is not broken 
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
				if (this->next)
				{
					this->next->prev = newNode;
					newNode->next = this->next;
				}
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

	std::vector<ClusteringFeature> tmpCF;
	std::vector<CFTreeNode*> tmpTreeNode;

	// move all elements to tmp Vectors
	for (int i = (int) oldNode->childCF.size() - 1; i >= 0; --i)
	{
		tmpCF.push_back(oldNode->childCF[i]);
		oldNode->childCF.pop_back();
		tmpTreeNode.push_back(oldNode->childNodes[i]);
		oldNode->childNodes.pop_back();
	}
	// add newNode to it
	tmpCF.push_back(newNode->getCF());
	tmpTreeNode.push_back(newNode);


	// choose farthest pair of entries
	int far1, far2;
	double farDis = 0.0, tmpDis;
	double tmpCentroid1[DIMENSIONS], tmpCentroid2[DIMENSIONS], tmpCentroidInsert[DIMENSIONS];
	for (int i = 0; i < tmpCF.size(); ++i)
	{
		for (int j = 0; j < tmpCF.size(); ++j)
		{
			tmpCF[i].calcCentroid(tmpCentroid1);
			tmpCF[j].calcCentroid(tmpCentroid2);
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

	CFTreeNode* newerNode = new CFTreeNode();

	newerNode->childCF.push_back(tmpCF[far2]);
	newerNode->childNodes.push_back(tmpTreeNode[far2]);

	oldNode->childCF.push_back(tmpCF[far1]);
	oldNode->childNodes.push_back(tmpTreeNode[far1]);

	tmpCF.erase(childCF.begin() + far2);
	tmpCF.erase(childCF.begin() + far1);
	tmpTreeNode.erase(childNodes.begin() + far2);
	tmpTreeNode.erase(childNodes.begin() + far1);

	// at this point tmpCFs contains all CF except of the farthest
	// now assign!
	int currentLength = (int) tmpCF.size();
	oldNode->childCF[0].calcCentroid(tmpCentroid1);
	newerNode->childCF[0].calcCentroid(tmpCentroid2);

	for (int i = currentLength - 1; i >= 0; --i)
	{	// Compare distances of the centroids and assign clusters to the closest
		tmpCF[i].calcCentroid(tmpCentroidInsert);
		if (calcDistance(tmpCentroid1, tmpCentroidInsert) <= calcDistance(tmpCentroid2, tmpCentroidInsert))
		{
			oldNode->childCF.push_back(tmpCF[i]);
			oldNode->childNodes.push_back(tmpTreeNode[i]);
		}
		else
		{
			newerNode->childCF.push_back(tmpCF[i]);
			newerNode->childNodes.push_back(tmpTreeNode[i]);
		}
		tmpCF.pop_back();
		tmpTreeNode.pop_back();
	}
	return newerNode;
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
	int currentLength = (int) oldNode->childCF.size();
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
	currentLength = (int) tmpCFs.size();
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
 * Get the CF
 * Input: --
 * Output: CF of the cluster (ClusteringFeature)
 * Effect: --
*/
ClusteringFeature CFTreeNode::getCF()
{
	ClusteringFeature* newCF =  new ClusteringFeature();
	double tmpLS[DIMENSIONS], tmpSS[DIMENSIONS];
	
	for (int i = 0; i < childCF.size(); i++)
	{
		this->childCF[i].getLS(tmpLS);
		this->childCF[i].getSS(tmpSS);
		newCF->addToLS(tmpLS);
		newCF->addToSS(tmpSS);
	}
	return *newCF;
}

CFTreeNode* CFTreeNode::getFirstElement()
{
	return this->childNodes[0];
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
	double distanceHelp = DBL_MAX, dblHelp, tmpCentr1[DIMENSIONS], tmpCentr2[DIMENSIONS];
	for (int i = 0; i < this->childCF.size(); ++i)
	{
		for (int j = 0; j < this->childCF.size(); ++j)
		{
			if (i != j)
			{
				this->childCF[i].calcCentroid(tmpCentr1);
				this->childCF[j].calcCentroid(tmpCentr2);
				dblHelp = calcDistance(tmpCentr1, tmpCentr2);
					if (dblHelp < distanceHelp)
						distanceHelp = dblHelp;
			}
		}
	}
	return distanceHelp;
}



void CFTreeNode::insertNode(ClusteringFeature cfInsert, CFTreeNode* nodeInsert)
{
	this->childCF.push_back(cfInsert);
	this->childNodes.push_back(nodeInsert);
}

