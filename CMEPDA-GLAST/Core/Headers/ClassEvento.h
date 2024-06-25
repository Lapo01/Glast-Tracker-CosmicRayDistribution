#include <TMath.h>
#include"Evento.h"
#include"EventoTrack.h"
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


/*
class Evento
{
public:
	int NEventi;
	int NHit;
	std::vector<int> Strip;
	std::vector<int> Layer;
	std::vector<int> ClusterDimension;
	std::vector<int> InitialStrip;
	std::vector<int> ClusterLayer;
	std::vector<double> ClusterPosizione;
	int Flags[3] = {0,0,0};
};
*/
/*
extern std::map<int,double> Zmap;
//std::map<int, double> Zmap; //questa cosa è altamente inefficiente: ogni volta che chiamo la funzione ne crea una nuova di mappa.
extern std::vector<int> XLayers;
Zmap.insert(std::make_pair(10, 3.1 - shift[0]));
Zmap.insert(std::make_pair(11, 6.3 - shift[1]));
Zmap.insert(std::make_pair(12,16.9 - shift[2]));
Zmap.insert(std::make_pair(13,20.1 - shift[3]));
Zmap.insert(std::make_pair(14,31.2 - shift[4]));
Zmap.insert(std::make_pair(20,-1.0 - shift[5]));
Zmap.insert(std::make_pair(21,10.1 - shift[6]));
Zmap.insert(std::make_pair(22,13.3 - shift[7]));
Zmap.insert(std::make_pair(23,24.6 - shift[8]));
Zmap.insert(std::make_pair(24,27.8 - shift[9]));

extern std::map<int,int> mapLayers;
mapLayers.insert(std::make_pair(14,13));
mapLayers.insert(std::make_pair(10,11));
mapLayers.insert(std::make_pair(20,11));
mapLayers.insert(std::make_pair(24,13));


extern std::vector<int> XLayers = {10, 11, 12, 13, 14};
*/




//std::map<int,double> Zmap;

//std::vector<int> XLayers = {10, 11, 12, 13, 14};


//std::map<int,int> mapLayers;



/*
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
class EventoTrack
{
public:
	std::vector<Track> TrackX;
	std::vector<Track> TrackY;
	int Nx(){return TrackX.size();}
	int Ny(){return TrackY.size();}
	int NEvento;
	
};

*/




double Theta(EventoTrack e, int i){
    //questa funzione ritorna l'angolo theta di una traccia i-esima.
    return atan(sqrt(e.TrackX[i].Fit[1]*e.TrackX[i].Fit[1]+ e.TrackY[i].Fit[1]*e.TrackY[i].Fit[1]))*180/3.14;


}

double Phi(EventoTrack e, int i){

    return atan2(e.TrackY[i].Fit[1],e.TrackX[i].Fit[1])*180/3.14;

}

double Angle(Track a, Track b, Track c, Track d){
     return (a.Fit[1]*c.Fit[1] +b.Fit[1]*d.Fit[1] + 1)/((sqrt(a.Fit[1]*a.Fit[1]+b.Fit[1]*b.Fit[1])+1)*(sqrt(c.Fit[1]*c.Fit[1]+d.Fit[1]*d.Fit[1])+1));
}



/*
class MyException : public exception 
{ 
private: 
	std::string message; 
  
public: 
	// Constructor accepts a const char* that is used to set 
	// the exception message 
	MyException(const char* msg) 
		: message(msg) 
	{ 
	} 
  
	// Override the what() method to return our message 
	const char* what() const throw() 
	{ 
		return message.c_str(); 
	} 
}; 
*/


double StripCoordinate(int Strip){
	const double StripPitch = 0.0228; //cm
	const double EdgeWidth = 0.1; //cm
	const double LadderSeparation = 0.02; //cm
	double coordinate = EdgeWidth + StripPitch*int(Strip) + (LadderSeparation + 2*EdgeWidth - StripPitch)*int(Strip/384); //formula sbagliata: non funziona...
return coordinate;};




double LayerCoordinate(int numero)
{
	double shift[10] = {0, 0.3631, -0.3683, 0, -0.2463, 0.6223, 0, 0.3241, -0.3331, 0}; //Fissando i layer X0,X3, Y1,Y4
	//double shift[10] = {0.};
	std::map<int, double> Zmap;
	Zmap.insert(std::make_pair(10, 3.1 - shift[0]));
	Zmap.insert(std::make_pair(11, 6.3 - shift[1]));
	Zmap.insert(std::make_pair(12,16.9 - shift[2]));
	Zmap.insert(std::make_pair(13,20.1 - shift[3]));
	Zmap.insert(std::make_pair(14,31.2 - shift[4]));
	Zmap.insert(std::make_pair(20,-1.0 - shift[5]));
	Zmap.insert(std::make_pair(21,10.1 - shift[6]));
	Zmap.insert(std::make_pair(22,13.3 - shift[7]));
	Zmap.insert(std::make_pair(23,24.6 - shift[8]));
	Zmap.insert(std::make_pair(24,27.8 - shift[9]));

	double posizione = Zmap[numero];
return posizione;
}

