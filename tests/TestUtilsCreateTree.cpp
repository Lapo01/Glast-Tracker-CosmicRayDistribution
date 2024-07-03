#include "UtilsRetina.h"
#include "UtilsRectifyTracks.h"
#include "GeneralUtils.h"
#include "UtilsMC.h"
#include "UtilsCreateTree.h"
#include<cassert>
#include"TSystem.h"

/**
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 */



void TestFillStripLayer(){
	Evento e;
	e.Layer = {10, 10, 10, 10};
	e.Strip = {100, 101, 102, 103};
	e.NHit = e.Layer.size();
	std::vector<int> StripLayer;
	std::map<int,int> MapHitPerPiano;
	int Layer = 10;
	int Hits = 0;
	MapHitPerPiano.insert(std::make_pair(Layer, Hits));
	
	FillStripLayer(e, StripLayer, MapHitPerPiano, Layer);
	
	if(StripLayer.size() != e.Layer.size()){
		throw std::exception();
		
	}
	
	
};

/**
 * 
 * This macro tests the reconstruction of a small sample of data for the CreateTree macro, 
 * 
 * 
 * 
 * @param fileinput Data in form of the first kind of abstraction: clustering.
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 */

void TestSample(TString fileinput){
	gSystem->AddDynamicPath("SharedLibs");
    gInterpreter->AddIncludePath("Headers");
    gSystem->Load("libUtilsRectifyTracks.so");
    gSystem->Load("libGeneralUtils.so");
    gSystem->Load("libUtilsCreateTree.so");
    gSystem->Load("libUtilsRetina.so");
    gSystem->Load("libUtilsMC.so");
	TString execline = ".L CreateTree/CreateTree.cpp";

	gROOT->ProcessLine(execline);
	gSystem->cd("..");
	gSystem->cd("Data/Datalif");

	execline = "CreateTree("+ fileinput +", \"TestSample.root\")";
	gROOT->ProcessLine(execline);

	Evento e;
	Evento *p = &e;
	TFile *input = new TFile("TestSample.root", "read");
	TTree *tree = (TTree*)input->Get("tree");

	tree->SetBranchAddress("evento", &p); 
	int entries = tree->GetEntries();
	
		
	for(int i = 0; i<entries; i++){
		tree->GetEntry(i);
			
		int NumberOfHits = 0;
		for(auto ClusterDim:e.ClusterDimension){
				NumberOfHits+= ClusterDim;
		}
	
		if(e.NHit != NumberOfHits){
			throw std::exception();
		}
		if(e.NHit != e.Strip.size()){
			throw std::exception();	
		}
	};
};

int main(){
	
	try{

		TestFillStripLayer();

	}
	catch(std::exception *e){
		std::cout<<"Attenzione! Errore nella funzione FillStripLayer: Il vettore StripLayer ed e.Layer non hanno la stessa dimensione."<<std::endl;
		abort();
		delete e;
	}



	try{
		TestSample("\"TkrDataTaking_333000379.lif\"");
	}
	catch(std::exception *e){
		std::cout<<"Attenzione! Vi è un errore nella fase di astrazione di clustering: la somma delle dimensioni dei cluster non coincide con il numero di hit.";
		gSystem->Exec("rm TestSample.root");
		delete e;
	}

	return 0;
}

