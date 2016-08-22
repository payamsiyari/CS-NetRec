#include "stdafx.h"
#include "GenCascs.h"

//************************************************************

GenCascs::GenCascs() {

}

/*GenCascs::GenCascs(const TStr DiffNet_FNM) {
 this->DiffNet_FNM = DiffNet_FNM;
 }*/

 GenCascs::GenCascs(const int& NCascades, const int& TModel, const double& alpha,
 const double& alpha2, const double& p, const double& beta,
 const double& std_waiting_time, const double& std_beta,
 const PNGraph GroundTruth, const TStr DiffNet_FNM) {

 this->NCascades = NCascades;
 this->TModel = TModel;
 this->alpha = alpha;
 this->alpha2 = alpha2;
 this->p = p;
 this->beta = beta;
 this->std_waiting_time = std_waiting_time;
 this->std_beta = std_beta;
 this->GroundTruth = GroundTruth;
 this->DiffusionNetFNm = DiffNet_FNM;
 }

// Generate Diffusion Network
void GenCascs::GenDiffNet() {

	int last = 0;

	//*********Generate Cascades********
	for (int i = 0; (i < NCascades) || ((double) EdgesUsed.Len() < -(double) NCascades / 100.0 * (double) GroundTruth->GetEdges()); i++) {

		TCascade C(alpha);
		GenCascades(C, EdgesUsed);
		AddCasc(C);
		// printf("Add Cascade : %d\n",i);
		if (EdgesUsed.Len() - last > 100) {
			last = EdgesUsed.Len();
			//printf("Cascades: %d, Edges used: %d/%f\n", GetCascs(), EdgesUsed.Len(),
					//-(double) NCascades / 100.0 * (double) GroundTruth->GetEdges());
		}
	}

	//********Generate Diffusion Network********

	DiffusionGraph = TNGraph::New();
	for (TIntPrIntH::TIter EI = EdgesUsed.BegI(); EI < EdgesUsed.EndI(); EI++) {
		DiffusionGraph->AddNode(EI.GetKey().Val1.Val);
		DiffusionGraph->AddNode(EI.GetKey().Val2.Val);
		DiffusionGraph->AddEdge(EI.GetKey().Val1.Val, EI.GetKey().Val2.Val);
	}

	// Save Cascades
	SaveCascades("Cascades.txt");
	SaveDiffNet();
	//printf("\nrun time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
}

void GenCascs::GenCascades(TCascade& C, TIntPrIntH& EdgesUsed) {

	TIntFltH InfectedNIdH; //The map of Infected Nodes and the infection times -> key: infected node value:time
	//InfectedBy: The map of edges -> key:infectedBy value:infected

	C.Model = TModel;
	double GlobalTime;
	int StartNId;

	if (GroundTruth->GetNodes() == 0)
		return;

	while (C.Len() < 2) {
		C.Clr();
		InfectedNIdH.Clr();
		C.InfectedBy.Clr();
		GlobalTime = 0;

		StartNId = GroundTruth->GetRndNId();
		InfectedNIdH.AddDat(StartNId) = GlobalTime;

		while (true) {
			// sort by time & get the oldest node that did not run infection
			InfectedNIdH.SortByDat(true);
			const int& NId = InfectedNIdH.BegI().GetKey();
			GlobalTime = InfectedNIdH.BegI().GetDat();

			// all the nodes has run infection
			if (GlobalTime == 1e12)
				break;

			// add current oldest node to the network and set its time
			C.Add(NId, GlobalTime);

			// run infection from the current oldest node
			const TNGraph::TNodeI NI = GroundTruth->GetNI(NId);
			for (int e = 0; e < NI.GetOutDeg(); e++) {
				// flip biased coin (set by beta)
				if (TInt::Rnd.GetUniDev() > beta + std_beta * TFlt::Rnd.GetNrmDev())
					continue;

				const int DstNId = NI.GetOutNId(e);

				// not infecting the parent
				if (C.InfectedBy.IsKey(NId) && C.InfectedBy.GetDat(NId).Val == DstNId)
					continue;

				double sigmaT;
				switch (TModel) {
				case 0:
					// exponential with alpha parameter
					sigmaT = (p * alpha + (1 - p) * alpha2) * TInt::Rnd.GetExpDev();
					while (!(sigmaT >= 1 && sigmaT < 100)) {
						sigmaT = (p * alpha + (1 - p) * alpha2) * TInt::Rnd.GetExpDev();
					}
					break;
					// power-law
				case 1:
					// power-law with alpha parameter
					sigmaT = sigmaT = p * TInt::Rnd.GetPowerDev(alpha) + (1 - p) * TInt::Rnd.GetPowerDev(alpha2);
					while (!(sigmaT >= 1 && sigmaT < 100)) {
						sigmaT = p * TInt::Rnd.GetPowerDev(alpha) + (1 - p) * TInt::Rnd.GetPowerDev(alpha2);
					} // skip too large time difference (>100 days)
					break;
				default:
					sigmaT = 1;
					break;
				}

				if (std_waiting_time > 0)
					sigmaT = TFlt::GetMx(0.0, sigmaT + std_waiting_time * TFlt::Rnd.GetNrmDev());

				double t1 = GlobalTime + sigmaT;

				// if a node is infected by another node and the new infection time is lower, replace the information (parent & time)
				if (InfectedNIdH.IsKey(DstNId)) {
					double t2 = InfectedNIdH.GetDat(DstNId);
					if (t2 > t1 && t2 != 1e12) {
						InfectedNIdH.GetDat(DstNId) = t1;
						C.InfectedBy.GetDat(DstNId) = NId;
					}
				} else {
					InfectedNIdH.AddDat(DstNId) = t1;
					C.InfectedBy.AddDat(DstNId) = NId;
				}
			}

			// we cannot delete key (otherwise, we cannot sort), so we assign a very big time
			InfectedNIdH.GetDat(NId) = 1e12;
		}

	}

	C.Sort();

	for (TIntH::TIter EI = C.InfectedBy.BegI(); EI < C.InfectedBy.EndI(); EI++) {
		TIntPr Edge(EI.GetDat().Val, EI.GetKey().Val);

		if (!EdgesUsed.IsKey(Edge))
			EdgesUsed.AddDat(Edge) = 0; //A map of edges and the number of presence at cascades -> key: edge value: N in C

		EdgesUsed.GetDat(Edge) += 1;
	}
}

