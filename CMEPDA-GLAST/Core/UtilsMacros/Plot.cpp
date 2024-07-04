/**
 * \file Plot.cpp 
 * 
 * Simply plots separately two histograms given their files .root
 *
 */



void Plot(TString f1, TString f2){

    TFile *One = new TFile(f1);
    TFile *Two = new TFile(f2);
    TH1F *firsthist =(TH1F*)One->Get("hist");
	TH1F *secondhist = (TH1F*)Two->Get("hist");



    TCanvas *c1 = new TCanvas();
    firsthist->Draw();



    TCanvas *c2 = new TCanvas();
    secondhist->Draw();











}