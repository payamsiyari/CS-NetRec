#ifndef snap_Sampling_h
#define snap_Sampling_h

#include "Snap.h"

using namespace TSnap;

class Sampling {
public:
    double sampleRate;
	PNGraph GroundTruth; // The Original Graph :: Input
	PNGraph DiffusionGraph;
	PNGraph SampledTree;
	PNGraph SampledGraph;
	int StartNId;
	int TreeDepth;

public:

    Sampling(double sampleRate, PNGraph GroundTruth);
	void RW(); // Random Walk Sampling
	void BFS(); // BFS Sampling
	void MHRW(); // MHRW Sampling
};

#endif

