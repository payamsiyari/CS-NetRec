#ifndef snap_DNS_h
#define snap_DNS_h

class DNS {
public:

	//****************GroundTruth Parameters*********************
	GenNets* genNets;
	TStr GroundTruthFNm;
	PNGraph GroundTruth; // The Original Graph

	//****************Diffusion Parameters****************
	int GenDiffusionNet;// "Generate GroundTruth or Read from File ::  0: Generate 1: Read from File(default:0)
	int NCascades; // If positive, number of cascades, if negative, percentage of edges used at least in one cascade (default:-95)
	int TModel;// Transmission model ::  0:exponential, 1:power law, 2:mixture of 2 exponentials (default:0)
	double alpha; // Alpha for transmission model (default:1)
	double alpha2;// Second alpha for mixture of exponentials/power laws transmission model (default:1);
	double p;// Mixture of exponential/power laws transmission model weight (1.0 means max weight in 1st exp) (default:1)
	double beta; // Beta for transmission model (default:0.5)
	double std_beta; // Std for Beta in the transmission model (default:0)
	double std_waiting_time;// Std for waiting time in the transmission model (default:0)
	double perc_rnd_nodes;// Percentage of random nodes in a cascade (default:0)
	double perc_rnd_removal;// Percentage of missing nodes in a cascade (default:0)
	TStr DiffusionNetFNm;
	PNGraph DiffusionGraph;// The Diffusion Graph over GroundTruth Graph
	double diffRate;
	GenCascs* genCascs;


	//****************Sampling Parameters*****************
	Sampling* sampling;
	int samplingMethod; // Type of Sampling Method :: 0:Random Walk, 1:BFS 2:MHRW (default:0)
	double sampleRate; // Sampling Rate from 0-1 (default:0.5)
	PNGraph SampledGraph;


	//******************DNS Paramaters********************
	int DNS_Method;
	PNGraph DNS_Graph;

	TIntPrIntH LinksImportance;
	TIntH NodesUsed;

	TIntPrFltH node_casc_prob;
	TIntPrIntH node_casc_count;

	//TIntFltH infectedNode_prob;
	TIntFltH infectingNode_prob;

	TIntFltH node_prob;
	TIntPrFltH edge_prob;

	Criteria* OriginalCriteria;
	Criteria* DNSCriteria;
	TStr st_criteria_RW;
	TStr st_criteria_BFS;
	TStr diff_criteria_RW;
	TStr diff_criteria_BFS;
	TStr DNS_criteria;

	//*****Estimation Criteria****

	double node_est_denom;
	double infectedNode_est_denom;
	double infectingNode_est_denom;
	double edge_est_denom;


	//****Seed Criteria****
	double seeds_PR;
	double seeds_Rec;
	double seeds_F; // F_measure

	double seeds_acc;
	double seeds_est;
	double seeds_est_acc;

	//****Importance Criterion****
	double DiffNetImportance;
	double DNSImportance;
	double importance_acc;
	double importance_est_acc;

	//***Leaves Criteria****
	double leaves_PR;
	double leaves_Rec;
	double leaves_F; // F_measure

	double leaves_acc;
	double leaves_est;
	double leaves_est_acc;

	//****Non_leaves Criteria****
	double non_leaves_PR;
	double non_leaves_Rec;
	double non_leaves_F; // F_measure

	double non_leaves_acc;
	double non_leaves_est;
	double non_leaves_est_acc;

	//***Diameter Criteria*****
	double diameter_Dist;

	//***Depth Criteria****
	double depth_acc;

	//TStr DNS_FNM;
	//TStr Criteria_FNM;


public:

	void LoadNetworkTxt(TSIn& SIn, PNGraph& network);
	void Gen_Structure_DNS();
	void Gen_Diffusion_DNS();
	void Gen_DNS();
	PNGraph ExtractSharedEdges(PNGraph Graph1, PNGraph Graph2);
	void Find_Nodes_Used();
	//******Depth*****
	int DiffMaxCascade();
	int DNSMaxCascade();
	double DiffAvgCascade();
	double DNSAvgCascade();


	void CalProbs();
	void CalEsts();
	void CalSeeds();
	//void CalSeedsEst(int approach);
	void CalSeedsEst();
	void CalLeaves();
	//void CalLeavesEst(int approach);
	void CalLeavesEst();
	void CalNonLeaves();
	void CalNonLeavesEst();
	void CalDiameter();
	void CalImportance();
	void CalImportanceEst();
	void CalDepth();
	void CalCriteria();
	void CalEstimatedCriteria();

	void SaveDNS(const TStr& DNS_FNM);
	void SaveCriteria(const TStr& Criteria_FNM);

};

#endif
