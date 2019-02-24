#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include "Point.h"
#include "Parameter.h"
#include "CFTreeNode.h"

int main();
void readCSV(std::string filename);
void writeCSVFile(std::ofstream&, std::string);
void insertPoint(unsigned int index);
void rebuild();
void deleteTree(CFTreeNode delRoot);
