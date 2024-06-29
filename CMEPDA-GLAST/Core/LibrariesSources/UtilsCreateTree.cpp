#include "UtilsCreateTree.h"


/**
 *  * \file UtilsCreateTree.cpp
 *	Questo file contiene funzioni utili per la prima fase di astrazione dei dati, prende il format di oggetto .lif  e restituisce un file .root con un tree.
 * 
 *
 * 
 *
 *
 */



/*******************************************************************************
 * This function finds dead and muted strips in the tracker, which one can safely assume to be equal to the set of strips that have never been hit if the data sample is big enough.
 * All variable are referenced from the maincode and most of those are inserted as inputs only to avoid allocating new memory inside this function.
 * @param file The input file given by the User in the CreateTree.cpp code
 *
 * @param NHit Variable given by reference from the maincode: used to read the file
 *
 * @param ciclo Utility variable used to keep count of the while iteration during the file reading
 *
 * @param LayersID Contains the ID's of the layers of the tracker
 *
 * @param mapping It maps raw string info of the layers to the int ID's
 *
 * @param MapDeiVettoriMutati It maps the layer ID's to the vectors containing the dead strips
 *
 *
 ******************************************************************************/
void FindMutedStrips(const std::string &file, int &NHit, int &ciclo, std::vector<int> &LayersID, std::map<std::string, int> &mapping, std::map<int, std::vector<int>> &MapDeiVettoriMutati ){
	std::string line; /** It is used for reading the file, it is used in fact as a placeholder for each file line using the command getline()  */
	std::vector<TH1F*> HitCounts; /** Vector containing the TH1F pointers of the histograms of hits for the Layers: those histograms have 1536 bins which is equal to the number of strips in each layers */
	std::map<int,TH1F*> HitMap; /** Maps Layer's ID to its histogram pointer        */

	for (int i = 0; i < 10; ++i) /** In this for loop we create the TH1F pointers and we fill the HitCounts vector and link layers' ID to its histogram.*/
	{
		TH1F *h = new TH1F(Form("Hit Distribution Layer %d", LayersID[i]), "Hit Distribution", 1536, 0, 1536);
		HitCounts.push_back(h);
		HitMap.insert(std::make_pair(LayersID[i], h));
	}
	
	std::ifstream myfile_in(file);
	if (myfile_in.is_open())
	{ 
        std::cout<<"ok"<<std::endl;	   
		while(getline(myfile_in, line))/*The line is loaded, remember each event correspond to a line*/
		{
			if(ciclo>0){ /*Line 0 is skipped as it is a title and contains no info.*/
				std::istringstream iss{line}; /*using iterators to access the needed info*/
				std::vector<std::string> words{ /*making a vector of words*/
				std::istream_iterator<std::string>(iss),
				std::istream_iterator<std::string>()
			    };
			    NHit = stoi(words[1]); /* Retrieve the number of hits for this event*/
			    for(int i = 0; i<NHit; i++)/* Fill the layer corresponding histogram with the hitten strips*/
			    {
			        HitMap[mapping[words[i+2+4*i]]] ->Fill(stoi(words[i+4+4*i]));
			    }
		    }
		    ciclo++; /* Keep account of the iteration */
		}
	}

    for(auto x:LayersID)/** In this for-loop we check for each layer the counts of hits for each strip, if the count is 0 then the strip is added to the list of dead/muted strips*/
	{
    	int StripBinContent =0;
		for(int i= 1; i<1537; i++)
		{
			StripBinContent= HitMap[x]->GetBinContent(i);
			if(StripBinContent==0)
			{
				MapDeiVettoriMutati[x].push_back(i);/** Push back the strip ID into the vector corresponding to the layer's ID */
			}
		}
	}

	myfile_in.clear();
	//myfile_in.seekg(0);
	myfile_in.close();/** Close file */

	
	for(auto x:HitCounts)/**Delete pointers*/
	{
		delete x;
	}
	ciclo = 0; 
}



/*******************************************************************************
 *  This function clears all the vectors in a object Evento and sets all its 
 *	flags to a default value of 1.
 *  It also clears other containers used in the loop to use them up for the next
 *	event to be abstracted.
 *	
 *  @param e  Instance of the object to be flused
 *
 *  @param MapHitPerPiano  Maps layer ID to its TH1F* container of hits
 *
 *  @param MapClusterPerPiano  Maps layer's ID to the number of clusters in the layer 
 *
 *  @param VettoreDifferenzeStripConsecutive  Vector used to see the difference between strips of the same layer in order to reconstruct clusters.
 *
 *  @param StripLayer  Vector containing the strips of a certain layer.
 *
 *	@param LayersID  Layer's ID vector passed by reference for convenience.
 *
 *	@param Nx  Number of hits on XZ vision.
 *
 *	@param Ny  Number of hits on YZ vision.
 *
 *
 ******************************************************************************/
