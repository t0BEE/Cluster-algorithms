#pragma once
#include <chrono>
#include <omp.h>
#include <thread>
#include "CFTreeNode.h"
#include "Point.h"

int main(int argc, char *argv[]);
int birch(std::string filename);
void writeBIRCH_CSVFile(std::ofstream &fileOStream, std::string filename);
void insertCF(ClusteringFeature addCF);
void rebuild();
void deleteTree(CFTreeNode* delRoot);
void pathCopy(CFTreeNode* newTree, CFTreeNode* oldTree);
void prevNextChain(CFTreeNode* node);
void calcTreeSize(CFTreeNode* node);

// k-Means
void kMeans_BIRCH(std::string filename);
void assignPoints_B();
