#include "kMeans.h"

std::vector<Point> total;
std::vector<Cluster> clusters;


// Assign points to clusters
// By calculating the distance to the centroid in iterative way
// if the next centroid is closer overwrite the closest integer
// Input: --
// Output: --
// Effect: cluster number of point is set & pointList of cluster will be increased in size
void assignPoints()
{
    int iterations = (int)total.size();
    int shortestDistance;
    double distance, tmpDis;

    #pragma omp parallel for private(shortestDistance, distance, tmpDis) shared(clusters, total)
    for (unsigned int i = 0; i < iterations; ++i)
    {
        shortestDistance = -1;
        distance = DBL_MAX;
        for (unsigned int k = 0; k < CLUSTER_TOTAL; ++k)
        {
            tmpDis = calcPointDistance(clusters[k].getCentroid(), total[i]);

            if (tmpDis < distance)
            {
                distance = tmpDis;
                shortestDistance = k;
            }
        }

        {
            if (total[i].getCluster() != shortestDistance)
            {
                if (total[i].getCluster() != -1)
                {
                    clusters[total[i].getCluster()].removePoint(total[i]);
                }
                clusters[shortestDistance].addPoint(total[i]);
            }
        }
    }
}

// Recalculate centroids of clusters
// Input: --
// Output: --
// Effect: centroids will be moved
void calcCentroids()
{
    #pragma omp parallel for schedule(dynamic)
	for (int i = 0; i < CLUSTER_TOTAL; ++i)
	{
		clusters[i].calcCentroid();
	}
}

// Read data from csv file
// Input: String file name
// Output: --
// Effect: size of total will be increased
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
		}catch(std::exception)
		{ }
		// erste Zeile schmei�t Exception! ("x","y")
	}
}

// Write results in csv file
// Input: fileStream , filename
// Output: --
// Effect: new file
void writeCSVFile(std::string filename)
{
    std::ofstream fileOStream;
	fileOStream.open((filename), std::ofstream::trunc);
	fileOStream << "a;b;c\n";
	for (unsigned int j = 0; j < total.size(); ++j)
	{
		for (int k = 0; k < DIMENSIONS; ++k)
		{
		fileOStream << std::to_string(total[j].getCoordinate(k)) << ";";
		}			
		fileOStream << std::to_string(total[j].getCluster()) << "\n";
	}
	// write Centroids in CSV with cluster number = total
	for (int j = 0; j < CLUSTER_TOTAL; ++j)
	{
		for (int k = 0; k < DIMENSIONS; ++k)
		{
			fileOStream << std::to_string(clusters[j].getCentroid().getCoordinate(k)) << ";";
		}			
		fileOStream << std::to_string(CLUSTER_TOTAL) << "\n";
	}
	fileOStream.close();
}

int k_Means(std::string filename)
{
    // Init phase -----------------------------
    // really randomize the pick
    //std::srand((unsigned int)std::time(nullptr));

    //read CSV
    readCSV(filename);

    // Init Clusters
    for (int i = 0; i < CLUSTER_TOTAL; ++i) {
        clusters.push_back(Cluster(i, total[(i * 11)%total.size()]));
        //clusters.push_back(Cluster(i, total[(unsigned int) std::rand()%total.size()]));
        //clusters.push_back(Cluster(i, Point(std::rand()%7, std::rand()%7)));
    }

    assignPoints();


    ////       start working       //////////////

    for (int i = 0; i < ITERARTIONS; ++i)
    {
        calcCentroids();
        assignPoints();
    }
    writeCSVFile(("K_output_Parallel.csv"));
    clusters.clear();
    total.clear();
    return 0;
}

int main(int argc, char *argv[])
{
    /*
    if(argc < 8)
    {
        std::cerr << "Need at least 5 parameters: testCaseName, measurementDelim, runs, input_data, dimensions, delimiter, epsilon, minPts" << std::endl;
        return 1;
    }

    std::string testCaseName = std::string(argv[1]);
    char measurementDelim = argv[2][0];
    int runs = std::stoi(argv[3], nullptr, 10);

    std::string dataFile(argv[4]);
    int dimensions = std::stoi(argv[5], nullptr, 10);
    char delim = argv[6][0];
    float epsilon = std::stof(argv[7], nullptr);
    int minPts = std::stoi(argv[8], nullptr, 10);
    */
    std::string testCaseName = "Development_kMeans_parallel";
    char measurementDelim = ';';
    int runs = 16;

    // warm up --> caches
    for (int i = 0; i < 2; ++i)
    {
        k_Means("../../Inputfiles/SampleOld.csv");
    }

    std::chrono::high_resolution_clock::time_point start_time, end_time;
    std::ofstream fileOStream;
    fileOStream.open(("K_Meassure_Parallel.csv"));
    std::cerr << testCaseName;
    fileOStream << testCaseName << std::endl;
    long long int avgTime = 0;

    for (int i = 0; i < runs; ++i)
    {
        start_time = std::chrono::high_resolution_clock::now();
        k_Means("../../Inputfiles/SampleOld.csv");
        end_time = std::chrono::high_resolution_clock::now();

        long long int timeMS = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
        std::cerr << measurementDelim << timeMS;
        fileOStream << "Run:" << i << "  --  " << timeMS << " ms" << std::endl;
        avgTime += timeMS;
    }
    std::cerr << std::endl;
    fileOStream << "----------------------------------" << std::endl << "Average Time: " << (avgTime/runs) << " ms"<< std::endl;
    fileOStream.close();
    return 0;
}