#pragma once
#define DIMENSIONS 2		// d
#define B_ENTRIES 6		// B = ( P / ( C * d )) 
#define L_ENTRIES 10	// L
#define MEMORY			// M = 0.05 * data size in Byte
#define PAGE 1024		// P
#define CONST_MAP_DIM	// C
#define DISK			// R = 0.2 * M

static double threshold_Value = 7.0;	// T

// inserting Points -- not clusters
// use D0 as distance meassurement
// Threshold condition is radius -- easier / faster