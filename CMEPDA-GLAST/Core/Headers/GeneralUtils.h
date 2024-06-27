#pragma once
#include<vector>
#include<iostream>
#include<map>
#include <TMath.h>
#include"Evento.h"
#include"EventoTrack.h"
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
#include <fstream>
#include <sstream>
#include <iterator>



extern std::vector<int> LayersID; /** Container of the layer's ID*/

extern std::map<std::string, int> MapLayerToID; /** It maps raw string to the layer's ID*/

extern std::map<int, double> Zmap;

extern std::vector<int> LayerX;

extern std::vector<int> LayerY;

double StripCoordinate(int &Strip);

double LayerCoordinate(int &ID);


double Error(int &StripIn, int &Dimension);

