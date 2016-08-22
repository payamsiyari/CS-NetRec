#include "stdafx.h"

int main(int argc, char* argv[]) {//*********(CHANGED BY P. SIYARI)********

	Env = TEnv(argc, argv, TNotify::StdNotify);
	Env.PrepArgs(
			TStr::Fmt(
					"\n Sampling Effect on Diffusion Networks. build: %s, %s. Time: %s",
					__TIME__, __DATE__, TExeTm::GetCurTm()));
	TExeTm ExeTm;

	//***********************************Getting Input Parameteres*****************************************
	Try

	DNS dns;

	//****************GroundTruth Parameters*****************
	dns.GroundTruthFNm = Env.GetIfArgPrefixStr("-n:", "network.txt",
			"Input ground-truth network (one file)");

	//****************Diffusion Parameters*****************
	/*dns.GenDiffusionNet =
			Env.GetIfArgPrefixInt(
					"-gd:",
					0,
					"Generate Diffusion Network or Read from File \n\
		    0: Generate Diffusin Network 1: Read from File(default:0)\n");*/
	//Generate Diffusion Network
	dns.NCascades =
			Env.GetIfArgPrefixInt(
					"-c:",
					-95,
					"If positive, number of cascades, if negative, percentage of edges used at least in one cascade (default:-95)\n");
	dns.TModel =
			Env.GetIfArgPrefixInt(
					"-m:",
					0,
					"Transmission model\n0:exponential, 1:power law, 2:mixture of 2 exponentials (default:0)\n"); // we allow for mixture of exponentials
	dns.alpha = Env.GetIfArgPrefixFlt("-a:", 1.0,
			"Alpha for transmission model (default:1)\n");
	dns.alpha2 =
			Env.GetIfArgPrefixFlt(
					"-a2:",
					1.0,
					"Second alpha for mixture of exponentials/power laws transmission model (default:1)\n");
	dns.p =
			Env.GetIfArgPrefixFlt(
					"-p:",
					1.0,
					"Mixture of exponential/power laws transmission model weight (1.0 means max weight in 1st exp) (default:1)\n");
	dns.beta = Env.GetIfArgPrefixFlt("-b:", 0.5,
			"Beta for transmission model (default:0.5)\n");
	dns.std_beta = Env.GetIfArgPrefixFlt("-stdb:", 0.0,
			"Std for Beta in the transmission model (default:0)\n");
	dns.std_waiting_time = Env.GetIfArgPrefixFlt("-stdwt:", 0.0,
			"Std for waiting time in the transmission model (default:0)\n");
	/*dns.perc_rnd_nodes = Env.GetIfArgPrefixFlt("-prnd:", 0.0,
			"Percentage of random nodes in a cascade (default:0)\n");
	dns.perc_rnd_removal = Env.GetIfArgPrefixFlt("-prmv:", 0.0,
			"Percentage of missing nodes in a cascade (default:0)\n");*/

	// Can not be Read From File, becuase we need cascades complete infromation
	dns.DiffusionNetFNm = Env.GetIfArgPrefixStr("-dn:", "DiffNet.txt",
			"Diffusion network (one file)");
	//****************Sampling Parameters*****************
	//dns.samplingMethod = Env.GetIfArgPrefixInt("-s:", 0, "Type of Sampling Method\n\
	0:Random Walk, 1:BFS 2:MHRW (default:0)\n");
	/*dns.sampleRate = Env.GetIfArgPrefixFlt("-sr:", 0.5,
			"Sampling Rate from 0-1 (default:0.5)\n");*/

	//****************DNS Parameters***********************
	//dns.DNS_Method = Env.GetIfArgPrefixInt("-dm:", 0, "Type of Sampling Base\n\
	0:Structure, 1:Diffusion (default:0)\n");
	//dns.DNS_FNM_RW = Env.GetIfArgPrefixStr("-o:", TStr("DNS.txt"), "Name of the DNS network (default:DNS)\n"); // Output
	/*dns.st_criteria_RW = Env.GetIfArgPrefixStr("-scr:",
			TStr("St_Criteria_RW.txt"), "Output Criteria (one file)\n"); // Output
	dns.st_criteria_BFS = Env.GetIfArgPrefixStr("-scb:",
			TStr("St_Criteria_BFS.txt"), "Output Criteria (one file)\n"); // Output

	dns.diff_criteria_RW = Env.GetIfArgPrefixStr("-dcr:",
			TStr("Diff_Criteria_RW.txt"), "Output Criteria (one file)\n"); // Output
	dns.diff_criteria_BFS = Env.GetIfArgPrefixStr("-dcb:",
			TStr("Diff_Criteria_BFS.txt"), "Output Criteria (one file)\n"); // Output

	dns.DNS_criteria = Env.GetIfArgPrefixStr("-dnc:",
			TStr("DNS_Criteria.txt"), "DNS Output Criteria (one file)\n"); // Output*/

	//***********************************************************************************************************************

	//****************Generate GroundTruth Network***********
	TFIn FInG(dns.GroundTruthFNm);
	dns.LoadNetworkTxt(FInG, dns.GroundTruth);

	//**************Generate Diffusion Network**************
	if (dns.NCascades < 0) {
		dns.diffRate = -(double) dns.NCascades / 100.00;
	}
	dns.genCascs = new GenCascs(dns.NCascades, dns.TModel, dns.alpha,
			dns.alpha2, dns.p, dns.beta, dns.std_waiting_time, dns.std_beta,
			dns.GroundTruth, dns.DiffusionNetFNm);
	dns.genCascs->GenDiffNet();
	//dns.DiffusionGraph = dns.genCascs->DiffusionGraph;



	//Load Diff Net (Not Applicable in This Problem:( :P )
	/*TFIn FInD(dns.DiffusionNetFNm);
		 dns.LoadNetworkTxt(FInD, dns.DiffusionGraph);
		 cout << "GE" << dns.GroundTruth->GetEdges() << endl;*/

	//***********************Generate DNS**********************

	//**************************RW************************
	/*dns.samplingMethod = 0;
	cout << endl << "**********Sample Rate: " << dns.sampleRate
			<< "***************" << endl << endl;
	cout << "**********Structure-based RW************" << endl << endl;
	dns.Gen_Structure_DNS();
	dns.CalCriteria();
	dns.SaveCriteria(dns.st_criteria_RW);

	cout << "***********Diffusion-based RW*************" << endl << endl;
	dns.Gen_Diffusion_DNS();
	dns.CalCriteria();
	dns.SaveCriteria(dns.diff_criteria_RW);

	//**************************BFS************************
	dns.samplingMethod = 1;
	cout << "************Structure-based BFS**********" << endl << endl;
	dns.Gen_Structure_DNS();
	dns.CalCriteria();
	dns.SaveCriteria(dns.st_criteria_BFS);

	cout << "************Diffusion-based BFS*************" << endl << endl;
	dns.Gen_Diffusion_DNS();
	dns.CalCriteria();
	dns.SaveCriteria(dns.diff_criteria_BFS);

	//*******************DNS Method*******************
	cout << "************DNS*************" << endl << endl;
	dns.CalProbs();
	dns.Gen_DNS();
	dns.CalCriteria();
	dns.CalEstimatedCriteria();
	dns.SaveCriteria(dns.DNS_criteria);*/

	Catch

	return 0;
}

