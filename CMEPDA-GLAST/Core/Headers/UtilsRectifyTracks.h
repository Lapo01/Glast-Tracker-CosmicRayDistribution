#pragma once
#include <TMath.h>
#include"Evento.h"
#include"EventoTrack.h"
#include "GeneralUtils.h"
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
#include "GeneralUtils.h"




double AngleCorrection(int &layer, double &mx, double &qx, double &my, double &qy);

double Error(int &StripIn, int &Dimension);

extern std::map<int,double> ErrorXYZ;

extern std::map<int, double> ErrorXYAngoloZ;

extern std::map<int, double> MapShift;

extern std::map<int, double> SoglieChiMappa;