double ClusterCoordinate(Evento e, int indice, int layer){
	double shift[10] = {0, -0.0081, -0.0099, 0, 0.0632, -0.0717, 0, 0.0178, 0.0003, 0}; //Questo shift è fatto con riferimenti X0, X3, Y1, Y4
	//double shift[10] = {0.};
	
	int stringa[10] = {10,11,12,13,14,20,21,22,23,24};
	std::map<int, double> MapShift;
	for(int i=0; i<10;i++)
	{
		MapShift.insert(std::make_pair(stringa[i],shift[i]));
	}
return e.ClusterPosizione[indice] - MapShift[layer];
}







double AngleCorrectionCal(int layer, double mx, double qx, double my, double qy, double alpha[10], double l[10]){//m e q sono i parametri della retta dell'altro piano
	//questo SERVE SOLO PER LA CALIBRAZIONE!!!!!!!!
	double Pos;
	if(layer<15)
	{
		Pos = my*LayerCoordinate(layer)+qy;
	}
	if(layer>15)
	{
		Pos = mx*LayerCoordinate(layer)+qx;
	}
	
	int stringa[10] = {10,11,12,13,14,20,21,22,23,24};	
	std::map<int, double> MapAngle;
	for(int j=0;j<10;j++)
	{
		MapAngle.insert(std::make_pair(stringa[j], (alpha[j]*Pos+l[j])));
	}

return MapAngle[layer];
}



double AngleCorrection(int layer, double mx, double qx, double my, double qy){//m e q sono i parametri della retta dell'altro piano
	//ad ogni punto, quando rifaccio il fit devo sottrarre questa funzione con i dati del primo fit 
	double alpha[10] = {-0.00092, -0.00064, 0.00176, 0.00188, -0.00161, 0.00246, 0.00032, 0.00010, 0.00145, 0.00125}; //Step 50, con correzione XY precedente
	double l[10] = {0.0415, 0.0303, -0.0341, -0.0429, -0.0047, -0.0146, 0.0130, 0.0143, -0.0174, -0.0184}; //Step 50, con correzione XY precedente
	//double alpha[10] = {-0.00084, -0.00055, 0.00178, 0.00185, -0.00169, 0.00218, -0.00001, -0.00024, 0.00119, 0.00123}; //Step 25, con correzione XY precedente
	//double l[10] = {0.0269, 0.0176, -0.0342, -0.0390, 0.0122, -0.0250, 0.0097, 0.0123, -0.0167, -0.0227}; //Step 25, con correzione XY precedente
	//double alpha[10] = {-0.001144, -0.000966, 0.001478, 0.001559, -0.002237, 0.002114, -0.000227, -0.000442, 0.001008, 0.001190}; //Step 15, con correzione XY precedente
	//double l[10] = {0.02735, 0.02172, -0.02778, -0.03184, 0.02892, -0.03055, 0.00962, 0.01273, -0.01482, -0.02335}; //Step 15, con correzione XY precedente
	double Pos;
	if(layer<15)
	{
		Pos = my*LayerCoordinate(layer)+qy;
	}
	if(layer>15)
	{
		Pos = mx*LayerCoordinate(layer)+qx;
	}
	
	int stringa[10] = {10,11,12,13,14,20,21,22,23,24};	
	std::map<int, double> MapAngle;
	for(int j=0;j<10;j++)
	{
		MapAngle.insert(std::make_pair(stringa[j], (alpha[j]*Pos+l[j])));
	}

return MapAngle[layer];
}





double ErrorXYZ(int Layer){
	std::map<int, double> Ambaraba;
	Ambaraba.insert(std::make_pair(10, 0.0254795));
	Ambaraba.insert(std::make_pair(11, 0.0182482));
	Ambaraba.insert(std::make_pair(12, 0.0242299));
	Ambaraba.insert(std::make_pair(13, 0.0239926));
	Ambaraba.insert(std::make_pair(14, 0.0698908));
	Ambaraba.insert(std::make_pair(20, 0.0536476));
	Ambaraba.insert(std::make_pair(21, 0.019532));
	Ambaraba.insert(std::make_pair(22, 0.0216202));
	Ambaraba.insert(std::make_pair(23, 0.0177527));
	Ambaraba.insert(std::make_pair(24, 0.022226));

	return Ambaraba[Layer];

}

