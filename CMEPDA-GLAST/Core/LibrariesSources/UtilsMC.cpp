/** 
 * \file UtilsMC.cpp
 * Source code for utils libraries used in MC simulation
 *
 **/

#include "UtilsMC.h"



std::vector<int> XLayer = {14,13,12,11,10};/** X Layer's ID ordered for checking consecutive hits condition.*/
std::vector<int> YLayer = {24,23,22,21,20}; /**  Layer's ID ordered for checking consecutive hits condition.*/


/**************************
 *  This function clears all members of an instance of the object Evento. It is used to reset the instance at each iteration of the MC.
 * 
 * @param e Instance of the object evento to be reset.
 *  
 **/
void ResetEvento(Evento &e){
	
    e.Layer.clear();
	e.Strip.clear();
	e.ClusterDimension.clear();
	e.InitialStrip.clear();
	e.ClusterLayer.clear();
	e.ClusterPosizione.clear();
};


/**************************
 *  This function clears all utils variable used in the MC. It is used to reset the variables at each iteration of the MC
 * 
 * @param TrackX Container of hitten strips position for the vision XZ
 * 
 * @param TrackY Container of hitten strp position for the vision YZ
 * 
 * @param ZlevelX Container of hitten layers Z position for the vision XZ
 * 
 * @param ZlevelY Container of hitten layers Z position for the vision YZ
 * 
 * @param ZlevelXPlaceHolder util variable used to check for if consecutive hits trigger condition is satisfied in the vision XZ
 * 
 * @param ZlevelYPlaceHolder util variable used to check for if consecutive hits trigger condition is satisfied in the vision YZ
 * 
 * @param NxHit variable that keeps count of the number of hits, used to check if the trigger condition of more than 2 hits is satisfied on the vision XZ
 * 
 * @param NyHit variable that keeps count of the number of hits, used to check if the trigger condition of more than 2 hits is satisfied on the vision YZ
 * 
 * @param epsilon variable that at each hit is multiplied by the mean efficiency of the hitten layer. 
 *  
 **/
void ResetUtils(std::vector<double> &TrackX, std::vector<double> &TrackY, std::vector<int> &ZlevelX, std::vector<int> &ZlevelY, std::vector<int> &ZlevelXPlaceHolder, std::vector<int> &ZlevelYPlaceHolder, int &NxHit, int &NyHit, double &epsilon){
    TrackX.clear();
	TrackY.clear();
	ZlevelX.clear();
	ZlevelY.clear();
    ZlevelYPlaceHolder.clear();
    ZlevelXPlaceHolder.clear();
    NxHit = 0; //reset hit
    NyHit = 0; //reset hit
	epsilon = 1; //reset efficiency. 
    



};


/**************************
 *  This function generates four random numbers: zenith angle theta, azimuth angle theta, point coordinates (x0, y0) on the plane of the top layers and then accordingly generates the projections.
 *  Theta is generated using the Inverse Transforming Sample Method, the pdf is a distribution sin(theta)*cos^3(theta). Phi is generated uniformly and also the coordinates (x0, y0).
 * @param auxiliary uniformly generated variable used in the Inverse Transforming Sample Method
 *  
 * @param theta Zenith Angle, it is one of the four generated numbers.
 * 
 * @param phi Azimuth Angle, it is one of the four generated numbers.
 * 
 * @param x0 X coordinate of point that belongs to the track on the plane of the top layer, it is one of the four generated numbers.
 * 
 * @param y0 Y coordinate of point that belongs to the track on the plane of the top layer,it is one of the four generated numbers.
 * 
 * @param mx Angular coefficient of XZ projection of the track.
 * 
 * @param my Angular coefficient of YZ projection of the track.
 * 
 * @param qx Intercept of XZ projection of the track.
 * 
 * @param qy Intercept of YZ projection of the track.
 * 
 * @param thetacalc util variable used to do calculation in radiants.
 * 
 * @param phicalc util variable used to do calculation in radiants.
 * 
 * @param Ztop Z coordinate of the top layer, used to calculate qx and qy
 * 
 * 
 **/
void GenerateRandomNumbers(double &auxiliary, double &theta, double &phi, double &x0, double &y0, double &mx, double &my , double &qx, double &qy, double &thetacalc, double &phicalc, double &Ztop){
        auxiliary = gRandom->Uniform(1); //generate util variable for Inverse Transforming Sample Method.
        theta = acos(pow(1-auxiliary, 1./4.))*180/3.14; //generate zenith angle 
        phi = gRandom->Uniform(-180, 180); //generate azimuth angle
        x0 = gRandom->Uniform(-100, StripCoordinate(1536) +100); //generate X coordinate of initial point
        y0 = gRandom->Uniform(-100, StripCoordinate(1536) +100); //generate Y coordinate of initial point
        thetacalc =theta*3.14/180;
        phicalc = phi*3.14/180;
        //here just find the correct sign value for mx and my.
        if((phi>-180)&&(phi<-90)){
            mx = +sqrt(tan(thetacalc)*tan(thetacalc)/(1+tan(phicalc)*tan(phicalc)));
            my = +sqrt(tan(phicalc)*tan(phicalc)*tan(thetacalc)*tan(thetacalc)/(1+tan(phicalc)*tan(phicalc)));
        }
        if((phi>-90)&&(phi<0)){
            mx = -sqrt(tan(thetacalc)*tan(thetacalc)/(1+tan(phicalc)*tan(phicalc)));
            my = +sqrt(tan(phicalc)*tan(phicalc)*tan(thetacalc)*tan(thetacalc)/(1+tan(phicalc)*tan(phicalc)));
        }
        if((phi>0)&&(phi<90)){
            mx = -sqrt(tan(thetacalc)*tan(thetacalc)/(1+tan(phicalc)*tan(phicalc)));
            my = -sqrt(tan(phicalc)*tan(phicalc)*tan(thetacalc)*tan(thetacalc)/(1+tan(phicalc)*tan(phicalc)));
        }
        if((phi>90)&&(phi<180)){
            mx = +sqrt(tan(thetacalc)*tan(thetacalc)/(1+tan(phicalc)*tan(phicalc)));
            my = -sqrt(tan(phicalc)*tan(phicalc)*tan(thetacalc)*tan(thetacalc)/(1+tan(phicalc)*tan(phicalc)));
        }
        
        //calculate Intercepts.
        qx = x0 - mx*Ztop;
        qy = y0 - my*Ztop;

};



