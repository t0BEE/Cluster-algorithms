#pragma once

#include "CFTreeNode.h"
#include "Point.h"


int main();
void readCSV(std::string filename);
void writeBIRCH_CSVFile(std::ofstream &fileOStream, std::string filename);
void insertCF(ClusteringFeature addCF);
void rebuild();
void deleteTree(CFTreeNode* delRoot);
void pathCopy(CFTreeNode* newTree, CFTreeNode* oldTree);
void prevNextChain(CFTreeNode* node);
void calcTreeSize(CFTreeNode* node);
