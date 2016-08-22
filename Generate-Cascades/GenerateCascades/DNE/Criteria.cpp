#include "stdafx.h"
#include "Criteria.h"


//************************************************************

Criteria::Criteria(PNGraph graph){
    //cerr<<"A criteria is constructed!"<<endl;
	this->graph = graph;
	seeds = new TLst<int>();
	leaves = new TLst<int>();
	nonLeaves = new TLst<int>();
	checkNodes();
}

void Criteria::checkNodes(){
	for (TNGraph::TNodeI NI = graph->BegNI(); NI < graph->EndNI(); NI++) {
	      if (NI.GetInDeg() == 0 && NI.GetOutDeg() != 0){
	    	  seeds->AddFront(NI.GetId());
	      } else if(NI.GetInDeg() != 0 && NI.GetOutDeg() == 0){
	    	  leaves->AddFront(NI.GetId());
	      }else if(NI.GetInDeg() != 0 && NI.GetOutDeg() != 0){
	    	  nonLeaves->AddFront(NI.GetId());
	      }
	}
}

int Criteria::getDiameter(){
    return GetBfsFullDiam(graph, graph->GetNodes(), true); // Input Graph, The Number of Test Nodes, is graph directed or not?
}

TLst<int>* Criteria::getSeeds(){

    return seeds;
}

TLst<int>* Criteria::getLeaves(){
    return leaves;
}

TLst<int>* Criteria::getNonLeaves(){
    return nonLeaves;
}

