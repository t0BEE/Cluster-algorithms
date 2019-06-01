#pragma once

#include <math.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <cfloat>



extern std::vector<double> threshold_Value;		// T
extern int b_Entries;
extern int l_Entries;
extern int max_tree_size;
extern int dimensions;
extern int current_tree_size;

//Help functions
void extern addTwoPoints(double* dOne, double* dTwo);

double extern calcDistance(double* dOne, double* dTwo);
