#ifndef snap_Criteria_h
#define snap_Criteria_h

#include "Snap.h"


class Criteria {
public:
    PNGraph graph;
	int diameter;// Depth
	int seedsNo;
	int leafsNo;
	int nonLeafsNo;
	TLst<int>* seeds ;
	TLst<int>* leaves;
	TLst<int>* nonLeaves;



public:

    Criteria(PNGraph graph);
    void checkNodes();
    int getDiameter();
    TLst<int>* getSeeds();
    TLst<int>* getLeaves();
    TLst<int>* getNonLeaves();
};

#endif

