#pragma once

#ifndef MAIN_H
#define MAIN_H

#include "point.h"
#include "cluster.h"
#include <iostream>
#include <vector>
#include <float.h>
#include <random>
#include <fstream>
#include <string>
#include <sstream>

double calcDistance(Point, Point);
int main();		// k = number of clusters
void assignPoints();
void readCSV(std::string filename);
void calcCentroids();
double calcDistance(Point, Point);

#endif // !MAIN_H


