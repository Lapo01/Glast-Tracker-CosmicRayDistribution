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
	std::vector<double> Fit; // [q, m, chi2, chi2Att, chi2n]
	std::vector<double> Error ;
	std::vector<double> ClusterPosition;
	std::vector<int> Layer;
	int NCluster(){return ClusterPosition.size();}
	
	
	friend bool operator==( const Track& a, const Track& b){
		int N = 0; 
		for(int i = 0; i< a.ClusterPosition.size(); i++){
			for(int j = 0; j< b.ClusterPosition.size(); j++){
				if(((a.ClusterPosition[i] - b.ClusterPosition[j])<0.02)&&(a.Layer[i] == b.Layer[j])){N++;}
			}
		} 
		return ((N>=2));//||(((a.Fit[1]-b.Fit[1])<0.4)&&((a.Fit[0] - b.Fit[0]))<0.5));
	
	}
	
	friend bool operator<(const Track& a, const Track& b){
		return a.ClusterPosition.size() < b.ClusterPosition.size();
		}
	friend bool operator>(const Track& a, const Track& b){
		return !(a<b);
		
		}
};