void FlushPlaceholder(Evento &e, std::map<int, int> &MapHitPerPiano, std::map<int,int> &MapClusterPerPiano, std::vector<int> &VettoreDifferenzeStripConsecutive, std::vector<int> &StripLayer, std::vector<int> &LayersID, int &Nx, int &Ny ){

	e.Layer.clear();
	e.Strip.clear();
	e.ClusterLayer.clear();
	e.ClusterDimension.clear();
	e.InitialStrip.clear();
	e.ClusterPosizione.clear();
	e.Flags[0] = 1;
	e.Flags[1] = 1;
	e.Flags[2] = 1;
	e.Flags[3] = 1;
	VettoreDifferenzeStripConsecutive.clear();
	StripLayer.clear();
	for(auto x:LayersID){
		MapHitPerPiano[x] = 0;
		MapClusterPerPiano[x] = 0;
	}
	Nx = 0;
	Ny = 0;
};



/*******************************************************************************
 * This function checks if the external strip of a cluster are adjacent to a dead/muted strips: if so the event is flagged and not used for tracking.
 *
 * @param e Instance of analyzed object passed by reference.
 *
 * @param MapDeiVettoriMutati Maps layer's ID to its muted strips containers
 *
 * @param StripLayer The vector containing the hitten strips in the layer in ascending order
 *
 * @param layer The considered layer ID
 *
 * @param LengthCluster THe length of the cluster, used to find external strips.
 *
 * @param j Integer of a for loop in the main code CreateTree.cpp, essential to reconstruct position of the external strips.
 *
 ******************************************************************************/
void CheckForFlagOne(Evento &e, std::map<int, std::vector<int>> &MapDeiVettoriMutati, std::vector<int> &StripLayer, int &layer, int &LengthCluster, int j ){


	if(LengthCluster >1){
		for(auto y:MapDeiVettoriMutati[layer])//check if the external strips are near to muted/dead strips
		{
			if (StripLayer[j-LengthCluster+1]-y ==1){//Flag 0 is not satisfied: the strip is near a dead/muted one			
				e.Flags[0] = 0;
			}
			if (y-StripLayer[j] ==1){ // Flag 0 is not satisfied: the strip is near a dead/muted one
				e.Flags[0] = 0;
			}		   	
		}
	}
	else
	{
		for(auto y:MapDeiVettoriMutati[layer])//check if the single strip composing the cluster is near a muted/dead one
		{	
			if (abs(y-StripLayer[j]) ==1){
				e.Flags[0] = 0;
			}	
		}
	}
}




/*******************************************************************************
 * The function itself is made in order to make the main code more comprehensible, 
 * it checks if the number of cluster in a layer is at maximum 1.
 * @param e Instance of the object passed by reference.
 *
 * @param NCluster the number of clusters in the layer.
 ******************************************************************************/
void CheckForFlagTwo(Evento &e, int &NCluster ){
	if(NCluster > 1) 
	{
		e.Flags[1] = 0;
	}

};


/*******************************************************************************
 * This function checks if the event satisfy Flag number four declared in the 	
 * object evento. 
 * The chosen criteria is the following: if in the event there are more than two layers
 * with more than two clusters, the Flag is set to False.  
 *
 * @param e Instance of the object to be analyzed passed by reference
 *
 * @param MapClusterPerPiano Maps layer's ID to the number of clusters in the layer 
 *
 * @param NLayerXZWithMoreThan2Cluster Number of layers on the XZ vision with more than two clusters
 *
 * @param NLayerYZWithMoreThan2Cluster Number of layers on the YZ vision with more than two clusters
 *
 * @param LayersID Layer's ID vector passed by reference for convenience.
 *
 ******************************************************************************/
void CheckForFlagThree(Evento &e, std::map<int,int> &MapClusterPerPiano, int &NLayerXZWithMoreThan2Cluster, int &NLayerYZWithMoreThan2Cluster, std::vector<int> &LayersID ){

	for(auto x: e.ClusterLayer){
		MapClusterPerPiano[x]++;
	}
	

	NLayerXZWithMoreThan2Cluster = 0;
	NLayerYZWithMoreThan2Cluster = 0;
		for(auto x:LayersID){
			if(x>15){
			   if((MapClusterPerPiano[x]>3)){
					NLayerYZWithMoreThan2Cluster++;
		    	}
			}
			if(x<15){
		 	   if((MapClusterPerPiano[x]>3)){
					NLayerXZWithMoreThan2Cluster++;
		    	}
			}
		}
	if((NLayerXZWithMoreThan2Cluster>2)||(NLayerYZWithMoreThan2Cluster>2)){
		e.Flags[3] =0;
	}

};


/*******************************************************************************
 * This function fills a placeholder vector with the hits of the same layer on 
 * the event.
 *
 * @param e Instance of the object to be copied passed by reference
 *
 * @param StripLayer Instance of the object to be copied passed by reference
 *
 * @param MapHitPerPiano Maps layer ID to its TH1F* container of hits
 *                        
 * @param Layer ID of theLayer that is being considered
 *
 *
 *
 ******************************************************************************/
void FillStripLayer(Evento &e, std::vector<int> &StripLayer, std::map<int,int> &MapHitPerPiano, int &Layer ){
	for(int i = 0; i<e.NHit; i++){
		if(e.Layer[i] == Layer){
			StripLayer.push_back(e.Strip[i]); 	
			MapHitPerPiano[Layer]++;
			}
	}
};





