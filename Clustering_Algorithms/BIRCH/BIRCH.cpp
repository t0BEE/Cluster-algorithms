#include "BIRCH.h"
#include "../k-Means/kMeans.h"

// TODO --- Thoughts #1 how is the tree balanced?
//			--> pathCopy() --> insertCluster()

using namespace kMeans;

std::vector<Point_B> total;
CFTreeNode* rootNode;
CFTreeNode* newTreeRoot;

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
	{
		current_tree_size++;
		if (rootNode->childNodes.size() == 0)
		{ // if rot was a leafNode it has to be split, too
			CFTreeNode* tmpNode;
			tmpNode = rootNode;
			rootNode = new CFTreeNode();
			rootNode->childNodes.push_back(tmpNode);
			rootNode->childCF.push_back(tmpNode->getCF());
			rootNode->childNodes.push_back(newNode);
			rootNode->childCF.push_back(newNode->getCF());
		}
		else if (rootNode->childNodes.size() < B_ENTRIES)
		{ // some space is left		
			rootNode->childNodes.push_back(newNode);
			rootNode->childCF.push_back(newNode->getCF());
		}
		else
		{ // split the root node because no space is left
			newNode = rootNode->splitNonLeaf(rootNode, newNode);
			CFTreeNode* newRoot = new CFTreeNode();
			newRoot->childNodes.push_back(rootNode);
			newRoot->childNodes.push_back(newNode);
			newRoot->childCF.push_back(rootNode->getCF());
			newRoot->childCF.push_back(newNode->getCF());
			rootNode = newRoot;
		}
	}
	// Run out of memory
	if (current_tree_size > MAXIMUM_TREE_SIZE)
	{
		rebuild();
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
		newTree->insertNode(oldTree->childCF[j], oldTree->childNodes[j]);
		if (oldTree->isLeafNode())
		{
			for (int i = 0; i < oldTree->childCF.size(); ++i)
			{
				newTreeRoot->insert(oldTree->childCF[i]);
			}
		}
		else
		{
			pathCopy(newTree->childNodes[j], oldTree->childNodes[j]);
		}
	}

	// clean tree
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
	delete delRoot;
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
	while (tmpNode->next != NULL)
	{
		distances.push_back(tmpNode->getclosestDistanceOfEntries());
		tmpNode = tmpNode->next;
	}
	for (int i = 0; i < distances.size() - 1; ++i)
	{
		distances.back() = distances.back() + distances[i];
	}
	threshold_Value = distances.back() / distances.size();

	for (int i = 0; i < distances.size(); ++i)
	{
		distances.pop_back();
	}

	// The tree is copied completely and the old tree deleted afterwards
	// During this process the Clusters are reentered

	newTreeRoot = new CFTreeNode();

	pathCopy(newTreeRoot, rootNode);

	deleteTree(rootNode);

}

int main()
{
    std::cout << "Start BIRCH!\n"; 
	rootNode = new CFTreeNode();
	ClusteringFeature newCF;
	current_tree_size = 1;
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
	int numberOfLeafNodes = 0;
	CFTreeNode* tmp = rootNode;
	while (!(tmp->isLeafNode()))
		tmp = tmp->getFirstElement();
	
	while (tmp->next)
	{
		numberOfLeafNodes++;
		tmp = tmp->next;
	}
	//kMeans::main();


	// Phase 4  --- TODO (optional)
	// refine the tree by redistributing the data points to the closest seed
	// centroids of phase 3 are seeds
}

