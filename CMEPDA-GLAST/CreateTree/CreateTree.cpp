#include"UtilsCreateTree.h"
#include"GeneralUtils.h"

/**
 *  * \file CreateTree.cpp
 *	This cpp macro takes as input a raw data file and returns a .root file with a tree. It performs a first abstraction of the data
 *
 * 
 *
 *
 */


/*******************************************************************************
 *This macro takes the raw data .lif file and produces a .root file containing a tree of instances of the object Evento.
 *Muted and dead strips are found, clustering is made.
 * @param fileinput The name of the input .lif file
 *
 * @param fileoutput The name of the output .root file
******************************************************************************/
void CreateTree(const std::string &fileinput, TString fileoutput){




	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//												SECTION 1 DECLARATION OF USED VARIABLES
	//	In this section all variables used up in the detection of muted strips and in the creation of the root file are declared.
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	

	int LengthCluster=1; // intero usato per contare la lunghezza dei cluster
	int NHit= 0; // Variabile usata come placeholder 
	int ciclo = 0;//variabile usata per contare le iterazioni dei cicli while 

	std::vector<int> VettoreDifferenzeStripConsecutive;// Vector of ints used to identify cluster and measure its number of strips (dimension)
	std::vector<int> StripLayer; // Used to count the strips in a layer
	int NLayerXZWithMoreThan2Cluster = 0; //  Used to count the number of layers with more than two clusters in the XZ vision
	int NLayerYZWithMoreThan2Cluster = 0; //  Used to count the number of layers with more than two clusters in the YZ vision
	int NClusterInTheLayer =0; //  Used to count the number of clusters in a layer 
	int FalseTrigger = 0; // Used to count the number of events that have a False Trigger condition
	int Nx = 0; // Used to count number of hits in the XZ vision
	int Ny = 0; // Used to count the number of hits in the YZ vision
	



	int HitPerPiano[10] = {0}; //Array containing the number of hit in the layers.
	std::map<int, int> MapHitPerPiano; //It maps the layer's ID to its respective number of hits
	for(int i=0; i<10;i++){
		MapHitPerPiano.insert(std::make_pair(LayersID[i],HitPerPiano[i]));
	}



	int ClusterPerPiano[10] = {0}; //Array containing the number of clusters in the layers.
	std::map<int,int> MapClusterPerPiano; //It maps the layer's ID to its respective number of clusters
	for(int i = 0; i<10; i++){
		MapClusterPerPiano.insert(std::make_pair(LayersID[i], ClusterPerPiano[i]));
	}
	

	std::vector<int> MutedStripX0;//vector that contains all dead/muted strips of layer X0
	std::vector<int> MutedStripX1;//vector that contains all dead/muted strips of layer X1
	std::vector<int> MutedStripX2;//vector that contains all dead/muted strips of layer X2
	std::vector<int> MutedStripX3;//vector that contains all dead/muted strips of layer X3
	std::vector<int> MutedStripX4;//vector that contains all dead/muted strips of layer X4
	std::vector<int> MutedStripY0;//vector that contains all dead/muted strips of layer Y0
	std::vector<int> MutedStripY1;//vector that contains all dead/muted strips of layer Y1
	std::vector<int> MutedStripY2;//vector that contains all dead/muted strips of layer Y2
	std::vector<int> MutedStripY3;//vector that contains all dead/muted strips of layer Y3
	std::vector<int> MutedStripY4;//vector that contains all dead/muted strips of layer Y4
	std::vector<std::vector<int>> MutedVector = {MutedStripX0,MutedStripX1,MutedStripX2,MutedStripX3,MutedStripX4,MutedStripY0,MutedStripY1,MutedStripY2,MutedStripY3,MutedStripY4}; // Container of MutedStripsVectors 




	std::map<int, std::vector<int>> MapDeiVettoriMutati; //maps the layer's ID to its respective muted strip container
	for (unsigned int i = 0; i<MutedVector.size(); i++){
		MapDeiVettoriMutati.insert(std::make_pair(LayersID[i],MutedVector[i]));	//inserting all pairs of IDs and muted strips containers
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																END SECTION 1
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//													SECTION 2: MUTED/DEAD STRIPS FINDING
	//	In this section a util function is called to find all muted strips in the tracker, an std::vector<TH1F*> is filled for each layer with the 
	//	hitten strips, then all strips that have a number of hits equal to 0 are added to a container std::vector<int>
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	FindMutedStrips(fileinput,NHit,ciclo,LayersID,MapLayerToID,MapDeiVettoriMutati); //Let's fill the MutedStripsXN/YN vectors

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																END SECTION 2
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//							SECTION 3: CREATING THE ROOT FILE AND FILLING THE TREE WITH THE INSTANCES OF CUSTOM OBJECT EVENTO
	// 	In this section a first subsection consist of setupping the creation of the output file, the input file .lif is read as a vector of words
	//	(strings separated by white space). Each line of the file corresponds to an event, so a loop is made, for each iteration the members Strip   
	//	and Layer are each filled respectively with the Strip number ID and Layer ID of all hits. Next subsection(2) a clustering abstraction and 
	// 	the flagging-check for the event is made. In the last subsection(3) there is a check if the event is False-triggered (does not satisfy 
	//   trigger condition). Note that it is not required that the hit are consecutive.
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

												////////////////////////////////////////////////////////
												//SUBSECTION 1: CREATION OF TREE AND FILE, HITS PHASE
												////////////////////////////////////////////////////////
	Evento evento;
	TFile *output = new TFile(fileoutput, "recreate");
	TTree *tree = new TTree("tree", "tree");
	tree->Branch("evento",&evento);
	

	std::string line; //declaring the std::string that will act as a placeholder for each line of the file
	std::ifstream myfile_in(fileinput); 
	if (myfile_in.is_open()){ 
		std::cout<<"ok"<<std::endl; 
		while(getline(myfile_in, line)){

			if(ciclo>0){ // Line 0 contains no info, we skip it 

				std::istringstream iss{line};
				std::vector<std::string> words //we make a vector of words rather than chars
				{
				std::istream_iterator<std::string>(iss),
				std::istream_iterator<std::string>()
			    };

				FlushPlaceholder(evento, MapHitPerPiano, MapClusterPerPiano, VettoreDifferenzeStripConsecutive, StripLayer, LayersID, Nx, Ny);	// Reset all variables used in the for loop, variable are reused for efficiency
							
				NHit = stoi(words[1]); // we use this placeholder because giving directly the value to the member NHit of class Evento raised a ValueError
				evento.NHit = NHit; // We set the NHit member of Evento
				evento.NEventi = ciclo; // We set the NEventi member of Evento

				for(int i = 0; i<evento.NHit; i++){ //Fill the members Layer and Strips of Evento
					evento.Layer.push_back(MapLayerToID[words[i+2+4*i]]);
					evento.Strip.push_back(stoi(words[i+4+4*i]));
				}


												////////////////////////////////////////////////////////
												//  	SUBSECTION 2: CLUSTERING AND FLAGGING
												////////////////////////////////////////////////////////
				for(auto x: LayersID){ //for each Layer do clustering, verify if flags 1,2,3 are satisfied
					
					FillStripLayer(evento, StripLayer, MapHitPerPiano, x); //Fill the container striplayer with the strips of the current layer
					
					
					LengthCluster=1; // reset var

					if (StripLayer.size() == 1) // The layer has only one strip, one may as well directly do clustering 
						{
							evento.ClusterDimension.push_back(LengthCluster); //Fill object evento
							evento.InitialStrip.push_back(StripLayer[0]);
							evento.ClusterLayer.push_back(x);
							evento.ClusterPosizione.push_back(StripCoordinate(StripLayer[0]));
							
							CheckForFlagOne(evento, MapDeiVettoriMutati, StripLayer, LengthCluster, x, -1); 
						}
						
					else if(StripLayer.size() == 0)//do nothing

					{
					}
					else //fill up the difference vector: Make vector of consecutive differences between strips of the same layer
					{ 
						std::sort(StripLayer.begin(), StripLayer.end()); //sort strip vector in order to calculate the vector of consecutive differences
						for(int j = 0; j<StripLayer.size()-1; j++) 
						{ 
							VettoreDifferenzeStripConsecutive.push_back(StripLayer[j+1] - StripLayer[j]); 
						}
					}


					if(StripLayer.size()>1) //Clustering in case of multiple hits
					{
						for(int j = 0; j<StripLayer.size(); j++)//loop on all strip of the layer
						{  
							if(VettoreDifferenzeStripConsecutive[j] == 1) // keep increasing the cluster dimension if there is a one, 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//						IMPORTANT COMMENT
							// One may think it is a mistake to loop up to StripLayer.size() and that actually one should go up to StripLayer.size()-1 for the vector 
							// VettoreDifferenzeStripConsecutive since its size is lower by 1, however this is useful to cover up the case of a last single cluster, which 
							// otherwise would not be counted.
							// (e.g. one could hypothesize to have the VettoreDifferenzeStripConsecutive = {1, 1, 2}, which correspond to two clusters with dimension 3 and 1.
							// Without going up to StripLayer.size(), the last cluster would not be counted.
							// 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							{	
									LengthCluster++;
							}
							else if((LengthCluster>1)) //Fill object with cluster data if cluster has more than one strip, turns False the Flag 2 and checks if Flag 1 must be turned off.
							{

								evento.ClusterDimension.push_back(LengthCluster); //Fill object evento
								evento.InitialStrip.push_back(StripLayer[j-LengthCluster+1]);
								evento.ClusterLayer.push_back(x);
								evento.Flags[2] =0;  //turns off Flag 2
								evento.ClusterPosizione.push_back((StripCoordinate(StripLayer[j])+StripCoordinate(StripLayer[j-LengthCluster+1]))/2.); //The cluster position is at half between the initial and last strip

								
								CheckForFlagOne(evento, MapDeiVettoriMutati, StripLayer, LengthCluster,x, j); 

								NClusterInTheLayer++; //keep account of number of clusters
								LengthCluster = 1; //reset
							}
							else//The only case remaining is a cluster of dimension 1
							{

								evento.ClusterDimension.push_back(LengthCluster);//Fill object evento
								evento.InitialStrip.push_back(StripLayer[j]);//Fill object evento
								evento.ClusterLayer.push_back(x);//Fill object evento
								evento.ClusterPosizione.push_back(StripCoordinate(StripLayer[j]));//Fill object evento
								NClusterInTheLayer++;		
								CheckForFlagOne(evento, MapDeiVettoriMutati, StripLayer, LengthCluster,x, j); 
							}

						}		
					}
					
					
					CheckForFlagTwo(evento, NClusterInTheLayer);//check if flag 2 is satisfied 


					StripLayer.clear();
					VettoreDifferenzeStripConsecutive.clear();
					NClusterInTheLayer = 0;

				}

				



				CheckForFlagThree(evento, MapClusterPerPiano, NLayerXZWithMoreThan2Cluster, NLayerYZWithMoreThan2Cluster, LayersID);




												////////////////////////////////////////////////////////
												//SUBSECTION 4: FALSE-TRIGGER CHECK AND OUTPUTTING FILE
												////////////////////////////////////////////////////////

				
				for(int i=10; i<15; i++) //Check all the hits in the respective vision.
				{ 
					if(MapHitPerPiano[i]>0){Nx++;}
					if(MapHitPerPiano[i+10]>0){Ny++;}
				}




				if((Nx>=3)&(Ny>=3)){ //Fill if this partial trigger condition is satisfied
					tree->Fill();
				}
				else{ //count up the event that does not respect partial trigger 
					FalseTrigger++;
				}
				
			}
			
			ciclo++;
		}
		output->Write();   //write output file
		output->Close();   //close output file
		myfile_in.close();   //close input file

		// Report on false trigger.
		std::cout<<"Eventi totali: "<<(ciclo-1)<<std::endl;
		std::cout<<"Eventi veri:  "<<(ciclo-1-FalseTrigger)<<std::endl;
		std::cout<<"Eventi con falsetrigger:  "<<FalseTrigger<<std::endl;
	}

}


