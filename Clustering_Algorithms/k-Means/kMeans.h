#pragma once
#include "kCluster.h"
#include <iostream>
#include <vector>
#include <cfloat>
#include <random>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <chrono>


namespace kMeans {
	double calcDistance(Point, Point);
    int main(int argc, char *argv[]);
    int k_Means(std::string filename);
	void assignPoints();
	void readCSV(std::string filename);
	void calcCentroids();
	void writeCSVFile(std::string);
}
