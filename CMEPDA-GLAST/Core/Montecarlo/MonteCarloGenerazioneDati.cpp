/**
 *  * \file MonteCarloGenerazioneDati.cpp
 *	MC generator for distribution on zenith and azimuth angle. The only request is to have at least three hits per vision, in order to generate a population that has 1 track per vision.
 *  The generated simulation is used for testing the algorithm retina. 
 *  We do not care about the realism of the distribution as the goal is to validate the reconstruction of the algorithm for 3D tracks. One suggestion for the future may be to easily validate 
 *  the recontruction for multi tracking, but that esulates from the goal of this project.
 * 
 *
 *
 */


#include "UtilsMC.h"








void MonteCarloGenerazioneDati(TString fileoutput, TString path){










    std::vector<double> StripPosition;
    for(int i =0; i<1535; i++){
        StripPosition.push_back(StripCoordinate(i));
    }







    int NBin = 1000;


    TH1F *ThetaExpectedDistribution = new TH1F("hist", "Zenith angle Theta Expected Distribution", NBin, 0, 90);
    TH1F *PhiExpectedDistribution = new TH1F("hist", "Azimuth angle Phi Expected Distribution", NBin, -180, 180);



    Evento e;
    TFile *output = new TFile(fileoutput, "recreate");
    TTree *tree = new TTree("tree", "tree");
    tree->Branch("evento",&e);


    int NxHit =0;
    int NyHit =0;
    double mx, my, qx, qy, theta, phi, x0, y0, x, y,thetacalc, phicalc, HittenStripPosition, auxiliary;
    double Ztop = 31.2; //è l'altezza del layer più alto (in cm). 31.2
    double MaxLength = StripCoordinate(1536);
    std::vector<double> TrackX;
    std::vector<double> TrackY;
    std::vector<int> ZlevelX;
    std::vector<int> ZlevelY;
    std::vector<int> ZlevelXPlaceHolder;
    std::vector<int> ZlevelYPlaceHolder;
    std::vector<int> DiffX;
    std::vector<int> DiffY;
    int Flag =1;
    double epsilon = 1;
    double yplaceholder, xplaceholder;
    int N = 0;
    for(int j = 0; j<10; j++){
        std::cout<<j<<std::endl;
        for(int i = 0; i <100000;i++){
    

            ResetEvento(e);

            ResetUtils(TrackX, TrackY, ZlevelX, ZlevelY, ZlevelXPlaceHolder, ZlevelYPlaceHolder, NxHit, NyHit, epsilon);

            GenerateRandomNumbers(auxiliary, theta, phi, x0, y0, mx, my, qx, qy, thetacalc, phicalc, Ztop);


            for(auto l:YLayer){
                y = my * Zmap[l] + qy ;
                xplaceholder = mx * Zmap[l] + qx;
                Flag =1;
                if((((y<StripCoordinate(0))||(y>MaxLength))||((y>StripCoordinate(383))&&(y<StripCoordinate(384)))||((y>StripCoordinate(767))&&(y<StripCoordinate(768)))||((y>StripCoordinate(1151))&&(y<StripCoordinate(1152)))||(y>StripCoordinate(1535))||((xplaceholder<StripCoordinate(0))||(xplaceholder>35)))){
                    Flag = 0;
                }
                HittenStripPosition = *std::lower_bound(StripPosition.begin(), StripPosition.end(), y);
                if(Flag ==1){
		            NyHit++;
		            ZlevelY.push_back(l);
		            TrackY.push_back(HittenStripPosition);
                }
            }

            for(auto l:XLayer){
                x = mx * Zmap[l] + qx ;
                yplaceholder = my * Zmap[l] + qy;
                Flag = 1;

                if((((x<StripCoordinate(0))||(x>MaxLength))||((x>StripCoordinate(383))&&(x<StripCoordinate(384)))||((x>StripCoordinate(767))&&(x<StripCoordinate(768)))||((x>StripCoordinate(1151))&&(x<StripCoordinate(1152)))||(x>StripCoordinate(1535))||((yplaceholder<StripCoordinate(0))&&(yplaceholder>35)))){  
                    Flag = 0;
                }
                HittenStripPosition = *std::lower_bound(StripPosition.begin(), StripPosition.end(), x);
                if(Flag ==1){
                    NxHit++;
                    TrackX.push_back(HittenStripPosition);
                    ZlevelX.push_back(l);
                }
            }

          

            if(NyHit<3){goto exit;}  // sto richiedendo almeno tre hit
	        if(NxHit<3){goto exit;}


	       
	


	        for(unsigned int jj = 0; jj<TrackX.size(); jj++){
			    	e.Layer.push_back(ZlevelX[jj]);
			    	e.ClusterDimension.push_back(1);
			    	e.InitialStrip.push_back(1);
			    	e.ClusterPosizione.push_back(TrackX[jj]);
				    e.ClusterLayer.push_back(ZlevelX[jj]);
				    e.Strip.push_back(1);
	        }

	        for(unsigned int jj = 0; jj<TrackY.size(); jj++){
			    	e.Layer.push_back(ZlevelY[jj]);
			    	e.ClusterDimension.push_back(1); // all cluster have dimension one
			    	e.InitialStrip.push_back(1); //we insert a placeholder value, since all generated cluster have dimension 1 this info is not needed.
			    	e.ClusterPosizione.push_back(TrackY[jj]); 
				    e.ClusterLayer.push_back(ZlevelY[jj]);
				    e.Strip.push_back(1);
	        }
            e.NEventi = j+i;
            e.NHit = e.Strip.size();
            e.Flags[0] =1; //just to render the file analyzable to retina.
            tree->Fill();
            N++;


            ThetaExpectedDistribution->Fill(theta);
            PhiExpectedDistribution->Fill(atan2(my,mx)*180/3.14);


            exit:;
        }
    }

    output->Write();   //write output file
	output->Close();   //close output file


    std::cout<<"Sono stati generati " <<N <<" eventi con 1 proiezione per traccia"<< std::endl;

    TString output1 = path + "AzimuthTest.root";
    TString output2 = path + "ZenithTest.root";
    PhiExpectedDistribution->SaveAs(output1);
    ThetaExpectedDistribution->SaveAs(output2);
}















