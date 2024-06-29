#pragma once
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



double Theta(EventoTrack &e, int i){
    //questa funzione ritorna l'angolo theta di una traccia i-esima.
    return atan(sqrt(e.TrackX[i].Fit[1]*e.TrackX[i].Fit[1]+ e.TrackY[i].Fit[1]*e.TrackY[i].Fit[1]))*180/3.14;


}

double Phi(EventoTrack &e, int i){

    return atan2(e.TrackY[i].Fit[1],e.TrackX[i].Fit[1])*180/3.14;

}

double Angle(Track &a, Track &b, Track &c, Track &d){
     return (a.Fit[1]*c.Fit[1] +b.Fit[1]*d.Fit[1] + 1)/((sqrt(a.Fit[1]*a.Fit[1]+b.Fit[1]*b.Fit[1])+1)*(sqrt(c.Fit[1]*c.Fit[1]+d.Fit[1]*d.Fit[1])+1));
}




void TrackXZ(Evento &e, TGraphErrors *Graph){

double xpos;
double zpos;
double error;
double errorz = 0.; //cm
int n = 0;
Graph->SetTitle("Track XZ; X [cm]; Z [cm]");
for (unsigned int i = 0; i<e.ClusterLayer.size(); i++){
	if(e.ClusterLayer[i]<15)
	{
		xpos = e.ClusterPosizione[i];
		zpos = LayerCoordinate(e.ClusterLayer[i]);
		n = Graph->GetN();
		error = Error(e.InitialStrip[i], e.ClusterDimension[i]);
		//errorz = sqrt(0.01+ error*m*m*error);
		Graph->SetPoint(n,xpos, zpos);
		Graph->SetPointError(n,error,errorz);
	}
}
}

void TrackYZ(Evento &e, TGraphErrors *Graph){

double ypos;
double zpos;
double error;
double errorz = 0.; //cm
int n = 0;
Graph->SetTitle("Track YZ; Y [cm]; Z [cm]");
for (unsigned int i = 0; i<e.ClusterLayer.size(); i++){
	if(e.ClusterLayer[i]>15)
	{
		ypos = e.ClusterPosizione[i];
		zpos = LayerCoordinate(e.ClusterLayer[i]);
		n = Graph->GetN();
		error = Error(e.InitialStrip[i], e.ClusterDimension[i]);
		//errorz = sqrt(0.01+ error*m*m*error);
		Graph->SetPoint(n,ypos, zpos);
		Graph->SetPointError(n,error,errorz);
	}
}

}







void ClusterManualTest(Evento &e){

	for (auto x:e.ClusterDimension){
		std::cout<<"Elenco le dimensioni dei cluster"<<'\n';
		std::cout<< x <<'\n';   
	}

	for (auto x :e.Strip){
		std::cout<<"Elenco tutte le strip dell'evento "<< x<<'\n';

	}
	for (auto x :e.Layer){
		std::cout<<"Elenco tutte i layer rispettivi dell'evento "<<x<<'\n';

	}
	for (auto x:e.InitialStrip){
		std::cout<<"Elenco le strip iniziali dei cluster "<< x<<'\n';

	}
	for (auto x:e.ClusterLayer){
		std::cout<<"Ora scrivo i layer delle prime strip dei cluster " << x <<'\n';


	}

};



















