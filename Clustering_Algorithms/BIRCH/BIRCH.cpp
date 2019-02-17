
#include "BIRCH.h"


// TODO --- Thoughts #1 how is the tree balanced?

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
		// erste Zeile schmei�t Exception! ("x","y")
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

	// TODO --- rebuild tree
	newTreeRoot = rootNode;
	// take the left most path and put it to the right of the new tree
	// Let all entries run again through the new tree

	pathCopy(newTreeRoot, rootNode);


}

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

int main()
{
    std::cout << "Start BIRCH!\n"; 
	rootNode = CFTreeNode();
	current_tree_size = 1;
	// read CSV
	readCSV("../../Inputfiles/Sample.csv");

	// Phase 1  --- TODO deeper look
	for (int k = 0; k < total.size(); ++k)
	{
		insertPoint(k);
	}

	// Phase 2  --- TODO (optional)


	// Phase 3  --- TODO
	// use an existing clustering algorithm --> k-means
	// nodes have a fixed size --> which means that it can not hold natural clusters
	// applied on a coarse summary of data
	// maybe start k-means on different Non-leaf nodes --> parallel


	// Phase 4  --- TODO (optional)
	// refine the tree by redistributing the data points to the closest seed

}

