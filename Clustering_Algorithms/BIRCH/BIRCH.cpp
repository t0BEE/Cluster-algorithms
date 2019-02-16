
#include "BIRCH.h"

std::vector<Point> total;
CFTreeNode rootNode;

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
	if (newNode.getNumberOfEntries() != 0)
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
	// paper: average of the distances between all nearest pairs of a leaf entries

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

