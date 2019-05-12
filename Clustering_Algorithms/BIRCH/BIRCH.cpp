#include "BIRCH.h"
#include "../k-Means/kMeans.h"


using namespace kMeans;

std::vector<Point_B> total;
CFTreeNode* rootNode;
CFTreeNode* newTreeRoot;
static int tree_height;
int current_tree_size = 0;
double threshold_Value = 0.0;

//prev and next when rebuilding
CFTreeNode* tmpPrev;

void readBIRCHCSV(std::string filename)
{

	std::ifstream input(filename);
	std::string line;
	while (std::getline(input, line))
	{
		// put line in stringstream to seperate 
		std::stringstream ststream(line);
		std::string sData[DIMENSIONS];
		double dData[DIMENSIONS];
		// seperated with ; in CSV  -- get value after value
		for (int i = 0; i < DIMENSIONS; ++i)
		{
			std::getline(ststream, sData[i], ';');
			dData[i] = std::stod(sData[i]);
		}
		try
		{
			total.push_back(Point_B(dData));
		}
		catch (std::exception)
		{
		}
		// erste Zeile schmeißt Exception! ("x","y")
	}
}

void insertCF(ClusteringFeature addCF)
{
	CFTreeNode* newNode;
	newNode = rootNode->insert(addCF);
	// If the root is split, the tree will grow in height
	if (newNode != nullptr)
	{ // root is splitted
		tree_height++;
		current_tree_size += 2; // the new node + a new root node

		if (rootNode->isLeafNode())
		{ // if root was a leafNode it has to be split, too
			CFTreeNode* tmpNode;
			tmpNode = rootNode;
			rootNode = new CFTreeNode();
			rootNode->childNodes.push_back(tmpNode);
			rootNode->childCF.push_back(tmpNode->getCF());
			rootNode->childNodes.push_back(newNode);
			rootNode->childCF.push_back(newNode->getCF());
		}
		else
		{ // create a new root and push the new node and old root to it
			CFTreeNode* newRoot = new CFTreeNode();
			
			// Add to the new root
			newRoot->childNodes.push_back(rootNode);			
			newRoot->childCF.push_back(rootNode->getCF());
			newRoot->childNodes.push_back(newNode);
			newRoot->childCF.push_back(newNode->getCF());
			rootNode = newRoot;
	
			// fix prev-next-leaf connection
			CFTreeNode* helpOne = rootNode->childNodes[0];
			CFTreeNode* helpTwo = rootNode->childNodes[1];
			while (!(helpOne->isLeafNode()))
			{
				helpOne = helpOne->childNodes.back();
				helpTwo = helpTwo->childNodes.front();
			}
			helpOne->next = helpTwo;
			helpTwo->prev = helpOne;
			
		}
	}
	// Run out of memory
	while (current_tree_size > MAXIMUM_TREE_SIZE)
	{
		rebuild();
		rootNode = newTreeRoot;
		newTreeRoot = nullptr;
		// calculate new current tree size
		current_tree_size = 0;
		calcTreeSize(rootNode);
	}
}

void calcTreeSize(CFTreeNode* node)
{
	for (int i = 0; i < node->childNodes.size(); ++i)
	{
		calcTreeSize(node->childNodes[i]);
	}
	current_tree_size++;
}

void prevNextChain(CFTreeNode* node)
{
	for (int i = 0; i < node->childNodes.size(); ++i)
	{
		prevNextChain(node->childNodes[i]);
	}
	if (node->isLeafNode())
	{
		if (!(tmpPrev)) tmpPrev = node;
		else
		{
			node->prev = tmpPrev;
			tmpPrev->next = node;
			tmpPrev = node;
			tmpPrev->next = nullptr;
		}
	}
}


// TODO :: think about the delay-split (default on) & Outlier Handling Option (default off) p.15
/**
 * left most path of the old tree is pushed recursively as the right most tree in new tree
 * if it reaches a leaf node --> the entries are reentred from the top 
 * Input: New tree node / old tree node (CFTreeNode)
 * Output: ---
 * Effect: a completely new tree is created
*/
void pathCopy(CFTreeNode* newTree, CFTreeNode* oldTree)
{
	for (int j = 0; j < oldTree->childCF.size(); ++j)
	{		
		if (oldTree->isLeafNode())
		{
			newTreeRoot->insert(oldTree->childCF[j]);
		}
		else
		{
			CFTreeNode *newCopiedNode = new CFTreeNode();

			newTree->insertNode(oldTree->childCF[j], newCopiedNode);
			pathCopy(newTree->childNodes.back(), oldTree->childNodes[j]);
			// update CF - where points are deleted
			for (int i = newTree->childCF.size() - 1; i >= 0; --i)
			{
				newTree->childCF.insert(newTree->childCF.begin(), newTree->childNodes[i]->getCF());
				newTree->childCF.pop_back();
			}
			// clean tree - delete nodes with no childCFs
			for (int j = 0; j < newTree->childCF.size(); ++j)
			{
				if (newTree->childCF[j].getNumberOfPoints() == 0)
				{
					delete newTree->childNodes[j];
					newTree->childNodes.erase(newTree->childNodes.begin() + j);
					newTree->childCF.erase(newTree->childCF.begin() + j);
				}
			}
		}
	}
}