void DNS::LoadNetworkTxt(TSIn& SIn, PNGraph& network) {
	network = TNGraph::New();
	TStr Line;

	// Add Nodes
	SIn.GetNextLn(Line);
	while (!SIn.Eof() && Line != "") {
		TStrV NIdV;
		Line.SplitOnAllCh(',', NIdV);
		network->AddNode(NIdV[0].GetInt());
		SIn.GetNextLn(Line);
	}

	// Add Edges
	while (!SIn.Eof()) {
		SIn.GetNextLn(Line);
		TStrV NIdV;
		Line.SplitOnAllCh(',', NIdV);
		network->AddEdge(NIdV[0].GetInt(), NIdV[1].GetInt());
	}

	printf("network nodes:%d edges:%d\n", network->GetNodes(),
			network->GetEdges());
}

void DNS::Gen_Structure_DNS() {
	cout << "Generate Structure-based Graph" << endl;

	//cout << "Begin Sampling" << endl;
	//cout << "diff rate : " << diffRate << endl;

	sampling = new Sampling(sampleRate * diffRate, GroundTruth);

	if (samplingMethod == 0) {
		sampling->RW();
	} else if (samplingMethod == 1) {
		sampling->BFS();
	}

	SampledGraph = sampling->SampledGraph;
	//cout << "Finish Sampling" << endl;

	cerr << "Diffusion Graph Nodes No. :" << DiffusionGraph->GetNodes() << endl;
	cerr << "Diffusion Graph Edges No. :" << DiffusionGraph->GetEdges() << endl
			<< endl;

	DNS_Graph = TNGraph::New();
	// Extract the shared edge of DiffusionGraph and SampledGraph
	DNS_Graph = ExtractSharedEdges(DiffusionGraph, SampledGraph); //Generate DNS Graph
	//cerr << "Finish Extracting" << endl;
	cerr << "DNS Graph Nodes No. : " << DNS_Graph->GetNodes() << endl;
	cerr << "DNS Graph Edges No. : " << DNS_Graph->GetEdges() << endl << endl;
}

void DNS::Gen_Diffusion_DNS() {

	cout << "Generate Diffusion-Based DNS Graph" << endl;
	//cout << "Begin Sampling" << endl;
	sampling = new Sampling(sampleRate, DiffusionGraph);

	if (samplingMethod == 0) {
		sampling->RW();
	} else if (samplingMethod == 1) {
		sampling->BFS();
	}

	//cout << "Finish Sampling" << endl;

	DNS_Graph = sampling->SampledGraph;
	cerr << "DNS Graph Nodes No. : " << DNS_Graph->GetNodes() << endl;
	cerr << "DNS Graph Edges No. : " << DNS_Graph->GetEdges() << endl << endl;
}

