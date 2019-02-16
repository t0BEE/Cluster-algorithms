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
		std::vector<Cluster> clustersInLeafNode;

	// return the new node in case of a split, otherwise return NULL
	CFTreeNode insertToLeaf(Point addPoint);

public:
	CFTreeNode();
	CFTreeNode(Cluster startingCluster);
	CFTreeNode insertPoint(Point addPoint);
	CFTreeNode insertNode(CFTreeNode addNode);
	void removeFromNode(int clusterIndex);
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
	CFTreeNode splitLeaf();
	CFTreeNode splitNonLeaf();
	int getNumberOfEntries();
	int getNumberOfClusterEntries();  // important to distinguish between leaf nodes and non-leaf
};