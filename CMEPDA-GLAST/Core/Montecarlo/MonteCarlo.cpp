/**
 *  * \file MonteCarlo.cpp
 *	This file makes a MC simulation in order to generated expected measured zenith angle and azimuth distribution. Those distributions are saved in a TH1F container inside a .root file
 *  The code keeps account of muted strips and simulate trigger condition of at least 3 consecutive hits per vision. 
 *  Smearing of hits, cuts on ill clusters, and cluster length dimension is not kept into account.
 *
 *
 */


#include"UtilsMC.h"








void MonteCarlo(const std::string fileinput, TString path){


	

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 														SECTION 1:												  //
	// In this section we initialize all variables used in the code, the tree to be outputted is initialized.		  //
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



    
    std::vector<double> StripPosition; //vector used to map hit to the activated strip position.
    for(int i =0; i<1535; i++){
        StripPosition.push_back(StripCoordinate(i));
    }




    //Create the map that links layer's ID to its muted strip container.
    std::vector<int> MutedStripX0;
    std::vector<int> MutedStripX1;
    std::vector<int> MutedStripX2;
    std::vector<int> MutedStripX3;
    std::vector<int> MutedStripX4;
    std::vector<int> MutedStripY0;
    std::vector<int> MutedStripY1;
    std::vector<int> MutedStripY2;
    std::vector<int> MutedStripY3;
    std::vector<int> MutedStripY4;
    std::vector<std::vector<int>> MutedVector = {MutedStripX0,MutedStripX1,MutedStripX2,MutedStripX3,MutedStripX4,MutedStripY0,MutedStripY1,MutedStripY2,MutedStripY3,MutedStripY4};
    std::map<int, std::vector<int>> MapMutedStrips;
    for(int i =0; i<10; i++){
        MapMutedStrips.insert(std::make_pair(LayersID[i], MutedVector[i]));
    }   






    int ciclo = 0; //util variable declared just to mantain compatility with the FindMutedStrips macro used in the UtilsCreateTree library
    int NHit = 0; //util variable declared just to mantain compatility with the FindMutedStrips macro used in the UtilsCreateTree library
    
    FindMutedStrips(fileinput,NHit,ciclo,LayersID,MapLayerToID,MapMutedStrips); //find muted/dead/inefficent strips


    std::vector<TH1F*> HitCounts; // Vector containing the TH1F pointers of the histograms of hits for the Layers: those histograms have 1536 bins which is equal to the number of strips in each layers /
	std::map<int,TH1F*> HitMap; // Maps Layer's ID to its histogram pointer        /

	
	//Declare histograms of hits in the layers for the MC simulation
    TString placename;
    for (int i = 0; i < 10; ++i) 
    {
        placename = "Hit Distribution Layer " + MapIDToLayer[LayersID[i]] + " 2 Maggio 2024";
    	TH1F *h = new TH1F("hist", placename, 1000, 0, StripCoordinate(1536));
	    HitCounts.push_back(h);
	    HitMap.insert(std::make_pair(LayersID[i], h));
    }







    int NBin = 1000//number of bins


	//declare histograms to be filled with generated zenith and azimuth distribution, and distribution of angles that satisfies trigger condition (ExpectedDistributions).
    TH1F *ThetaTotalCount = new TH1F("hist", "Zenith angle Theta Generated Distribution", NBin, 0, 90);
    TH1F *ThetaExpectedDistribution = new TH1F("hist", "Zenith angle Theta Expected Distribution", NBin, 0, 90);
    TH1F *PhiExpectedDistribution = new TH1F("hist", "Azimuth angle Phi Expected Distribution", NBin, -180, 180);
    TH1F *PhiTotalCount = new TH1F("hist", "Azimuth angle Phi Generated Distribution", NBin, -180, 180);
    TH1F *HitXDistribution = new TH1F("hist", "Hits on Vision X Distribution", 6 , 0, 6);
    TH1F *HitYDistribution = new TH1F("hist", "Hits on Vision Y Distribution", 6 , 0, 6);



    int NxHit =0;//util variable used to check for number of hits in vision XZ
    int NyHit =0;//util variable used to check for number of hits in vision XY
    
    double mx, my, qx, qy, theta, phi, x0, y0, x, y,thetacalc, phicalc, HittenStripPosition, auxiliary;//util variable used in the generation of random numbers and hits in the layers.
    double Ztop = 31.2; // Height of the highest layer.
    double MaxLength = StripCoordinate(1536); //The maximum length at which a layer is sensible to a hit 
    std::vector<double> TrackX; //placeholder container for position of hitten strips on the XZ vision
    std::vector<double> TrackY //placeholder container for position of hitten strips on the YZ vision
    std::vector<int> ZlevelX;//placeholder container for position of hitten layers on the XZ vision
    std::vector<int> ZlevelY;//placeholder container for position of hitten layers on the YZ vision
    std::vector<int> ZlevelXPlaceHolder; //util variable used to check for consecutive hits trigger condition on the XZ vision.
    std::vector<int> ZlevelYPlaceHolder; //util variable used to check for consecutive hits trigger condition on the YZ vision.
    std::vector<int> DiffX;//util variable used to check for consecutive hits trigger condition on the XZ vision.
    std::vector<int> DiffY;//util variable used to check for consecutive hits trigger condition on the YZ vision.
    double epsilon = 1; //util variable used to keep count of the efficiency after each hit. 
    int Flag =1; //flag used to check if a hit is valid.
    double yplaceholder, xplaceholder; //placeholder value for the position of the hit along a strip.







	

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 														SECTION 2																																		  //
	// In this section we make the MC simulation: four pseudo random numbers are generated: the zenith angle,the azimuth angle and the coordinates (x0, y0) on the plane of the highest layers.				  //
	// Projections' Intercepts and Angular Coefficients are calculated given those four numbers, following this generation the hits are generated and the trigger condition of at least 3 consecutive hits	  //
	// per vision is checked, taking into account the presence of dead/muted/inefficient strips.																											  //
	// If the trigger condition is satisfied, expected measured distributions and hit distributions are filled.																								  //
	// Whether the  the distribution of generated numbers are filled.																																		  //
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




    gRandom = new TRandom3(0);//set random seed.


    for(int j = 0; j<10; j++){
        for(int i = 0; i <1000000;i++){
    
            ResetUtils(TrackX, TrackY, ZlevelX, ZlevelY, ZlevelXPlaceHolder, ZlevelYPlaceHolder, NxHit, NyHit, epsilon); //reset utils

            GenerateRandomNumbers(auxiliary, theta, phi, x0, y0, mx, my, qx, qy, thetacalc, phicalc, Ztop); //generate zenith, azimuth angles and the first point of the track on the top layer.

			//for each Y layer generate hits.
            for(auto l:YLayer){
                y = my * Zmap[l] + qy ; //hit coordinate along y
                xplaceholder = mx * Zmap[l] + qx; //hit coordinate along x (so along the strip)
                Flag =1; //reset Hit Flag.
                //check if hit is valid
                if((((y<StripCoordinate(0))||(y>MaxLength))||((y>StripCoordinate(383))&&(y<StripCoordinate(384)))||((y>StripCoordinate(767))&&(y<StripCoordinate(768)))||((y>StripCoordinate(1151))&&(y<StripCoordinate(1152)))||(y>StripCoordinate(1535))||((xplaceholder<StripCoordinate(0))||(xplaceholder>35)))){
                    Flag = 0;
                }
        

                HittenStripPosition = *std::lower_bound(StripPosition.begin(), StripPosition.end(), y);//calculate hitten strips
                
                //check if the strip is not close to a muted/inefficient/Dead one.
                for(auto Muted:MapMutedStrips[l]){
                    if(abs(StripCoordinate(Muted) -HittenStripPosition)<0.001){
                        Flag = 0;
                    }
                }
                
                //insert hits positions and keep account of mean layer efficiency if hit is valid.
                if(Flag ==1){
		            epsilon = epsilon*EfficiencyMap[l];
		            NyHit++;
		            ZlevelY.push_back(l);
		            TrackY.push_back(HittenStripPosition);
                }
            }
            
			//for each X layer generate hits.
            for(auto l:XLayer){
                x = mx * Zmap[l] + qx ;//hit coordinate along x
                yplaceholder = my * Zmap[l] + qy;//hit coordinate along y (so along the strip)
                Flag = 1; //reset Hit Flag
				
				//Check if hit is valid
                if((((x<StripCoordinate(0))||(x>MaxLength))||((x>StripCoordinate(383))&&(x<StripCoordinate(384)))||((x>StripCoordinate(767))&&(x<StripCoordinate(768)))||((x>StripCoordinate(1151))&&(x<StripCoordinate(1152)))||(x>StripCoordinate(1535))||((yplaceholder<StripCoordinate(0))&&(yplaceholder>35)))){  
                    Flag = 0;
                }
            
            
                HittenStripPosition = *std::lower_bound(StripPosition.begin(), StripPosition.end(), x); //calculate hitten strip position
                
                //check if the strip is not close to a muted/inefficient/Dead one.
                for(auto Muted:MapMutedStrips[l]){
                    if(abs(StripCoordinate(Muted)- HittenStripPosition)<0.001){
                       Flag = 0;
                    }
                }
                
                //insert hits positions and keep account of mean layer efficiency if hit is valid.
                if(Flag ==1){
	    		    epsilon = epsilon*EfficiencyMap[l];
                    NxHit++;
                    TrackX.push_back(HittenStripPosition);
                    ZlevelX.push_back(l);
                }
            }


			if((NyHit<3)|(NxHit<3)){goto exit;}  // check if the trigger condition of 3 hits per vision is satisfied


			//Setup utils variables  for checking the trigger condition of consecutive hits on each vision.
            ZlevelYPlaceHolder = ZlevelY;
            ZlevelXPlaceHolder = ZlevelX;
            std::adjacent_difference(ZlevelXPlaceHolder.begin(), ZlevelXPlaceHolder.end(), ZlevelXPlaceHolder.begin());
	        std::adjacent_difference(ZlevelYPlaceHolder.begin(), ZlevelYPlaceHolder.end(), ZlevelYPlaceHolder.begin());

            
	
			//check if the consecutive hits trigger condition is satisfied on the vision YZ
	        for(unsigned int m = 1; m<ZlevelY.size(); m++){
		        if(ZlevelYPlaceHolder[m] != -1){goto exit;}
	        }
			//check if the consecutive hits trigger condition is satisfied on the vision XZ
            for(unsigned int m = 1; m<ZlevelX.size(); m++){
	    	    if(ZlevelXPlaceHolder[m] != -1){goto exit;} 
	        }
	



            //Fill map with the hits
            for(unsigned int jj = 0; jj< TrackX.size(); jj++){
                HitMap[ZlevelX[jj]]->Fill(TrackX[jj]);
            }
            //Fill map with the hits
            for(unsigned int jj = 0; jj< TrackY.size(); jj++){
                HitMap[ZlevelY[jj]]->Fill(TrackY[jj]);
            }


			//Fill Distributions.
            ThetaExpectedDistribution->Fill(theta, 1/epsilon);
            PhiExpectedDistribution->Fill(atan2(my,mx)*180/3.14, 1/epsilon);
            HitXDistribution->Fill(NxHit);
            HitYDistribution->Fill(NyHit);
            exit:
            ThetaTotalCount->Fill(theta);
            PhiTotalCount->Fill(phi);
        }
    }



	//Save Distributions in root files.
    TString output1 = path +"AzimuthGeneratedDistribution.root";
    TString output2 = path +"ZenithGeneratedDistribution.root";
    TString output3 = path +"ZenithExpectedDistribution.root";
    TString output4 = path +"AzimuthExpectedDistribution.root";

    PhiTotalCount->SetTitle("MC generated distribution of azimuth angle");
    PhiTotalCount->GetXaxis()->SetTitle("Azimuth angle #phi");
    PhiTotalCount->GetYaxis()->SetTitle("Counts");


    ThetaTotalCount->SetTitle("MC generated distribution of zenith angle");
    ThetaTotalCount->GetXaxis()->SetTitle("Zenith angle #theta");
    ThetaTotalCount->GetYaxis()->SetTitle("Counts");



    ThetaExpectedDistribution->SetTitle("MC simulation of expected measured Zenith angle distribution");
    ThetaExpectedDistribution->GetXaxis()->SetTitle("Zenith angle #theta");
    ThetaExpectedDistribution->GetYaxis()->SetTitle("Counts");



    PhiExpectedDistribution->SetTitle("MC simulation of expected measured azimuth angle distribution");
    PhiExpectedDistribution->GetXaxis()->SetTitle("Azimuth angle #phi");
    PhiExpectedDistribution->GetYaxis()->SetTitle("Counts");



    PhiTotalCount->SaveAs(output1);
    ThetaTotalCount->SaveAs(output2);
    ThetaExpectedDistribution->SaveAs(output3);
    PhiExpectedDistribution->SaveAs(output4);

    TString Nome;
    for(auto x : LayersID){
        HitMap[x]->SetBinContent(1, 0);
        TCanvas *cmap = new TCanvas();
        HitMap[x]->GetYaxis()->SetTitle("Number of hits");
        HitMap[x]->GetXaxis()->SetTitle("Strip Position");
        HitMap[x]->Draw();
        Nome = "SimulatedStripOccupancy" + to_string(x)+ ".pdf";
    }




}