void GenCascs::AddCasc(const TCascade& Cascade) {
	CascV.Add(Cascade);
}
void GenCascs::AddCasc(const TStr& CascStr) {
	TStrV NIdV;
	CascStr.SplitOnAllCh(';', NIdV);
	TCascade C;
	for (int i = 0; i < NIdV.Len(); i++) {
		TStr NId, Tm;
		NIdV[i].SplitOnCh(NId, ',', Tm);
		IAssert( IsNodeNm(NId.GetInt()));
		GetNodeInfo(NId.GetInt()).Vol = GetNodeInfo(NId.GetInt()).Vol + 1;
		C.Add(NId.GetInt(), Tm.GetFlt());
	}
	C.Sort();
	CascV.Add(C);
}

void GenCascs::AddNodeNm(const int& NId, const TNodeInfo& Info) {
	NodeNmH.AddDat(NId, Info);
}

int GenCascs::GetCascs() {
	return CascV.Len();
}

void GenCascs::SaveCascades(const TStr& OutFNm) {
	TFOut FOut(OutFNm);

	// write nodes to file
	for (TNGraph::TNodeI NI = GroundTruth->BegNI(); NI < GroundTruth->EndNI(); NI++) {
		FOut.PutStr(TStr::Fmt("%d,%d\r\n", NI.GetId(), NI.GetId())); // nodes
	}

	FOut.PutStr("\r\n");

	// write cascades to file
	for (int i = 0; i < CascV.Len(); i++) {
		TCascade &C = CascV[i];
		int j = 0;
		for (THash<TInt, THitInfo>::TIter NI = C.NIdHitH.BegI(); NI < C.NIdHitH.EndI(); NI++, j++) {
			if (j > 0)
				FOut.PutStr(TStr::Fmt(";%d,%f", NI.GetDat().NId.Val, NI.GetDat().Tm.Val));
			else
				FOut.PutStr(TStr::Fmt("%d,%f", NI.GetDat().NId.Val, NI.GetDat().Tm.Val));
		}

		if (C.Len() >= 1)
			FOut.PutStr(TStr::Fmt("\r\n"));
	}
}

void GenCascs::LoadCascadesTxt(TSIn& SIn) {
	TStr Line;
	SIn.GetNextLn(Line);
	while (!SIn.Eof() && Line != "") {
		TStrV NIdV;
		Line.SplitOnAllCh(',', NIdV);
		AddNodeNm(NIdV[0].GetInt(), TNodeInfo(NIdV[1], 0));
		SIn.GetNextLn(Line);
	}
	printf("All nodes read!\n");
	while (!SIn.Eof()) {
		SIn.GetNextLn(Line);
		AddCasc(Line);
	}
	printf("All cascades read!\n");
}

