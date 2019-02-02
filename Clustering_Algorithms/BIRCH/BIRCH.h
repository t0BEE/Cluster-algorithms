#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include "Point.h"
#include "Parameter.h"

int main();
void readCSV(std::string filename);
void writeCSVFile(std::ofstream&, std::string);
