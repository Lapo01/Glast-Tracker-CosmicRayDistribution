import ROOT
import argparse
from argparse import RawTextHelpFormatter
from loguru import logger
import os
import numpy as np
import time
import multiprocessing as mp
parser = argparse.ArgumentParser(prog='Interfaccia Fermi-Glast Data', 
description ='Interfaccia per astrazione dati ed analisi del tracker Fermi-Glast',formatter_class=RawTextHelpFormatter)



parser.add_argument("infile",
                    help = "Name of the file to be analyzed")
parser.add_argument("-o", "--outfile", default = 'Out', 
                    help = "Name of the output file")

parser.add_argument("-Clustering", action = "store_true",
                    help = "Takes raw data file and return abstraction of data in clustering as a tree in a root file.")

parser.add_argument("-Tracking", action = "store_true",
                    help = "Takes the root file created in the clustering phase and returns a tree in a root file")

parser.add_argument("-Results", action = "store_true",
                    help = "Prints out the Zenith and Azimuth angle Cosmic Ray distributions")

parser.add_argument("-MCDist", action = "store_true",
                    help = "Prints out a montecarlo simulation for the zenith and azimuth angle distribution")

    


def Clustering(fileinput, fileoutput ):
    ROOT.gROOT.LoadMacro(os.path.join("Core", "CreateTree", "CreateTree.cpp"))
    ROOT.CreateTree(fileinput, fileoutput)







if __name__ == "__main__":
    args = parser.parse_args()  
    

    ROOT.gROOT.LoadMacro("rootlogon.C")
    ROOT.rootlogon() #Setup enviroment paths.
    




    if args.Clustering: 
        

        if not (os.path.exists(os.path.join("Data", "Dataroot", "ClusterAbstractionData"))):
            os.mkdir((os.path.join("Data", "Dataroot", "ClusterAbstractionData")))

        if not (os.path.isfile(os.path.join("Data","Datalif",args.infile))):
            logger.error("IL FILE INSERITO NON ESISTE NELLA CARTELLA DEI DATI LIF. SEI SICURO DI AVER INSERITO IL NOME CORRETTO?")
            raise Exception("Il file inserito non esiste.")
                  
        if(not args.infile.endswith(".lif")):
            logger.error("IL FILE INSERITO HA IL FORMAT SBAGLIATO. ERRORE")
            raise Exception("Il file inserito non è del format .lif atteso.")
        
        Clustering(os.path.join("Data","Datalif",args.infile), os.path.join("Data","Dataroot","ClusterAbstractionData", args.outfile+ ".root"))
        logger.info("FATTO! Effettuata prima fase di astrazione dati, il file è stato salvato nel path ")
        logger.info(os.path.join("Data","Dataroot","ClusterAbstractionData", args.outfile+ ".root"))


    if args.Tracking:
        fileinput = os.path.join("Data", "Dataroot", "ClusterAbstractionData",  args.infile)

        if not(os.path.exists(os.path.join("Data", "Dataroot", "TrackingAbstractionData"))):
            os.mkdir((os.path.join("Data", "Dataroot", "TrackingAbstractionData")))    

        if not(os.path.isfile(fileinput)):
            logger.error("IL FILE INSERITO NON ESISTE NELLA CARTELLA DEI DATI CLUSTER. SEI SICURO DI AVER INSERITO IL NOME CORRETTO?")
            raise Exception("Il file inserito non esiste.")
        
        fileoutput = args.outfile
        ROOT.gROOT.LoadMacro(os.path.join("Core", "TrackingRetina", "Retina.cpp++O"))
        ROOT.gROOT.LoadMacro(os.path.join("Core", "TrackingRetina", "UnifyTree.cpp"))
        ROOT.gROOT.LoadMacro(os.path.join("Core", "TrackingRetina", "RectifyTracks.cpp"))

        def AnalysePartOfData(j):
            logger.info("Inizio processo numero " + str(j))
            Stringsave = os.path.join("Data", "Dataroot","TrackingAbstractionData", fileoutput +str(j)+ ".root")
            ROOT.Retina(fileinput, Stringsave, j,  os.cpu_count())
            logger.info("Fine processo numero " + str(j))

        Iterations = range(os.cpu_count())
        pool = mp.Pool(processes = os.cpu_count())
        results = pool.map(AnalysePartOfData, Iterations)
        pool.close() #boh non la chiude da sola! bug DI PYTHON!!!!!!

        UtilsList = [None]*os.cpu_count()



        for i, item in enumerate(Iterations):
            UtilsList[i] =  os.path.join("Data" , "Dataroot" ,"TrackingAbstractionData",fileoutput +str(item) +".root")
        ROOT.UnifyTree(os.cpu_count(), os.path.join("Data", "Dataroot", "TrackingAbstractionData" ,fileoutput))

        for i, item in enumerate(UtilsList):
            os.remove(UtilsList[i])
        ROOT.RectifyTracks(os.path.join("Data", "Dataroot","TrackingAbstractionData",  fileoutput + ".root"), os.path.join("Data", "Dataroot","TrackingAbstractionData",  fileoutput + ".root"))
        logger.info("TAGLIO IN CHI2 EFFETTUATO")

    if args.Results:
        1
    


