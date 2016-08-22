#include "stdafx.h"
#include "cascinf.h"

int main(int argc, char* argv[]) {
  Env = TEnv(argc, argv, TNotify::StdNotify);
  Env.PrepArgs(TStr::Fmt("\nGenerate different synthetic networks & cascades. build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
  TExeTm ExeTm;
  Try

  const int TNetwork = Env.GetIfArgPrefixInt("-t:", 0, "Type of network to generate\n\
  	  0:kronecker, 1:forest fire (default:0)\n");
  const TStr NetworkParams = Env.GetIfArgPrefixStr("-g:", TStr("0.5 0.5; 0.5 0.5"), "Parameters for the network (default:0.5 0.5; 0.5 0.5)\n");

  const int NNodes = Env.GetIfArgPrefixInt("-n:", 32, "Number of nodes (default:100)\n");
  const int NEdges = Env.GetIfArgPrefixInt("-e:", 64, "Number of edges (default:100)\n");
  const int NCascades = Env.GetIfArgPrefixInt("-c:", -95, "If positive, number of cascades, if negative, percentage of edges used at least in one cascade (default:-95)\n");
  const int TModel = Env.GetIfArgPrefixInt("-m:", 0, "Transmission model\n0:exponential, 1:power law, 2:mixture of 2 exponentials (default:0)\n");

  // we allow for mixture of exponentials
  const double alpha = Env.GetIfArgPrefixFlt("-a:", 1.0, "Alpha for transmission model (default:1)\n");
  const double alpha2 = Env.GetIfArgPrefixFlt("-a2:", 1.0, "Second alpha for mixture of exponentials/power laws transmission model (default:1)\n");
  const double p = Env.GetIfArgPrefixFlt("-p:", 1.0, "Mixture of exponential/power laws transmission model weight (1.0 means max weight in 1st exp) (default:1)\n");
  const double beta = Env.GetIfArgPrefixFlt("-b:", 0.5, "Beta for transmission model (default:0.5)\n");
  const TStr FileName = Env.GetIfArgPrefixStr("-f:", TStr("example"), "Name of the network (default:example)\n");

  const double std_beta = Env.GetIfArgPrefixFlt("-stdb:", 0.0, "Std for Beta in the transmission model (default:0)\n");
  const double std_waiting_time = Env.GetIfArgPrefixFlt("-stdwt:", 0.0, "Std for waiting time in the transmission model (default:0)\n");
  const double perc_rnd_nodes = Env.GetIfArgPrefixFlt("-prnd:", 0.0, "Percentage of random nodes in a cascade (default:0)\n");
  const double perc_rnd_removal = Env.GetIfArgPrefixFlt("-prmv:", 0.0, "Percentage of missing nodes in a cascade (default:0)\n");

  TNetInfBs NIB;

  // Generate GroundTruth and Save
  NIB.GenerateGroundTruth(TNetwork, NNodes, NEdges, NetworkParams);
  NIB.SaveGroundTruth(TStr::Fmt("%s-network.txt", FileName.CStr()));

  // Generate Cascades
  TIntPrIntH EdgesUsed;
  int last = 0;

  for (int i=0; (i<NCascades) || ((double)EdgesUsed.Len() < -(double)NCascades/100.0 * (double)NIB.GroundTruth->GetEdges()); i++) {
	  TCascade C(alpha);
	  NIB.GenNoisyCascade(C, TModel, alpha, beta, EdgesUsed, alpha2, p, std_waiting_time, std_beta, perc_rnd_nodes, perc_rnd_removal);
	  NIB.AddCasc(C);
		printf("Add cascade:  %d \n",i);

	  if ( EdgesUsed.Len() - last > 100) {
		  last = EdgesUsed.Len();
		  printf("Cascades: %d, Edges used: %d/%f\n", NIB.GetCascs(), EdgesUsed.Len(), -(double)NCascades/100.0 * (double)NIB.GroundTruth->GetEdges());
	  }
  }

  // Save Cascades
  NIB.SaveCascades(TStr::Fmt("%s-cascades.txt", FileName.CStr()));

  Catch
  printf("\nrun time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  return 0;
}