/**
 * Deletes the tree recursively
 * Input: root node of the tree to delete (CFTreeNode)
 * Output: ---
 * Effect: 
*/
void deleteTree(CFTreeNode* delRoot)
{
	if (!(delRoot->isLeafNode()))
	{
		for (int j = 0; j < delRoot->childNodes.size(); ++j)
		{
			deleteTree(delRoot->childNodes[j]);
		}
	}
	// delete is not necessary - the old nodes are used in the new tree
	//delete delRoot;
	delRoot = nullptr;
}

/*
void writeCSVFile(std::ofstream &fileOStream, std::string filename)
{
	fileOStream.open((filename));
	fileOStream << "a;b;c\n";
	for (unsigned int j = 0; j < total.size(); ++j)
	{
		fileOStream << std::to_string(total[j].getX()) << ";" << std::to_string(total[j].getY()) << ";" << std::to_string(total[j].getCluster()) << "\n";
	}
	// write Centroids in CSV with cluster number = total
	for (int j = 0; j < CLUSTER_TOTAL; ++j)
	{
		fileOStream << std::to_string(clusters[j].getCentroid().getX()) << ";" << std::to_string(clusters[j].getCentroid().getY()) << ";" << std::to_string(CLUSTER_TOTAL) << "\n";
	}
	fileOStream.close();
}
*/

void rebuild()
{
	// change threshould value || the paper has no perfect solution --> TODO research
	// paper: average of the distances between nearest pairs of leaf entries in all leaf nodes
	// find first leaf node
	CFTreeNode* tmpNode = rootNode;
	std::vector<double> distances;

	while (!tmpNode->isLeafNode())
	{
		tmpNode = tmpNode->getFirstElement();
	}
	while (tmpNode->next != nullptr)
	{
		distances.push_back(tmpNode->getclosestDistanceOfEntries());
		tmpNode = tmpNode->next;
	}
	distances.push_back(tmpNode->getclosestDistanceOfEntries());
	// Add all other distances on the last item - (-2) because the last item may not be used twice
	for (int i = 0; i < distances.size() - 2; ++i)
	{
		distances.back() = distances.back() + distances[i];
	}
	threshold_Value = distances.back() / distances.size();

	distances.clear();


	// The tree is copied completely and the old tree deleted afterwards
	// During this process the Clusters are reentered

	newTreeRoot = new CFTreeNode();

	pathCopy(newTreeRoot, rootNode);
	// prev - next pointer chain
	prevNextChain(newTreeRoot);
	// delete old tree
	deleteTree(rootNode);

}

int main()
{
    std::cout << "Start BIRCH!\n"; 
	rootNode = new CFTreeNode();
	ClusteringFeature newCF;
	current_tree_size++;
	tree_height = 1;
	// read CSV
	readBIRCHCSV("../../Inputfiles/Sample.csv");
	// Phase 1
	double tmpLS[DIMENSIONS], tmpSS[DIMENSIONS];
	for (int k = 0; k < total.size(); ++k)
	{
		for (int i = 0; i < DIMENSIONS; ++i)
		{
			tmpLS[i] = total[k].getCoordinate(i);
			tmpSS[i] = pow(tmpLS[i], 2);
		}
		newCF = ClusteringFeature(1, tmpLS, tmpSS);
		insertCF(newCF);
	}
	std::cout << std::endl;
	// Phase 2  --- TODO (optional)
	/*
	some clustering methods perform well in terms of speed and quality
	close the gap between output of phase 1 and best performance of phase 3
	*/

	// Phase 3  --- TODO
	// use an existing clustering algorithm --> k-means
	// nodes have a fixed size --> which means that it can not hold natural clusters
	// applied on a coarse summary of data
	// maybe start k-means on different Non-leaf nodes --> parallel
	/*
	int numberOfLeafNodes = 0;
	CFTreeNode* tmp = rootNode;
	while (!(tmp->isLeafNode()))
		tmp = tmp->getFirstElement();
	
	while (tmp->next)
	{
		numberOfLeafNodes++;
		tmp = tmp->next;
	}
	kMeans::main();
	*/

	// Phase 4  --- TODO (optional)
	// refine the tree by redistributing the data points to the closest seed
	// centroids of phase 3 are seeds
}

