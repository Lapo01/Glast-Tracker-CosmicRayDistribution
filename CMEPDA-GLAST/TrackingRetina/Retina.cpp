
/**
 *  * \file Retina.cpp
 *	This cpp macro takes as input a raw data file and returns a .root file with a tree. It performs a first abstraction of the data
 *
 * 
 *
 *
 */

#include<iostream>
#include<vector>
#include<sstream>
#include"ClassEvento.h"

double Distance(double q, double m, double xpos, double zpos){
	
	return abs(xpos - m*zpos - q)/sqrt(1+m*m);
	
	
	}


void Retina2(const std::string file, const std::string fileoutput, int iteration){
	//////////////////////////////////////////////////////////////	
	//Qui costruiamo la retina del nostro algoritmo di tracking //
	//////////////////////////////////////////////////////////////	
	std::vector<std::vector<std::pair<double, double> > > ColumnVector;
	
	double m_in = -4;
	double m_fin = 4;
	double q_in = -30 ;
	int strip = 1536;
	double q_fin = 65;
	std::vector<std::pair<double, double>> Column;
	double N = 200; //detta il passo dello spazio delle fasi lungo 
	double M = 400;
	
	for(int i = 0; i<N; i++){
		
		for(int j = 0; j<M ; j++){
			Column.push_back(std::make_pair(m_in + (m_fin-m_in)*i/N, q_in + (q_fin-q_in)*j/M));
			
			}
		ColumnVector.push_back(Column);
		Column.clear();
		} 
		
	//////////////////////////////////////////////////////////////	
	// Ora capiamo come leggere i dati senza far esplodere il pc//  	
	//                                                          //
	//////////////////////////////////////////////////////////////	
	
	
	

		
		
	
	
	
	
	
	
	
	
	
	//////////////////////////////////////////////////////////////
	// Creiamo il tree e gli oggetti placeholder da riempire.   //
  	//                  										//
	//////////////////////////////////////////////////////////////
	
	Track trackxplaceholder;
	Track trackyplaceholder;
	std::vector<Track> VettoreMarionetta;
	double mx, qx, my, qy, chi2X, chi2Y,chi2AttX,chi2AttY;
	int n = 0;
	double soglia = 300;
	int count = 0;
	std::map<int, double> Zmap; 
	std::vector<Track> multipleTracksX;
	std::vector<Track> multipleTracksY;
	std::vector<Track> FinalFilteredTracksX;
	std::vector<Track> FinalFilteredTracksY;
	std::vector<Track> CopiaVettore;
	std::vector < std::vector <Track>>  VettoreTracceUniche;
	
	Zmap.insert(std::make_pair(10, 3.1 ));
	Zmap.insert(std::make_pair(11, 6.3));
	Zmap.insert(std::make_pair(12,16.9));
	Zmap.insert(std::make_pair(13,20.1));
	Zmap.insert(std::make_pair(14,31.2));
	Zmap.insert(std::make_pair(20,-1.0));
	Zmap.insert(std::make_pair(21,10.1));
	Zmap.insert(std::make_pair(22,13.3));
	Zmap.insert(std::make_pair(23,24.6));
	Zmap.insert(std::make_pair(24,27.8));
	
	
	EventoTrack etrack;
	TString FileName= file;
	Evento e; 
	Evento *p = &e;
	TFile *input = new TFile(FileName,"read");
	TTree *tree = (TTree*)input->Get("tree");
	tree->SetBranchAddress("eventoprova", &p); 
	int entries = tree->GetEntries();
	
	
	
	TString FileOutput= fileoutput;
	TFile *output = new TFile(FileOutput,"recreate");
	TTree *treetrack = new TTree("treetrack", "treetrack");
	treetrack->Branch("etrack",&etrack);
	
	
	
	int index;
	std::vector <double> Vec;
	std::vector <int> VecIndex;
	
	int LayerX[5] = {10,11,12,13,14};
	int LayerY[5]={20,21,22,23,24};
	
	
	
	
	
	auto start = std::chrono::high_resolution_clock::now();
	TF1 *lineX = new TF1("lineX", "[0] + [1]*x", -2, 50);
	lineX->SetParName(0,"q");
	lineX->SetParName(1,"m");
	TF1 *lineY = new TF1("lineY", "[0] + [1]*x", -2, 50);
	lineY->SetParName(0,"q");
	lineY->SetParName(1,"m");
	std::cout<<iteration*entries/8<<std::endl;
	std::cout<<(1+iteration)*entries/8<<std::endl;
	for(int i = iteration*entries/8; i<(1+iteration)*entries/8; i++){
		//cout<<i<<endl;
		tree->GetEntry(i);
		count = 0;
		if((e.Flags[0] ==0)&(e.Flags[1]==0)){ // METTI UGUALE A 0 SE HAI IL MONTECARLO! PER LE DISTRIBUZIONI SIMULATE NON HAI DELLE FLAG ATTIVE.
		//NOTA QUA DEVI RESETTARE L'OGGETTO PLACEHOLDER EventoTrack ,
		multipleTracksY.clear();
		multipleTracksX.clear();
		CopiaVettore.clear();
		VettoreTracceUniche.clear();
		FinalFilteredTracksY.clear();
		FinalFilteredTracksX.clear();
		etrack.TrackY.clear();
		etrack.TrackX.clear();
		
		for(auto ColumnElement: ColumnVector){
			  
		
			for(auto element:ColumnElement){
				trackyplaceholder.Fit.clear();
				trackyplaceholder.Error.clear();
				trackyplaceholder.ClusterPosition.clear();
				trackyplaceholder.Layer.clear();
				
				
				trackxplaceholder.Fit.clear();
				trackxplaceholder.Error.clear();
				trackxplaceholder.ClusterPosition.clear();
				trackxplaceholder.Layer.clear();
				VecIndex.clear();
				Vec.clear();		
				
				TGraphErrors *XZ = new TGraphErrors();
				TGraphErrors *YZ = new TGraphErrors();
				
				qx = element.second;
				qy = element.second;
				mx = element.first;
				my = element.first;
				
				lineX->SetParameters(0, qx);
				lineX->SetParameters(1, mx);
				lineY->SetParameters(0, qy);
				lineY->SetParameters(1, my);

				for(auto l:LayerX){
				for(int j = 0; j<e.ClusterPosizione.size(); j++){
					if((e.ClusterLayer[j] == l)&&(Distance(qx, mx ,e.ClusterPosizione[j], Zmap[l])<0.5)){
						Vec.push_back(Distance(qx, mx ,e.ClusterPosizione[j], Zmap[l]));	
						VecIndex.push_back(j);
					}
				}
					if(VecIndex.size() !=0){
					n = XZ->GetN();
					index = std::distance(Vec.begin(),std::min_element(Vec.begin(), Vec.end()));
					XZ->SetPoint(n, Zmap[l], e.ClusterPosizione[VecIndex[index]]);
					XZ->SetPointError(n, 0, Error(e.InitialStrip[VecIndex[index]], e.ClusterDimension[VecIndex[index]]));
					trackxplaceholder.ClusterPosition.push_back(e.ClusterPosizione[VecIndex[index]]);
					trackxplaceholder.Error.push_back(Error(e.InitialStrip[VecIndex[index]], e.ClusterDimension[VecIndex[index]]));
					trackxplaceholder.Layer.push_back(e.ClusterLayer[VecIndex[index]]);
					VecIndex.clear();
					Vec.clear();
					}
				}
				for(auto l:LayerY){
				for(int j = 0; j<e.ClusterPosizione.size(); j++ ){
					if((e.ClusterLayer[j] == l)&&(Distance(qy, my ,e.ClusterPosizione[j], Zmap[l])<0.5)){
						Vec.push_back(Distance(qy, my ,e.ClusterPosizione[j], Zmap[l]));	
						VecIndex.push_back(j);
					}

				}
					if(VecIndex.size() !=0){
					n = YZ->GetN();
					index = std::distance(Vec.begin(),std::min_element(Vec.begin(), Vec.end()));
					YZ->SetPoint(n, Zmap[l], e.ClusterPosizione[VecIndex[index]]);
					YZ->SetPointError(n, 0, Error(e.InitialStrip[VecIndex[index]], e.ClusterDimension[VecIndex[index]]));
					trackyplaceholder.ClusterPosition.push_back(e.ClusterPosizione[VecIndex[index]]);
					trackyplaceholder.Error.push_back(Error(e.InitialStrip[VecIndex[index]], e.ClusterDimension[VecIndex[index]]));
					trackyplaceholder.Layer.push_back(e.ClusterLayer[VecIndex[index]]);
					VecIndex.clear();
					Vec.clear();
					}
					
				}
					
					
						
					if((XZ->GetN()>=3)){
						XZ->Fit("lineX","qEX0");
						mx = lineX->GetParameter(1);
						qx = lineX->GetParameter(0);
						chi2X = lineX->GetChisquare();
						chi2AttX = XZ->GetN() - 2;
						trackxplaceholder.Fit.push_back(qx);
						trackxplaceholder.Fit.push_back(mx);
						trackxplaceholder.Fit.push_back(chi2X);
						trackxplaceholder.Fit.push_back(chi2AttX);
						trackxplaceholder.Fit.push_back(chi2X/chi2AttX);
						multipleTracksX.push_back(trackxplaceholder);
					}
					if(YZ->GetN()>=3){						
						YZ->Fit("lineY","qEX0");
						my = lineY->GetParameter(1);
						qy = lineY->GetParameter(0);
						chi2Y = lineY->GetChisquare();
						chi2AttY = YZ->GetN() - 2;
						trackyplaceholder.Fit.push_back(qy);
						trackyplaceholder.Fit.push_back(my);
						trackyplaceholder.Fit.push_back(chi2Y);
						trackyplaceholder.Fit.push_back(chi2AttY);
						trackyplaceholder.Fit.push_back(chi2Y/chi2AttY);
						multipleTracksY.push_back(trackyplaceholder);
					}

				
				////////////////////////////////////////////////////////
				// A questo punti reiteriamo il processo              //
				////////////////////////////////////////////////////////
				
				
				
				delete XZ;
				delete YZ;
		} 
	
		}

	


	int count2 = 0;

	if(multipleTracksX.size()>0){
	CopiaVettore = multipleTracksX;
	while(CopiaVettore.size()!=0){
		VettoreTracceUniche.push_back(VettoreMarionetta);
		VettoreTracceUniche[count2].push_back(CopiaVettore[0]);
		CopiaVettore.erase(std::remove_if(CopiaVettore.begin(), CopiaVettore.end(), [CopiaVettore](Track pp){return pp == CopiaVettore[0];}), CopiaVettore.end());
		count2++;

	}
	
	for(auto b : VettoreTracceUniche){
		for(auto c:multipleTracksX){
				if(c == b[0]){
					b.push_back(c);
					count++;
					}
			}
	
		std::sort(b.begin(), b.end());
		FinalFilteredTracksX.push_back(b[count]);
		count = 0;
		}
	}
	VettoreTracceUniche.clear();
	count2=0;
	count = 0;
	CopiaVettore.clear();
	VettoreMarionetta.clear();
	if(multipleTracksY.size()>0){
	CopiaVettore = multipleTracksY;
	while(CopiaVettore.size()!=0){
		VettoreTracceUniche.push_back(VettoreMarionetta);
		VettoreTracceUniche[count2].push_back(CopiaVettore[0]);
		CopiaVettore.erase(std::remove_if(CopiaVettore.begin(), CopiaVettore.end(), [CopiaVettore](Track cc){return cc== CopiaVettore[0];}), CopiaVettore.end());
		count2++;
	}
	
	for(auto b : VettoreTracceUniche){
		for(auto c:multipleTracksY){
				if(c == b[0]){
					b.push_back(c);
					count++;

				}
			
			
			}
		std::sort(b.begin(), b.end());
		FinalFilteredTracksY.push_back(b[count]);
		count = 0;
		}
	}

	while(FinalFilteredTracksY.size() != 0){
		etrack.TrackY.push_back(FinalFilteredTracksY[0]);
		FinalFilteredTracksY.erase(std::remove_if(FinalFilteredTracksY.begin(), FinalFilteredTracksY.end(), [FinalFilteredTracksY](Track cc){return cc== FinalFilteredTracksY[0];}), FinalFilteredTracksY.end());


	}
	while(FinalFilteredTracksX.size() != 0){
		etrack.TrackX.push_back(FinalFilteredTracksX[0]);
		FinalFilteredTracksX.erase(std::remove_if(FinalFilteredTracksX.begin(), FinalFilteredTracksX.end(), [FinalFilteredTracksX](Track cc){return cc== FinalFilteredTracksX[0];}), FinalFilteredTracksX.end());


	}
	
	
	
	
	
	
	treetrack->Fill();
	
	}//fine if FlagContinue
	exit:;
	}//fine ciclo for
	output->Write();
	output->Close();


	

	auto endtime = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endtime - start);

	cout << "Ci sono voluti "<<duration.count() << " microsecondi"<<endl;

	
	
	
	TGraphErrors *XZlol = new TGraphErrors();
	TGraphErrors *YZlol = new TGraphErrors();
	
	
	
	
	
	TrackFitYZ(e, YZlol, Zmap);
	TrackFitXZ(e, XZlol, Zmap);
	TCanvas *XZPlane = new TCanvas();
	XZlol->Draw("*A");
	
	if(etrack.TrackX.size()>0){
		for(auto track:etrack.TrackX){
			cout<<track.Fit[0]<<endl;
			cout<<track.Fit[1]<<endl;
			lineX->SetParameter(0, track.Fit[0]);
			lineX->SetParameter(1, track.Fit[1]);
			lineX->Draw("same");
		}
	}
	/*lineX->SetParameter(0,33.8);
	lineX->SetParameter(1, -0.16);
	lineX->Draw("same"); 
*/


	TCanvas *YZPlane = new TCanvas();
	YZlol->Draw("*A");
	if(etrack.TrackY.size()>0){
		for(auto track:etrack.TrackY){
			cout<<track.Fit[0]<<endl;
			cout<<track.Fit[1]<<endl;
			lineY->SetParameter(0, track.Fit[0]);
			lineY->SetParameter(1, track.Fit[1]);
			lineY->Draw("same");
		}
	}
	
	
	
	
	
	
	
	
	}
