#pragma once
#include "UtilsRetina.h"
#include "UtilsRectifyTracks.h"
#include "GeneralUtils.h"
#include "UtilsMC.h"
#include "UtilsCreateTree.cpp"



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
	e.Layer = [10, 10, 10, 10];
	e.Strip = [100, 101, 102, 103];
	
	std::vector<int> StripLayer;
	std::map<int,int> MapHitPerPiano;
	int Layer = 10;
	int Hits = 0;
	MapHitPerPiano.insert(std::make_pair(Layer, Hits));
	
	FillStripLayer(e, StripLayer, MapHitPerPiano, Layer);
	
	if(StripLayer.size() != e.Layer.size()){
		throw "Error: The Vector StripLayer does not have the same amount of strips in the layer. The function FillStripLayer in the library UtilsCreateTree is malfunctioning."
		
		}
	
	
	
	}

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
	Evento e;
	Evento *p = &e;
	TFile *input = new TFile(fileinput, "read");
	tree->SetBranchAddress("evento", &p);
	int entries = tree->GetEntries();
	
		
	for(int i = 0; i<entries; i++){
			tree->GetEntry(i);
			
			int NumberOfHits = 0;
			for(auto ClusterDim:e.ClusterDimension){
					NumberOfHits+= ClusterDim;
			}
	
		if(e.NHit != NumberOfHits){
			throw std::exception("Error: There is at least one event in which the sum of the dimension of the clusters does not match the number of hits: the CreateTree macro is malfunctioning.");
		}
		if(e.NHit != e.Strip.size()){
			throw std::exception("Error: the number of hitten strips does not match the number of hits in the event : the CreateTree macro is malfunctoning.");
			
		}
	}
	
	
	
	
	
	
	}

