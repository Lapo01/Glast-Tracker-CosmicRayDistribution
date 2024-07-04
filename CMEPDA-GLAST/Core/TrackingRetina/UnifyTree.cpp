/**
 * \file UnifyTree.cpp 
 * 
 * This code takes the produced file .root from each core and from each file takes the tree inside and unify them into a unique tree that is put in a file .root, this file is the final output of the tracking abstraction phase
 *
 */

#include<iostream>
#include<vector>
#include<sstream>

/**
 * 
 *
 * 
 * @param iterations it essentialy is the number of cores, this name is included in the produced files from the macro Retina which is run on all cores of the pc.
 *
 * @param savefile util variable containing name and path of the file to be produced
 */
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
