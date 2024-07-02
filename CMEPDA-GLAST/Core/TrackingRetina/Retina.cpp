
/**
 *  * \file Retina.cpp
 *	This cpp macro takes as input a raw data file and returns a .root file with a tree. It performs the second abstraction of the data from clusters to potential tracks. 
 *  However, the tracks found have to be later corrected with systematics error from relative disalignment and rotations between layers of the apparatus.  
 *  The code is subdivided in three sections: 
 *	The first section consists in the creation of placeholders, utils variables and the trees to be read and filled in the code. A phase space in (m, q) is created 
 *  The second section checks for all the cells in the phase space for potential tracks, potential tracks are found if they consists of more than 2 points.
 *  The third section filters all equal tracks, by equal we mean those that have more than 1 point in common,  and choose among the groups of equal tracks those that are "greater",
 *  by greater we mean the one that has the greater number of points.
 *
 */

#include<iostream>
#include<vector>
#include<sstream>
#include<map>
#include"UtilsRetina.h"
#include"GeneralUtils.h"



void Retina(TString file, TString fileoutput, int iteration, int numcores){

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 															SECTION 1: CREATE PHASE SPACE, CREATE PLACEHOLDERS AND CREATE TREES TO BE READ AND FILLED									//
	//																																													    //
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////	
	//Create the phase space cell                               //
	//////////////////////////////////////////////////////////////	


	std::vector<std::vector<std::pair<double, double> > > Grid;
	BuildGrid(Grid);
	
	
	//////////////////////////////////////////////////////////////
	// Create placeholders and utils variables.                 //
  	//                  										//
	//////////////////////////////////////////////////////////////
	

	////////////////////////////////
	// Variables used in section 2//
	////////////////////////////////
	Track TrackXPlaceholder; //placeholder for the track found in each cell of the phase space in the vision XZ
	Track TrackYPlaceholder; //placeholder for the track found in each cell of the phase space in the vision YZ
	std::vector<Track> multipleTracksX; //placeholder variable to be filled with all tracks found in the event in the vision XZ 
	std::vector<Track> multipleTracksY;	//placeholder variable to be filled with all tracks found in the event in the vision XZ
	double mx, qx, my, qy, chi2X, chi2Y,chi2AttX,chi2AttY; //placeholders variables for track fitting
	int n = 0; // util variable used to keep count of the number of points found as it is needed to fill a TGraphError object 
	int index; // util used to identify the closest point in a layer
	std::vector <double> Vec;// vector filled with the distances of all points in a layer that are close by 0.5 cm from the track identified by the phase space cell
	std::vector <int> VecIndex; //indexes of the elements that are close by 0.5 cm from the track of the member ClusterPosizione in the object Evento.

	// just the linear fit function for each vision. 
	TF1 *lineX = new TF1("lineX", "[0] + [1]*x", -2, 50);
	TF1 *lineY = new TF1("lineY", "[0] + [1]*x", -2, 50);
	


	////////////////////////////////
	// Variables used in section 3//
	////////////////////////////////
	
	std::vector<Track> VettoreMarionetta; //util variable: container that will be used to contain all equal tracksz
	
	int count = 0; //util variable used to keep count of the number of unique tracks
	
	std::vector<Track> FinalFilteredTracksX;//placeholder variable to be filled with all UNIQUE tracks found in the event in the vision XZ, between equal tracks the greater one is taken, see docs on Track Class
	std::vector<Track> FinalFilteredTracksY;//placeholder variable to be filled with all UNIQUE tracks found in the event in the vision YZ, between equal tracks the greater one is taken, see docs on Track Class
	std::vector<Track> CopiaVettore; //util variable used to identify unique tracks
	std::vector < std::vector <Track>>  VettoreTracceUniche; // container of container of equal tracks, used to choose the greater track between equals.
	
	//////////////////////////////////////////////////////////////
	// Create the tree to be read and the tree to be written    //
  	//                  										//
	//////////////////////////////////////////////////////////////


	Evento e; 
	Evento *p = &e;
	TFile *input = new TFile(file,"read");
	TTree *tree = (TTree*)input->Get("tree");
	tree->SetBranchAddress("evento", &p); 
	int entries = tree->GetEntries();
	
	
	EventoTrack etrack;	
	TFile *output = new TFile(fileoutput,"recreate");
	TTree *treetrack = new TTree("treetrack", "treetrack");
	treetrack->Branch("etrack",&etrack);
	
	
	
	
	
	
	
	
	auto start = std::chrono::high_resolution_clock::now();
	
	
	for(int i = iteration*entries/numcores; i<(iteration+1)*entries/numcores; i++){
		tree->GetEntry(i);
		count = 0;
		if(e.Flags[0] ==1){ //we are choosing events that do not have ill clusters. That is specified by the first flag.
			EventReset(multipleTracksY, multipleTracksX, etrack, CopiaVettore, VettoreTracceUniche, FinalFilteredTracksX, FinalFilteredTracksY);
			etrack.NEvento = e.NEventi;
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//																			SECTION 2: SEARCH FOR TRACKS 																														  //
			// In this section, for EACH CELL of the phase space created, the code checks for each vision and for each layer the closest points by 0.5 cm, if there are more than two points than a placeholder track is filled and a linear  //
			// fit, the placeholders TrackXPlaceholder and TrackYPlaceHolder are filled and cleared for EACH CELL, after the search is done for the single cell the placeholders MultipletracksX and MultipleTracksY are filled with		  //
			// the found tracks and are to be filtered in the next sections of the code. Points are not cleared after being put into a track as that may introduce a bias.																	  //
			// 																																																								  //
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


			for(auto ColumnElement: Grid){ //for each column of the phase space
			  
				//for all cells of the phase space in the chosen column
				for(auto cell:ColumnElement)
				{


					//clear placeholders 
					CellReset(cell, TrackXPlaceholder, TrackYPlaceholder, VecIndex, Vec, qx, qy, mx, my, lineX, lineY);


					TGraphErrors *XZ = new TGraphErrors();
					TGraphErrors *YZ = new TGraphErrors();
				
								
					//find the points closest by 0.5 cm for each LayerX
					for(auto l:LayerX){
						for(unsigned int j = 0; j<e.ClusterPosizione.size(); j++){ //push all points close by 0.5 cm from road, those are to be filtered up
							if((e.ClusterLayer[j] == l)&&(Distance(qx, mx ,e.ClusterPosizione[j], Zmap[l])<0.5)){
								Vec.push_back(Distance(qx, mx ,e.ClusterPosizione[j], Zmap[l]));	
								VecIndex.push_back(j);
							}
						}
						if(VecIndex.size() !=0){ //if there are any points close by 0.5 cm search the closest and fill the TGraphErrors XZ
							n = XZ->GetN();
							index = std::distance(Vec.begin(),std::min_element(Vec.begin(), Vec.end()));
							XZ->SetPoint(n, Zmap[l], e.ClusterPosizione[VecIndex[index]]);
							XZ->SetPointError(n, 0, Error(e.InitialStrip[VecIndex[index]], e.ClusterDimension[VecIndex[index]]));
							TrackXPlaceholder.ClusterPosition.push_back(e.ClusterPosizione[VecIndex[index]]);
							TrackXPlaceholder.Error.push_back(Error(e.InitialStrip[VecIndex[index]], e.ClusterDimension[VecIndex[index]]));
							TrackXPlaceholder.Layer.push_back(e.ClusterLayer[VecIndex[index]]);
							VecIndex.clear();//reset utils
							Vec.clear();     //reset utils
						}
					}

					//find the points closest by 0.5 cm for each LayerY
					for(auto l:LayerY){
						for(unsigned int j = 0; j<e.ClusterPosizione.size(); j++ ){//push all points close by 0.5 cm from road
							if((e.ClusterLayer[j] == l)&&(Distance(qy, my ,e.ClusterPosizione[j], Zmap[l])<0.5)){
								Vec.push_back(Distance(qy, my ,e.ClusterPosizione[j], Zmap[l]));	
								VecIndex.push_back(j);
							}	
						}
						if(VecIndex.size() !=0){//if there are any points close by 0.5 cm search the closest and fill the TGraphErrors YZ
							n = YZ->GetN();
							index = std::distance(Vec.begin(),std::min_element(Vec.begin(), Vec.end())); //find the closest point
							YZ->SetPoint(n, Zmap[l], e.ClusterPosizione[VecIndex[index]]);
							YZ->SetPointError(n, 0, Error(e.InitialStrip[VecIndex[index]], e.ClusterDimension[VecIndex[index]]));
							TrackYPlaceholder.ClusterPosition.push_back(e.ClusterPosizione[VecIndex[index]]);
							TrackYPlaceholder.Error.push_back(Error(e.InitialStrip[VecIndex[index]], e.ClusterDimension[VecIndex[index]]));
							TrackYPlaceholder.Layer.push_back(e.ClusterLayer[VecIndex[index]]);
							VecIndex.clear(); //reset utils
							Vec.clear();      //reset utils
						}
					}




						//If there are more than 3 points, make a fit and keep filling placeholder with info on the fit values and chisq
						if((XZ->GetN()>=3)){
							XZ->Fit("lineX","qEX0");
							mx = lineX->GetParameter(1);
							qx = lineX->GetParameter(0);
							chi2X = lineX->GetChisquare();
							chi2AttX = XZ->GetN() - 2;
							TrackXPlaceholder.Fit.push_back(qx);
							TrackXPlaceholder.Fit.push_back(mx);
							TrackXPlaceholder.Fit.push_back(chi2X);
							TrackXPlaceholder.Fit.push_back(chi2AttX);
							TrackXPlaceholder.Fit.push_back(chi2X/chi2AttX);
							multipleTracksX.push_back(TrackXPlaceholder);
						}	

						//If there are more than 3 points, make a fit and keep filling placeholder with info on the fit values and chisq
						if(YZ->GetN()>=3){						
							YZ->Fit("lineY","qEX0");
							my = lineY->GetParameter(1);
							qy = lineY->GetParameter(0);
							chi2Y = lineY->GetChisquare();
							chi2AttY = YZ->GetN() - 2;
							TrackYPlaceholder.Fit.push_back(qy);
							TrackYPlaceholder.Fit.push_back(my);
							TrackYPlaceholder.Fit.push_back(chi2Y);
							TrackYPlaceholder.Fit.push_back(chi2AttY);
							TrackYPlaceholder.Fit.push_back(chi2Y/chi2AttY);
							multipleTracksY.push_back(TrackYPlaceholder);
						}


					////////////////////////////////////////////////////////
					// END PROCESS FOR SINGLE PHASE SPACE CELL            //
					////////////////////////////////////////////////////////



					delete XZ;
					delete YZ;
				} 

			}




			int count2 = 0; //reset utils variable	

			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//																			SECTION 3 : REMOVE DUPLICATES																														  //
			//The aim of this section is to remove all duplicate tracks from the event: two tracks are equal if they have more than 1 point in common, one track is greater than another if 												  //			
			//it has more points than the other. 																																															  //
			//In this section all equal tracks are found and put in groups of equal tracks, the filled container is a vector of vectors of Tracks called VettoreTracceUniche, in each element of this vector of vectors						  //
			//all equal tracks are put. Later each element of the VettoreTracceUniche (so all groups of equal tracks) is ordered and the greater track is taken (the one which has the most points).										  //
			//																																																								  //
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




			//Remove Duplicates for XZ vision, the logic here is to take the "greater" tracks upon all the equals one, greater operator overloading is defined in the libraries.
			if(multipleTracksX.size()>0){
				CopiaVettore = multipleTracksX;//copy vector 
				while(CopiaVettore.size()!=0){//while there still are unique tracks remove all tracks equal to the first element of the CopiaVettore vector
					VettoreTracceUniche.push_back(VettoreMarionetta);//fill the vector of vectors of tracks with an empty container for each type of unique track, later to be filled with all equal tracks
					VettoreTracceUniche[count2].push_back(CopiaVettore[0]);//push the container of equal tracks with the first element of the found tracks
					CopiaVettore.erase(std::remove_if(CopiaVettore.begin(), CopiaVettore.end(), [CopiaVettore](Track pp){return pp == CopiaVettore[0];}), CopiaVettore.end());//remove all tracks that are equal to the first track found.
					count2++;//keep count of the number of unique tracks

				}

				for(auto b : VettoreTracceUniche){ //for each container of unique tracks, find all tracks that are equal
					for(auto c:multipleTracksX){ //iterate on all found tracks from section 2
							if(c == b[0]){ // if the track is equal to the first element of the container of equal tracks, add it to the container of equal tracks.
								b.push_back(c);
								count++; //keep count of the length of the container of equal tracks as this number is needed to choose the greater track(the one with the most point)
								}
						}

					std::sort(b.begin(), b.end());
					FinalFilteredTracksX.push_back(b[count]);
					count = 0;
				}
			}

			//reset utils
			VettoreTracceUniche.clear();
			count2=0;
			count=0;
			CopiaVettore.clear();
			VettoreMarionetta.clear();		

			//Remove Duplicates for YZ vision, the logic here is to take the "greater" tracks upon all the equals one, greater operator overloading is defined in the libraries.
			if(multipleTracksY.size()>0){
				CopiaVettore = multipleTracksY; //copy vector 
				while(CopiaVettore.size()!=0){ //while there still are unique tracks remove all tracks equal to the first element of the CopiaVettore vector
					VettoreTracceUniche.push_back(VettoreMarionetta); //fill the vector of vectors of tracks with an empty container for each type of unique track, later to be filled with all equal tracks
					VettoreTracceUniche[count2].push_back(CopiaVettore[0]);
					CopiaVettore.erase(std::remove_if(CopiaVettore.begin(), CopiaVettore.end(), [CopiaVettore](Track cc){return cc== CopiaVettore[0];}), CopiaVettore.end());  
					count2++;
				}

				for(auto b : VettoreTracceUniche){ //for each container of unique tracks, find all tracks that are equal
					for(auto c:multipleTracksY){//iterate on all found tracks from section 2
						if(c == b[0]){// if the track is equal to the first element of the container of equal tracks, add it to the container of equal tracks.
							b.push_back(c);
							count++;//keep count of the length of the container of equal tracks as this number is needed to choose the greater track(the one with the most point)
						}
					}
					std::sort(b.begin(), b.end()); //order in crescent order the vector of equal tracks
					FinalFilteredTracksY.push_back(b[count]); //take the greater Track
					count = 0; //reset util variable.
				}
			}



			//final filter to remove duplicate projections on YZ vision
			while(FinalFilteredTracksY.size() != 0){
				etrack.TrackY.push_back(FinalFilteredTracksY[0]);
				FinalFilteredTracksY.erase(std::remove_if(FinalFilteredTracksY.begin(), FinalFilteredTracksY.end(), [FinalFilteredTracksY](Track cc){return cc== FinalFilteredTracksY[0];}), FinalFilteredTracksY.end());
			}
			//final filter to remove duplicate projections on XZ vision
			while(FinalFilteredTracksX.size() != 0){
				etrack.TrackX.push_back(FinalFilteredTracksX[0]);
				FinalFilteredTracksX.erase(std::remove_if(FinalFilteredTracksX.begin(), FinalFilteredTracksX.end(), [FinalFilteredTracksX](Track cc){return cc== FinalFilteredTracksX[0];}), FinalFilteredTracksX.end());
			}
			treetrack->Fill();
		}
	}
	output->Write();
	output->Close();


	

	auto endtime = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endtime - start);

	cout << "Ci sono voluti "<<duration.count() << " microsecondi a finire il processo "<< iteration <<endl;

	
	
	
	
	
	
	
	}
