#pragma once
#define DIMENSIONS 2			// d
#define B_ENTRIES 21			// B = ( P / ( C * d )) 
#define L_ENTRIES 21			// L
#define MEMORY 1048576			// 1MB || M = 0.05 * data size in Byte || how much memory do you like to assign
#define PAGE_SIZE 1024			// P
#define DISK					// R = 0.2 * M
#define MAXIMUM_TREE_SIZE 1024	// MEMORY / PAGE_SIZE
static double threshold_Value = 0.0;	// T
static double const_map_dimension = sizeof(double) * 3;			// C
static double cf_entry_size = const_map_dimension * DIMENSIONS;	// C * d
static int current_tree_size;
// inserting Points -- not clusters
// use D0 as distance meassurement
// Threshold condition is radius -- easier / faster