void DNS::Gen_DNS(){

	//******Start Node should be from Diffusion Graph to avoid exploring****
	SampledGraph.Clr();
	int StartNId;
	double tempSampleRate = sampleRate * diffRate;
	do {
		StartNId = DiffusionGraph->GetRndNId();
		//cout << GroundTruth->IsNode(StartNId) << endl;

	} while (!DiffusionGraph->IsNode(StartNId)
			|| (DiffusionGraph->GetNI(StartNId).GetOutDeg() == 0 && DiffusionGraph->GetNI(StartNId).GetInDeg() == 0));

	int edges = GroundTruth->GetEdges();
	SampledGraph = TNGraph::New();
	int currentNode =StartNId;
	TIntPr currentEdge;
	int DstNode;
	double outProbMax = 0;
	int outMax = -1;
	double inProbMax = 0;
	int inMax = -1;

	int chooseDir;
	int choose = false;
	int currentEdges;
	int nextEdges;

	while(SampledGraph->GetEdges() < edges * tempSampleRate){

		currentEdges = SampledGraph->GetEdges();
		SampledGraph->AddNode(currentNode);
		if(GroundTruth->GetNI(currentNode).GetInDeg() == 0 && GroundTruth->GetNI(currentNode).GetOutDeg() == 0){
			currentNode = StartNId;
			continue;
		}

		outProbMax = 0;
		inProbMax = 0;
		outMax = -1;
		inMax = -1;

		//cout<<"Process Out Nodes"<<endl;
		for (int o = 0; o < GroundTruth->GetNI(currentNode).GetOutDeg(); o++) {  // out-links
			DstNode = GroundTruth->GetNI(currentNode).GetOutNId(o);
			SampledGraph->AddNode(DstNode);
			SampledGraph->AddEdge(currentNode, DstNode);
			TIntPr outEdge(currentNode, DstNode);
			if(outProbMax <= edge_prob.GetDat(outEdge)){
				outProbMax = edge_prob.GetDat(outEdge);
				//cout<<"outProbMax: "<<outProbMax<<endl;
				outMax = o;
			}
		}

		//cout<<"Process In Nodes"<<endl;

		for (int i = 0; i < GroundTruth->GetNI(currentNode).GetInDeg(); i++) {  // In-links
			DstNode = GroundTruth->GetNI(currentNode).GetInNId(i);
			SampledGraph->AddNode(DstNode);
			SampledGraph->AddEdge(DstNode, currentNode);
			TIntPr inEdge(DstNode, currentNode);
			if(inProbMax <= edge_prob.GetDat(inEdge)){
				inProbMax = edge_prob.GetDat(inEdge);
				inMax = i;
			}
		}
		//cout<<"Current Node :"<<currentNode<<endl;

		if(outProbMax >= inProbMax)
			currentNode = GroundTruth->GetNI(currentNode).GetOutNId(outMax);
		else
			currentNode = GroundTruth->GetNI(currentNode).GetInNId(inMax);

		//cout<<"outProbMax: "<<outProbMax<<endl;
		//cout<<"InProbMAx: "<<inProbMax<<endl;

		/*int temp;
		//cout<<"before choose"<<endl;
		//cout<<"out: "<<outMax<<endl;
		//cout<<"in: "<<inMax<<endl;
		choose = false;
		while(!choose){
			chooseDir = rand() % 2;
			//	cout<<"choose rand : "<<chooseRand<<endl;
			if(chooseDir == 0 && GroundTruth->GetNI(currentNode).GetOutDeg() != 0){
				temp = GroundTruth->GetNI(currentNode).GetOutNId(outMax);
				//	cout<<"choose :0"<<endl;
				choose = true;
			}
			else if(chooseDir == 1 && GroundTruth->GetNI(currentNode).GetInDeg() != 0){
				temp = GroundTruth->GetNI(currentNode).GetInNId(inMax);
				//	cout<<"choose 1"<<endl;
				choose = true;
			}
		}

		//cout<<"after choose"<<endl;

		currentNode = temp;*/

		nextEdges = SampledGraph->GetEdges();
		if(nextEdges - currentEdges == 0){
			do {
				currentNode = DiffusionGraph->GetRndNId();
				//cout << GroundTruth->IsNode(StartNId) << endl;

			} while (!DiffusionGraph->IsNode(currentNode)
					|| (DiffusionGraph->GetNI(currentNode).GetOutDeg() == 0 && DiffusionGraph->GetNI(currentNode).GetInDeg() == 0));

		}

		//cout<<"temp: "<<temp<<endl;
		//cout<<"Sampled Graph Nodes: "<<SampledGraph->GetNodes()<<endl;
		//cout<<"Sampled Graph Edges: "<<SampledGraph->GetEdges()<<endl;

	}
	http://www.google.com/
	cout<<endl;
	cout<<"Sampled Graph Nodes: "<<SampledGraph->GetNodes()<<endl;
	cout<<"Sampled Graph Edges: "<<SampledGraph->GetEdges()<<endl;
	cout<<endl;

	DNS_Graph = TNGraph::New();
	// Extract the shared edge of DiffusionGraph and SampledGraph
	DNS_Graph = ExtractSharedEdges(DiffusionGraph, SampledGraph); //Generate DNS Graph

	cerr << "DNS Graph Nodes No. : " << DNS_Graph->GetNodes() << endl;
	cerr << "DNS Graph Edges No. : " << DNS_Graph->GetEdges() << endl << endl;

}

PNGraph DNS::ExtractSharedEdges(PNGraph Graph1, PNGraph Graph2) {
	PNGraph OutGraph = TNGraph::New();
	// traverse the edges
	for (TNGraph::TEdgeI EI = Graph1->BegEI(); EI < Graph1->EndEI(); EI++) {
		if (Graph2->IsEdge(EI.GetSrcNId(), EI.GetDstNId(), true)) {
			//cout<<"Add one edge"<<endl;
			OutGraph->AddNode(EI.GetSrcNId());
			OutGraph->AddNode(EI.GetDstNId());
			OutGraph->AddEdge(EI.GetSrcNId(), EI.GetDstNId());
		}
	}
	return OutGraph;
}

