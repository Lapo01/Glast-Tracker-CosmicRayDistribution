#include"GeneralUtils.h"

/**
 * \file TrackSee.cpp 
 * 
 * This file contains functions to display a random event with N projections on the XZ vision and M projections on the YZ vision. .
 *
 */




/**
 * This function returns TGraphErrors containers with the points in the tracks for the XZ vision.
 *  
 * @param e The chosen event to display
 *
 * 
 */
TGraphErrors *TrackXZ(EventoTrack &e){
    TGraphErrors *XZ =new TGraphErrors(); //initialize TGraphErrors
    int n; //initialize util variable to fill TGraphErrors instance
    TString Title = "Track XZ  Event ID: "+ to_string(e.NEvento) +  "; X [cm]; Y [cm] " ; //TGraphErrors title.
    XZ->SetTitle(Title); 


    //for all XZ projections fill the TGraphErrors with their cluster info.
    for(auto track:e.TrackX){
        for(int i= 0; i<track.ClusterPosition.size(); i++){
            n = XZ->GetN();
            XZ->SetPoint(n,LayerCoordinate(track.Layer[i]), track.ClusterPosition[i] );
            XZ->SetPointError(n, 0,track.Error[i]);
        }
    }

    return XZ;
}



/**
 * This function returns TGraphErrors containers with the points in the tracks for the YZ vision.
 *  
 * @param e The chosen event to display
 *
 * 
 */
TGraphErrors *TrackYZ(EventoTrack &e){
    TGraphErrors *YZ =new TGraphErrors();//initialize TGraphErrors
    int n;//initialize util variable to fill TGraphErrors instance
    TString Title = "Track YZ Event ID: " + to_string(e.NEvento) + "; Y [cm]; Y [cm] ";//TGraphErrors title.
    YZ->SetTitle(Title);

    //for all YZ projections fill the TGraphErrors with their cluster info.

    for(auto track:e.TrackY){
        for(int i= 0; i<track.ClusterPosition.size(); i++){
            n = YZ->GetN();
            YZ->SetPoint(n,LayerCoordinate(track.Layer[i]), track.ClusterPosition[i]);
            YZ->SetPointError(n, 0, track.Error[i]);
        }
        
    }


    return YZ;

}


/**
 * This function checks if the vector containing the ID of the entries with N and M projection for XZ and YZ vision has size 0.
 *
 *  
 * @param a vector containing the number ID of the entries with N and M projection for XZ and YZ vision
 *
 * 
 */
void CheckLength(std::vector<int> &a){
    if(a.size() ==0){
        throw std::exception();
    }


}










void TrackSee(TString fileinput, int N, int M){



    gRandom = new TRandom(0); //make new pseudorandom seed


    //initialize tree to be read
    EventoTrack e; 
	EventoTrack *p = &e;
	TFile *input = new TFile(fileinput,"read");
	TTree *tree = (TTree*)input->Get("treetrack");
	tree->SetBranchAddress("etrack", &p); 
	int entries = tree->GetEntries();
    std::vector<int> GoodEntries; //vector of entry ID's

    //push back to the GoodEntries vector the entry ID that satisfy the condition of having N and M projections respectively on the vision XZ and YZ
    for(int i = 0; i<entries; i++){
        tree->GetEntry(i);
        if((e.TrackX.size() == N) &(e.TrackY.size() == M)){
                GoodEntries.push_back(i);
        }
    }
    


    //see if there are any event with the number N and M of projections.
    try {
        CheckLength(GoodEntries);

    }
    //catch the exception
    catch(std::exception &except){
        std::cout<<"Non esistono eventi con questo numero di proiezione per visione!"<<std::endl;
        exit(EXIT_FAILURE);
        delete except;
    }
    //pick a random entry
    int RandomPick = int(gRandom->Uniform(0, GoodEntries.size()));

    std::cout<<GoodEntries[RandomPick]<<std::endl;
    tree->GetEntry(GoodEntries[RandomPick]);




    //plot the events with their tracks.
    TCanvas *XZVision = new TCanvas();
    TGraphErrors *XZ =TrackXZ(e);
    XZ->Draw("*A");
    for(auto track:e.TrackX){
        TF1 *line = new TF1("line", "[0] + [1]*x", -3, 40);
        line->SetParameter(0, track.Fit[0]);
		line->SetParameter(1, track.Fit[1]);
        line->Draw("same");
    }
    TCanvas *YZVision = new TCanvas();
    TGraphErrors *YZ =TrackYZ(e);
    YZ->Draw("*A");
     for(auto track:e.TrackY){
        TF1 *line = new TF1("line", "[0] + [1]*x", -3, 40);
        line->SetParameter(0, track.Fit[0]);
		line->SetParameter(1, track.Fit[1]);
        line->Draw("same");
    }


}