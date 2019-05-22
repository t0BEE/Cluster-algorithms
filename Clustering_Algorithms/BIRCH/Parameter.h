#pragma once
#define DIMENSIONS 2			// d
#define B_ENTRIES 4			// B = ( P / ( C * d ))     Non leaf nodes	-- Standard: 21
#define L_ENTRIES 4			// L						Leaf nodes		-- Standard: 21
#define MEMORY 50			// 1MB || M = 0.05 * data size in Byte || how much memory do you like to assign	-- Ausgerechnet : 1048576
#define PAGE_SIZE 1024			// P
#define DISK					// R = 0.2 * M
#define MAXIMUM_TREE_SIZE 15	// MEMORY / PAGE_SIZE Standard:1024

#include <math.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <cfloat>


extern double threshold_Value;		// T
static double const_map_dimension = sizeof(double) * 3;			// C
static double cf_entry_size = const_map_dimension * DIMENSIONS;	// C * d
extern int current_tree_size;
// inserting Points -- not clusters
// use D0 as distance meassurement
// Threshold condition is radius -- easier / faster

//Help functions
void extern addTwoPoints(double* dOne, double* dTwo);

double extern calcDistance(double* dOne, double* dTwo);