void GenCascs::SaveDiffNet() {

	TFOut FOut(DiffusionNetFNm);

	for (TNGraph::TNodeI NI = DiffusionGraph->BegNI(); NI < DiffusionGraph->EndNI(); NI++) {
		//FOut.PutStr(TStr::Fmt("%d\n", NI.GetId())); //
		FOut.PutStr(TStr::Fmt("%d,%d\r\n", NI.GetId(), NI.GetId())); // nodes
	}

	FOut.PutStr("\r\n");

	for (TIntPrIntH::TIter EI = EdgesUsed.BegI(); EI < EdgesUsed.EndI(); EI++) {
		//TIntPr Edge(EI.GetDat().Val, EI.GetKey().Val);
		FOut.PutStr(TStr::Fmt("%d,%d\n", EI.GetKey().Val1.Val, EI.GetKey().Val2.Val));
		//***************Saving Link attendance in Cascades***********************
		//FOut.PutStr(TStr::Fmt("%d %d %d\n", EI.GetKey().Val1.Val,EI.GetKey().Val2.Val, EI.GetDat.Val));
	}
}

void GenCascs::LoadGroundTruthTxt(TSIn& SIn) {
	GroundTruth = TNGraph::New();
	TStr Line;

	// Add Nodes
	SIn.GetNextLn(Line);
	while (!SIn.Eof() && Line != "") {
		TStrV NIdV;
		Line.SplitOnAllCh(',', NIdV);
		GroundTruth->AddNode(NIdV[0].GetInt());
		SIn.GetNextLn(Line);
	}

	// Add Edges
	while (!SIn.Eof()) {
		SIn.GetNextLn(Line);
		TStrV NIdV;
		Line.SplitOnAllCh(',', NIdV);
		GroundTruth->AddEdge(NIdV[0].GetInt(), NIdV[1].GetInt());
	}

	printf("network nodes:%d edges:%d\n", GroundTruth->GetNodes(), GroundTruth->GetEdges());
}
/*
int main(int argc, char* argv[]) {

	Env = TEnv(argc, argv, TNotify::StdNotify);
	Env.PrepArgs(TStr::Fmt("\n Sampling Effect on Diffusion Networks. build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
	TExeTm ExeTm;

	Try
		cout << "salam" << endl;
		GenCascs* genCascs = new GenCascs();

		//Generate Diffusion Network
		TStr GroundTruthFNm = Env.GetIfArgPrefixStr("-gn:", "network.txt", "Input ground-truth network (one file)");

		genCascs->NCascades = Env.GetIfArgPrefixInt("-c:", -95,
				"If positive, number of cascades, if negative, percentage of edges used at least in one cascade (default:-95)\n");
		genCascs->TModel = Env.GetIfArgPrefixInt("-m:", 0,
				"Transmission model\n0:exponential, 1:power law, 2:mixture of 2 exponentials (default:0)\n"); // we allow for mixture of exponentials
		genCascs->alpha = Env.GetIfArgPrefixFlt("-a:", 1.0, "Alpha for transmission model (default:1)\n");
		genCascs->alpha2 = Env.GetIfArgPrefixFlt("-a2:", 1.0,
				"Second alpha for mixture of exponentials/power laws transmission model (default:1)\n");
		genCascs->p = Env.GetIfArgPrefixFlt("-p:", 1.0,
				"Mixture of exponential/power laws transmission model weight (1.0 means max weight in 1st exp) (default:1)\n");
		genCascs->beta = Env.GetIfArgPrefixFlt("-b:", 0.5, "Beta for transmission model (default:0.5)\n");
		genCascs->std_beta = Env.GetIfArgPrefixFlt("-stdb:", 0.0, "Std for Beta in the transmission model (default:0)\n");
		genCascs->std_waiting_time = Env.GetIfArgPrefixFlt("-stdwt:", 0.0, "Std for waiting time in the transmission model (default:0)\n");
		genCascs->perc_rnd_nodes = Env.GetIfArgPrefixFlt("-prnd:", 0.0, "Percentage of random nodes in a cascade (default:0)\n");
		genCascs->perc_rnd_removal = Env.GetIfArgPrefixFlt("-prmv:", 0.0, "Percentage of missing nodes in a cascade (default:0)\n");

		// Save to File
		genCascs->DiffusionNetFNm = Env.GetIfArgPrefixStr("-dn:", "DiffNet.txt", "Input Diffusion network (one file)");

		TFIn FInG(GroundTruthFNm);
		genCascs->LoadGroundTruthTxt(FInG);
		cout << "Load GroundTruth" << endl;

		if (genCascs->NCascades < 0) {
			genCascs->diffRate = -(double) genCascs->NCascades / 100.00;
		}
		genCascs->GenDiffNet();
	Catch
}
*/
/*void GenCascs::GenNoisyCascade(TCascade& C, TIntPrIntH& EdgesUsed) {

 TIntPrIntH EdgesUsedC; // list of used edges for a single cascade
 GenCascade(C, EdgesUsedC);

 // store keys
 TIntV KeyV;
 C.NIdHitH.GetKeyV(KeyV);
 Nasrini
 // store first and last time
 double tbeg = TFlt::Mx, tend = TFlt::Mn;
 for (int i=0; i < KeyV.Len(); i++) {
 if (tbeg > C.NIdHitH.GetDat(KeyV[i]).Tm) tbeg = C.NIdHitH.GetDat(KeyV[i]).Tm;
 if (tend < C.NIdHitH.GetDat(KeyV[i]).Tm) tend = C.NIdHitH.GetDat(KeyV[i]).Tm;
 }

 // remove PercRndRemoval% of the nodes of the cascades


 if (PercRndRemoval > 0) {
 for (int i=KeyV.Len()-1; i >= 0; i--) {
 if (TFlt::Rnd.GetUniDev() < PercRndRemoval) {
 // remove from the EdgesUsedC the ones affected by the removal
 TIntPrV EdgesToRemove;
 for (TIntPrIntH::TIter EI = EdgesUsedC.BegI(); EI < EdgesUsedC.EndI(); EI++) {
 if ( (KeyV[i]==EI.GetKey().Val1 && C.IsNode(EI.GetKey().Val2) && C.GetTm(KeyV[i]) < C.GetTm(EI.GetKey().Val2)) ||
 (KeyV[i]==EI.GetKey().Val2 && C.IsNode(EI.GetKey().Val1) && C.GetTm(KeyV[i]) > C.GetTm(EI.GetKey().Val1)) ) {
 EI.GetDat() = EI.GetDat()-1;

 if (EI.GetDat()==0)
 EdgesToRemove.Add(EI.GetKey());
 }
 }

 for (int er=0; er<EdgesToRemove.Len(); er++)
 EdgesUsedC.DelKey(EdgesToRemove[er]);

 C.Del(KeyV[i]);
 }
 }

 // defrag the hash table, otherwise other functions can crash
 C.NIdHitH.Defrag();
 }

 // Substitute PercRndNodes% of the nodes for a random node at a random time
 if (PercRndNodes > 0) {
 for (int i=KeyV.Len()-1; i >= 0; i--) {
 if (TFlt::Rnd.GetUniDev() < PercRndNodes) {
 // remove from the EdgesUsedC the ones affected by the change
 TIntPrV EdgesToRemove;
 for (TIntPrIntH::TIter EI = EdgesUsedC.BegI(); EI < EdgesUsedC.EndI(); EI++) {
 if ( (KeyV[i]==EI.GetKey().Val1 && C.IsNode(EI.GetKey().Val2) && C.GetTm(KeyV[i]) < C.GetTm(EI.GetKey().Val2)) ||
 (KeyV[i]==EI.GetKey().Val2 && C.IsNode(EI.GetKey().Val1) && C.GetTm(KeyV[i]) > C.GetTm(EI.GetKey().Val1)) ) {
 EI.GetDat() = EI.GetDat()-1;

 if (EI.GetDat()==0)
 EdgesToRemove.Add(EI.GetKey());
 }
 }

 for (int er=0; er<EdgesToRemove.Len(); er++)
 EdgesUsedC.DelKey(EdgesToRemove[er]);

 printf("Old node n:%d t:%f --", KeyV[i].Val, C.GetTm(KeyV[i]));
 C.Del(KeyV[i]);

 // not repeating a label
 double tnew = 0;
 int keynew = -1;
 do {
 tnew = tbeg + TFlt::Rnd.GetUniDev()*(tend-tbeg);
 keynew = Graph->GetRndNId();
 } while (KeyV.IsIn(keynew));

 printf("New node n:%d t:%f\n", keynew, tnew);

 C.Add(keynew, tnew);
 KeyV.Add(keynew);
 }
 }
 }

 // add to the aggregate list (EdgesUsed)
 EdgesUsedC.Defrag();

 for (int i=0; i<EdgesUsedC.Len(); i++) {
 if (!EdgesUsed.IsKey(EdgesUsedC.GetKey(i))) EdgesUsed.AddDat(EdgesUsedC.GetKey(i)) = 0;

 EdgesUsed.GetDat(EdgesUsedC.GetKey(i)) += 1;
 }
 }
 */
