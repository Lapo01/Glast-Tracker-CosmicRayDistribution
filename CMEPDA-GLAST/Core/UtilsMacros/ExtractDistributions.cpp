/**
 * \file ExtractDistributions.cpp 
 * 
 * This code takes takes as input a .root file containing the second abstraction of trackin data and outputs the measured zenith and azimuth distributions. Also returns the number of Nx projection vs Ny projections.
 *
 */

#include "GeneralUtils.h"


/**
 * This macro takes as input two filepaths, the first is the path of the .root file that contains data in form of tracks. The second is a path where to print the root files containing the distributions.
 *  
 * @param fileoutput path to the File that contains the tree with tracks
 * 
 * @param path path for the output .root files that contains the distributions.
 * 
 * 
 */
void ExtractDistributions(TString fileinput, TString path){

    //initialize tree to be read
    EventoTrack e; 
	EventoTrack *p = &e;
	TFile *input = new TFile(fileinput,"read");
	TTree *tree = (TTree*)input->Get("treetrack");
	tree->SetBranchAddress("etrack", &p); 
	int entries = tree->GetEntries();


    //initialize histograms to be filled 
    TH1F *MeasuredZenithDistribution = new TH1F("hist", "Distribuzione ricostruita dell'angolo zenith", 1000, 0, 90);
    TH1F *MeasuredAzimuthDistribution = new TH1F("hist", "Distribuzione ricostruita dell'angolo azimuth", 1000, -180, 180);
    TH2F *TrackXvsTrackYNumbers = new TH2F("hist2d","Tracce trovate X e Y", 10, 0,10, 10, 0, 10);


    //fill histograms and save them into root files
    for(int i = 0; i<entries; i++){
        tree->GetEntry(i);
        if((e.Nx()==1)&&(e.Ny()==1)){
            MeasuredZenithDistribution->Fill(Theta(e, 0));
            MeasuredAzimuthDistribution->Fill(Phi(e, 0));
        }
        TrackXvsTrackYNumbers->Fill(e.Nx(), e.Ny());
    }
    TString output1 = path + "ZenithDistribution.root";
    TString output2 = path + "AzimuthDistribution.root";
    TString output3 = path + "NXTracksvsNYTracks.root";





    MeasuredZenithDistribution->SetTitle("Reconstructed Zenith Distribution");
    MeasuredZenithDistribution->GetXaxis()->SetTitle("Zenith angle #theta");
    MeasuredZenithDistribution->GetYaxis()->SetTitle("Counts");

    MeasuredAzimuthDistribution->SetTitle("Reconstructed Azimuth Distribution");
    MeasuredAzimuthDistribution->GetXaxis()->SetTitle("Azimuth angle #phi");
    MeasuredAzimuthDistribution->GetYaxis()->SetTitle("Counts");

    TrackXvsTrackYNumbers->SetTitle("Number of projections distributions");
    TrackXvsTrackYNumbers->GetYaxis()->SetTitle("Number of projection on the XZ vision");
    TrackXvsTrackYNumbers->GetYaxis()->SetTitle("Number of projection on the YZ vision");


    TCanvas *c = new TCanvas();
    TrackXvsTrackYNumbers->Draw("colz TEXT");

    //save hist
    MeasuredZenithDistribution->SaveAs(output1);
    MeasuredAzimuthDistribution->SaveAs(output2);
    TrackXvsTrackYNumbers->SaveAs(output3);
}
