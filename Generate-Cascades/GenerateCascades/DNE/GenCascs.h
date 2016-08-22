#ifndef snap_GenCascs_h
#define snap_GenCascs_h

#include "Snap.h"
#include "Cascade.h"

class GenCascs {
public:
    int NCascades; // If positive, number of cascades;
    //if negative, percentage of edges used at least in one cascade(Diffusion Rate) (default:-50)
	int TModel;// Transmission model ::  0:exponential, 1:power law, 2:mixture of 2 exponentials (default:0)
	double alpha; // Alpha for transmission model (default:1)
  	double alpha2;// Second alpha for mixture of exponentials/power laws transmission model (default:1);
  	double p;// Mixture of exponential/power laws transmission model weight (1.0 means max weight in 1st exp) (default:1)
  	double beta; //"Beta for transmission model (default:0.5)
  	double std_beta; // Std for Beta in the transmission model (default:0)
	double std_waiting_time;// Std for waiting time in the transmission model (default:0)
	double perc_rnd_nodes;// Percenta.ge of random nodes in a cascade (default:0)
	double perc_rnd_removal;// Percentage of missing nodes in a cascade (default:0)
	double diffRate;
	TStr DiffusionNetFNm;


	PNGraph GroundTruth; // The Original Graph
	PNGraph DiffusionGraph; // The Diffusion Graph under GroundTruthGraph
    TVec<TCascade> CascV;
    THash<TInt, TNodeInfo> NodeNmH;
    TIntPrIntH EdgesUsed;


public:

    GenCascs();
   // GenCascs(const TStr DiffNet_FNM);
	GenCascs(const int& NCascades, const int& TMode, const double& alpha, const double& alpha2, const double& p,
	const double& beta,	const double& std_waiting_time, const double& std_beta, const PNGraph GroundTruth, const TStr DiffNet_FNM);
	void GenDiffNet();
	void GenCascades(TCascade& C, TIntPrIntH& EdgesUsed);
    void AddCasc(const TCascade& Cascade);
    void AddCasc(const TStr& CascStr);
    void AddNodeNm(const int& NId, const TNodeInfo& Info);
    bool IsNodeNm(const int& NId) const { return NodeNmH.IsKey(NId); }
    TNodeInfo GetNodeInfo(const int& NId) const { return NodeNmH.GetDat(NId); }
    int GetCascs();
	void GenNoisyCascades();
	void SaveCascades(const TStr& OutFNm);
	void LoadCascadesTxt(TSIn& SIn);
	void SaveDiffNet();
	void LoadGroundTruthTxt(TSIn& SIn);

};

#endif