double ErrorXYAngoloZ(int Layer){
	std::map<int, double> Ambaraba;

	//correzione fatta su retina
	/*Ambaraba.insert(std::make_pair(10, 0.0231989));
	Ambaraba.insert(std::make_pair(11, 0.0175281));
	Ambaraba.insert(std::make_pair(12, 0.0184174));
	Ambaraba.insert(std::make_pair(13, 0.0170521));
	Ambaraba.insert(std::make_pair(14, 0.0414208));
	Ambaraba.insert(std::make_pair(20, 0.0411782));
	Ambaraba.insert(std::make_pair(21, 0.0172039));
	Ambaraba.insert(std::make_pair(22, 0.0184462));
	Ambaraba.insert(std::make_pair(23, 0.0241886));
	Ambaraba.insert(std::make_pair(24, 0.0244736));*/

	//correzione fatta su lobster
	Ambaraba.insert(std::make_pair(10, 0.0132882));
	Ambaraba.insert(std::make_pair(11, 0.0101872));
	Ambaraba.insert(std::make_pair(12, 0.0110667));
	Ambaraba.insert(std::make_pair(13, 0.0111949));
	Ambaraba.insert(std::make_pair(14, 0.0321004));
	Ambaraba.insert(std::make_pair(20, 0.0317406));
	Ambaraba.insert(std::make_pair(21, 0.011021));
	Ambaraba.insert(std::make_pair(22, 0.0112294));
	Ambaraba.insert(std::make_pair(23, 0.0106568));
	Ambaraba.insert(std::make_pair(24, 0.0130159));

	return Ambaraba[Layer];

}



double Error(int StripIn, int Dimension){
	int StripFin = StripIn + Dimension;
	double error = (StripCoordinate(StripFin)-StripCoordinate(StripIn))/(sqrt(12.));
	if(Dimension==1)
	{
		error = 0.0228/sqrt(12.);
	}
	return error; 
}







int IndiceVettore(std::vector<int> vettore, int numero)
{
	int indice;
    // Cerca il numero nel vettore
    auto trovato = std::find(vettore.begin(), vettore.end(), numero);
    // Verifica se il numero è stato trovato
    if (trovato != vettore.end()) 
    {
		indice = std::distance(vettore.begin(), trovato);
    } else 
    {
		indice = -1;
    }
    return indice;
}




void TrackXZ(Evento e, TGraphErrors *Graph){

double xpos;
double zpos;
double error;
double errorz = 0.; //cm
int n = 0;
Graph->SetTitle("Track XZ; X [cm]; Z [cm]");
for (int i = 0; i<e.ClusterLayer.size(); i++){
	if(e.ClusterLayer[i]<15)
	{
		xpos = e.ClusterPosizione[i];
		zpos = LayerCoordinate(e.ClusterLayer[i]);
		n = Graph->GetN();
		error = Error(e.InitialStrip[i], e.ClusterDimension[i]);
		//errorz = sqrt(0.01+ error*m*m*error);
		Graph->SetPoint(n,xpos, zpos);
		Graph->SetPointError(n,error,errorz);
	}
}
}

void TrackYZ(Evento e, TGraphErrors *Graph){

double ypos;
double zpos;
double error;
double errorz = 0.; //cm
int n = 0;
Graph->SetTitle("Track YZ; Y [cm]; Z [cm]");
for (int i = 0; i<e.ClusterLayer.size(); i++){
	if(e.ClusterLayer[i]>15)
	{
		ypos = e.ClusterPosizione[i];
		zpos = LayerCoordinate(e.ClusterLayer[i]);
		n = Graph->GetN();
		error = Error(e.InitialStrip[i], e.ClusterDimension[i]);
		//errorz = sqrt(0.01+ error*m*m*error);
		Graph->SetPoint(n,ypos, zpos);
		Graph->SetPointError(n,error,errorz);
	}
}

}

