#pragma once

#include "Point.h"
#include "Parameter.h"
#include "ClusteringFeature.h"
#include "CLuster.h"
#include <vector>
#include <math.h>

class CFTreeNode
{
private:
	// if the cluster is a Leaf Node it contains Points in the cluster vector
	// if it is a Non Leaf Node the cluster vector is empty and the childCluster vector contains data
	ClusteringFeature cf;
	bool isLeafNode;
	Centroid centroid;
	double radius;
	double diameter;

	// Non Leaf Node Variables
	std::vector<CFTreeNode> childCluster;

	// Leaf Node Variables
	CFTreeNode* prev;
	CFTreeNode* next;
	std::vector<Cluster> clustersOfPoints;

public:
	CFTreeNode();
	void insertPoint(Point addPoint);
	void removeFromCluster(Point rmPoint);
	double calcDistance(Point pEins, Point pZwei);
	ClusteringFeature getCF();
	bool getIsLeafNode();	
	void changeLeafNode(bool value);
	void recalculateCF();
	void calcCentroid();
	Point getCentroid();
	void update();
	double calcRadius();
	double calcDiameter();
};