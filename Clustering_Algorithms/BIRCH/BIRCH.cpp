
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
		// erste Zeile schmei�t Exception! ("x","y")
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
		newRoot.insertNode(newNode);
		newRoot.insertNode(rootNode);
		rootNode = newRoot;
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

	// read CSV
	readCSV("../../Inputfiles/Sample.csv");

	// Phase 1
	for (int k = 0; k < total.size(); ++k)
	{
		insertPoint(k);
	}

	// Phase 2  --- TODO
}