void DNS::Find_Nodes_Used() {

	NodesUsed.Clr();
	int src;
	int dst;
	//cout<<"Links Importance: "<<LinksImportance.Len()<<endl;

	for (TIntPrIntH::TIter EI = genCascs->EdgesUsed.BegI();	EI < genCascs->EdgesUsed.EndI(); EI++) {

		src = EI.GetKey().Val1.Val;
		dst = EI.GetKey().Val2.Val; //Destination is important in infected nodes.
		TIntPr edge(src, dst);

		if (!NodesUsed.IsKey(src))
			NodesUsed.AddDat(src) = 0;

		if (!NodesUsed.IsKey(dst))
			NodesUsed.AddDat(dst) = 0;

		NodesUsed.GetDat(src) += genCascs->EdgesUsed.GetDat(edge);
		NodesUsed.GetDat(dst) += genCascs->EdgesUsed.GetDat(edge);
	}
}

void DNS::CalProbs() {

	//cout << "CalProbs Started" << endl;

	Find_Nodes_Used();

	node_casc_prob.Clr();
	node_casc_count.Clr();
	//infectedNode_prob.Clr();
	node_prob.Clr();
	edge_prob.Clr();
	double prob_temp = 0;
	int src;
	int dst;

	//*********Calculate Probabilities of Nodes at the Cascades*********

	for (int i = 0; i < genCascs->CascV.Len(); i++) {
		TCascade &C = genCascs->CascV[i];

		//infectedBy is a map of infected and infecting nodes (key & value)
		for (TIntH::TIter EI = C.InfectedBy.BegI(); EI < C.InfectedBy.EndI();
				EI++) {
			src = EI.GetDat().Val;
			dst = EI.GetKey().Val;

			TIntPr pair1(src, i); // NID and Casc_num
			TIntPr pair2(dst, i);

			TIntPr edge(EI.GetDat().Val, EI.GetKey().Val);


			if (!node_casc_prob.IsKey(pair1)) {
				node_casc_prob.AddDat(pair1) = 0;
				node_casc_count.AddDat(pair1) = 0;
			}

			if (!node_casc_prob.IsKey(pair2)) {
				node_casc_prob.AddDat(pair2) = 0;
				node_casc_count.AddDat(pair2) = 0;
			}

			/*if (!infectedNode_prob.IsKey(dst)) {
				infectedNode_prob.AddDat(dst) = 0;
			}*/

			if (!infectingNode_prob.IsKey(src)) {
				infectingNode_prob.AddDat(src) = 0;
			}

			if(!node_prob.IsKey(src)){
				node_prob.AddDat(src) = 0;
			}

			if(!node_prob.IsKey(dst)){
				node_prob.AddDat(dst) = 0;
			}

			if(!edge_prob.IsKey(edge)){
				edge_prob.AddDat(edge) = 0;
			}

			prob_temp = C.TransProb(EI.GetDat().Val, EI.GetKey().Val);

			node_casc_prob.GetDat(pair1) += prob_temp;
			node_casc_prob.GetDat(pair2) += prob_temp;

			node_casc_count.GetDat(pair1) ++;
			node_casc_count.GetDat(pair2) ++;

			//infectedNode_prob.GetDat(dst) += prob_temp;
			infectingNode_prob.GetDat(src) += prob_temp;

			edge_prob.GetDat(edge) += prob_temp;
		}
	}

	/*for (TIntPrFltH::TIter nc = node_casc_prob.BegI(); nc < node_casc_prob.EndI(); nc++){
			node_prob.GetDat(nc.GetKey().Val1.Val) = nc.GetDat();
	}*/
	/*for (TIntPrFltH::TIter nc = node_casc_prob.BegI(); nc < node_casc_prob.EndI(); nc++){
		node_prob.GetDat(nc.GetKey().Val1.Val) = nc.GetDat() / node_casc_count.GetDat(nc.GetKey());
	}

	for (TIntFltH::TIter itr = node_prob.BegI(); itr < node_prob.EndI();itr++) {
		//cerr<<"node importance key : "<<itr.GetKey().Val<<endl;
		itr.GetDat() /= NodesUsed.GetDat(itr.GetKey().Val);
		//cerr<<"node Prob: "<<itr.GetDat()<<endl;
	}*/

	for(TIntPrFltH::TIter ei = edge_prob.BegI(); ei < edge_prob.EndI(); ei++ ){
		//cout<<"edge prob key: "<<ei.GetKey().Val1.Val<<" "<<ei.GetKey().Val2.Val<<endl;
		ei.GetDat() /= genCascs->EdgesUsed.GetDat(ei.GetKey());
		//ei.GetDat() *= genCascs->EdgesUsed.GetDat(ei.GetKey());
		//cout<<"edge prob key: "<<ei.GetKey().Val1.Val<<" "<<ei.GetKey().Val2.Val<<endl;
	}

	/*
	//******************Set Node Prob by Edge Prob****************
	for(TIntPrFltH::TIter ei = edge_prob.BegI(); ei < edge_prob.EndI(); ei++ ){
		node_prob.GetDat(ei.GetKey().Val1.Val) += ei.GetDat();
		node_prob.GetDat(ei.GetKey().Val2.Val) += ei.GetDat();
	}*/

	//******************Set Node Prob by Max of Edge Prob****************
	for(TIntPrFltH::TIter ei = edge_prob.BegI(); ei < edge_prob.EndI(); ei++ ){
		if(node_prob.GetDat(ei.GetKey().Val1.Val) < ei.GetDat())
			node_prob.GetDat(ei.GetKey().Val1.Val) = ei.GetDat();
		if(node_prob.GetDat(ei.GetKey().Val2.Val) < ei.GetDat())
			node_prob.GetDat(ei.GetKey().Val2.Val) = ei.GetDat();
	}

	/*for (TIntFltH::TIter itr = infectedNode_prob.BegI(); itr < infectedNode_prob.EndI();itr++) {
			//cerr<<"node importance key : "<<itr.GetKey().Val<<endl;
			itr.GetDat() /= NodesUsed.GetDat(itr.GetKey().Val);
			//cerr<<"node Prob: "<<itr.GetDat()<<endl;
	}*/


	//**********For Initializing other Nodes and Edges*****
	for (TNGraph::TNodeI NI = GroundTruth->BegNI(); NI < GroundTruth->EndNI(); NI++) {
		if (!node_prob.IsKey(NI.GetId())) {
			node_prob.AddDat(NI.GetId()) = 0;
		}

		if (!infectingNode_prob.IsKey(NI.GetId())) {
			infectingNode_prob.AddDat(NI.GetId()) = 0.01;
		}

	}

	for (TNGraph::TEdgeI EI = GroundTruth->BegEI(); EI < GroundTruth->EndEI(); EI++) {
		TIntPr e(EI.GetSrcNId(), EI.GetDstNId());
		if (!edge_prob.IsKey(e)) {
			edge_prob.AddDat(e) = 0;
		}
	}

	//cout << "Calculating Probabilities Finished" << endl;
}

