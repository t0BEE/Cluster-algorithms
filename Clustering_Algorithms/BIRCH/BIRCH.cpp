
#include "BIRCH.h"
#include "kMeans.h"

// TODO --- Thoughts #1 how is the tree balanced?
//			--> pathCopy() --> insertCluster()

std::vector<Point> total;
CFTreeNode rootNode;
CFTreeNode newTreeRoot;

void readCSV(std::string filename)
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
			total.push_back(Point(dData));
		}
		catch (std::exception)
		{
		}
		// erste Zeile schmeißt Exception! ("x","y")
	}
}

void insertPoint(unsigned int index)
{
	CFTreeNode newNode;
	newNode = rootNode.insertPoint(total[index]);
	// If the root is split, the tree will grow in height
	if (newNode.getNumberOfChildEntries() != 0)
	{
		CFTreeNode newRoot = CFTreeNode();
		current_tree_size++;
		newRoot.insertNode(newNode);
		newRoot.insertNode(rootNode);
		rootNode = newRoot;
	}
	// Run out of memory
	if (current_tree_size > MAXIMUM_TREE_SIZE)
	{
		rebuild();
	}
}


void rebuild()
{
	// change threshould value || the paper has no perfect solution --> TODO research
	// paper: average of the distances between nearest pairs of leaf entries in all leaf nodes
	// find first leaf node
	CFTreeNode tmpNode = rootNode;
	std::vector<double> distances;

	while (!tmpNode.getIsLeafNode())
	{
		tmpNode = tmpNode.getElement(0);
	}
	while (tmpNode.next != NULL)
	{
		distances.push_back(tmpNode.getclosestDistanceOfEntries());
		tmpNode = *tmpNode.next;
	}
	for (int i = 0; i < distances.size()-1; ++i)
	{
		distances.back() = distances.back() + distances[i];
	}
	threshold_Value = distances.back();

	// The tree is copied completely and the old tree deleted afterwards
	// During this process the Clusters are reentered
	newTreeRoot = rootNode;
	pathCopy(newTreeRoot, rootNode);
	deleteTree(rootNode);

}


// TODO :: think about the delay-split (default on) & Outlier Handling Option (default off) p.15
/**
 * left most path of the old tree is pushed recursively as the right most tree in new tree
 * if it reaches a leaf node --> the entries are reentred from the top 
 * Input: New tree node / old tree node (CFTreeNode)
 * Output: ---
 * Effect: a completely new tree is created
*/
void pathCopy(CFTreeNode newTree, CFTreeNode oldTree)
{
	for (int j = 0; j < oldTree.getNumberOfChildEntries(); ++j)
	{
		newTree.insertNode(oldTree.getElement(j));
		if (oldTree.getElement(j).getIsLeafNode())
		{
			for (int i = 0; i < oldTree.getElement(j).getNumberOfClusterEntries(); ++i)
			{
				newTreeRoot.insertCluster(oldTree.getElement(j).getClusterElement(i));
			}
		}
		else
		{
			pathCopy(newTree.getElement[j], oldTree.getElement[j]);
		}
	}
	// Check all leaf nodes if they are empty
	if (newTree.getElement(0).getIsLeafNode())
	{
		for (int j = newTree.getNumberOfChildEntries() - 1; j >= 0; ++j)
		{
			// If the leaf is empty, delete it
			if (newTree.getElement(j).getNumberOfClusterEntries() == 0)
				newTree.removeFromNode(j);
		}
	}
	// Check for non leaf nodes if they are empty
	else
	{
		for (int j = newTree.getNumberOfChildEntries() - 1; j >= 0; ++j)
		{
			// If a non leaf node is empty, delete it
			if (newTree.getElement(j).getNumberOfChildEntries() == 0)
				newTree.removeFromNode(j);
		}
	}
}

/**
 * Deletes the tree recursively
 * Input: root node of the tree to delete (CFTreeNode)
 * Output: ---
 * Effect: 
*/
void deleteTree(CFTreeNode delRoot)
{
	if (!delRoot.getIsLeafNode())
	{
		for (int j = 0; j < delRoot.getNumberOfChildEntries(); ++j)
		{
			deleteTree(delRoot.getElement(j));
		}
	}
	delete &delRoot;
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

int main(int argc, char **argv)
{
    std::cout << "Start BIRCH!\n"; 
	rootNode = CFTreeNode();
	current_tree_size = 1;
	// read CSV
	readCSV("../../Inputfiles/Sample.csv");

	// Phase 1
	for (int k = 0; k < total.size(); ++k)
	{
		insertPoint(k);
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
	CFTreeNode* tmp = &rootNode;
	while (!(tmp->getIsLeafNode()))
		tmp = &(tmp->getElement[0]);
	
	while (tmp->next)
	{
		numberOfLeafNodes++;
		tmp = tmp->next;
	}
	kMeans::main();


	// Phase 4  --- TODO (optional)
	// refine the tree by redistributing the data points to the closest seed
	// centroids of phase 3 are seeds
}