void TrackFitXZ(Evento e, TGraphErrors *Graph,std::map<int,double> Zmap, int layerveto = -1, std::vector<double> shift = {0,0,0,0,0,0,0,0,0,0}){ //inverte gli assi

double xpos;
double zpos;
double error;
double errorz = 0.; //cm
int s = 0;
Graph->SetTitle("Track XZ; Z [cm]; X [cm]");
for (int i = 0; i<e.ClusterLayer.size(); i++){ 
	if((e.ClusterLayer[i]<15)&(e.ClusterLayer[i]!= layerveto))
	{
		xpos = ClusterCoordinate(e, i, e.ClusterLayer[i]);
		//xpos = e.ClusterPosizione[i];
		zpos = LayerCoordinate(e.ClusterLayer[i]);
		//cout<<xpos<<endl;
		//cout<<zpos<<endl;
		s = Graph->GetN();
		error = Error(e.InitialStrip[i], e.ClusterDimension[i]);
		//errorz = sqrt(0.01+ error*m*m*error);
		Graph->SetPoint(s,zpos, xpos);
		Graph->SetPointError(s,errorz,error);
	}
}

}

void TrackFitYZ(Evento e, TGraphErrors *Graph, std::map<int,double> Zmap,int layerveto = -1, std::vector<double> shift = {0,0,0,0,0,0,0,0,0,0} ){ //inverte gli assi

double ypos;
double zpos;
double error;
double errorz = 0.; //cm
int s = 0;
Graph->SetTitle("Track YZ; Z [cm]; Y [cm]");
for (int i = 0; i<e.ClusterLayer.size(); i++){
	if((e.ClusterLayer[i]>15)&(e.ClusterLayer[i]!= layerveto))
	{
		ypos = ClusterCoordinate(e, i, e.ClusterLayer[i]);
		//ypos = e.ClusterPosizione[i];
		zpos = LayerCoordinate(e.ClusterLayer[i]);
		s = Graph->GetN();
		error = Error(e.InitialStrip[i], e.ClusterDimension[i]);
		//errorz = sqrt(0.01+ error*m*m*error);
		Graph->SetPoint(s,zpos, ypos);
		Graph->SetPointError(s,errorz,error);
	}
}

}





void TrackFitXZVec(TGraphErrors *Graph, std::vector<int> Layers, std::vector<double> Clusters, std::vector<double> Errors){ //inverte gli assi

double xpos;
double zpos;
double error;
double errorz = 0.; //cm
int s = 0;
Graph->SetTitle("Track XZ; Z [cm]; X [cm]");
for (int i = 0; i<Layers.size(); i++)
	{
		xpos = Clusters[i];
		zpos = LayerCoordinate(Layers[i]);
		
		error = Errors[i];
		s = Graph->GetN();
		Graph->SetPoint(s,zpos, xpos);
		Graph->SetPointError(s,0,error);
	}
}

void TrackFitYZVec(TGraphErrors *Graph, std::vector<int> Layers, std::vector<double> Clusters, std::vector<double> Errors){ //inverte gli assi

double xpos;
double zpos;
double error;
double errorz = 0.; //cm
int n = 0;
Graph->SetTitle("Track YZ; Z [cm]; X [cm]");
for (int i = 0; i<Layers.size(); i++)
	{
		xpos = Clusters[i];
		zpos = LayerCoordinate(Layers[i]);
		error = Errors[i];
		n = Graph->GetN();
		Graph->SetPoint(n,zpos, xpos);
		Graph->SetPointError(n,0,error);
	}
}





void ClusterManualTest(Evento e){

	for (auto x:e.ClusterDimension){
		std::cout<<"Elenco le dimensioni dei cluster"<<'\n';
		std::cout<< x <<'\n';   
	}

	for (auto x :e.Strip){
		std::cout<<"Elenco tutte le strip dell'evento "<< x<<'\n';

	}
	for (auto x :e.Layer){
		std::cout<<"Elenco tutte i layer rispettivi dell'evento "<<x<<'\n';

	}
	for (auto x:e.InitialStrip){
		std::cout<<"Elenco le strip iniziali dei cluster "<< x<<'\n';

	}
	for (auto x:e.ClusterLayer){
		std::cout<<"Ora scrivo i layer delle prime strip dei cluster " << x <<'\n';


	}
	//Ora, se hai fatto il lavoro bene a monte, dovresti verificare che i layer associati ai cluster siano effettivamente  quelli giusti...



};




