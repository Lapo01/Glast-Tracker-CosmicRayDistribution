



double CompareDistributions(TString f1, TString f2, TString path){
    TFile *One = new TFile(f1);
    TFile *Two = new TFile(f2);
    TH1F *firsthist =(TH1F*)One->Get("hist");
	TH1F *secondhist = (TH1F*)Two->Get("hist");

    double NormOne = 0;
    double NormTwo = 0;
    for(int i = 0; i<1000; i++){
		NormOne+= firsthist->GetBinContent(i);
		NormTwo+= secondhist->GetBinContent(i);
		}

    firsthist->Scale(1/NormOne);
	secondhist->Scale(1/NormTwo);

    TCanvas *c = new TCanvas();


    firsthist->Rebin(40);
    secondhist->Rebin(40);
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




    return secondhist->KolmogorovTest(firsthist);

};

