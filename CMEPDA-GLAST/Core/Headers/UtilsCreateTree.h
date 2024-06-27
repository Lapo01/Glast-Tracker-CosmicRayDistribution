#pragma once
#include "Evento.h"
#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <iterator>
#include <TMath.h>
#include "TROOT.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TGraphErrors.h"
#include "TRandom.h"
#include "TCanvas.h"
#include "TFrame.h"
#include "TDatime.h"
#include "TF1.h"
#include "TFile.h"
#include "TStopwatch.h"
#include "TTree.h"
#include <iostream>
#include <vector>
#include <map>



void FindMutedStrips(const std::string &file, int &NHit, int &ciclo, std::vector<int> &stringa,  std::map<std::string, int> &mapping, std::map<int, std::vector<int>> &MapDeiVettoriMutati);

void CheckForFlagTwo(Evento &e, int &NCluster);

void CheckForFlagOne(Evento &e, std::map<int, std::vector<int>> &MapDeiVettoriMutati, std::vector<int> &StripLayer, int &LengthCluster,int &layer,  int j );

void CheckForFlagThree(Evento &e, std::map<int,int> &MapClusterPerPiano, int &aaaax, int &aaaay, std::vector<int> &LayersID);

void FlushPlaceholder(Evento &e, std::map<int, int> &MapHitPerPiano, std::map<int,int> &MapClusterPerPiano, std::vector<int> &VettoreDifferenzeStripConsecutive, std::vector<int> &StripLayer, std::vector<int> &LayersID, int &Nx, int &Ny);

void FillStripLayer(Evento &e, std::vector<int> &StripLayer, std::map<int,int> &MapHitPerPiano, int &Layer);


double StripCoordinate(int Strip);


