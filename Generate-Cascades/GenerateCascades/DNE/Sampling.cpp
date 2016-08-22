#include "stdafx.h"
#include "Sampling.h"

//************************************************************

Sampling::Sampling(double sampleRate, PNGraph GroundTruth) {
	this->sampleRate = sampleRate;
	this->GroundTruth = GroundTruth;
}

void Sampling::RW() {

	do {
		StartNId = GroundTruth->GetRndNId();
		//cout << GroundTruth->IsNode(StartNId) << endl;

	} while (!GroundTruth->IsNode(StartNId)
			|| (GroundTruth->GetNI(StartNId).GetOutDeg() == 0 && GroundTruth->GetNI(StartNId).GetInDeg() == 0));
	//cout << "StartNId: " << StartNId << endl;
	//cout<<"Random Walk"<<endl;

	int edges = GroundTruth->GetEdges();
	SampledGraph = TNGraph::New();
	int currentNode =StartNId;
	int DstNode;
	int outRand;
	int inRand;
	int chooseRand;
	int choose = false;
	int currentEdges;
	int nextEdges;

	while(SampledGraph->GetEdges() < edges * sampleRate){
		currentEdges = SampledGraph->GetEdges();

		SampledGraph->AddNode(currentNode);
		if(GroundTruth->GetNI(currentNode).GetInDeg() == 0 && GroundTruth->GetNI(currentNode).GetOutDeg() == 0){
			currentNode = StartNId;
			continue;
		}

		for (int o = 0; o < GroundTruth->GetNI(currentNode).GetOutDeg(); o++) {  // out-links
			DstNode = GroundTruth->GetNI(currentNode).GetOutNId(o);
			SampledGraph->AddNode(DstNode);
			SampledGraph->AddEdge(currentNode, DstNode);
			outRand = rand() % GroundTruth->GetNI(currentNode).GetOutDeg();
		}

		for (int i = 0; i < GroundTruth->GetNI(currentNode).GetInDeg(); i++) {  // In-links
			DstNode = GroundTruth->GetNI(currentNode).GetInNId(i);
			SampledGraph->AddNode(DstNode);
			SampledGraph->AddEdge(DstNode, currentNode);
			inRand = rand() % GroundTruth->GetNI(currentNode).GetInDeg();

		}
		//cout<<"Current Node :"<<currentNode<<endl;


		int temp;

		choose = false;
		while(!choose){
			chooseRand = rand() % 2;
			//	cout<<"choose rand : "<<chooseRand<<endl;
			if(chooseRand == 0 && GroundTruth->GetNI(currentNode).GetOutDeg() != 0){
				temp = GroundTruth->GetNI(currentNode).GetOutNId(outRand);
				//	cout<<"choose :0"<<endl;
				choose = true;
			}
			else if(chooseRand == 1 && GroundTruth->GetNI(currentNode).GetInDeg() != 0){
				temp = GroundTruth->GetNI(currentNode).GetInNId(inRand);
				//	cout<<"choose 1"<<endl;
				choose = true;
			}
		}

		currentNode = temp;

		nextEdges = SampledGraph->GetEdges();
		if(nextEdges - currentEdges == 0){
			do {
				currentNode = GroundTruth->GetRndNId();
				//cout << GroundTruth->IsNode(StartNId) << endl;

			} while (!GroundTruth->IsNode(currentNode)
					|| (GroundTruth->GetNI(currentNode).GetOutDeg() == 0 && GroundTruth->GetNI(currentNode).GetInDeg() == 0));

		}

		//cout<<"temp: "<<temp<<endl;
		//cout<<"Sampled Graph Nodes: "<<SampledGraph->GetNodes()<<endl;
		//cout<<"Sampled Graph Edges: "<<SampledGraph->GetEdges()<<endl;

	}

	cout<<endl;
	cout<<"Sampled Graph Nodes: "<<SampledGraph->GetNodes()<<endl;
	cout<<"Sampled Graph Edges: "<<SampledGraph->GetEdges()<<endl;
	cout<<endl;
}

void Sampling::BFS() {

	TBreathFS<PNGraph> bfs(GroundTruth, false);

	do {
		StartNId = GroundTruth->GetRndNId();
		//cout << GroundTruth->IsNode(StartNId) << endl;

	} while (!GroundTruth->IsNode(StartNId)
			|| (GroundTruth->GetNI(StartNId).GetOutDeg() == 0 && GroundTruth->GetNI(StartNId).GetInDeg() == 0));

	//cout << "StartNId: " << StartNId << endl;
	//cout << "Customized BFS:" << endl;
	//bfs.DoBfs(StartNId, true, false, -1, TInt::Mx);
	bfs.CustomizedBfs(StartNId, true, true, -1, TInt::Mx, sampleRate);

	SampledGraph = bfs.BFSGraph;
	TreeDepth = bfs.TreeDepth;
	cout << "Sampled Graph Number of Nodes : " << SampledGraph->GetNodes() << endl;
	cout << "Sampled Graph Number of Edges : " << SampledGraph->GetEdges() << endl;
	//SampledTree = GetBfsTree(GroundTruth, StartNId, true, true);

	TreeDepth = 0;
	for (int i = 0; i < bfs.NIdDistH.Len(); i++) {
		TreeDepth = TMath::Mx(TreeDepth, bfs.NIdDistH[i].Val);
	}
	//cout << "Sampled Graph Tree Depth " << TreeDepth << endl;

	//
}

void Sampling::MHRW() {
}
