#pragma once
#define DIMENSIONS 2			// d
#define B_ENTRIES 21			// B = ( P / ( C * d ))     Non leaf nodes 
#define L_ENTRIES 21			// L						Leaf nodes
#define MEMORY 1048576			// 1MB || M = 0.05 * data size in Byte || how much memory do you like to assign
#define PAGE_SIZE 1024			// P
#define DISK					// R = 0.2 * M
#define MAXIMUM_TREE_SIZE 1024	// MEMORY / PAGE_SIZE

#include <math.h>

static double threshold_Value = 0.0;	// T
static double const_map_dimension = sizeof(double) * 3;			// C
static double cf_entry_size = const_map_dimension * DIMENSIONS;	// C * d
static int current_tree_size;
// inserting Points -- not clusters
// use D0 as distance meassurement
// Threshold condition is radius -- easier / faster

//Help functions
void addTwoPoints(double* dOne, double* dTwo)
{
	for (int i = 0; i < DIMENSIONS; ++i)
	{
		dOne[i] = dOne[i] + dTwo[i];
	}
}

double calcDistance(double* dOne, double* dTwo)
{
	double retValue = 0.0;
	for (int i = 0; i < DIMENSIONS; ++i)
	{
		retValue += pow(dOne[i] - dTwo[i], 2.0);
	}
	return sqrt(retValue);
}