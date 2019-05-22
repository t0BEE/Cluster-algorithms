#pragma once

#include "Point.h"

class Cluster_B
{
public:
    std::vector<Point_B> clusterList;
    Point_B centroid;

    void calcCentroid();
};