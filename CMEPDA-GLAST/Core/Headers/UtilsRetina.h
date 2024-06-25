
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <iterator>
#include <utility>
#include "TF1.h"
#include "ClassEvento.h"
inline double Distance(double q, double m, double xpos, double zpos);


void BuildGrid(std::vector<std::vector<std::pair<double, double>>> Grid);

void EventReset(std::vector<Track> &multipleTracksY, std::vector<Track> &multipleTracksX, EventoTrack &etrack, std::vector<Track> &CopiaVettore, std::vector<Track> &VettoreTracceUniche, std::vector<Track> &FinalFilteredTracksX, std::vector<Track> &FinalFilteredTracksY);

void CellReset(std::pair<double, double> &element, Track &TrackX, Track &TrackY, std::vector <int> &VecIndex, std::vector <double> &Vec, double &qx, double &qy, double &mx, double &my, TF1 &lineX, TF1 &lineY);