void DNS::CalEsts(){
	//*********Cal Denominator of estimation formula********
	infectedNode_est_denom = 0;
	infectingNode_est_denom = 0;
	node_est_denom = 0;
	edge_est_denom = 0;
	for (TNGraph::TNodeI NI = DNS_Graph->BegNI(); NI < DNS_Graph->EndNI();
			NI++) {
		node_est_denom += (1.0 / node_prob.GetDat(NI.GetId()).Val);
		infectingNode_est_denom += (1.0 / infectingNode_prob.GetDat(NI.GetId()).Val);
	}

	for (TNGraph::TEdgeI EI = DNS_Graph->BegEI(); EI < DNS_Graph->EndEI(); EI++) {
		TIntPr e(EI.GetSrcNId(), EI.GetDstNId());
		edge_est_denom += (1.0 / edge_prob.GetDat(e));
	}

	//*********Cal Sigma_Pi*****
	/*sigma_Pi = 0;
		 //the for should be done on dns nodes not all nodes!
		 for (TIntH::TIter EI = NodesUsed.BegI(); EI < NodesUsed.EndI(); EI++) {
		 //cout<<"Nodes : "<<EI.GetDat()<<endl;
		 //sigma_Pi += (1.0 / ( EI.GetDat() ));//
		 sigma_Pi += (1.0 / ( DiffusionGraph->GetNI(EI.GetKey()).GetDeg() ));
		 //sigma_Pi += (1.0 / ( EI.GetDat() * DiffusionGraph->GetNI(EI.GetKey()).GetDeg()));
		 //cout<<"Deg : "<< DNS_Graph->GetNI(EI.GetKey()).GetDeg()<<endl;
		 }*/
	cout << "CalEsts Finished" << endl;
}

void DNS::CalSeeds() {

	//cout << "CalSeeds" << endl;
	seeds_PR = 0;
	seeds_Rec = 0;
	seeds_F = 0;

	//printf("Original Seeds No:%d \n", OriginalCriteria->getSeeds()->Len());
	//printf("DNS Seeds No:%d \n", DNSCriteria->getSeeds()->Len());

	TLstNd<int>* Nd = DNSCriteria->getSeeds()->First();

	while (Nd != NULL) {
		if (OriginalCriteria->getSeeds()->SearchForw(Nd->Val)) {
			seeds_PR++;
			seeds_Rec++;
		}
		Nd = Nd->Next();
	}

	seeds_PR = seeds_PR / DNSCriteria->getSeeds()->Len();
	seeds_Rec = seeds_Rec / OriginalCriteria->getSeeds()->Len();
	seeds_acc = seeds_Rec;
	seeds_F = (2 * seeds_PR * seeds_Rec) / (seeds_PR + seeds_Rec);
}

void DNS::CalSeedsEst() {


	double nom = 0;
	printf("Original Seeds No:%d \n", OriginalCriteria->getSeeds()->Len());
	printf("DNS Seeds No:%d \n", DNSCriteria->getSeeds()->Len());
	//cout<<"infecting_est_denom: "<<infectingNode_est_denom<<endl;

	TLstNd<int>* Nd = DNSCriteria->getSeeds()->First();

	while (Nd != NULL) {

		if (OriginalCriteria->getSeeds()->SearchForw(Nd->Val)) {

			//*****Importance Approach*****
			//nom += (1.0 / ( NodesUsed.GetDat(Nd->Val) ));
			//******Degree Approach*******
			//nom += (1.0 / ( DiffusionGraph->GetNI(Nd->Val).GetDeg()) );
			//******Importance & Degree Approach*****
			//nom += (1.0 / ( NodesUsed.GetDat(Nd->Val) * DiffusionGraph->GetNI(Nd->Val).GetDeg() ));

			//*****New Approach*****
			/*if (!infectedNode_prob.IsKey(Nd->Val)) {
				infectedNode_prob.AddDat(Nd->Val) = 1;
			}*/
			//nom += (1.0 / infectedNode_prob.GetDat(Nd->Val));
			//cout<<"node_prob: "<<node_prob.GetDat(Nd->Val)<<endl;
			//nom += (1.0 / node_prob.GetDat(Nd->Val));
			nom += (1.0 / infectingNode_prob.GetDat(Nd->Val));
			//cout<<"nom: "<<nom<<endl;
		}
		Nd = Nd->Next();
	}

	seeds_est = (nom / infectingNode_est_denom) * DiffusionGraph->GetNodes();

	//seeds_est = (nom / node_est_denom) * DiffusionGraph->GetNodes();
	//seeds_est = (nom / node_est_denom) * OriginalCriteria->getSeeds()->Len();


	seeds_est_acc = 1
			- (abs(OriginalCriteria->getSeeds()->Len() - seeds_est)
					/ OriginalCriteria->getSeeds()->Len());

	//cout << "Seeds Estimation " << seeds_est << endl;
}

