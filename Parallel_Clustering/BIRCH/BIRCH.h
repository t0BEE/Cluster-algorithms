#pragma once

#include "CFTreeNode.h"
#include "Point.h"
#include <chrono>
#include <omp.h>

int main(int argc, char *argv[]);
int birch(std::string filename);
void writeBIRCH_CSVFile(std::ofstream &fileOStream, std::string filename);
void insertCF(ClusteringFeature addCF, int treeID);
void rebuild(int treeID);
void deleteTree(CFTreeNode* delRoot);
void pathCopy(CFTreeNode* newTree, CFTreeNode* oldTree, int treeID);
void prevNextChain(CFTreeNode* node);
void calcTreeSize(CFTreeNode* node, int treeID);

// k-Means
void kMeans_BIRCH(std::string filename);
void assignPoints_B();