void Potential2TrackXZFinder(Evento e){
    std::vector<double> TrackOne;
    std::vector<double> TrackTwo;
    std::vector<double> FirstLayer;
    std::vector<double> PositionXFirst;
    std::vector<double> PositionXSecond;
    int StartingLayer;

    
std::map<int, double> Zmap; 

Zmap.insert(std::make_pair(10, 3.1 ));
Zmap.insert(std::make_pair(11, 6.3));
Zmap.insert(std::make_pair(12,16.9));
Zmap.insert(std::make_pair(13,20.1));
Zmap.insert(std::make_pair(14,31.2));
Zmap.insert(std::make_pair(20,-1.0));
Zmap.insert(std::make_pair(21,10.1));
Zmap.insert(std::make_pair(22,13.3));
Zmap.insert(std::make_pair(23,24.6));
Zmap.insert(std::make_pair(24,27.8));


std::map<int,int> mapLayers;
mapLayers.insert(std::make_pair(14,13));
mapLayers.insert(std::make_pair(10,11));
mapLayers.insert(std::make_pair(20,11));
mapLayers.insert(std::make_pair(24,13));


std::vector<int> XLayers = {10, 11, 12, 13, 14};






    
    for(auto x:e.ClusterLayer){
        if(x==14){
            PositionXFirst.push_back(x);  
        }
        if(x ==10){
            PositionXSecond.push_back(x);
        
        }
    }
    
    if(abs(PositionXSecond[0]- PositionXSecond[1]) < abs(PositionXFirst[0]- PositionXFirst[1])){
        StartingLayer =  14;
    
    }
    else {
        StartingLayer = 10;
    }
    
    
    for(int i = 0; i< e.ClusterLayer.size(); i++){
        if(e.ClusterLayer[i] == StartingLayer){
            FirstLayer.push_back(e.ClusterPosizione[i]);
        }
    }
    if (FirstLayer.size() ==2){ //questo if non è necessario.
        TrackOne.push_back(FirstLayer[0]);
        TrackTwo.push_back(FirstLayer[1]);
        for(int j = 0;j < e.ClusterLayer.size(); j++){
            if(e.ClusterLayer[j] ==  mapLayers[StartingLayer]){
                if(abs(TrackOne[0] - e.ClusterLayer[j]) <abs(TrackOne[0] - e.ClusterLayer[j])){ //qua chiedo se è più vicino al punto 0...
                    TrackOne.push_back(e.ClusterPosizione[j]);
                
                }
                else{
                TrackTwo.push_back(e.ClusterPosizione[j]);
                }    
            }
        } //ok, dopo aver fatto questo ho creato due track potenziali, ora quello che devo fare è prendere i punti che distano il minimo dalle rette create.
    }
    int mFirst = (TrackOne[0]-TrackOne[1])/(Zmap[StartingLayer] - Zmap[mapLayers[StartingLayer]]);             //sarebbe punto x4 -x3 o punto x0-x1
    int mSecond = (TrackTwo[0]-TrackTwo[1])/(Zmap[StartingLayer] - Zmap[mapLayers[StartingLayer]]);
    int qFirst = TrackOne[0] - mFirst*Zmap[StartingLayer]; 
    int qSecond = TrackTwo[0] - mSecond*Zmap[StartingLayer];
    
    TrackOne.clear();
    TrackTwo.clear();
    for(auto x:XLayers){ //ok, ora devo trovare per ogni layer il punto più vicino alle due tracce trovate.
        for(int i = 0; i<e.ClusterLayer.size();i++){                    
            if((e.ClusterLayer[i] == x)&((abs(e.ClusterPosizione[i]*mFirst -Zmap[e.ClusterLayer[i]] + qFirst)/sqrt(1+mFirst*mFirst))<0.2))
            {
                TrackOne.push_back(e.ClusterPosizione[i]);
            }
            else
            {
                TrackTwo.push_back(e.ClusterPosizione[i]);
            }
            
    
        }    
    }
    //Ok, ora hai le tracce candidate...gli eventi devono avere due punti per layer.
}









/*
void UniTestCluster(Evento e){
	int position=0;


	for(int i = 0; i < e.ClusterDimension.size();i++){
		if(e.ClusterLayer[i] !=e.Layer[i+position]){
				try{
						throw MyException("Fratm il codice non funziona");				 
				}
				catch (MyException& exc) { 
					// Catch and handle our custom exception 
					cout << "Caught an exception: " << exc.what() << endl; 
					cout << e.ClusterLayer[i]<< " " <<e.Layer[i]<< " "<<i <<'\n';
				} 
		exit(0);
		}

		position += e.ClusterDimension[i]-1;
	}

	for(auto x:e.ClusterDimension){
		position += x;
	}
	if(e.NHit == position){
	std::cout<<"Test Passed: La somma delle dimensioni dei cluster e il numero degli hit coincidono."<<'\n';
	}
	else{
		try{
						throw MyException("La somma delle dimensioni dei cluster ed il numero di Hit non coincidono. Error");				 
				}
				catch (MyException& exc) { 
					// Catch and handle our custom exception 
					std::cout << "Caught an exception: " << exc.what() << std::endl; 
					std::cout<<position << " "<<e.NHit<<'\n';
				} 
	exit(0);



	}
	std::cout<<position << " "<<e.NHit<<'\n';

}
*/









