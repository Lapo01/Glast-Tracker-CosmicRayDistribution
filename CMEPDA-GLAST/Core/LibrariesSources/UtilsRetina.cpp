#include"UtilsRetina.h"

/**
 *  * \file UtilsRetina.cpp
 *	This cpp macro takes as input a raw data file and returns a .root file with a tree. It performs a first abstraction of the data
 *
 * 
 *
 *
 */


inline double Distance(double q, double m, double xpos, double zpos){
	return abs(xpos - m*zpos - q)/sqrt(1+m*m);
}


void BuildGrid(std::vector<std::vector<std::pair<double, double>>> &Grid){

///////////////////////////////////////
// Might be useful to include the option to choose the grid one's like.
///////////////////////////////////////
    double m_in = -4; //cm
	double m_fin = 4; //cm
	double q_in = -30; //cm
	int strip = 1536;
	double q_fin = 65;
	std::vector<std::pair<double, double>> Column;
	double N = 200; //detta il passo dello spazio delle fasi lungo 
	double M = 400;
	
	for(int i = 0; i<N; i++){
		
		for(int j = 0; j<M ; j++){
			Column.push_back(std::make_pair(m_in + (m_fin-m_in)*i/N, q_in + (q_fin-q_in)*j/M));
			
			}
		Grid.push_back(Column);
		Column.clear();
	} 

};


void EventReset(std::vector<Track> &multipleTracksY, std::vector<Track> &multipleTracksX, EventoTrack &etrack, std::vector<Track> &CopiaVettore, std::vector<Track> &VettoreTracceUniche, std::vector<Track> &FinalFilteredTracksX, std::vector<Track> &FinalFilteredTracksY){

	multipleTracksY.clear();
	multipleTracksX.clear();
	CopiaVettore.clear();
	VettoreTracceUniche.clear();
	FinalFilteredTracksY.clear();
	FinalFilteredTracksX.clear();
	etrack.TrackY.clear();
	etrack.TrackX.clear();

};


void CellReset(std::pair<double, double> &element, Track &TrackX, Track &TrackY, std::vector <int> &VecIndex, std::vector <double> &Vec, double &qx, double &qy, double &mx, double &my, TF1 *lineX, TF1 *lineY){

	TrackX.Fit.clear();
	TrackX.Error.clear();
	TrackX.ClusterPosition.clear();
	TrackX.Layer.clear();
				
	
	TrackY.Fit.clear();
	TrackY.Error.clear();
	TrackY.ClusterPosition.clear();
	TrackY.Layer.clear();
	VecIndex.clear();
	Vec.clear();
	qx = element.second;
	qy = element.second;
	mx = element.first;
	my = element.first;
				
	lineX->SetParameters(0, qx);
	lineX->SetParameters(1, mx);
	lineY->SetParameters(0, qy);
	lineY->SetParameters(1, my);





};