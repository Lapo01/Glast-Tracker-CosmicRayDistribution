#include"UtilsRetina.h"

/**
 *  * \file UtilsRetina.cpp
 *	Source code for the library that contains utils for the Retina macro
 *
 * 
 *
 *
 */


/*********************************************************************************************************
 *This function returns the distance between a straight line and a cluster
 *@param q Intercept of the rect 
 *
 *@param m Angular coefficient of the rect
 *
 *@param xpos X or Y position of the cluster
 *
 *@param zpos Z position of the cluster
 *
 *
 ********************************************************************************************************/
double Distance(double q, double m, double xpos, double zpos){
	return abs(xpos - m*zpos - q)/sqrt(1+m*m);
};



/*********************************************************************************************************
 * This function takes a grid and builds up a phase space of 100x200 cells.
 *
 *@param Grid Grid to be created
 *
 *
 *
 ********************************************************************************************************/
void BuildGrid(std::vector<std::vector<std::pair<double, double>>> &Grid){

///////////////////////////////////////
// Might be useful to include the option to choose the grid N M hyperparameters .
///////////////////////////////////////
    double m_in = -4; //cm
	double m_fin = 4; //cm
	double q_in = -30; //cm
	int strip = 1536;
	double q_fin = 65;
	std::vector<std::pair<double, double>> Column;
	double N = 100; //Phase space number of cells along the angular coefficient direction
	double M = 200; //Phase space number of cells along the intercept direction
	
	for(int i = 0; i<N; i++){
		
		for(int j = 0; j<M ; j++){
			Column.push_back(std::make_pair(m_in + (m_fin-m_in)*i/N, q_in + (q_fin-q_in)*j/M));
			
			}
		Grid.push_back(Column);
		Column.clear();
	} 

};
/********************************************************************************************************
 *
 * This function resets all utils variables for the event.
 *
 *
 *@param multipleTracksY Util variable that contains all potential tracks found on the grid for the vision YZ
 *
 *@param multipleTracksX Util variable that contains all potential tracks found on the grid for the vision XZ
 *
 *@param etrack Instance of EventoTrack, it's the instance that is going to be written on the tree. Must be reset at each event
 *
 *@param Copiavettore Util variable
 *
 *@param VettoreTracceUniche Container of containers of equal tracks
 *
 *@param FinalFilteredTracksX Container of filtered tracks
 *
 *@param FinalFilteredTracksY Container of filtered tracks
 ********************************************************************************************************/

void EventReset(std::vector<Track> &multipleTracksY, std::vector<Track> &multipleTracksX, EventoTrack &etrack, std::vector<Track> &CopiaVettore, std::vector<std::vector<Track>> &VettoreTracceUniche, std::vector<Track> &FinalFilteredTracksX, std::vector<Track> &FinalFilteredTracksY){

	multipleTracksY.clear();
	multipleTracksX.clear();
	CopiaVettore.clear();
	VettoreTracceUniche.clear();
	FinalFilteredTracksY.clear();
	FinalFilteredTracksX.clear();
	etrack.TrackY.clear();
	etrack.TrackX.clear();

};

/*********************************************************************************************************
 * This function resets all utils variable used in the single cell analysis.
 *
 *
 *@param element pair containing the (q,m) info, it is the cell 
 *
 *@param TrackX Track object containing the info on the potential track on the considered cell in the vision XZ
 *
 *@param TrackY Track object containing the info on the potential track on the considered cell in the vision YZ
 *
 *@param VecIndex Indexes of the elements of the member ClusterPosizione in the instance of the Evento that are close by 0.5 cm from the straight line of the cell .
 *
 *@param Vec Vector filled with the distances of all points in a layer that are close by 0.5 cm from the track identified by the phase space cell
 *
 *@param qx Value of intercept for the chosen cell on XZ vision
 *
 *@param qy Value of intercept for the chosen cell on YZ vision
 *
 *@param mx Value of angular coefficient for the chosen cell on XZ vision
 *
 *@param my Value of angular coefficient for the chosen cell on YZ vision
 *
 *@param lineX linear function for fitting, initial parameters for fitting are settled in this function
 *
 *@param lineY linear function for fitting, initial parameters for fitting are settled in this functions
 *
 *
 ********************************************************************************************************/
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



