
/**
 * \file UtilsMC.h
 *
 * Headers for library UtilsMC
 *
 */
#pragma once
#include "GeneralUtils.h"
#include "UtilsCreateTree.h"

extern std::vector<int> XLayer;

extern std::vector<int> YLayer;

void ResetEvento(Evento &e);

void ResetUtils(std::vector<double> &TrackX, std::vector<double> &TrackY, std::vector<int> &ZlevelX, std::vector<int> &ZlevelY, std::vector<int> &ZlevelXPlaceHolder, std::vector<int> &ZlevelYPlaceHolder, int &NxHit, int &NyHit, double &epsilon);


void GenerateRandomNumbers(double &auxiliary, double &theta, double &phi, double &x0, double &y0, double &mx, double &my , double &qx, double &qy, double &thetacalc, double &phicalc, double &Ztop);

