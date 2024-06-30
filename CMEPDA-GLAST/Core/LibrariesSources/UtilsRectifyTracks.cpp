/**
 *  \file UtilsRectifyTracks.cpp
 * Source code for the library that contains utils for the RectifyTracks macro. A function called Layercoordinate that corrects shifts along z direction is in the GeneralUtils library.
 */

#include "UtilsRectifyTracks.h"




/*******************************************************************************
 * This function corrects relative rotations for events that have one projection
 * for each vision. 3D info is needed for this correction.
 *
 *@param layer Layer ID
 *
 *@param mx Angular coefficient of track on XZ vision
 *
 *@param qx Intercept of track on XZ vision
 *
 *@param my Angular coefficient of track on YZ vision
 *
 *@param qy Intercept of track on YZ vision
 *
 *
 ******************************************************************************/
double AngleCorrection(int &layer, double &mx, double &qx, double &my, double &qy){//m e q sono i parametri della retta dell'altro piano
	//ad ogni punto, quando rifaccio il fit devo sottrarre questa funzione con i dati del primo fit 
	double alpha[10] = {-0.00092, -0.00064, 0.00176, 0.00188, -0.00161, 0.00246, 0.00032, 0.00010, 0.00145, 0.00125}; //Step 50, con correzione XY precedente
	double l[10] = {0.0415, 0.0303, -0.0341, -0.0429, -0.0047, -0.0146, 0.0130, 0.0143, -0.0174, -0.0184}; //Step 50, con correzione XY precedente
	
	double Pos =0;
	if(layer<15)
	{
		Pos = my*LayerCoordinate(layer)+qy;
	}
	if(layer>15)
	{
		Pos = mx*LayerCoordinate(layer)+qx;
	}
	
	int LayersID[10] = {10,11,12,13,14,20,21,22,23,24};	
	std::map<int, double> MapAngle;
	for(int j=0;j<10;j++)
	{
		MapAngle.insert(std::make_pair(LayersID[j], (alpha[j]*Pos+l[j])));
	}

return MapAngle[layer];
};



std::map<int, double> ErrorXYZ{ /**Map that links the layer ID to a rescaled error based on the width of residuals. Correction used for event that do not have one track per vision.*/
	{10, 0.0254795},
	{11,0.0182482},
	{12, 0.0242299},
	{13, 0.0239926},
	{14, 0.0698908},
	{20, 0.0536476},
	{21, 0.019532 },
	{22, 0.0216202},
	{23, 0.0177527},
	{24, 0.022226}
};


std::map<int, double> ErrorXYAngoloZ{ /** Map that links the layer ID to a rescaled error based on the width of residuals. Correction used for event that have one projection per vision*/
	{10, 0.0132882},
	{11, 0.0101872},
	{12, 0.0110667},
	{13, 0.0111949},
	{14, 0.0321004},
	{20, 0.0317406},
	{21, 0.011021 },
	{22, 0.0112294},
	{23, 0.0106568},
	{24, 0.0130159},
};

std::map<int, double> MapShift{ /** Map that links the layer ID to its shift along the X or Y direction */
	{10, 0.},
	{11, -0.0081},
	{12, -0.0099},
	{13, 0.},
	{14, 0.0632},
	{20, -0.0717},
	{21, 0. },
	{22, 0.0178},
	{23, 0.0003},
	{24, 0.}	
};

std::map<int, double> SoglieChiMappa{ /** Chisquare distribution cuts at 95%, based on NOMINAL CHISQUARE DISTRIBUTION, not the measured one */
	{1, 3.841},
	{2, 5.991},
	{3,7.815 }
};
