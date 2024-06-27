/**
 *  \file EventoTrack.h
 * Header file for the object EventoTrack
 */


#pragma once
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
	std::vector<Track> TrackX; /** Container of the projection on the vision XZ */
	std::vector<Track> TrackY; /** Container of the projection on the vision YZ */
	int Nx()/** Method that returns the number of tracks on the vision XZ */
	{return TrackX.size();}
	int Ny()/** Method that returns the number of tracks on the vision YZ */
	{return TrackY.size();}
	int NEvento; /** Event ID */
	
};