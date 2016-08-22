#ifndef snap_GenNets_h
#define snap_GenNets_h

#include "Snap.h"

class GenNets {
public:
    int TNetwork;
    int NNodes;
    int NEdges;
    TStr NetworkParams;
	PNGraph GroundTruth; // The Original Graph :: Input
	TStr GroundTruthFNm;

public:

    GenNets();
	//GenNets(const int& TNetwork, const int& NNodes, const int& NEdges, const TStr& NetworkParams);
	void GenerateGroundTruth();
	void SaveGroundTruth();
};

#endif
