/**
 *  * \file MonteCarlo.cpp
 *	Questo file effettua una simulazione MC per l'apparato. Sono simulati il trigger, le strip mutate.
 *  Non è stato simulato il taglio effettuato sui cluster, lo smearing degli hit basato sui residui e la simulazione della lunghezza dei cluster.
 * 
 *
 * 
 *
 *
 */


#include"UtilsMC.h"


void UnitTestVoidSpaces(double y){
	double MaxLength = StripCoordinate(1536);

	std::vector<double> StripPosition;

 
	for(int i =0; i<1536; i++){
		//std::vector<double> StripEnds = {StripCoordinate(i), StripCoordinate(i+1)};
		StripPosition.push_back(StripCoordinate(i+1));
	}
	if((((y<0)||(y>MaxLength))||((y>StripCoordinate(384))&&(y<StripCoordinate(385)))||((y>StripCoordinate(768))&&(y<StripCoordinate(769)))||((y>StripCoordinate(1152))&&(y<StripCoordinate(1153)))||(y>StripCoordinate(1536)))){cout<<"Ho hittato uno spazio vuoto!Unit Test passato!La mappa non conta come hit gli spazi oltre la strip 1 e 1536, e gli spazi tra i vari ladder!"<<endl;}
    auto HittenStripPosition = *std::lower_bound(StripPosition.begin(), StripPosition.end(), y);
    cout<< "La pos è"<<HittenStripPosition <<endl;
}

void UnitTestOfmap(int Strip){
	
	std::vector<double> StripPosition;

 
	for(int i =0; i<1536; i++){
		StripPosition.push_back(StripCoordinate(i+1));
	}

		double y = StripCoordinate(Strip);
		double MaxLength = StripCoordinate(1536);
        if((((y<0)||(y>MaxLength))||((y>StripCoordinate(384))&&(y<StripCoordinate(385)))||((y>StripCoordinate(768))&&(y<StripCoordinate(769)))||((y>StripCoordinate(1152))&&(y<StripCoordinate(1153)))||(y>StripCoordinate(1536)))){cout<<"Ho hittato uno spazio vuoto!"<<endl;}
            auto HittenStripPosition = *std::lower_bound(StripPosition.begin(), StripPosition.end(), y);
            
        if(abs(y-HittenStripPosition)<0.001){cout<<"La mappa Funziona!, la strip inserita nella UT è nella pos "<<StripCoordinate(Strip) <<"cm, quella invece mappata è " << HittenStripPosition << " cm"<<endl;}

        
}