void DNS::CalLeaves() {

	//cout << "CalLeaves" << endl;
	leaves_PR = 0;
	leaves_Rec = 0;
	leaves_F = 0;

	//printf("Original Leaves No:%d \n", OriginalCriteria->getLeaves()->Len());
	//printf("DNS Leaves No:%d \n", DNSCriteria->getLeaves()->Len());

	TLstNd<int>* Nd = DNSCriteria->getLeaves()->First();

	while (Nd != NULL) {
		if (OriginalCriteria->getLeaves()->SearchForw(Nd->Val)) {
			leaves_PR++;
			leaves_Rec++;
		}
		Nd = Nd->Next();
	}

	leaves_PR = leaves_PR / DNSCriteria->getLeaves()->Len();
	leaves_Rec = leaves_Rec / OriginalCriteria->getLeaves()->Len();
	leaves_acc = leaves_Rec;
	leaves_F = 2 * (leaves_PR * leaves_Rec) / (leaves_PR + leaves_Rec);
}

void DNS::CalLeavesEst() {

	double nom = 0;
	printf("Original Leaves No:%d \n", OriginalCriteria->getLeaves()->Len());
	printf("DNS Leaves No:%d \n", DNSCriteria->getLeaves()->Len());

	TLstNd<int>* Nd = DNSCriteria->getLeaves()->First();

	while (Nd != NULL) {

		if (OriginalCriteria->getLeaves()->SearchForw(Nd->Val)) {
			//nom += (1.0 / ( NodesUsed.GetDat(Nd->Val) ));
			//nom += (1.0 / (DiffusionGraph->GetNI(Nd->Val).GetDeg()));
			//nom += (1.0 / ( NodesUsed.GetDat(Nd->Val) * DiffusionGraph->GetNI(Nd->Val).GetDeg() ));
			//nom += (1.0 / infectedNode_prob.GetDat(Nd->Val));
			nom += (1.0 / node_prob.GetDat(Nd->Val));
		}

		Nd = Nd->Next();
	}

	//leaves_est = (nom / est_denom) * DiffusionGraph->GetNodes();
	//leaves_est = (nom / node_est_denom) * DiffusionGraph->GetNodes();
	leaves_est = (nom / node_est_denom) * OriginalCriteria->getLeaves()->Len();



	leaves_est_acc = 1
			- (abs(OriginalCriteria->getLeaves()->Len() - leaves_est)
					/ OriginalCriteria->getLeaves()->Len());

	cout << "Leaves Estimation " << leaves_est << endl;
}

void DNS::CalNonLeaves() {
	//cout << "CalNonLeaves" << endl;
	non_leaves_PR = 0;
	non_leaves_Rec = 0;
	non_leaves_F = 0;

	//printf("Original Non Leaves No:%d \n", OriginalCriteria->getNonLeaves()->Len());
	//printf("DNS Non Leaves No:%d \n", DNSCriteria->getNonLeaves()->Len());

	TLstNd<int>* Nd = DNSCriteria->getNonLeaves()->First();

	while (Nd != NULL) {
		if (OriginalCriteria->getNonLeaves()->SearchForw(Nd->Val)) {
			non_leaves_PR++;
			non_leaves_Rec++;
		}
		Nd = Nd->Next();
	}

	non_leaves_PR = non_leaves_PR / DNSCriteria->getNonLeaves()->Len();
	non_leaves_Rec = non_leaves_Rec / OriginalCriteria->getNonLeaves()->Len();
	non_leaves_F = 2 * (non_leaves_PR * non_leaves_Rec)																													/ (non_leaves_PR + non_leaves_Rec);
}

void DNS::CalNonLeavesEst(){

	double nom = 0;
	printf("Original NonLeaves No:%d \n", OriginalCriteria->getNonLeaves()->Len());
	printf("DNS NonLeaves No:%d \n", DNSCriteria->getNonLeaves()->Len());

	TLstNd<int>* Nd = DNSCriteria->getNonLeaves()->First();

	while (Nd != NULL) {

		if (OriginalCriteria->getNonLeaves()->SearchForw(Nd->Val)) {
			//nom += (1.0 / ( NodesUsed.GetDat(Nd->Val) ));
			//nom += (1.0 / (DiffusionGraph->GetNI(Nd->Val).GetDeg()));
			//nom += (1.0 / ( NodesUsed.GetDat(Nd->Val) * DiffusionGraph->GetNI(Nd->Val).GetDeg() ));
			//nom += (1.0 / infectedNode_prob.GetDat(Nd->Val));
			nom += (1.0 / node_prob.GetDat(Nd->Val));
		}

		Nd = Nd->Next();
	}

	//non_leaves_est = (nom / node_est_denom) * DiffusionGraph->GetNodes();
	non_leaves_est = (nom / node_est_denom) * OriginalCriteria->getNonLeaves()->Len();

	non_leaves_est_acc = 1
			- (abs(OriginalCriteria->getNonLeaves()->Len() - non_leaves_est)
					/ OriginalCriteria->getNonLeaves()->Len());

	cout << "NonLeaves Estimation " << non_leaves_est << endl;
}

