#include "stdafx.h"
#include "GenNets.h"

//************************************************************

GenNets::GenNets() {
}

/*GenNets::GenNets(const int& TNetwork, const int& NNodes, const int& NEdges,
 const TStr& NetworkParams) {
 this->TNetwork = TNetwork;
 this->NNodes = NNodes;
 this->NEdges = NEdges;
 this->NetworkParams = NetworkParams;
 }*/

void GenNets::GenerateGroundTruth() {
	/*TKronMtx SeedMtx;
	 TStr MtxNm;

	 switch (TNetwork) {
	 // 2-dimension kronecker network
	 case 0:
	 SeedMtx = TKronMtx::GetMtx(NetworkParams.CStr()); // 0.5,0.5,0.5,0.5

	 printf("\n*** Seed matrix:\n");
	 SeedMtx.Dump();

	 GroundTruth = TKronMtx::GenFastKronecker(SeedMtx, (int)TMath::Log2(NNodes), NEdges, true, 0);

	 break;

	 // forest fire network
	 case 1:*/
	printf("Forest Fire graph for Ground Truth\n");
	TStrV NetworkParamsV;
	NetworkParams.SplitOnAllCh(';', NetworkParamsV);

	TFfGGen FF(false, // BurnExpFireP
			NetworkParamsV[0].GetInt(), // StartNNodes (1)
			NetworkParamsV[1].GetFlt(), // ForwBurnProb (0.2)
			NetworkParamsV[2].GetFlt(), // BackBurnProb (0.17)GenNets genNets
			NetworkParamsV[3].GetInt(), // DecayProb (1)
			NetworkParamsV[4].GetInt(), // Take2AmbasPrb (0)
			NetworkParamsV[5].GetInt()); // OrphanPrb (0)

	FF.GenGraph(NNodes, false);
	GroundTruth = FF.GetGraph();
	cout << "Finished" << endl;

	//break;
	//}
}

void GenNets::SaveGroundTruth() {
	TFOut FOut(GroundTruthFNm);

	// write nodes to file
	for (TNGraph::TNodeI NI = GroundTruth->BegNI(); NI < GroundTruth->EndNI(); NI++) {
		FOut.PutStr(TStr::Fmt("%d,%d\r\n", NI.GetId(), NI.GetId())); // nodes
	}

	FOut.PutStr("\r\n");

	// write edges to file (not allowing self loops in the network)
	for (TNGraph::TEdgeI EI = GroundTruth->BegEI(); EI < GroundTruth->EndEI(); EI++) {
		// not allowing self loops in the Kronecker network
		if (EI.GetSrcNId() != EI.GetDstNId())
			FOut.PutStr(TStr::Fmt("%d,%d\r\n", EI.GetSrcNId(), EI.GetDstNId()));
	}
}

int main(int argc, char* argv[]) {

	Try
		Env = TEnv(argc, argv, TNotify::StdNotify);
		Env.PrepArgs(
				TStr::Fmt("\n Sampling Effect on Diffusion Networks. build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
		TExeTm ExeTm;

		GenNets* genNets = new GenNets();

		//***************Generate GroundTruth Network*******************
		genNets->TNetwork = Env.GetIfArgPrefixInt("-t:", 0, "Type of network to generate \n 0:kronecker, 1:forest fire (default:0)\n");
		genNets->NNodes = Env.GetIfArgPrefixInt("-n:", 32, "Number of nodes (default:32)\n");
		genNets->NEdges = Env.GetIfArgPrefixInt("-e:", 64, "Number of edges (default:64)\n");
		genNets->NetworkParams = Env.GetIfArgPrefixStr("-g:", TStr("0.5 0.5; 0.5 0.5"),
				"Parameters for the network (default:0.5 0.5; 0.5 0.5)\n");

		// Save to File
		genNets->GroundTruthFNm = Env.GetIfArgPrefixStr("-fn:", "network.txt", "Output ground-truth network (one file)");

		//****************Generate GroundTruth Network***********

		genNets->GenerateGroundTruth();
		genNets->SaveGroundTruth();
	Catch

}
