/**
 * \file CompareDistributions.cpp 
 * 
 * This code takes as input two strings and compares them with a Kolmogorv Test, after that the distribution are each normalized and a pdf is produced with both. It returns a p-value.
 *
 */



/**
 * This macro takes as input two root files containing hists, compares the distributions with a Kolmogorov Test and returns a plot with both the normalized distributions. It returns a p-value.
 *  
 * @param f1 File containing the first distribution
 * 
 * @param f2 File containing the second distribution
 * 
 * @param path Path where to print the pdf in which the two distributions are compared.
 * 
 */
double CompareDistributions(TString f1, TString f2, TString path){


    //get histograms from .root files
    TFile *One = new TFile(f1);
    TFile *Two = new TFile(f2);
    TH1F *firsthist =(TH1F*)One->Get("hist");
	TH1F *secondhist = (TH1F*)Two->Get("hist");



    //normalize histograms
    double NormOne = 0;
    double NormTwo = 0;
    for(int i = 0; i<1000; i++){
		NormOne+= firsthist->GetBinContent(i);
		NormTwo+= secondhist->GetBinContent(i);
        if(i>10000){
            firsthist->SetBinContent(i, 0);
            secondhist->SetBinContent(i, 0);


        }
	}

    firsthist->Scale(1/NormOne);
	secondhist->Scale(1/NormTwo);

    double test = secondhist->KolmogorovTest(firsthist);

    //plot distributions and run Kolmogorov Test
    TCanvas *c = new TCanvas();


    firsthist->Rebin(10);
    secondhist->Rebin(10);
    TLegend *l = new TLegend();
    l->AddEntry(firsthist, firsthist->GetTitle());
    l->AddEntry(secondhist, secondhist->GetTitle());
    firsthist->SetLineColor(kRed);
    firsthist->Draw("HISTSame");
    secondhist->Draw("HISTSame");
    l->Draw("same");

    TPaveText *pt = new TPaveText();
    TString text = "Kolmogorov Test returns p-value of " + std::to_string(secondhist->KolmogorovTest(firsthist));
    pt->AddText(text);
    pt->Draw("same");
    c->SaveAs(path);

    


    return test;

};

