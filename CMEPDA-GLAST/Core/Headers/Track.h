/**
 *  \file Track.h
 * Headers for the object Track
 */

#pragma once
#include <TMath.h>
#include "TROOT.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TGraphErrors.h"
#include "TRandom.h"
#include "TCanvas.h"
#include "TFrame.h"
#include "TDatime.h"
#include "TF1.h"
#include "TFile.h"
#include "TStopwatch.h"
#include "TTree.h"
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <iterator>
class Track
{
public:

	
	/**  
	 *Container for fit useful variable q, m, chi2, chi2Att, chi2n is the order in which the variables are inserted
	 */
	std::vector<double> Fit;

	/** Container of the error for the position of each cluster */
	
	std::vector<double> Error ;
	/** Container of the cluster positions */
	std::vector<double> ClusterPosition; 
	 /** Container of the cluster layers ID */
	std::vector<int> Layer;


	
	int NCluster()/** Returns the number of clusters in the tracks  */
	{
		return ClusterPosition.size();
	}
	

	/************
	 * Equal operator for tracks: tracks are equal if more than 1 point is in common. 
	 *
	 *
	 *********/
	friend bool operator==( const Track& a, const Track& b){
		int N = 0; 
		for(unsigned int i = 0; i< a.ClusterPosition.size(); i++){
			for(unsigned int j = 0; j< b.ClusterPosition.size(); j++){
				if(((a.ClusterPosition[i] - b.ClusterPosition[j])<0.02)&&(a.Layer[i] == b.Layer[j])){N++;}
			}
		} 
		return ((N>=2));
	
	}
	/************
	 * Less operator for tracks: a track is less than another if it has less clusters.
	 *
	 *
	 *********/
	friend bool operator<(const Track& a, const Track& b){
		return a.ClusterPosition.size() < b.ClusterPosition.size();
		}
	/************
	 * Greater operator for tracks: based on less operator.
	 *
	 *
	 *********/
	friend bool operator>(const Track& a, const Track& b){
		return !(a<b);
		
		}
};