#include "UtilsRetina.h"
#include "UtilsRectifyTracks.h"
#include "GeneralUtils.h"
#include "UtilsMC.h"
#include "UtilsCreateTree.h"
#include<cassert>
#include"TSystem.h"






void UnitTestVoidSpaces(double y){
	double MaxLength = StripCoordinate(1536);

	
	if(!(((y<0)||(y>MaxLength))||((y>StripCoordinate(384))&&(y<StripCoordinate(385)))||((y>StripCoordinate(768))&&(y<StripCoordinate(769)))||((y>StripCoordinate(1152))&&(y<StripCoordinate(1153)))||(y>StripCoordinate(1536)))){
        throw std::exception();

    }
}

void UnitTestOfmap(int Strip, std::vector<double> &StripPosition){
	
		double y = StripCoordinate(Strip);
        auto HittenStripPosition = *std::lower_bound(StripPosition.begin(), StripPosition.end(), y);
        
        if(!(fabs(y-HittenStripPosition)<0.001)){
            throw std::exception();

        }
        
}





int main(){

    std::vector<double> StripPosition;
	for(int i =0; i<1536; i++){
		StripPosition.push_back(StripCoordinate(i+1));
	}

    try{
        UnitTestOfmap(383,StripPosition);

        UnitTestOfmap(384,StripPosition);

        UnitTestOfmap(385,StripPosition);
    }
    catch(std::exception *e){
        std::cout<<"La strip non viene mappata alla sua posizione originale, errore."<<std::endl;
        abort();
        delete e;

    }

    try{
        UnitTestVoidSpaces((StripCoordinate(384) + StripCoordinate(385))/2.);
        UnitTestVoidSpaces(-1);
        UnitTestVoidSpaces(1000);
    }
    catch(std::exception *e){
        std::cout<<"Gli spazi non sensibili vengono considerati come hit."<<std::endl;
        abort();
        delete e;
    }


    return 0;
}