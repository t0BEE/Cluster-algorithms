#pragma once
#include "kPoint.h"
#include "kCluster.h"
#include "kParameter.h"
#include <iostream>
#include <vector>
#include <cfloat>
#include <random>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>

namespace kMeans {
	double calcDistance(Point, Point);
	int main();		// k = number of clusters
	void assignPoints();
	void readCSV(std::string filename);
	void calcCentroids();
	double calcDistance(Point, Point);
	void writeCSVFile(std::ofstream&, std::string);
}

