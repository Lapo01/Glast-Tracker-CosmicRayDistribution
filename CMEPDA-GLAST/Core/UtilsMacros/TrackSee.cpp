#include"GeneralUtils.h"




TGraphErrors *TrackXZ(EventoTrack &e){
    TGraphErrors *XZ =new TGraphErrors();
    int n;
    TString Title = "Track XZ  Event ID: "+ to_string(e.NEvento) +  "; X [cm]; Y [cm] " ;
    XZ->SetTitle(Title);



    for(auto track:e.TrackX){
        for(int i= 0; i<track.ClusterPosition.size(); i++){
            n = XZ->GetN();
            XZ->SetPoint(n,LayerCoordinate(track.Layer[i]), track.ClusterPosition[i] );
            XZ->SetPointError(n, 0,track.Error[i]);
        }
    }

    return XZ;
}
TGraphErrors *TrackYZ(EventoTrack &e){
    TGraphErrors *YZ =new TGraphErrors();
    int n;

    TString Title = "Track YZ Event ID: " + to_string(e.NEvento) + "; Y [cm]; Y [cm] ";

    YZ->SetTitle(Title);
    for(auto track:e.TrackY){
        for(int i= 0; i<track.ClusterPosition.size(); i++){
            n = YZ->GetN();
            YZ->SetPoint(n,LayerCoordinate(track.Layer[i]), track.ClusterPosition[i]);
            YZ->SetPointError(n, 0, track.Error[i]);
        }
        
    }


    return YZ;

}



void CheckLength(std::vector<int> &a){
    if(a.size() ==0){
        throw std::exception();
    }


}










void TrackSee(TString fileinput, int N, int M){

    gRandom = new TRandom3(0);
    EventoTrack e; 
	EventoTrack *p = &e;
	TFile *input = new TFile(fileinput,"read");
	TTree *tree = (TTree*)input->Get("treetrack");
	tree->SetBranchAddress("etrack", &p); 
	int entries = tree->GetEntries();
    std::vector<int> GoodEntries;
    for(int i = 0; i<entries; i++){
        tree->GetEntry(i);
        if((e.TrackX.size() == N) &(e.TrackY.size() == M)){
                GoodEntries.push_back(i);
        }
    }

    try {
        CheckLength(GoodEntries);

    }

    catch(std::exception &except){
        std::cout<<"Non esistono eventi con questo numero di proiezione per visione!"<<std::endl;
        exit(EXIT_FAILURE);
    }
    int RandomPick = int(gRandom->Uniform(0, GoodEntries.size()));

    std::cout<<GoodEntries[RandomPick]<<std::endl;
    tree->GetEntry(GoodEntries[RandomPick]);


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