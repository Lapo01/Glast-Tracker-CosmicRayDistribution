#include "GeneralUtils.h"

/**
 *  * \file GeneralUtils.cpp
 *	Questo file contiene mappe usate globalmente in tutti i codici.
 * 
 *
 * 
 *
 *
 */





/**
 * This function returns the zenith angle for an event with a number of projections equal to 1.
 * 
 *
 *
 * @param e The instance of the object EventoTrack containing the fitting parameters info on the projectios
 *
 * @param i Parameter putted for post-compatibility in case one wanted to do 3D multitracking.
 *
 *
 *
 */

double Theta(EventoTrack &e, int i){
    //questa funzione ritorna l'angolo theta di una traccia i-esima.
    return atan(sqrt(e.TrackX[i].Fit[1]*e.TrackX[i].Fit[1]+ e.TrackY[i].Fit[1]*e.TrackY[i].Fit[1]))*180/3.14;


};


/**
 * This function returns the azimuth angle for an event with a number of projections equal to 1.
 * 
 *
 *
 * @param e The instance of the object EventoTrack containing the fitting parameters info on the projectios
 *
 * @param i Parameter putted for post-compatibility in case one wanted to do 3D multitracking.
 *
 *
 *
 */
double Phi(EventoTrack &e, int i){

    return atan2(e.TrackY[i].Fit[1],e.TrackX[i].Fit[1])*180/3.14;

};














std::vector<int> LayersID = {10,11,12,13,14,20,21,22,23,24}; /** Container of the layer's ID*/




std::map<std::string, int> MapLayerToID/** It maps raw string to the layer's ID*/
                                {
                                {"X0", 10},
                                {"X1",11},
                                {"X2", 12},
                                {"X3", 13},
                                {"X4", 14},
                                {"Y0", 20},
                                {"Y1", 21},
                                {"Y2", 22},
                                {"Y3", 23},
                                {"Y4", 24}
                                }; 

std::map<int, std::string> MapIDToLayer/** It maps Layer's ID to its raw string  */
{
                                {10,"X0"},
                                {11,"X1"},
                                {12,"X2"},
                                {13,"X3"},
                                {14,"X4"},
                                {20,"Y0"},
                                {21,"Y1"},
                                {22,"Y2"},
                                {23,"Y3"},
                                {24,"Y4"}
};


std::map<int, double> EfficiencyMap{ /** Maps Layer's ID to its mean efficiency */
                                {10,0.84},
                                {11,0.857},
                                {12,1.},
                                {13,0.91},
                                {14,0.61},
                                {20,0.86},
                                {21,0.95},
                                {22,1.},
                                {23,0.56},
                                {24,0.75}
};


std::map<int, double> Zmap{ /** Maps Layer's ID to its height in centimeters */
    {10, 3.1},
    {11, 6.3},
    {12, 16.9},
    {13, 20.1},
    {14, 31.2},
    {20, -1.0},
    {21, 10.1},
    {22, 13.3},
    {23, 24.6},
    {24, 27.8}
};


std::vector<int> LayerX= {10,11,12,13,14}/** ID of XZ Layers */;
std::vector<int> LayerY={20,21,22,23,24}/** ID of XZ Layers */;



/*******************************************************************************
 * This function calculate given the strip number its position in cm.
 *
 * @param Strip It is the strip number ID (goes from 1 to 1536)
 ******************************************************************************/
double StripCoordinate(int Strip ){

   
	
	const double StripPitch = 0.0228; // cm
	const double EdgeWidth = 0.1; //cm
	const double LadderSeparation = 0.02; //cm
	double coordinate = EdgeWidth + StripPitch*int(Strip) + (LadderSeparation + 2*EdgeWidth - StripPitch)*int(Strip/384); 
return coordinate;};




/*******************************************************************************
 * This function corrects shifts along the Z directions given the layer.
 *@param ID The layer ID
 *
 *
 ******************************************************************************/
double LayerCoordinate(int &ID){
	double posizione;
	double shift[10] = {0, 0.3631, -0.3683, 0, -0.2463, 0.6223, 0, 0.3241, -0.3331, 0}; //Fissando i layer X0,X3, Y1,Y4
    
    if(ID<15){
	    posizione = Zmap[ID] - shift[ID-10];
    }
    if(ID>15){
        posizione = Zmap[ID] - shift[ID-15];
    }
return posizione;
};



/*******************************************************************************
 * This function returns the error on the position of a cluster assuming a uniform 
 * distribution between the position of the initial strip and the last strip of the cluster.
 *@param StripIn Initial strip of the cluster
 *@param Dimension Dimension of the cluster
 *
 ******************************************************************************/
double Error(int &StripIn, int &Dimension){
	int StripFin = StripIn + Dimension;
	double error = (StripCoordinate(StripFin)-StripCoordinate(StripIn))/(sqrt(12.));
	if(Dimension==1)
	{
		error = 0.0228/sqrt(12.);
	}
	return error; 
};
	

