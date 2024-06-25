import ROOT
import argparse
from argparse import RawTextHelpFormatter
from loguru import logger
import os
import numpy



parser = argparse.ArgumentParser(prog='Interfaccia Fermi-Glast Data', 
description ='Interfaccia per astrazione dati ed analisi del tracker Fermi-Glast',formatter_class=RawTextHelpFormatter)

parser.add_argument("infile",
                    help = "Name of the file to be analyzed")
parser.add_argument("outfile",
                    help = "Name of the output file")

parser.add_argument("-Clustering", action = "store_true",
                    help = "Takes raw data file and return abstraction of data in clustering as a tree in a root file.")

parser.add_argument("-Tracking", action = "store_true",
                    help = "Takes the root file created in the clustering phase and returns a tree in a root file")

parser.add_argument("-Results", action = "store_true",
                    help = "Prints out the Zenith and Azimuth angle Cosmic Ray distributions")

parser.add_argument("--MCDist", action = "store_true",
                    help = "Prints out a montecarlo simulation for the zenith and azimuth angle distribution")

    


def Clustering(fileinput, fileoutput ):
    ROOT.gROOT.LoadMacro("Core/CreateTree/CreateTree.cpp")
    ROOT.CreateTree(fileinput, fileoutput)
    






if __name__ == "__main__":
    args = parser.parse_args()  
    

    ROOT.gROOT.LoadMacro("rootlogon.C")
    ROOT.rootlogon() #Setup enviroment paths.
    
    if args.Clustering: 
        
        if not (os.path.isfile(os.path.join("Data","Datalif",args.infile))):
            logger.error("IL FILE INSERITO NON ESISTE.")
            raise Exception("Il file inserito non esiste.")          
        if not (args.infile.endswith(".lif")):
            logger.error("IL FORMAT DEL FILE INSERITO NON E' VALIDO! L'ESTENSIONE E' .lif")
            raise Exception("Inserisci un format di file valido.")
        if not (args.outfile.endswith(".root")):
            logger.error("IL FORMAT DEL FILE INSERITO NON E' VALIDO! L'ESTENSIONE E' .root")
            raise Exception("Inserisci un format di file valido")

        Clustering(os.path.join("Data","Datalif",args.infile), os.path.join("Data","Dataroot", args.outfile))


    if args.Tracking:
        1


    if args.Results:
        1
    


