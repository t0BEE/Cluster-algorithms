#include "main.h"
#define CLUSTER_TOTAL 2   // = k
#define ITERARTIONS 6

std::vector<Point> total;
std::vector<Cluster> clusters;
int randomInt = 0;



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

void calcCentroids()
{
	for (int i = 0; i < CLUSTER_TOTAL; ++i)
	{
		clusters[i].calcCentroid();
	}
}

void readCSV(std::string filename)
{
	std::ifstream input(filename);
	std::string line;
	while (std::getline(input, line))
	{
		// put line in stringstream to seperate 
		std::stringstream ststream(line);
		std::string dataX;
		std::string dataY;
		// seperated with ; in CSV  -- get first value X
		std::getline(ststream, dataX, ';');
		// -- get second value Y
		std::getline(ststream, dataY, ';');
		try
		{
			total.push_back(Point(std::stod(dataX), std::stod(dataY)));
		}catch(std::exception)
		{ }
		// erste Zeile schmeißt Exception! ("x","y")
	}
}

// Manhatten distance
double calcDistance(Point one, Point two)
{
	double distance = abs((one.getX() - two.getX()));
	distance += abs((one.getY() - two.getY()));
	return distance;
}

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

int main()
{
	// Init phase ----------------------------- 
	std::ofstream csvOutputfile;

	// really randomize the pick
	std::srand((unsigned int)std::time(nullptr));

	//read CSV
	readCSV("../../Sample.csv");
	std::cout << "--- start ---" << std::endl;
	std::cout << "Number of Points: " << total.size() << std::endl;
	std::cout << "Number of Clusters: " << CLUSTER_TOTAL << std::endl;
	// Init Clusters
	for (int i = 0; i < CLUSTER_TOTAL; ++i) {
		//clusters.push_back(Cluster(i, total[std::rand()%total.size()]));
		clusters.push_back(Cluster(i, Point(std::rand()%7, std::rand()%7)));
	}

	assignPoints();
	writeCSVFile(csvOutputfile, "outpuBefore.csv");

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