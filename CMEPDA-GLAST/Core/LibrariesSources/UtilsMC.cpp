/** 
 * \file UtilsMC.cpp
 * Source code for utils libraries used in MC simulation
 *
 **/

#include "UtilsMC.h"



std::vector<int> XLayer = {14,13,12,11,10};
std::vector<int> YLayer = {24,23,22,21,20};


void ResetEvento(Evento &e){
    e.Layer.clear();
	e.Strip.clear();
	e.ClusterDimension.clear();
	e.InitialStrip.clear();
	e.ClusterLayer.clear();
	e.ClusterPosizione.clear();
};

void ResetUtils(std::vector<double> &TrackX, std::vector<double> &TrackY, std::vector<int> &ZlevelX, std::vector<int> &ZlevelY, std::vector<int> &ZlevelXPlaceHolder, std::vector<int> &ZlevelYPlaceHolder, int &NxHit, int &NyHit, double &epsilon){
    TrackX.clear();
	TrackY.clear();
	ZlevelX.clear();
	ZlevelY.clear();
    ZlevelYPlaceHolder.clear();
    ZlevelXPlaceHolder.clear();
    NxHit = 0; //resetto hit
    NyHit = 0; //resetto hit
	epsilon = 1; //resetto l'efficienza. 
    



};


void GenerateRandomNumbers(double &auxiliary, double &theta, double &phi, double &x0, double &y0, double &mx, double &my , double &qx, double &qy, double &thetacalc, double &phicalc, double &Ztop){
        auxiliary = gRandom->Uniform(1);
        theta = acos(pow(1-auxiliary, 1./4.))*180/3.14; //questo è il theta distribuito con distribuzione cos^3*sin    
        phi = gRandom->Uniform(-180, 180);
        x0 = gRandom->Uniform(-100, StripCoordinate(1536) +100);
        y0 = gRandom->Uniform(-100, StripCoordinate(1536) +100);
        thetacalc =theta*3.14/180;
        phicalc = phi*3.14/180;
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
        qx = x0 - mx*Ztop;
        qy = y0 - my*Ztop;

};