void MonteCarlo(const std::string fileinput, TString path){





    std::vector<double> StripPosition;
    for(int i =0; i<1535; i++){
        StripPosition.push_back(StripCoordinate(i));
    }




    //Creo la mappa dei vettori mutati, e poi creo la mappa delle strip mutate per layer.
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






    int ciclo = 0;
    int NHit = 0;
    
    FindMutedStrips(fileinput,NHit,ciclo,LayersID,MapLayerToID,MapMutedStrips);


    std::vector<TH1F*> HitCounts; // Vector containing the TH1F pointers of the histograms of hits for the Layers: those histograms have 1536 bins which is equal to the number of strips in each layers /
	std::map<int,TH1F*> HitMap; // Maps Layer's ID to its histogram pointer        /


    TString placename;
    for (int i = 0; i < 10; ++i) 
    {
        placename = "Hit Distribution Layer " + MapIDToLayer[LayersID[i]] + " 2 Maggio 2024";
    	TH1F *h = new TH1F("hist", placename, 1000, 0, StripCoordinate(1536));
	    HitCounts.push_back(h);
	    HitMap.insert(std::make_pair(LayersID[i], h));
    }







    int NBin = 1000;

    TH1F *ThetaTotalCount = new TH1F("hist", "Zenith angle Theta Generated Distribution", NBin, 0, 90);
    TH1F *ThetaExpectedDistribution = new TH1F("hist", "Zenith angle Theta Expected Distribution", NBin, 0, 90);
    TH1F *PhiExpectedDistribution = new TH1F("hist", "Azimuth angle Phi Expected Distribution", NBin, -180, 180);
    TH1F *PhiTotalCount = new TH1F("hist", "Azimuth angle Phi Generated Distribution", NBin, -180, 180);
    TH1F *HitXDistribution = new TH1F("hist", "Hits on Vision X Distribution", 6 , 0, 6);
    TH1F *HitYDistribution = new TH1F("hist", "Hits on Vision Y Distribution", 6 , 0, 6);

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
    double epsilon = 1;
    int Flag =1;
    double yplaceholder, xplaceholder;

    for(int j = 0; j<10; j++){
        for(int i = 0; i <1000000;i++){
    
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
                for(auto Muted:MapMutedStrips[l]){
                    if(abs(StripCoordinate(Muted) -HittenStripPosition)<0.001){
                        Flag = 0;
                    }
                }
                if(Flag ==1){
		            epsilon = epsilon*EfficiencyMap[l];
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
                }//qua sto richiedendo che il punto sia dentro il rivelatore e fuori dagli spazi tra i ladder.
            
            
                HittenStripPosition = *std::lower_bound(StripPosition.begin(), StripPosition.end(), x);

                for(auto Muted:MapMutedStrips[l]){
                    if(abs(StripCoordinate(Muted)- HittenStripPosition)<0.001){
                       Flag = 0;
                    }
                }
                if(Flag ==1){
	    		    epsilon = epsilon*EfficiencyMap[l];
                    NxHit++;
                    TrackX.push_back(HittenStripPosition);
                    ZlevelX.push_back(l);
                }
            }

            ZlevelYPlaceHolder = ZlevelY;
            ZlevelXPlaceHolder = ZlevelX;



            std::adjacent_difference(ZlevelXPlaceHolder.begin(), ZlevelXPlaceHolder.end(), ZlevelXPlaceHolder.begin());
	        std::adjacent_difference(ZlevelYPlaceHolder.begin(), ZlevelYPlaceHolder.end(), ZlevelYPlaceHolder.begin());

            if(NyHit<3){goto exit;}  // sto richiedendo almeno tre hit
	        if(NxHit<3){goto exit;}


	        for(unsigned int m = 1; m<ZlevelY.size(); m++){
		        if(ZlevelYPlaceHolder[m] != -1){goto exit;} //sostanzialmente il trigger non funziona: non ho hit consecutivi
	        }

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



            ThetaExpectedDistribution->Fill(theta, 1/epsilon);
            PhiExpectedDistribution->Fill(atan2(my,mx)*180/3.14, 1/epsilon);
            HitXDistribution->Fill(NxHit);
            HitYDistribution->Fill(NyHit);
            exit:
            ThetaTotalCount->Fill(theta);
            PhiTotalCount->Fill(phi);
        }
    }




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

//////////////////////////////////
//SEZIONE DI UNIT TESTING       //
//////////////////////////////////


// 1 Verifichiamo che la mappa funzioni
UnitTestOfmap(383);

UnitTestOfmap(384);

UnitTestOfmap(385);
 
// 2 Verifichiamo che le strip mutate vengano tolte
 
 
int provamutata = MapMutedStrips[10][0];
for(auto Muted:MapMutedStrips[10]){
               if(Muted == provamutata){
					cout<<"Success: il for sulle mappe mutate funziona!"<<endl;
                }
}
//2 Verifichiamo che gli spazi vuoti vengano colpiti 
UnitTestVoidSpaces((StripCoordinate(384) + StripCoordinate(385))/2.);
UnitTestVoidSpaces(-1);
UnitTestVoidSpaces(1000);
UnitTestVoidSpaces(9.1);
UnitTestVoidSpaces(9.05);


}















