
/** 
 *  \file GeneralUtils.h
 *
 * Headers for library GeneralUtils
 *
 */

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
#include<algorithm>
#include <numeric>




double Theta(EventoTrack &e, int i);/** Returns zenith angle for track */

double Phi(EventoTrack &e, int i);/** Returns azimuth angle for track */

extern std::vector<int> LayersID; /** Container of the layer's ID*/

extern std::map<std::string, int> MapLayerToID; /** It maps raw string to the layer's ID*/

extern std::map<int, std::string> MapIDToLayer; /** It maps layer's ID to raw string */

extern std::map<int, double> EfficiencyMap; /** It maps Layer's ID to its mean efficiency */

extern std::map<int, double> Zmap; /** It maps layer's ID to its height along Z */

extern std::vector<int> LayerX; /** vector of Layer X ID's */

extern std::vector<int> LayerY; /** vector of Layer Y ID's */

double StripCoordinate(int Strip); /** Function that returns the coordinate of a strip in cm */

double LayerCoordinate(int &ID); /** Function used in the correction of shifts along z: returns the corrected z position for a layer. */


double Error(int &StripIn, int &Dimension); /** Function that returns the error on the position of a cluster */