void DNS::CalImportance() {
	//cout << "CalImportance" << endl;
	DiffNetImportance = 0;
	DNSImportance = 0;
	LinksImportance.Clr();

	for (TIntPrIntH::TIter EI = genCascs->EdgesUsed.BegI();	EI < genCascs->EdgesUsed.EndI(); EI++) {

		TIntPr Edge(EI.GetKey().Val1.Val, EI.GetKey().Val2.Val);
		DiffNetImportance += genCascs->EdgesUsed.GetDat(Edge);

		if (DNS_Graph->IsEdge(EI.GetKey().Val1.Val, EI.GetKey().Val2.Val,true)) {

			TIntPr dns_edge(EI.GetKey().Val1.Val, EI.GetKey().Val2.Val);

			if (!LinksImportance.IsKey(dns_edge)) {
				LinksImportance.AddDat(dns_edge) = genCascs->EdgesUsed.GetDat(
						dns_edge);
			}
		}
	}

	for (TIntPrIntH::TIter EI = LinksImportance.BegI();
			EI < LinksImportance.EndI(); EI++) {

		TIntPr Edge(EI.GetKey().Val1.Val, EI.GetKey().Val2.Val);
		DNSImportance += LinksImportance.GetDat(Edge);
	}

	//cout<<"DiffImportacne : "<<DiffNetImportance<<endl;
	//cout<<"DNSImportance: "<<DNSImportance<<endl;
	importance_acc = 1 - (abs((DNSImportance - DiffNetImportance)) / DiffNetImportance);
}

void DNS::CalImportanceEst(){

	importance_est_acc = 0;
	for (TIntPrIntH::TIter EI = LinksImportance.BegI(); EI < LinksImportance.EndI(); EI++) {

		TIntPr Edge(EI.GetKey().Val1.Val, EI.GetKey().Val2.Val);
		importance_est_acc += LinksImportance.GetDat(Edge) / edge_prob.GetDat(Edge);
		//importance_est_acc += genCascs->EdgesUsed.GetDat(Edge) / edge_prob.GetDat(Edge);
	}

	importance_est_acc =  (importance_est_acc / edge_est_denom ) * DiffusionGraph->GetEdges() ;
	importance_est_acc = 1 - ( abs(DiffNetImportance - importance_est_acc ) / DiffNetImportance );

}

void DNS::CalDiameter() {
	//cout << "Cal Diameter" << endl;
	diameter_Dist = 0;
	diameter_Dist = ((double) DNSCriteria->getDiameter()
			- (double) OriginalCriteria->getDiameter())
			/ (double) OriginalCriteria->getDiameter();
}

int DNS::DiffMaxCascade() {
	int maxLenght = 0;
	for (int i = 0; i < genCascs->CascV.Len(); i++) {
		TCascade &C = genCascs->CascV[i];
		if (C.Len() > maxLenght)
			maxLenght = C.Len();
	}

	return maxLenght;
}

double DNS::DiffAvgCascade(){
	double sum = 0;
	for (int i = 0; i < genCascs->CascV.Len(); i++) {
		TCascade &C = genCascs->CascV[i];
		sum += (C.Len() - 1);
	}
	sum = sum / (double)genCascs->CascV.Len();

	return sum;
}

int DNS::DNSMaxCascade() {
	int maxPiece = 0;
	int thisPiece;
	for (int i = 0; i < genCascs->CascV.Len(); i++) {
		thisPiece = 0;
		TCascade &C = genCascs->CascV[i];

		for (TIntH::TIter EI = C.InfectedBy.BegI(); EI < C.InfectedBy.EndI();
				EI++) {
			//cout<<EI.GetDat().Val<<" to "<<EI.GetKey().Val<<endl;
			if (DNS_Graph->IsEdge(EI.GetDat().Val, EI.GetKey().Val, true)) {
				thisPiece++;
			}
		}
		if (thisPiece > maxPiece)
			maxPiece = thisPiece;
	}
	return maxPiece;
	//*********Does not Work!**********

	//		for (int j = 0; j < C.Len(); j++){
	//			int n = C.GetNode(j);
	//			int p = C.GetParent(n);
	//			cout<<p<<" to "<<n<<endl;
	//			if (DNS_Graph->IsEdge(p, n, true)) {
	//				cout << p<<"," <<n <<"is Edge"<<endl;
	//			}
	//		}
}

double DNS::DNSAvgCascade() {
	double cascsNum = 0;
	double sum = 0;
	bool consider = false;

	for (int i = 0; i < genCascs->CascV.Len(); i++) {
		TCascade &C = genCascs->CascV[i];
		consider = false;

		//cout<<"Cascade Length:"<<C.Len()<<endl;
		//cout<<"Infected Lenth: "<<C.InfectedBy.Len()<<endl;

		for (TIntH::TIter EI = C.InfectedBy.BegI(); EI < C.InfectedBy.EndI();
				EI++) {
			if (DNS_Graph->IsEdge(EI.GetDat().Val, EI.GetKey().Val, true)) {
				if(!consider){
					cascsNum ++;
					consider = true;
				}
				sum++;
			}
		}
	}
	//cout<<"Real Cascades Num: "<<genCascs->CascV.Len()<<endl;
	cout<< "sum:"<<sum<<endl;
	cout<<"Sampled Cascade Num: "<<cascsNum<<endl;

	sum = sum / cascsNum;
	cout<<"avg : "<<sum<<endl;
	return sum;
}

