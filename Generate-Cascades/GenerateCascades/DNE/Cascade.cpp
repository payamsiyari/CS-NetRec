#include "stdafx.h"
#include "Cascade.h"

double TCascade::TransProb(const int& NId1, const int& NId2) const {
	//cout<<"TransProb Started"<<endl;
	if (! IsNode(NId1) || ! IsNode(NId2)) { return Eps; }
	if (GetTm(NId1) >= GetTm(NId2)) { return Eps; }
	if (Model==0)
		return Alpha*exp(-Alpha*(GetTm(NId2)-GetTm(NId1))); // exponential
	else if (Model==1)
		return (Alpha-1)*pow((GetTm(NId2)-GetTm(NId1)), -Alpha); // power-law
	else
		return Alpha*(GetTm(NId2)-GetTm(NId1))*exp(-0.5*Alpha*pow(GetTm(NId2)-GetTm(NId1), 2)); // rayleigh

	return (-1);
}

TStr TCascade::GetProb(const PNGraph& G) {//*********(CHANGED BY P. SIYARI)********
    //double P = 0;
    double P = 1;
    bool pChanged = false;
    TStr treeOut="";
    for (int n = 0; n < Len(); n++) {
      const int DstNId = GetNode(n);
      const double DstTm = GetTm(DstNId);
      TNGraph::TNodeI NI = G->GetNI(DstNId);
      //double MxProb = log(Eps);
        double MxProb = Eps;

        //cout << "MxProb:" << MxProb << endl;
      int BestParent = -1;
      for (int e = 0; e < NI.GetInDeg(); e++) {
        const int SrcNId = NI.GetInNId(e);
        if (IsNode(SrcNId) && GetTm(SrcNId) < DstTm) {
          //const double Prob = log(TransProb(SrcNId, DstNId));
            const double Prob = TransProb(SrcNId, DstNId);
            //cout << "Prob:" << Prob << endl;
            if (MxProb < Prob) { MxProb = Prob;  BestParent = SrcNId; }
        }
      }
      NIdHitH.GetDat(DstNId).Parent = BestParent;
      //P += MxProb;
        //cout << "MxProb':" << MxProb << endl;
        TNGraph::TNodeI NI2;
        if (MxProb > Eps){
            NI2 = G->GetNI(BestParent);
            const TStr fmt3 = treeOut + "%d-%f";
            treeOut=TStr::Fmt(fmt3.GetCStr(),NI2.GetId(),GetTm(BestParent));
            const TStr fmt1 = treeOut + ",%d-%f;";
            treeOut=TStr::Fmt(fmt1.GetCStr(),NI.GetId(),GetTm(DstNId));
            P *= MxProb;
            pChanged = true;
        }
        //cout << "P:" << P << endl;
    }
    //cout << "---End of Cascade---" << endl;
    if (pChanged){
        const TStr fm4 = treeOut + "%f";
        return TStr::Fmt(fm4.GetCStr(),P);
    }
    return 0;
}

// init prob of a cascade in an empty graph
void TCascade::InitProb() {
    CurProb = log(Eps) * Len();
    for (int i = 0; i < Len(); i++) {
      NIdHitH[i].Parent = -1; }
}

// update the cascade probability given a new edge (N1, N2) in the graph
double TCascade::UpdateProb(const int& N1, const int& N2, const bool& UpdateProb) {
    if (!IsNode(N1) || !IsNode(N2)) { return CurProb; }
    if (GetTm(N1) >= GetTm(N2)) { return CurProb; }
    const double P1 = log(TransProb(GetParent(N2), N2));
    const double P2 = log(TransProb(N1, N2)); // N1 influences N2
    if (P1 < P2) {
      if (UpdateProb) { // the edge is there, update the CurProb and best Parent
        CurProb = CurProb - P1 + P2;
        NIdHitH.GetDat(N2).Parent = N1;
      } else {
        return CurProb - P1 + P2; }
    }
    return CurProb;
}
