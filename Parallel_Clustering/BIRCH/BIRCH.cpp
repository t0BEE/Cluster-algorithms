#include "BIRCH.h"
#define BUFFERSIZE 20

std::vector<Point_B> total;
CFTreeNode* rootNode;
CFTreeNode* newTreeRoot;
std::vector<Point_B*> centroids;
static int tree_height;
int current_tree_size, dimensions, page_size, cf_entry_size, memory;
int max_tree_size, b_Entries, l_Entries;
double threshold_Value, radius;
char delim;
ClusteringFeature* buffer[BUFFERSIZE];
bool bufferDone;

std::chrono::high_resolution_clock::time_point startTime, endTime, read_time, write_time, phase_time;
/*
 * startTime
 *      - read data
 * read_time
 *      - BIRCH - Phase 1
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

bool checkEmptyBuffer()
{
    for (int i = 0; i < BUFFERSIZE; ++i) {
        if(buffer[i] != nullptr) return false;
    }
    return true;
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

void prepareInput()
{
    if (bufferDone) return;
    std::vector<Point_B> unused;
    int buffIndex = 0;
    
    for (int k = 0; k < total.size(); ++k) {
        unused.push_back(total[k]);
    }

    #pragma omp parallel shared(buffIndex, unused) num_threads(12)
    {

        double tmpLS[dimensions];
        long double tmpSS[dimensions];

        int start = 0;
        bool picked = false;

            while(unused.size() > 0)
            {

                #pragma omp critical (vector)
                {//use first point as baseline
                    while(unused[start].cluster == -4 && start < unused.size())
                    {
                        start++;
                    }
                    if(unused[start].cluster != -4)
                    {
                        unused[start].cluster = -4;
                        picked = true;
                    }
                }
                if (picked)
                {
                    picked = false;
                    unused[start].getCoordinates(tmpLS);

                    for (int d = 0; d < dimensions; ++d) {
                        tmpSS[d] = tmpLS[d] * tmpLS[d];
                    }
                    ClusteringFeature* newCF= new ClusteringFeature(1, tmpLS, tmpSS);

                    // check if other points are in area close to baseline
                    for (int i = 0; i < unused.size(); ++i) {
                        unused[i].getCoordinates(tmpLS);
                        for (int d = 0; d < dimensions; ++d)
                        {
                            tmpSS[d] = tmpLS[d] * tmpLS[d];
                        }
                        //if CF is not used
                        #pragma omp critical (vector)
                        {
                            if (unused[i].cluster != -4)
                            {
                                if (newCF->absorbCF(ClusteringFeature(1, tmpLS, tmpSS)))
                                {
                                    unused[i].cluster= -4;
                                }
                            }
                        }
                    }
                    // add to buffer if not full
                    while (buffer[buffIndex] != nullptr)
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    }
                    buffer[buffIndex] = newCF;
                    buffIndex++;
                    if (buffIndex == BUFFERSIZE) buffIndex = 0;
                }
                #pragma omp single
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }
                #pragma omp for schedule(static,1)
                for (int i = unused.size() - 1; i >=0 ; --i) {
                    if (unused[i].cluster == -4)
                    {
                        #pragma omp critical (vector)
                        unused.erase(unused.begin() + i);
                    }
                }

                start = 0;
            }
            bufferDone = true;

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
    double tmpLS[dimensions] ;

    for (int l = 0; l < rootNode->childCF.size(); ++l)
    {
        rootNode->childCF[l].getLS(tmpLS);
        for (int d = 0; d < dimensions; ++d)
        {
            tmpLS[d] = tmpLS[d] / rootNode->childCF[l].getNumberOfPoints();
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
    threshold_Value = 5 * distances.back() / distances.size(); //TODO: threshold Value anschauen

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
    bufferDone = false;
    for (int i = 0; i < BUFFERSIZE; ++i) {
        buffer[i] = nullptr;
    }

    // read CSV
    readBIRCHCSV(filename);

    // radius
    radius = 0.0;
    long double tmpCentroid[dimensions];
    double tmpP1[dimensions], tmpP2[dimensions];
    double coor[6], tmpDouble;
    coor[0] = DBL_MAX;
    coor[1] = 0.0;
    coor[2] = DBL_MAX;
    coor[3] = 0.0;
    for (int d = 0; d < dimensions; ++d)
    {
        tmpCentroid[d] = 0.0;
    }
    for (int j = 0; j < total.size(); ++j)
    {
        for (int d = 0; d < dimensions; ++d)
        {
            tmpDouble = total[j].getCoordinate(d);
            if (d == 0)
            {
                if (tmpDouble > coor[1]) coor[1] = tmpDouble;
                if (tmpDouble < coor[0]) coor[0] = tmpDouble;
            }
            if (d == 1)
            {
                if (tmpDouble > coor[3]) coor[3] = tmpDouble;
                if (tmpDouble < coor[2]) coor[2] = tmpDouble;
            }
            tmpCentroid[d] += tmpDouble;
        }
    }
    coor[4] = coor[1] - coor[0];
    coor[5] = coor[3] - coor[2];

    double avgRange = (coor[4] + coor[5]) /2;


    for (int d = 0; d < dimensions; ++d)
    {
        tmpP1[d] = tmpCentroid[d] / total.size();
    }

    for (int j = 0; j < total.size(); ++j)
    {
        total[j].getCoordinates(tmpP2);
        radius += pow( calcDistance(tmpP2, tmpP1),2);
    }
    radius = sqrt(radius / (total.size()));
    threshold_Value = avgRange / (radius *10);

    // Phase 1
    read_time = std::chrono::high_resolution_clock::now();

    #pragma omp parallel num_threads(2) shared(buffer, bufferDone, total)
    {
        #pragma omp single nowait
        {
            prepareInput();
        }
        #pragma omp single nowait
        {
            int bufferIndex = 0;
            while (!bufferDone || !checkEmptyBuffer())
            {
                    while(buffer[bufferIndex] == nullptr)
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    }
                    insertCF(*buffer[bufferIndex]);
                    buffer[bufferIndex] = nullptr;
                    bufferIndex ++;
                    if(bufferIndex == BUFFERSIZE) bufferIndex = 0;
            }
        }

    }

    // write leaf CFs in CSV
    writeBIRCH_CSVFile(csvOutputfile, "outputBIRCH_" + filename);
    phase_time = std::chrono::high_resolution_clock::now();
    //Phase 3
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
    omp_set_nested(1);


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
    fileOStream.open(("BIRCH_Parallel_" + dataFile));
    std::cerr << testCaseName;
    fileOStream << testCaseName << std::endl;
    long long int avgRead = 0;
    long long int avgBIRCH = 0;
    long long int avgKMeans = 0;
    long long int avgWrite = 0;

    for (int i = 0; i < runs; ++i) {
        std::cerr << "Testrun #" << i << std::endl;
        startTime = std::chrono::high_resolution_clock::now();
        birch(dataFile);
        endTime = std::chrono::high_resolution_clock::now();

        long long int timeRead = std::chrono::duration_cast<std::chrono::microseconds>(read_time - startTime).count();
        long long int timeBIRCH = std::chrono::duration_cast<std::chrono::microseconds>(phase_time - read_time).count();
        long long int timeKMeans = std::chrono::duration_cast<std::chrono::microseconds>(write_time - phase_time).count();
        long long int timeWrite = std::chrono::duration_cast<std::chrono::microseconds>(endTime - write_time).count();

        fileOStream << "Run:" << i << "  --  Read: " << timeRead << " ms  --- BIRCH Phase 1: " << timeBIRCH << " ms  --- k-Means Phase 3: " << timeKMeans << " ms  --- Write: " << timeWrite << "ms" << std::endl;
        avgRead += timeRead;
        avgBIRCH += timeBIRCH;
        avgKMeans += timeKMeans;
        avgWrite += timeWrite;

    }
    fileOStream << "----------------------------------" << std::endl << "Average Read: " << (avgRead/runs) << " ms  --- Average BIRCH Phase 1: ";
    fileOStream << (avgBIRCH/runs) << " ms  --- Average k-Means Phase 3: " << (avgKMeans/runs) << " ms  --- Average Write: " << (avgWrite/runs)<< "ms" << std::endl;
    fileOStream.close();

    // standard output to be piped in extra file
    std::cout << dataFile << "_BIRCH_Par;" << (avgRead/runs) << ";" << (avgBIRCH/runs) << ";" << (avgKMeans/runs) << ";" << (avgWrite/runs) << std::endl;

    return 0;
}

