#include <chrono>
#include "BIRCH.h"


std::vector<Point_B> total;
CFTreeNode* rootNode;
CFTreeNode* newTreeRoot;
std::vector<Point_B*> centroids;
static int tree_height;
int current_tree_size, dimensions, page_size, cf_entry_size, memory;
int max_tree_size, b_Entries, l_Entries;
double threshold_Value;
char delim;

std::chrono::high_resolution_clock::time_point startTime, endTime, phase_time;


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
		std::string sData[dimensions];
		double dData[dimensions];
		// seperated with ; in CSV  -- get value after value
		for (int i = 0; i < dimensions; ++i)
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
		// invalid lines throw exceotion --> first line ("x","y")
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
		{ // if root was a leafNode it has to be split
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
	while (current_tree_size > max_tree_size)
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
			for (int i = (int) newTree->childCF.size() - 1; i >= 0; --i)
			{
				newTree->childCF.insert(newTree->childCF.begin(), newTree->childNodes[i]->getCF());
				newTree->childCF.pop_back();
			}
			// clean tree - delete nodes with no childCFs
			for (int k = 0; k < newTree->childCF.size(); ++k)
			{
				if (newTree->childCF[k].getNumberOfPoints() == 0)
				{
					delete newTree->childNodes[k];
					newTree->childNodes.erase(newTree->childNodes.begin() + k);
					newTree->childCF.erase(newTree->childCF.begin() + k);
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
	delete delRoot;
}


void writeBIRCH_CSVFile(std::ofstream &fileOStream, std::string filename)
{
	fileOStream.open((filename), std::ofstream::trunc);
	fileOStream << "NumberofPoints";
	for (int i = 0; i < dimensions; ++i)
	{
		fileOStream << ";LS[" << i << "]";
	}
	for (int i = 0; i < dimensions; ++i)
	{
		fileOStream << ";SS[" << i << "]";
	}
	fileOStream << "\n";
	CFTreeNode *tmpNode = rootNode;
	double tmpLS[dimensions];
    long double tmpSS[dimensions];
	while (!(tmpNode->isLeafNode())) tmpNode = tmpNode->childNodes.front();
	while (tmpNode->next)
	{
		for (int i = 0; i < tmpNode->childCF.size(); ++i)
		{
			tmpNode->childCF[i].getLS(tmpLS);
			tmpNode->childCF[i].getSS(tmpSS);
			fileOStream << tmpNode->childCF[i].getNumberOfPoints();
			for (int j = 0; j < dimensions; ++j)
			{
				fileOStream << ";" << tmpLS[j];
			}
			for (int j = 0; j < dimensions; ++j)
			{
				fileOStream << ";" << tmpSS[j];
			}
			fileOStream << "\n";
		}
		tmpNode = tmpNode->next;
	}
	for (int i = 0; i < tmpNode->childCF.size(); ++i)
	{
		tmpNode->childCF[i].getLS(tmpLS);
		tmpNode->childCF[i].getSS(tmpSS);
		fileOStream << tmpNode->childCF[i].getNumberOfPoints();
		for (int j = 0; j < dimensions; ++j)
		{
			fileOStream << ";" << tmpLS[j];
		}
		for (int j = 0; j < dimensions; ++j)
		{
			fileOStream << ";" << tmpSS[j];
		}
		fileOStream << "\n";
	}

	fileOStream.close();
}

void kMeans_BIRCH(std::string filename)
{
    // Get centroids & create Cluster
    CFTreeNode* tmpNode;
    tmpNode = rootNode;
    double tmpLS[dimensions] ;
    std::vector<Point_B*> centroids;

    //TODO: leaf nodes als CLustercentroids?
    while(!(tmpNode->isLeafNode()))
    {
        tmpNode = tmpNode->childNodes[0];
    }
    while (tmpNode->next != nullptr)
    {
        for (int j = 0; j < tmpNode->childCF.size(); ++j) {
            tmpNode->childCF[j].getLS(tmpLS);
            for (int d = 0; d < dimensions; ++d)
            {
                tmpLS[d] = tmpLS[d] / tmpNode->childCF[j].getNumberOfPoints();
            }
            centroids.push_back(new Point_B(tmpLS));
        }
        tmpNode = tmpNode->next;
    }
    for (int j = 0; j < tmpNode->childCF.size(); ++j) {
        tmpNode->childCF[j].getLS(tmpLS);
        for (int i = 0; i < dimensions; ++i)
        {
            tmpLS[i] = tmpLS[i] / tmpNode->childCF[j].getNumberOfPoints();
        }
        centroids.push_back(new Point_B(tmpLS));
    }

    // Create cluster
    for (int k = 0; k < centroids.size(); ++k)
    {
        clusters.push_back(new Cluster_B());
        centroids[k]->getCoordinates(tmpLS);
        clusters[k]->centroid.setCoordinates(tmpLS);
    }
    // k-means iterations
    assignPoints_B();
    for (int i = 0; i < 6; ++i) // 6 iterations
    {
        for (int k = 0; k < clusters.size(); ++k)
        {
            clusters[k]->calcCentroid();
            clusters[k]->clusterList.clear();
        }
        assignPoints_B();
    }

    // write result
    std::ofstream fOutput;
    fOutput.open(("finalOutput_" + filename));
    fOutput << "x;y;c\n";
    double tmpPoint[dimensions];
    for (int i = 0; i < clusters.size(); ++i)
    {
        for (int j = 0; j < clusters[i]->clusterList.size(); ++j)
        {
            clusters[i]->clusterList[j].getCoordinates(tmpPoint);
            for (int k = 0; k < dimensions; ++k)
            {
                fOutput << tmpPoint[k] << ";";
            }
            fOutput << (i + 1) << "\n";
        }
    }
    for (int i = 0; i < clusters.size(); ++i)
    {
        clusters[i]->centroid.getCoordinates(tmpPoint);
        for (int k = 0; k < dimensions; ++k)
        {
            fOutput << tmpPoint[k] << ";";
        }
        fOutput << clusters.size() + 3 << "\n";
    }
    fOutput.close();
}

void assignPoints_B()
{
    for (unsigned int i = 0; i < total.size(); ++i)
    {
        int shortestDistance = -1;
        double tmpPointOne[dimensions], tmpCentroid[dimensions];
        double distance = DBL_MAX, tmpDis;
        for (unsigned int k = 0; k < clusters.size(); ++k)
        {
            clusters[k]->centroid.getCoordinates(tmpCentroid);
            total[i].getCoordinates(tmpPointOne);
            tmpDis = calcDistance(tmpCentroid, tmpPointOne);
            if (tmpDis < distance)
            {
                distance = tmpDis;
                shortestDistance = k;
            }
        }
        clusters[shortestDistance]->clusterList.push_back(total[i]);
    }
}

void rebuild()
{
	// change threshould value || the paper has no perfect solution
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

int birch(std::string filename)
{
	threshold_Value = 0.0;
	current_tree_size = 0;
	rootNode = new CFTreeNode();
	ClusteringFeature newCF;
	current_tree_size++;
	tree_height = 1;
	std::ofstream csvOutputfile;
	// read CSV
	readBIRCHCSV(filename);
	// Phase 1
    std::cerr << "    --> Phase 1" << std::endl;
	double tmpLS[dimensions];
    long double tmpSS[dimensions];
	for (int k = 0; k < total.size(); ++k)
	{
		for (int i = 0; i < dimensions; ++i)
		{
			tmpLS[i] = total[k].getCoordinate(i);
			tmpSS[i] = pow(tmpLS[i], 2);
		}
		newCF = ClusteringFeature(1, tmpLS, tmpSS);
		insertCF(newCF);
	}
	// write leaf CFs in CSV
	writeBIRCH_CSVFile(csvOutputfile, "outputBIRCH_" + filename);
    phase_time = std::chrono::high_resolution_clock::now();
    std::cerr << "    --> Phase 3" << std::endl;
	kMeans_BIRCH(filename);
	centroids.clear();
	total.clear();
	deleteTree(rootNode);
	return 0;
}

int main(int argc, char *argv[])
{
	if (argc < 7)
	{
		std::cerr << "Need at least 5 parameters: testCaseName, measurementDelim, runs, input_data, dimensions, delimiter, page size in Byte" << std::endl;
		return 1;
	}

	std::string testCaseName = std::string(argv[1]);
	char measurementDelim = argv[2][0];
	int runs = std::stoi(argv[3], nullptr, 10);
    std::string dataFile(argv[4]);

	dimensions = std::stoi(argv[5], nullptr, 10);
	delim = argv[6][0];
	page_size = std::stoi(argv[7], nullptr, 10);

    std::ifstream file(dataFile, std::ifstream::in | std::ifstream::binary);

    file.seekg(0, std::ios::end);
    memory = (int) (file.tellg() * 0.1);
    file.close();

    cf_entry_size = sizeof(int) + (sizeof(double) + sizeof(long double)) * dimensions;
    max_tree_size = memory / page_size;
    b_Entries = page_size / cf_entry_size;
    l_Entries = page_size / cf_entry_size;

    std::cerr << "TestCaseName: " << testCaseName << " --- measurementDelim: " << measurementDelim << " --- runs: " << runs << " --- dataFile: " << dataFile;
    std::cerr << " ---dimensions: " << dimensions << " --- delim: " << delim << " --- page_size: " << page_size/1024 << "KB --- memory: " << memory/1024 <<"KB" << std::endl;

    // warm up
    for (int i = 0; i < 2; ++i)
    {
		std::cerr << "warmUP #" << i << std::endl;
		birch(dataFile);
    }
	std::cerr << "warmUP DONE" << std::endl;
	std::ofstream fileOStream;
	fileOStream.open(("BIRCH_Sequential_" + dataFile));
	std::cerr << testCaseName;
	fileOStream << testCaseName << std::endl;
	long long int avgTime = 0;
	long long int avgPhase = 0;

	for (int i = 0; i < runs; ++i) {
		std::cerr << "Testrun #" << i << std::endl;
		startTime = std::chrono::high_resolution_clock::now();
		birch(dataFile);
		endTime = std::chrono::high_resolution_clock::now();

		long long int timeMS = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
		long long int phaseMS = std::chrono::duration_cast<std::chrono::microseconds>(phase_time - startTime).count();

		std::cerr << timeMS << "ms" << std::endl;
		fileOStream << "Run:" << i << "  --  " << timeMS << " ms  --- BIRCH phase 1: " << phaseMS << "ms"<< std::endl;
		avgTime += timeMS;
        avgPhase += phaseMS;
	}
	fileOStream << "----------------------------------" << std::endl << "Average Time: " << (avgTime/runs) << " ms  --- Phase 1 Average: " << (avgPhase/runs) << "ms" << std::endl;
	fileOStream.close();

	return 0;
}