void DNS::CalDepth(){
	double diffavgcasc = DiffAvgCascade();
	//cout << "CalDepth" << endl;
	depth_acc = 0;
	/*cout<<"Diff Max Cascade Length: "<<DiffMaxCascade()<<endl;
	cout<<"DNS Max Cascade Length : "<<DNSMaxCascade()<<endl;
	depth_acc = (double)DNSMaxCascade() / (double)DiffMaxCascade();*/
	//cout << "Finish CalDepth" << endl;

	depth_acc = 1 - ( abs ( diffavgcasc - DNSAvgCascade() ) / diffavgcasc );
}

void DNS::CalCriteria() {

	OriginalCriteria = new Criteria(DiffusionGraph);
	DNSCriteria = new Criteria(DNS_Graph);
	//OriginalCriteria->checkNodes();
	//DNSCriteria->checkNodes();

	CalSeeds();
	//CalLeaves();
	//CalNonLeaves();
	CalImportance();
	//CalDiameter();
	CalDepth();
}

void DNS::CalEstimatedCriteria(){

	CalEsts();
	CalSeedsEst();
	//CalLeavesEst();
	//CalNonLeavesEst();
	CalImportanceEst();
}

void DNS::SaveCriteria(const TStr& Criteria_FNM) {

	//cout << "Save Criteria" << endl;
	TFOut FOut(Criteria_FNM);

	/*FOut.PutStr(
			TStr::Fmt(
					"Seeds_PR		Seeds_F			Leaves_PR		Leaves_F		N_Leaves_PR			N_Leaves_F		Importance_PR		Depth_PR 	 	Seeds_est_Acc		Leaves_est_Acc		non_Leaves_est_Acc 			Importance_est_acc\r\n"));

	FOut.PutStr(TStr::Fmt("%f		", seeds_PR)); // Seeds Precision
	//FOut.PutStr(TStr::Fmt("%f		", seeds_Rec)); // Seeds Recall
	FOut.PutStr(TStr::Fmt("%f		", seeds_F)); // Seeds F_measure

	FOut.PutStr(TStr::Fmt("%f		", leaves_PR)); // Leaves Precision
	//FOut.PutStr(TStr::Fmt("%f		", leaves_Rec)); // Leaves Recall
	FOut.PutStr(TStr::Fmt("%f		", leaves_F)); // Leaves F_measure

	FOut.PutStr(TStr::Fmt("%f		", non_leaves_PR)); // Non Leaves Precision
	//FOut.PutStr(TStr::Fmt("%f		", non_leaves_Rec)); // Non Leaves Recall
	FOut.PutStr(TStr::Fmt("%f			", non_leaves_F)); // Non Leaves F_measure
	FOut.PutStr(TStr::Fmt("%f			", importance_acc));
	//FOut.PutStr(TStr::Fmt("%f		", diameter_Dist));
	FOut.PutStr(TStr::Fmt("%f			", depth_acc));
	//FOut.PutStr(TStr::Fmt("%f		", seeds_acc));
	FOut.PutStr(TStr::Fmt("%f			", seeds_est_acc));
	//	FOut.PutStr(TStr::Fmt("%f		", leaves_acc));
	FOut.PutStr(TStr::Fmt("%f			", leaves_est_acc));
	FOut.PutStr(TStr::Fmt("%f			", non_leaves_est_acc));
	FOut.PutStr(TStr::Fmt("%f			", importance_est_acc));
	*/
	FOut.PutStr(
			TStr::Fmt(
					"Seeds_F		Seeds_Acc			Importance_Acc		Depth_Acc 	 	Seeds_est_acc		Importance_est_acc\r\n"));

	FOut.PutStr(TStr::Fmt("%f		", seeds_F)); // Seeds Precision
	FOut.PutStr(TStr::Fmt("%f		", seeds_Rec)); // Seeds F_measure
	FOut.PutStr(TStr::Fmt("%f			", importance_acc));
	FOut.PutStr(TStr::Fmt("%f			", depth_acc));
	FOut.PutStr(TStr::Fmt("%f			", seeds_est_acc));
	FOut.PutStr(TStr::Fmt("%f			", importance_est_acc));

}

void DNS::SaveDNS(const TStr& DNS_FNM) {
	//cerr << "SaveDNS" << endl;
	TFOut FOut(DNS_FNM);
	// write nodes to file
	for (TNGraph::TNodeI NI = DNS_Graph->BegNI(); NI < DNS_Graph->EndNI();
			NI++) {
		FOut.PutStr(TStr::Fmt("%d,%d\r\n", NI.GetId(), NI.GetId())); // nodes
	}

	FOut.PutStr("\r\n");

	// write edges to file (not allowing self loops in the network)
	for (TNGraph::TEdgeI EI = DNS_Graph->BegEI(); EI < DNS_Graph->EndEI();
			EI++) {
		// not allowing self loops in the Kronecker network
		if (EI.GetSrcNId() != EI.GetDstNId())
			FOut.PutStr(TStr::Fmt("%d,%d\r\n", EI.GetSrcNId(), EI.GetDstNId()));
	}
	//cerr << "Finish SaveDNS" << endl;
}
