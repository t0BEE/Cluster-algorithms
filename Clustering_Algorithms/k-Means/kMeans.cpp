#include "kMeans.h"



std::vector<Point> total;
std::vector<Cluster> clusters;
int randomInt = 0;


// Assign points to clusters
// By calculating the distance to the centroid in iterative way
// if the next centroid is closer overwrite the closest integer
// Input: --
// Output: --
// Effect: cluster number of point is set & pointList of cluster will be increased in size
void assignPoints()
{
	for (unsigned int i = 0; i < total.size(); ++i)
	{
		int shortestDistance = -1;
		double distance = DBL_MAX, tmpDis;
		for (unsigned int k = 0; k < clusters.size(); ++k)
		{
			tmpDis = calcDistance(clusters[k].getCentroid(), total[i]);
			if (tmpDis < distance)
			{
				distance = tmpDis;
				shortestDistance = k;
			}
		}
		if (total[i].getCluster() != shortestDistance)
		{
			if (total[i].getCluster() != -1)
			{
				clusters[total[i].getCluster()].removePoint(total[i]);
			}
			total[i].setCluster(shortestDistance);
			clusters[shortestDistance].addPoint(total[i]);
		}
	}
}

// Recalculate centroids of clusters
// Input: --
// Output: --
// Effect: centroids will be moved
void calcCentroids()
{
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
		// erste Zeile schmeißt Exception! ("x","y")
	}
}

// Calculate the distance between 2 points
// Manhatten distance
// Input: 2 Points
// Output: double value of distance
// Effect: --
double calcDistance(Point one, Point two)
{
	double distance = 0;
	for (int i = 0; i < DIMENSIONS; ++i)
	{
		distance += abs(one.getCoordinate(i) - two.getCoordinate(i));
	}
	return distance;
}

// Write results in csv file
// Input: fileStream , filename
// Output: --
// Effect: new file
void writeCSVFile(std::ofstream &fileOStream, std::string filename)
{
	fileOStream.open((filename));
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

int main(int argc, char **argv)
{
	// Init phase ----------------------------- 
	std::ofstream csvOutputfile;

	// really randomize the pick
	std::srand((unsigned int)std::time(nullptr));

	//read CSV
	readCSV("../../Inputfiles/Sample.csv");
	std::cout << "--- start ---" << std::endl;
	std::cout << "Number of Points: " << total.size() << std::endl;
	std::cout << "Number of Clusters: " << CLUSTER_TOTAL << std::endl;
	// Init Clusters
	for (int i = 0; i < CLUSTER_TOTAL; ++i) {
		clusters.push_back(Cluster(i, total[(unsigned int) std::rand()%total.size()]));
		//clusters.push_back(Cluster(i, Point(std::rand()%7, std::rand()%7)));
	}

	assignPoints();
	writeCSVFile(csvOutputfile, "outputBefore.csv");

	for (int i = 0; i < CLUSTER_TOTAL; ++i) {
		clusters[i].printList();
	}

	////       start working       //////////////
	
	for (int i = 0; i < ITERARTIONS; ++i)
	{
		calcCentroids();
		assignPoints();
		writeCSVFile(csvOutputfile, ("output" + std::to_string(i) + ".csv"));
	}

	std::cout << "--- ----- ---" << std::endl;
	for (int i = 0; i < CLUSTER_TOTAL; ++i) 
	{
		clusters[i].printList();
	}

	return 0;
}