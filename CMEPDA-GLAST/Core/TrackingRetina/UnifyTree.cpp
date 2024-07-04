#include<iostream>
#include<vector>
#include<sstream>


void UnifyTree(int iterations, TString savefile){
	
	TList * list = new TList();
	for(int i = 0; i< iterations; i++){
		TString i_name = to_string(i);
		TString filename = savefile + i_name + ".root" ;
		TFile *file = new TFile(filename, "read");
		EventoTrack etrack;
		EventoTrack *e = &etrack;
		TTree *tree = (TTree*)file->Get("treetrack");
		tree->SetBranchAddress("etrack", &e); 
		list->Add(tree);		
	}

	EventoTrack etrack;
	TString savefilename = savefile + ".root";
	TFile *output = new TFile(savefilename,"recreate");
	TTree *newtree = TTree::MergeTrees(list);
	newtree->SetName("treetrack");
	newtree->Branch("etrack",&etrack);
	newtree->Write();
	output->Write();
	output->Close();
	
	
	
	
	
	
	

}
