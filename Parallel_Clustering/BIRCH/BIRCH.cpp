
#include "BIRCH.h"


std::vector<Point_B> total;
std::vector<CFTreeNode*> rootNode;
std::vector<CFTreeNode*> newTreeRoot;
CFTreeNode* mergedRoot;
std::vector<Point_B*> centroids;
static int tree_height;
int current_tree_size, dimensions, page_size, cf_entry_size, memory;
int max_tree_size, b_Entries, l_Entries, numTrees;
std::vector<double> threshold_Value;
char delim;

std::chrono::high_resolution_clock::time_point startTime, endTime, read_time, write_time, phase_time,merge_time;
/*
 * startTime
 *      - read data
 * read_time
 *      - BIRCH - Phase 1
 * merge_time
 *      - mergeTrees
 * phase_time
 *      - k-means - Phase 3
 * write_time
 *      - write data
 * endTime
 * */


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
		catch (std::exception&)
		{
		}
		// invalid lines throw exceotion --> first line ("x","y")
	}
}

void insertCF(ClusteringFeature addCF, int treeID)
{
	CFTreeNode* newNode;
	newNode = rootNode[treeID]->insert(addCF, treeID);
	// If the root is split, the tree will grow in height
	if (newNode != nullptr)
	{ // root is splitted
		tree_height++;
		current_tree_size += 2; // the new node + a new root node

		if (rootNode[treeID]->isLeafNode())
		{ // if root was a leafNode it has to be split
			CFTreeNode* tmpNode;
			tmpNode = rootNode[treeID];
			rootNode[treeID] = new CFTreeNode();
			rootNode[treeID]->childNodes.push_back(tmpNode);
			rootNode[treeID]->childCF.push_back(tmpNode->getCF());
			rootNode[treeID]->childNodes.push_back(newNode);
			rootNode[treeID]->childCF.push_back(newNode->getCF());
		}
		else
		{ // create a new root and push the new node and old root to it
			CFTreeNode* newRoot = new CFTreeNode();
			
			// Add to the new root
			newRoot->childNodes.push_back(rootNode[treeID]);
			newRoot->childCF.push_back(rootNode[treeID]->getCF());
			newRoot->childNodes.push_back(newNode);
			newRoot->childCF.push_back(newNode->getCF());
			rootNode[treeID] = newRoot;
	
			// fix prev-next-leaf connection
			CFTreeNode* helpOne = rootNode[treeID]->childNodes[0];
			CFTreeNode* helpTwo = rootNode[treeID]->childNodes[1];
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
		rebuild(treeID);
		rootNode[treeID] = newTreeRoot[treeID];
		newTreeRoot[treeID] = nullptr;
		// calculate new current tree size
		current_tree_size = 0;
		calcTreeSize(rootNode[treeID]);
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
void pathCopy(CFTreeNode* newTree, CFTreeNode* oldTree, int treeID)
{
	for (int j = 0; j < oldTree->childCF.size(); ++j)
	{		
		if (oldTree->isLeafNode())
		{
			newTreeRoot[treeID]->insert(oldTree->childCF[j], treeID);
		}
		else
		{
			CFTreeNode *newCopiedNode = new CFTreeNode();

			newTree->insertNode(oldTree->childCF[j], newCopiedNode);
			pathCopy(newTree->childNodes.back(), oldTree->childNodes[j], treeID);
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
	CFTreeNode *tmpNode = mergedRoot;
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
    double tmpLS[dimensions] ;

    for (int l = 0; l < mergedRoot->childCF.size(); ++l)
    {
        mergedRoot->childCF[l].getLS(tmpLS);
        for (int d = 0; d < dimensions; ++d)
        {
            tmpLS[d] = tmpLS[d] / mergedRoot->childCF[l].getNumberOfPoints();
        }
        centroids.push_back(new Point_B(tmpLS));
    }

    double tmpCent[centroids.size()][dimensions];
    int centSize[centroids.size()];
    // k-means iterations
    assignPoints_B();
    for (int i = 0; i < 4; ++i) // 4 iterations
    {
    // calculate centroids
        for (int j = 0; j < centroids.size(); ++j)
        {
            centSize[j] = 0;
            for (int d = 0; d < dimensions; ++d) {
                tmpCent[j][d] = 0;
            }
        }
        for (int j = 0; j < total.size(); ++j)
        {
            for (int d = 0; d < dimensions; ++d)
            {
                tmpCent[total[j].cluster][d] += total[j].getCoordinate(d);
            }
            centSize[total[j].cluster]++;
        }
        for (int m = 0; m < centroids.size(); ++m) {
            for (int d = 0; d < dimensions; ++d) {
                tmpCent[m][d] = tmpCent[m][d] / centSize[m];
            }
		}

        assignPoints_B();
    }

    write_time = std::chrono::high_resolution_clock::now();
    // write result
    std::ofstream fOutput;
    fOutput.open(("finalOutput_" + filename));
    fOutput << "x;y;c\n";
    double tmpPoint[dimensions];
    for (int i = 0; i < total.size(); ++i)
    {
         total[i].getCoordinates(tmpPoint);
         for (int k = 0; k < dimensions; ++k)
         {
             fOutput << tmpPoint[k] << ";";
         }
         fOutput << (total[i].cluster + 1) << "\n";
    }
    for (int i = 0; i < centroids.size(); ++i)
    {
        centroids[i]->getCoordinates(tmpPoint);
        for (int k = 0; k < dimensions; ++k)
        {
            fOutput << tmpPoint[k] << ";";
        }
        fOutput << centroids.size() + 3 << "\n";
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
        for (unsigned int k = 0; k < centroids.size(); ++k)
        {
            centroids[k]->getCoordinates(tmpCentroid);
            total[i].getCoordinates(tmpPointOne);
            tmpDis = calcDistance(tmpCentroid, tmpPointOne);
            if (tmpDis < distance)
            {
                distance = tmpDis;
                shortestDistance = k;
            }
        }
        total[i].cluster = shortestDistance;
    }
}

void rebuild(int treeID)
{
	// change threshould value || the paper has no perfect solution
	// paper: average of the distances between nearest pairs of leaf entries in all leaf nodes
	// find first leaf node
	CFTreeNode* tmpNode = rootNode[treeID];
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
	threshold_Value[treeID] = 5 * distances.back() / distances.size(); //TODO: threshold Value anschauen

	distances.clear();


	// The tree is copied completely and the old tree deleted afterwards
	// During this process the Clusters are reentered

	newTreeRoot[treeID] = new CFTreeNode();

	pathCopy(newTreeRoot[treeID], rootNode[treeID], treeID);
	// prev - next pointer chain
	prevNextChain(newTreeRoot[treeID]);
	// delete old tree
	deleteTree(rootNode[treeID]);

}

int birch(std::string filename)
{

    	current_tree_size = 0;
    for (int j = 0; j < numTrees; ++j) {
        rootNode.push_back(new CFTreeNode());
        threshold_Value.push_back(0.0);
        newTreeRoot.push_back(nullptr);
    }


	current_tree_size++;
	tree_height = 1;
	std::ofstream csvOutputfile;
	// read CSV
	readBIRCHCSV(filename);
	// Phase 1
    read_time = std::chrono::high_resolution_clock::now();
    max_tree_size = max_tree_size / numTrees;


    #pragma omp parallel num_threads(numTrees) private(current_tree_size, tree_height, newTreeRoot)
    {
        double tmpLS[dimensions];
        long double tmpSS[dimensions];
        ClusteringFeature newCF;

        int treeID = omp_get_thread_num();

        #pragma omp for schedule(dynamic)
        for (int k = 0; k < total.size(); ++k)
        {
            for (int i = 0; i < dimensions; ++i)
            {
                tmpLS[i] = total[k].getCoordinate(i);
                tmpSS[i] = pow(tmpLS[i], 2);
            }
            newCF = ClusteringFeature(1, tmpLS, tmpSS);
            insertCF(newCF, treeID);
        }
    }
    merge_time = std::chrono::high_resolution_clock::now();

    // merge the trees, pick the one with the biggest threshold value to be the baseline
    int biggestThreshold = 0;
    for (int l = 1; l < numTrees; ++l) {
        if(threshold_Value[biggestThreshold] < threshold_Value[l])
        {
            biggestThreshold = l;
        }
    }

    CFTreeNode *tmpNode;
    for (int m = 0; m < rootNode.size(); ++m) {
        if(m != biggestThreshold) {
            tmpNode = rootNode[m];
            while (!(tmpNode->isLeafNode())) tmpNode = tmpNode->getFirstElement();
            while (tmpNode != nullptr) {
                for (int i = 0; i < tmpNode->childCF.size(); ++i) {
                    insertCF(tmpNode->childCF[i], biggestThreshold);
                }
                tmpNode = tmpNode->next;
            }
        }
    }
    mergedRoot = rootNode[biggestThreshold];

	// write leaf CFs in CSV
	writeBIRCH_CSVFile(csvOutputfile, "outputBIRCH_" + filename);
    phase_time = std::chrono::high_resolution_clock::now();
    //Phase 3
	kMeans_BIRCH(filename);
	centroids.clear();
	total.clear();

    deleteTree(mergedRoot);
	rootNode.clear();
    newTreeRoot.clear();
	threshold_Value.clear();
	mergedRoot = nullptr;
	return 0;
}

int main(int argc, char *argv[])
{
	if (argc < 8)
	{
		std::cerr << "Need at least 5 parameters: testCaseName, measurementDelim, runs, input_data, dimensions, delimiter, page size in Byte, number of trees" << std::endl;
		return 1;
	}

	std::string testCaseName = std::string(argv[1]);
	char measurementDelim = argv[2][0];
	int runs = std::stoi(argv[3], nullptr, 10);
    std::string dataFile(argv[4]);

	dimensions = std::stoi(argv[5], nullptr, 10);
	delim = argv[6][0];
	page_size = std::stoi(argv[7], nullptr, 10);
	numTrees = std::stoi(argv[8], nullptr, 10);

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
	long long int avgRead = 0;
	long long int avgBIRCH = 0;
	long long int avgKMeans = 0;
	long long int avgWrite = 0;
	long long int avgMerge = 0;

	for (int i = 0; i < runs; ++i) {
		std::cerr << "Testrun #" << i << std::endl;
		startTime = std::chrono::high_resolution_clock::now();
		birch(dataFile);
		endTime = std::chrono::high_resolution_clock::now();

		long long int timeRead = std::chrono::duration_cast<std::chrono::microseconds>(read_time - startTime).count();
		long long int timeBIRCH = std::chrono::duration_cast<std::chrono::microseconds>(merge_time - read_time).count();
		long long int timeMerge = std::chrono::duration_cast<std::chrono::microseconds>(phase_time - merge_time).count();
		long long int timeKMeans = std::chrono::duration_cast<std::chrono::microseconds>(write_time - phase_time).count();
		long long int timeWrite = std::chrono::duration_cast<std::chrono::microseconds>(endTime - write_time).count();

		fileOStream << "Run:" << i << "  --  Read: " << timeRead << " ms  --- BIRCH Build Phase 1: " << timeBIRCH << " ms  --- Tree merge: " << timeMerge <<" ms  --- k-Means Phase 3: " << timeKMeans << " ms  --- Write: " << timeWrite << "ms" << std::endl;
		avgRead += timeRead;
		avgBIRCH += timeBIRCH;
		avgMerge += timeMerge;
		avgKMeans += timeKMeans;
		avgWrite += timeWrite;

	}
	fileOStream << "----------------------------------" << std::endl << "Average Read: " << (avgRead/runs) << " ms  --- Average BIRCH Phase 1: ";
	fileOStream << (avgBIRCH/runs) << " ms  --- Average tree merging: " << (avgMerge/runs) << " ms  --- Average k-Means Phase 3: " << (avgKMeans/runs) << " ms  --- Average Write: " << (avgWrite/runs)<< "ms" << std::endl;
	fileOStream.close();

	// standard output to be piped in extra file
	std::cout << dataFile << "_BIRCH_Seq;" << (avgRead/runs) << ";" << (avgBIRCH/runs) << ";" << (avgMerge/runs) << ";" << (avgKMeans/runs) << ";" << (avgWrite/runs) << std::endl;

	return 0;
}

