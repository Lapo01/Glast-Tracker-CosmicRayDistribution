/**
 *  * \file PlotClusterResults.cpp
 *	This cpp macro prints the distribution of the number of clusters in an event, the distribution of the dimensions of clusters and the distribution of the positions of clusters in the layer X2.
 *
 * 
 *
 *
 */

#include "GeneralUtils.h"
#include "UtilsCreateTree.h"





void PlotClusterResults(TString fileinput){
    


    //////////////////////////////////////
    //   Load tree
    /////////////////////////////////////
    Evento e;
	Evento *p = &e;
	TFile *input = new TFile(fileinput, "read");
	TTree *tree = (TTree*)input->Get("tree");
    tree->SetBranchAddress("evento", &p); 
    int entries = tree->GetEntries();




    //////////////////////////////////////
    //   Define Histograms
    /////////////////////////////////////
    TH1F *PositionDistribution = new TH1F("hist", "Cluster Position Distribution of Layer X2", 2000, 0, 35);

    TH1F *ClusterDim = new TH1F("hist", "Cluster Dimension Distribution", 100, 0, 100);

    TH1F *ClusterNumber = new TH1F("hist", "Cluster Number Distribution", 100,0, 100);


    //////////////////////////////////////
    //   Fill Histograms
    /////////////////////////////////////
    //for all events
    for(int i = 0; i<entries; i++){

        tree->GetEntry(i);//get i-th entry
        ClusterNumber->Fill(e.ClusterPosizione.size()); //fill the distribution of the number of clusters with the number of clusters in the event.


        //for all clusters of the layer 12... fill the position distribution histogram with their positions.
        for(int j = 0; j< e.ClusterPosizione.size(); j++){
            if(e.ClusterLayer[j] == 12){
                PositionDistribution->Fill(e.ClusterPosizione[j]);
            }
        }

        //for all clusters fill the dimension distribution with the clusters' dimensions.
        for(auto x:e.ClusterDimension){
            ClusterDim->Fill(x);
        }

    }

    //////////////////////////////////////
    //   Plot histograms
    /////////////////////////////////////
    PositionDistribution->GetXaxis()->SetTitle("Position [cm]");
    PositionDistribution->GetYaxis()->SetTitle("Counts");
    TCanvas *c1 = new TCanvas();
    PositionDistribution->Draw("HISTSame");


    ClusterDim->GetXaxis()->SetTitle("Number of Hits");
    ClusterDim->GetYaxis()->SetTitle("Counts");
    TCanvas *c2 = new TCanvas();
    c2->SetLogy();
    ClusterDim->Draw("HISTSame");

    ClusterNumber->GetXaxis()->SetTitle("Number of Clusters");
    ClusterNumber->GetYaxis()->SetTitle("Counts");
    TCanvas *c3 = new TCanvas();
    c3->SetLogy();
    ClusterNumber->Draw("HISTSame");

}