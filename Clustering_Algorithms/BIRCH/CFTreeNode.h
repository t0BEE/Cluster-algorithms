#pragma once

#include "ClusteringFeature.h"

class CFTreeNode
{
private:
	// return the new node in case of a split, otherwise return NULL
	CFTreeNode* insertToLeaf(ClusteringFeature addCF);

public:
	// if the cluster is a Leaf Node it contains no childNodes but pointer to 
	// next and previous Nodes to enable a fast scanning
	std::vector<ClusteringFeature> childCF;
	// Non Leaf Node Variables
	std::vector<CFTreeNode*> childNodes;

	// Leaf Node Variables
		CFTreeNode* prev = NULL;
		CFTreeNode* next = NULL;

	CFTreeNode();
	void getCentroid(double* buffer);
	bool isLeafNode();
	void splitLeaf(CFTreeNode* oldNode, CFTreeNode* newNode);
	ClusteringFeature getCF();
	CFTreeNode* insert(ClusteringFeature addCF);
	CFTreeNode* splitNonLeaf(CFTreeNode* oldNode, CFTreeNode* newNode);
	CFTreeNode* getFirstElement();
	double getclosestDistanceOfEntries();
	void insertNode(ClusteringFeature cfInsert, CFTreeNode* nodeInsert);
};