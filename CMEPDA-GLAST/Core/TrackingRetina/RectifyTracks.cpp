/**
 *  * \file RectifyTracks.cpp
 *	This cpp macro takes as input a .root data file with a tree and returns a .root file with a tree. It takes the found potential tracks from the Retina algorithm 
 *  and corrects the positions on each vision with the shifts calculated, and if there is only one projection per vision than a correction in angle can be applied as 
 *  there is no ambiguity in the 3D reconstruction (if there are more than 1 projections for at least one vision then there is ambiguity.)
 *  After correcting the shifts, a cut in chisquare is made a 95% of the NOMINAL INTEGRAL for a chisquare distribution with 1,2,3 dof.
 *
 */


#include<iostream>
#include<vector>
#include<sstream>
#include<map>
#include"GeneralUtils.h"
#include"UtilsRectifyTracks.h"


void RectifyTracks(TString file, TString fileoutput){
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																	SECTION 1: CREATE UTILS VARIABLE, TREES TO BE READ AND FILLED, 
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////
	// CREATE TREE TO BE READ 
	///////////////////////////////////////////////
	EventoTrack etrack;



	EventoTrack e; 
	EventoTrack *p = &e;
	TFile *input = new TFile(file,"read");
	TTree *tree = (TTree*)input->Get("treetrack");
	tree->SetBranchAddress("etrack", &p); 
	int entries = tree->GetEntries();

	///////////////////////////////////////////////
	// CREATE TREE TO BE WRITTEN
	///////////////////////////////////////////////
	TFile *output = new TFile(fileoutput,"recreate");
	TTree *treetrackoutput = new TTree("treetrack", "treetrack");
	treetrackoutput->Branch("etrack",&etrack);


	///////////////////////////////////////////////
	// CREATE UTILS VARIABLES AND PLACEHOLDERS
	///////////////////////////////////////////////

	//simply create the fit functions for a track
	TF1 *lineX = new TF1("lineX", "[0] + [1]*x", -2, 50); 
	TF1 *lineY = new TF1("lineY", "[0] + [1]*x", -2, 50);
	
	int n;  //variable used to fill the TGraphErrors.
	double mx, qx, my, qy, chi2X, chi2Y,chi2AttX,chi2AttY; //placeholders variables for fitting.
	double theta, phi; //placeholders variables to find the angle between tracks that have 1 projection for each layer.
	std::vector<size_t> TracksXToBeErased; //container of the index of the tracks in the container TrackX of the object EventoTrack that do not pass the cut in chisquare on the XZ vision, those are to be removed from the object EventoTrack.
	std::vector<size_t> TracksYToBeErased; //container of the index of the tracks in the container TrackY of the object EventoTrack that do not pass the cut in chisquare on the YZ vision, those are to be removed from the object EventoTrack.
	TH1F *chi2XHist = new TH1F("h", "h", 2000,0 ,50);
	TH1F *oldchi2XHist = new TH1F("h", "h", 2000,0 ,50);
	TH1F *ThetaNoCut = new TH1F("hist","Theta Distribution with no cut", 1000,0, 90);

	TH1F *thetaHist = new TH1F("h", "h", 1000,0 ,90);

	auto start = std::chrono::high_resolution_clock::now();
	for(int i =0; i<entries; i++){
		tree->GetEntry(i);
		TracksXToBeErased.clear(); //reset utils variable
		TracksYToBeErased.clear(); //reset utils variable 

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//																SECTION 2: WORK WITH EVENTS THAT HAVE ONLY 1 PROJECTION FOR EACH VISION
		//In this section we work with event that have 1 potential track for each vision, correction for shifts along XYZ and for relative rotations are made, 
		//a fit is the made with the corrected data and a cut in chisquare is made at 95% of the integral of the NOMINAL CHISQUARE DISTRIBUTION for the corresponding dof. 
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if((e.Nx()==1) && (e.Ny()==1)){
			//copy object from the read tree to the object on the tree to be written
			
			etrack = e;
			//container of points where to make lineare fit
			TGraphErrors *XZ = new TGraphErrors();
			TGraphErrors *YZ = new TGraphErrors();
			
			//setup variables for finding the fit 
			mx = e.TrackX[0].Fit[1]; 
			my = e.TrackY[0].Fit[1];
			qx = e.TrackX[0].Fit[0];
			qy = e.TrackY[0].Fit[0];
			ThetaNoCut->Fill(atan(sqrt(mx*mx + my*my))*180/3.14);

			if(etrack.TrackX[0].Fit[3] == 3){
				oldchi2XHist->Fill(etrack.TrackX[0].Fit[2]);

			}
			

			//correct the projection on the XZ vision for relative shifts and relative angle between layers
			for(unsigned int jj = 0; jj<etrack.TrackX[0].ClusterPosition.size(); jj++){
				n = XZ->GetN();
				etrack.TrackX[0].ClusterPosition[jj] += -AngleCorrection(etrack.TrackX[0].Layer[jj], mx,qx,my,qy) - MapShift[etrack.TrackX[0].Layer[jj]];
				etrack.TrackX[0].Error[jj] = ErrorXYAngoloZ[etrack.TrackX[0].Layer[jj]]; 
				XZ->SetPoint(n, LayerCoordinate(etrack.TrackX[0].Layer[jj]), etrack.TrackX[0].ClusterPosition[jj] );
				XZ->SetPointError(n, 0, etrack.TrackX[0].Error[jj]);
			}
			//correct the projection on the YZ vision for relative shifts and relative angle between layers
			for(unsigned int jj = 0; jj<etrack.TrackY[0].ClusterPosition.size(); jj++){
				n = YZ->GetN();
				etrack.TrackY[0].ClusterPosition[jj] += -AngleCorrection(etrack.TrackY[0].Layer[jj], mx,qx,my,qy) - MapShift[etrack.TrackY[0].Layer[jj]];
				etrack.TrackY[0].Error[jj] = ErrorXYAngoloZ[etrack.TrackY[0].Layer[jj]]; 
				YZ->SetPoint(n, LayerCoordinate(etrack.TrackY[0].Layer[jj]), etrack.TrackY[0].ClusterPosition[jj]);
				YZ->SetPointError(n, 0,etrack.TrackY[0].Error[jj]);
			}



			//simply do the linear fit for each vision.
			lineX->SetParameter(0,qx);
			lineX->SetParameter(1,mx);
			

			XZ->Fit("lineX","qEX0");
			mx = lineX->GetParameter(1);
			qx =lineX->GetParameter(0);
			chi2X =lineX->GetChisquare();
			chi2AttX =XZ->GetN()-2;
			
			lineY->SetParameter(0,qy);
			lineY->SetParameter(1,my);

			YZ->Fit("lineY","qEX0");
			my = lineY->GetParameter(1);
			qy =lineY->GetParameter(0);
			chi2Y =lineY->GetChisquare();
			chi2AttY = YZ->GetN() - 2;

			//calculate phi and theta angle for 3D track.
			phi = atan2(my,mx)*180/3.14;
			theta = atan(sqrt(mx*mx+my*my))*180/3.14;
			
			if(chi2AttX ==3){ 
				chi2XHist->Fill(chi2X);
			}
			
			//if the track passes the cut, fill the object with the info on the track, else remove the track
			if((chi2X<SoglieChiMappa[chi2AttX])){
			
				etrack.TrackX[0].Fit[0]= qx;
				etrack.TrackX[0].Fit[1]= mx;
				etrack.TrackX[0].Fit[2]= chi2X;
				etrack.TrackX[0].Fit[3]= chi2AttX;
				etrack.TrackX[0].Fit[4]= chi2X/chi2AttX;
			}

			else{
				etrack.TrackX.pop_back();
			}

			//if the track passes the cut, fill the object with the info on the track, else remove the track

			if((chi2Y<SoglieChiMappa[chi2AttY])){

				etrack.TrackY[0].Fit[0]= qy;
				etrack.TrackY[0].Fit[1]= my;
				etrack.TrackY[0].Fit[2]= chi2Y;
				etrack.TrackY[0].Fit[3]= chi2AttY;
				etrack.TrackY[0].Fit[4]= chi2Y/chi2AttY;
			}
			else{
				etrack.TrackY.pop_back();
			}


			if((chi2Y<SoglieChiMappa[chi2AttY])&(chi2X<SoglieChiMappa[chi2AttX])){
				thetaHist->Fill(theta);
			}

			delete XZ; //delete containers for points
			delete YZ; //delete containers for points
		}


		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//																SECTION 3: WORK WITH EVENTS THAT DO NOT HAVE ONLY 1 PROJECTION FOR AT LEAST A VISION
		//In this section we work with event that do not have 1 potential track for at least one vision, correction for shifts along XYZ is made, 
		//a fit is the made with the corrected data and a cut in chisquare is made at 95% of the integral of the NOMINAL CHISQUARE DISTRIBUTION for the corresponding dof. 
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		else
		{
			etrack = e; //copy object from the read tree to the object on the tree to be written
			size_t mm = 0; //util variable, placeholder for the index of the tracks that do not pass the chisquare cuts.

			//corrects each projection with shifts on XZ, angle correction is not made as there are ambiguities in the reconstruction.
			for(auto track:etrack.TrackX){
				TGraphErrors *XZ = new TGraphErrors();
				//correct shifts 
				for(unsigned int jj = 0; jj<track.ClusterPosition.size(); jj++){
					track.ClusterPosition[jj] += - MapShift[track.Layer[jj]]; 
					track.Error[jj] = ErrorXYZ[track.Layer[jj]]; 
					XZ->SetPoint(n, LayerCoordinate(track.Layer[jj]), track.ClusterPosition[jj] );
					XZ->SetPointError(n, 0, track.Error[jj]);
				}
				//simply do fit
				lineX->SetParameter(0,qx);
				lineX->SetParameter(1,mx);
				XZ->Fit("lineX","qEX0");
				mx = lineX->GetParameter(1);
				qx =lineX->GetParameter(0);
				chi2X =lineX->GetChisquare();
				chi2AttX =XZ->GetN()-2;
				//if cut is passed, than fill object with new fit values, else put index of tracks on the container of indexes of tracks to be erased.
				if((chi2X<SoglieChiMappa[chi2AttX])){
				
					track.Fit[0]= qx;
					track.Fit[1]= mx;
					track.Fit[2]= chi2X;
					track.Fit[3]= chi2AttX;
					track.Fit[4]= chi2X/chi2AttX;
				}
				else{
					TracksXToBeErased.push_back(mm);
				}
				mm++;
				delete XZ;
				
			}

			mm =0; //reset utils

			//corrects each projection with shifts on YZ, angle correction is not made as there are ambiguities in the reconstruction.
			for(auto track:etrack.TrackY){
				TGraphErrors *YZ = new TGraphErrors();
				//correct shifts 
				for(unsigned int jj = 0; jj<track.ClusterPosition.size(); jj++){
					track.ClusterPosition[jj] += - MapShift[track.Layer[jj]]; 
					track.Error[jj] = ErrorXYZ[track.Layer[jj]]; 
					YZ->SetPoint(n, LayerCoordinate(track.Layer[jj]), track.ClusterPosition[jj] );
					YZ->SetPointError(n, 0, track.Error[jj]);
				}
				//simply do fit
				lineY->SetParameter(0,qy);
				lineY->SetParameter(1,my);
				YZ->Fit("lineX","qEX0");
				my = lineY->GetParameter(1);
				qy =lineY->GetParameter(0);
				chi2Y =lineY->GetChisquare();
				chi2AttY =YZ->GetN()-2;
				//if cut is passed, than fill object with new fit values, else put index of tracks on the container of indexes of tracks to be erased.
				if((chi2Y<SoglieChiMappa[chi2AttY])){
					track.Fit[0]= qy;
					track.Fit[1]= my;
					track.Fit[2]= chi2Y;
					track.Fit[3]= chi2AttY;
					track.Fit[4]= chi2Y/chi2AttY;
				}
				else{
					TracksYToBeErased.push_back(mm);
				}
				mm++;
				delete YZ;
				
			}

			//erase tracks that do not pass the chisquare cut on the XZ vision
			for(auto it = TracksXToBeErased.rbegin(); it !=TracksXToBeErased.rend();++it){
				etrack.TrackX.erase(etrack.TrackX.begin() + *it);

			}
			//erase tracks that do not pass the chisquare cut on the YZ vision
			for(auto it = TracksYToBeErased.rbegin(); it !=TracksYToBeErased.rend(); ++it){
				etrack.TrackY.erase(etrack.TrackY.begin() + *it);
			}
		}

		treetrackoutput->Fill();
		
	}
	oldchi2XHist->SaveAs("oldchi2Dist.root");
	ThetaNoCut->SaveAs("OldThetaDist.root");
	chi2XHist->SaveAs("chi2Dist.root");
	thetaHist->SaveAs("ThetaDist.root");
	
	output->Write();
	output->Close();
	auto endtime = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endtime - start);

	cout << "Ci sono voluti "<<duration.count() << " microsecondi a togliere le sistematiche e ad effettuare il taglio in chi2."<<endl;

	
	
	
	
	
}
