// BIRCH.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "BIRCH.h"

std::vector<Point> total;


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
		}
		catch (std::exception)
		{
		}
		// erste Zeile schmeiﬂt Exception! ("x","y")
	}
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
    std::cout << "Hello World!\n"; 
}

