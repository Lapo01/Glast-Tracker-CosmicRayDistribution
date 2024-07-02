/**
 *  * \file MonteCarloGenerazioneDati.cpp
 *	MC generator for distribution on zenith and azimuth angle. The only request is to have at least three hits per vision, in order to generate a population that has 1 track per vision.
 *  The generated simulation is used for testing the tracking algorithm retina. 
 *  We do not care about the realism of the distribution as the goal is to validate the reconstruction of the algorithm for 3D tracks. One suggestion for the future may be to easily validate 
 *  the recontruction for multi tracking easily including noise, cluster dimension distribution, residuals smearing and etc,  but that esulates from the goal of this project.
 * 
 *
 *
 */


#include "UtilsMC.h"








void MonteCarloGenerazioneDati(TString fileoutput, TString path){





	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 														SECTION 1:												  //
	// In this section we initialize all variables used in the code, the tree to be outputted is initialized.		  //
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    std::vector<double> StripPosition; //vector used to map hit to the activated strip position.
    for(int i =0; i<1535; i++){
        StripPosition.push_back(StripCoordinate(i));
    }







    int NBin = 1000; //set number of bins for histograms

	//Initialize containers for the distributions that are expected after the track passes the trigger condition of the experimental apparatus
    TH1F *ThetaExpectedDistribution = new TH1F("hist", "Zenith angle Theta Expected Distribution", NBin, 0, 90);
    TH1F *PhiExpectedDistribution = new TH1F("hist", "Azimuth angle Phi Expected Distribution", NBin, -180, 180);


	//Setup tree to be read 
    Evento e;
    TFile *output = new TFile(fileoutput, "recreate");
    TTree *tree = new TTree("tree", "tree");
    tree->Branch("evento",&e);

	//utils variables
    int NxHit =0; //number of hits on the XZ vision, on each iteration the code checks if the hits are more than 3. 
    int NyHit =0; //number of hits on the YZ vision, on each iteration the code checks if the hits are more than 3. 
    double mx, my, qx, qy, theta, phi, x0, y0, x, y,thetacalc, phicalc, HittenStripPosition, auxiliary; //utils variables for MC data generation and for hitten strip.
    double Ztop = 31.2; //height of the highest layer between the XZ and YZ vision.
    double MaxLength = StripCoordinate(1536); //The maximum length at which a layer is sensible to a hit 
    
    
    
    
    std::vector<double> TrackX; //placeholder container for position of hitten strips on the XZ vision
    std::vector<double> TrackY //placeholder container for position of hitten strips on the YZ vision
    std::vector<int> ZlevelX;//placeholder container for position of hitten layers on the XZ vision
    std::vector<int> ZlevelY;//placeholder container for position of hitten layers on the YZ vision
    std::vector<int> ZlevelXPlaceHolder;//utils variable to check if the trigger condition of consecutive hits is satisfied. In this code is not used, however since the function ResetUtils from library UtilsMC require this input, to mantain compatibility we declare it
    std::vector<int> ZlevelYPlaceHolder;//utils variable to check if the trigger condition of consecutive hits is satisfied. In this code is not used, however since the function ResetUtils from library UtilsMC require this input, to mantain compatibility we declare it
    int Flag =1; //Flag used to check if a
    double epsilon = 1; // utils varible to keep count of efficiency. In this code is not used, however since the function ResetUtils from library UtilsMC require this input, to mantain compatibility we declare it
    double yplaceholder, xplaceholder; //utils variables used to check at which point a strip has been hitten.
    int N = 0; //util variable to keep count of generated events that passes the condition of having 1 track per vision.
    
    
    
    
    
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 														SECTION 2:												 					  //
	// In this section we generate the distribution of events firstly utils variables and the object evento filled in the tree are reset  //
	// Next four random numbers are generated: theta, phi and the first point (x0, y0) on the plane of the highest layer, from this 	  //
	// numbers all hits are generated for each layer, if there are more than 3 hits for each vision, the tree is filled.				  //
	// 																																	  //
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

    //Init the MC simulation
    gRandom = new TRandom3(0); //set random seed.

    for(int j = 0; j<10; j++){
        for(int i = 0; i <100000;i++){
    

            ResetEvento(e); //reset object evento to be filled.

            ResetUtils(TrackX, TrackY, ZlevelX, ZlevelY, ZlevelXPlaceHolder, ZlevelYPlaceHolder, NxHit, NyHit, epsilon); //reset utils

            GenerateRandomNumbers(auxiliary, theta, phi, x0, y0, mx, my, qx, qy, thetacalc, phicalc, Ztop);

			//for each Y layer check if a hit has been registered
            for(auto l:YLayer){
				//calculate hit point coordinate
                y = my * Zmap[l] + qy ; 
                xplaceholder = mx * Zmap[l] + qx;
                Flag =1; /reset Flag
                
                //check if a hit is made, if not turn the flag to 0.
                if((((y<StripCoordinate(0))||(y>MaxLength))||((y>StripCoordinate(383))&&(y<StripCoordinate(384)))||((y>StripCoordinate(767))&&(y<StripCoordinate(768)))||((y>StripCoordinate(1151))&&(y<StripCoordinate(1152)))||(y>StripCoordinate(1535))||((xplaceholder<StripCoordinate(0))||(xplaceholder>35)))){
                    Flag = 0;
                }
                
                HittenStripPosition = *std::lower_bound(StripPosition.begin(), StripPosition.end(), y); //calculate the strip position in cm.
                
                //if flag is turned on, register the hit 
                if(Flag ==1){
		            NyHit++; //increase number of hits
		            ZlevelY.push_back(l); //pushback layer height
		            TrackY.push_back(HittenStripPosition); //Push back strip position
                }
            }
            
			//for each X layer check if a hit has been registered

            for(auto l:XLayer){
				//calculate hit point coordinate
                x = mx * Zmap[l] + qx ;
                yplaceholder = my * Zmap[l] + qy;
                Flag = 1;
                //check if a hit is made, if not turn the flag to 0.
                if((((x<StripCoordinate(0))||(x>MaxLength))||((x>StripCoordinate(383))&&(x<StripCoordinate(384)))||((x>StripCoordinate(767))&&(x<StripCoordinate(768)))||((x>StripCoordinate(1151))&&(x<StripCoordinate(1152)))||(x>StripCoordinate(1535))||((yplaceholder<StripCoordinate(0))&&(yplaceholder>35)))){  
                    Flag = 0;
                }
                HittenStripPosition = *std::lower_bound(StripPosition.begin(), StripPosition.end(), x); //calculate the hitten strip position in cm.
                if(Flag ==1){
                    NxHit++;//increase number of hits
                    TrackX.push_back(HittenStripPosition); //pushback layer height
                    ZlevelX.push_back(l); //Push back strip position
                }
            }

          
			//if the event does not satisfy the condition of more than 2 hits per vision, go to next iteration.
            if(NyHit<3)||NxHit<3){
				goto exit;
			}  


	       
	

			//fill data for XZ projection

	        for(unsigned int jj = 0; jj<TrackX.size(); jj++){
			    	e.Layer.push_back(ZlevelX[jj]);//push back the position in Z of the hitten layer.
			    	e.ClusterDimension.push_back(1);// all cluster have dimension one
			    	e.InitialStrip.push_back(1); //we insert a placeholder value, since all generated cluster have dimension 1 this info is not needed.
			    	e.ClusterPosizione.push_back(TrackX[jj]); //push back the position of the cluster, which is equal to the position of the hitten strip.
				    e.ClusterLayer.push_back(ZlevelX[jj]);//push back the position in Z of the hitten layer.
				    e.Strip.push_back(1);// we insert a placeholder value as this info is not needed. 
	        }
			//fill data for YZ projection
	        for(unsigned int jj = 0; jj<TrackY.size(); jj++){
			    	e.Layer.push_back(ZlevelY[jj]);//push back the position in Z of the hitten layer.
			    	e.ClusterDimension.push_back(1); // all cluster have dimension one
			    	e.InitialStrip.push_back(1); //we insert a placeholder value, since all generated cluster have dimension 1 this info is not needed.
			    	e.ClusterPosizione.push_back(TrackY[jj]); //push back the position of the cluster, which is equal to the position of the hitten strip.
				    e.ClusterLayer.push_back(ZlevelY[jj]); //push back the position in Z of the hitten layer 
				    e.Strip.push_back(1);// we insert a placeholder value as this info is not needed. 
	        }
	        
	        /Fill other info on the event.
            e.NEventi = j+i;
            e.NHit = e.Strip.size();
            e.Flags[0] =1; //just to render the file analyzable to retina. Retina requires this flag to be turned on.
            tree->Fill(); //fill the tree
            N++; //keep count of valid events

			//fill angle distributions of event that are inserted into the tree
            ThetaExpectedDistribution->Fill(theta);
            PhiExpectedDistribution->Fill(atan2(my,mx)*180/3.14);


            exit:; //exit in case the event do not satisfy the request to have more than 2 hits per vision
        }
    }

    output->Write();   //write output file
	output->Close();   //close output file


    std::cout<<"Sono stati generati " <<N <<" eventi con 1 proiezione per traccia"<< std::endl;


	//Save angle distributions of event that are inserted into the tree, those are later to be compared with the reconstructed one by the tracking algorithm.
    TString output1 = path + "AzimuthTest.root";
    TString output2 = path + "ZenithTest.root";
    PhiExpectedDistribution->SaveAs(output1);
    ThetaExpectedDistribution->SaveAs(output2);
}















