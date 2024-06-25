#include <TMath.h>
#include"Track.h"
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


class EventoTrack
{
public:
	std::vector<Track> TrackX;
	std::vector<Track> TrackY;
	int Nx(){return TrackX.size();}
	int Ny(){return TrackY.size();}
	int NEvento;
	
};