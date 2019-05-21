
#include "Cluster.h"

void Cluster_B::calcCentroid()
{
    double coordinates[DIMENSIONS];
    for (int i = 0; i < DIMENSIONS; ++i)
    {
        coordinates[i] = 0.0;
        for (int j = 0; j < clusterList.size(); ++j)
        {
            coordinates[i] += clusterList[i].getCoordinate(i);
        }
        coordinates[i] = coordinates[i] / clusterList.size();
    }

    this->centroid.setCoordinates(coordinates);
}