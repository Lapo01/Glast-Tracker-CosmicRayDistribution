"""! @file Interface.py 
Interface used to call c++ macros. 
Several options are put.

"""


import ROOT
import argparse
from argparse import RawTextHelpFormatter
from loguru import logger
import os
import time
import multiprocess as mp
import subprocess
parser = argparse.ArgumentParser(prog='Interfaccia Fermi-Glast Data', 
description ='Interfaccia per astrazione dati ed analisi del tracker Fermi-Glast',formatter_class=RawTextHelpFormatter)


parser.add_argument("-i","--infile",
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


parser.add_argument("-TestReconstruction", action = "store_true",
                    help = "Generate a sample of MC distribution and feeds it to the tracking algorithm. It returns a Kolmogorov Smirnov Test p-value for both zenith and azimuth distributions")

parser.add_argument("-TrackSee", action = "store_true",
                    help = "Displays a reconstructed event with its tracks on both visions.")

parser.add_argument("-xz", "--Nx",
                    help = "Number of tracks XZ that one want to visualize")

parser.add_argument("-yz", "--Ny",
                    help = "Number of tracks YZ that one want to visualize")
##
# @param fileinput
# 
# @param fileoutput
#
#
# 
# 
# 
# 
# 
#  
   


def Clustering(fileinput, fileoutput ):
    """! Docs for clustering
    This function performs the first abstractin of data by calling the adeguate c++ macros.

    @param fileinput Name of the input file with extension, the path is not necessary.
    
    @param fileoutput Name of the outputfile with no extension., the output file is put in the folder Dataroot/ClusterAbstractionData with the name fileoutput.root
    """



    ROOT.gROOT.LoadMacro(os.path.join("Core", "CreateTree", "CreateTree.cpp")) #load macro
    ROOT.CreateTree(fileinput, fileoutput) #call macro
    logger.info("FATTO! Effettuata prima fase di astrazione dati, il file è stato salvato nel path ")
    logger.info(os.path.join("Data","Dataroot","ClusterAbstractionData", args.outfile+ ".root"))



def Tracking(fileinput, fileoutput) :
    """!Docs for Tracking
    This function performs the second abstractin of data by calling the Retina, RectifyTracks and UnifyTree macros. 
    It subdivides the data into a number of bunches equal to the number of pc cores, then each cores performs data abstraction on different bunches.
    At the end the UnifyTree macro is called and all created tree are merge into a unique tree put into .root file.
    It is advised to not interact with the pc throughout this process as this may cause slow-downs due to deprivation of resources.
    @param fileinput Name of the input file with extension, the path is not necessary.
    
    @param fileoutput Name of the outputfile with no extension, the output file is put in the folder Dataroot/TrackingAbstractionData with the name fileoutput.root
    """
    ROOT.gROOT.LoadMacro(os.path.join("Core", "TrackingRetina", "Retina.cpp++O"))
    ROOT.gROOT.LoadMacro(os.path.join("Core", "TrackingRetina", "UnifyTree.cpp"))
    ROOT.gROOT.LoadMacro(os.path.join("Core", "TrackingRetina", "RectifyTracks.cpp"))
    def AnalysePartOfData(j):
        logger.info("Inizio processo numero " + str(j))
        Stringsave = os.path.join("Data", "Dataroot","TrackingAbstractionData", fileoutput +str(j)+ ".root")
        ROOT.Retina(fileinput, Stringsave, j,  os.cpu_count())
        logger.info("Fine processo numero " + str(j))

    if os.cpu_count()<2:
        logger.warning("Attenzione: Il pc che stai utilizzando sembrerebbe avere meno di due core disponibili, potrebbe volerci un pò di tempo... Sui dataset forniti fino ad un massimo di 2 ore nel caso peggiore.")

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
    strOut = "TAGLIO IN CHI2 EFFETTUATO, IL NOME DEL FILE CREATO E' " + fileoutput + ".root"
    logger.info(strOut)


def TestReconstruction():
    """!Docs for testing the tracking algorithm
    """
    ROOT.gROOT.LoadMacro(os.path.join("Core", "Montecarlo", "MonteCarloGenerazioneDati.cpp++O"))
    ROOT.gROOT.LoadMacro(os.path.join("Core", "TrackingRetina", "Retina.cpp++O"))
    ROOT.gROOT.LoadMacro(os.path.join("Core", "TrackingRetina", "UnifyTree.cpp"))
    ROOT.gROOT.LoadMacro(os.path.join("Core", "UtilsMacros", "CompareDistributions.cpp"))
    ROOT.gROOT.LoadMacro(os.path.join("Core", "UtilsMacros", "ExtractDistributions.cpp"))


    ROOT.MonteCarloGenerazioneDati(os.path.join("Data", "MCSimulations","TestingRetinaAlgorithm", "MCDist.root"), os.path.join("Data", "MCSimulations","TestingRetinaAlgorithm", "Sample"))
    def AnalysePartOfData(j):
        logger.info("Inizio processo numero " + str(j))
        Stringsave = os.path.join("Data","MCSimulations","TestingRetinaAlgorithm", "MCReconstructed" +str(j)+ ".root")
        ROOT.Retina(os.path.join("Data", "MCSimulations", "TestingRetinaAlgorithm","MCDist.root"), Stringsave, j,  os.cpu_count())
        logger.info("Fine processo numero " + str(j))

    Iterations = range(os.cpu_count())
    pool = mp.Pool(processes = os.cpu_count())
    results = pool.map(AnalysePartOfData, Iterations)
    pool.close() #boh non la chiude da sola! bug DI PYTHON!!!!!!

    UtilsList = [None]*os.cpu_count()



    for i, item in enumerate(Iterations):
        UtilsList[i] =  os.path.join("Data"  ,"MCSimulations","TestingRetinaAlgorithm","MCReconstructed" +str(item) +".root")
        
    ROOT.UnifyTree(os.cpu_count(), os.path.join("Data", "MCSimulations" ,"TestingRetinaAlgorithm","MCReconstructed"))

    for i, item in enumerate(UtilsList):
        os.remove(UtilsList[i])

    ROOT.ExtractDistributions(os.path.join("Data", "MCSimulations", "TestingRetinaAlgorithm","MCReconstructed.root"), os.path.join("Data", "MCSimulations", "TestingRetinaAlgorithm","Reconstructed"), )
    KolmogorovTestResultTheta = round(ROOT.CompareDistributions(os.path.join("Data","MCSimulations" ,"TestingRetinaAlgorithm","ReconstructedZenithDistribution.root"), os.path.join("Data", "MCSimulations", "TestingRetinaAlgorithm","SampleZenithTest.root"), os.path.join("Data", "MCSimulations", "TestingRetinaAlgorithm","ResultComparisonZenith.pdf")),2)
    KolmogorovTestResultPhi = round(ROOT.CompareDistributions(os.path.join("Data","MCSimulations" ,"TestingRetinaAlgorithm","ReconstructedAzimuthDistribution.root"), os.path.join("Data", "MCSimulations", "TestingRetinaAlgorithm","SampleAzimuthTest.root"),os.path.join("Data", "MCSimulations", "TestingRetinaAlgorithm","ResultComparisonAzimuth.pdf")),2)
    OutText = "Il test di kolmogorov effettuato sulla distribuzione zenith generata e su quella ricostruita ritorna un p-value di "+str(KolmogorovTestResultTheta)
    logger.info(OutText)
    OutText = "Il test di kolmogorov effettuato sulla distribuzione azimuth generata e su quella ricostruita ritorna un p-value di "+str(KolmogorovTestResultPhi)
    logger.info(OutText)
    subprocess.call(["xdg-open", os.path.join("Data", "MCSimulations", "TestingRetinaAlgorithm","ResultComparisonAzimuth.pdf")])
    subprocess.call(["xdg-open", os.path.join("Data", "MCSimulations", "TestingRetinaAlgorithm","ResultComparisonZenith.pdf")])





def Montecarlo(fileinput,fileoutput):
    """!Docs for doing MC simulation
    """
    ROOT.gROOT.LoadMacro(os.path.join("Core", "Montecarlo", "MonteCarlo.cpp++O"))
    logger.info("Avviata simulazione MC: potrebbe volerci massimo qualche minuto per ottenere le distribuzioni attese.")
    ROOT.MonteCarlo(os.path.join("Data", "Datalif", fileinput), fileoutput)
    strOut = "FATTO: LE DISTRIBUZIONI MONTECARLO GENERATE E LE DISTRIBUZIONI MONTECARLO ATTESE DALLA MISURA SONO STATE SALVATE NELLA CARTELLA " + os.path.join("Data","MCSimulations") 
    logger.info(strOut)

def Results(fileinput, fileoutput):
    """!Docs function that shows results.
    """
    ROOT.gROOT.LoadMacro(os.path.join("Core", "UtilsMacros", "CompareDistributions.cpp"))
    ROOT.gROOT.LoadMacro(os.path.join("Core", "UtilsMacros", "ExtractDistributions.cpp"))

    ROOT.ExtractDistributions(fileinput, os.path.join("Data", "ResultsImages","Measured"))
    KolmogorovTestResultTheta = round(ROOT.CompareDistributions(os.path.join("Data", "ResultsImages", "MeasuredZenithDistribution.root"), os.path.join(fileoutput + "ZenithExpectedDistribution.root"), os.path.join("Data", "ResultsImages", "ComparisonMCRealDataZenith.pdf")),2)
    KolmogorovTestResultPhi = round(ROOT.CompareDistributions(os.path.join("Data", "ResultsImages", "MeasuredAzimuthDistribution.root"), os.path.join(fileoutput + "AzimuthExpectedDistribution.root"), os.path.join("Data", "ResultsImages", "ComparisonMCRealDataAzimuth.pdf")),2)
    OutText = "Il test di kolmogorov effettuato sulla distribuzione zenith simulata e su quella misurata ritorna un p-value di "+str(KolmogorovTestResultTheta)
    logger.info(OutText)
    OutText = "Il test di kolmogorov effettuato sulla distribuzione azimuth simulata e su quella misurata ritorna un p-value di "+str(KolmogorovTestResultPhi)
    logger.info(OutText)
    subprocess.call(["xdg-open", os.path.join("Data", "ResultsImages", "ComparisonMCRealDataZenith.pdf")])
    subprocess.call(["xdg-open",  os.path.join("Data", "ResultsImages", "ComparisonMCRealDataAzimuth.pdf")])






def process_file(args):
    1


if __name__ == "__main__":
    args = parser.parse_args()  
    

    ROOT.gROOT.LoadMacro("rootlogon.C")
    ROOT.rootlogon() #load shared libraries.
    




    if args.Clustering: 
        if not (os.path.exists(os.path.join("Data", "Dataroot", "ClusterAbstractionData"))):
            os.mkdir((os.path.join("Data", "Dataroot", "ClusterAbstractionData")))
        if not (os.path.isfile(os.path.join("Data","Datalif",args.infile))):
            logger.error("IL FILE INSERITO NON ESISTE NELLA CARTELLA DEI DATI LIF. SEI SICURO DI AVER INSERITO IL NOME CORRETTO?")
            raise Exception("Il file inserito non esiste.")
        if(not args.infile.endswith(".lif")):
            logger.error("IL FILE INSERITO HA IL FORMAT SBAGLIATO. ERRORE")
            raise Exception("Il file inserito non è del format .lif atteso.")
        fileinput = os.path.join("Data","Datalif",args.infile)
        fileoutput = os.path.join("Data","Dataroot","ClusterAbstractionData", args.outfile+ ".root")
        Clustering(fileinput, fileoutput)
        

    if args.Tracking:
        if not args.infile:
            logger.error("INSERISCI UN FILE DA ANALIZZARE USANDO LA FLAG -i")
            raise Exception("NON HAI INSERITO UN FILE DA ANALIZZARE.")     
        fileinput = os.path.join("Data", "Dataroot", "ClusterAbstractionData",  args.infile)
        if not(os.path.exists(os.path.join("Data", "Dataroot", "TrackingAbstractionData"))):
            os.mkdir((os.path.join("Data", "Dataroot", "TrackingAbstractionData")))    
        if not(os.path.isfile(fileinput)):
            logger.error("IL FILE INSERITO NON ESISTE NELLA CARTELLA DEI DATI CLUSTER. SEI SICURO DI AVER INSERITO IL NOME CORRETTO?")
            raise Exception("Il file inserito non esiste.")
        fileoutput = args.outfile
        Tracking(fileinput, fileoutput)


    if args.TestReconstruction:
        if not(os.path.exists(os.path.join("Data", "MCSimulations","TestingRetinaAlgorithm",))):
            os.mkdir((os.path.join("Data", "MCSimulations","TestingRetinaAlgorithm",)))  
        TestReconstruction()
         

    if args.MCDist:
        if not args.infile:
            logger.error("INSERISCI UN FILE DA ANALIZZARE USANDO LA FLAG -i DALLA CARTELLA DEI DATI .LIF, E' FONDAMENTALE PER LA SIMULAZIONE AL FINE DI TROVARE LE STRIP INEFFICIENTI/MORTE/MUTATE")
            raise Exception("NON HAI INSERITO UN FILE DA ANALIZZARE.")
        if not (os.path.isfile(os.path.join("Data","Datalif",args.infile))):
            logger.error("IL FILE INSERITO NON ESISTE NELLA CARTELLA DEI DATI LIF. SEI SICURO DI AVER INSERITO IL NOME CORRETTO?")
            raise Exception("Il file inserito non esiste.")
        fileinput = args.infile
        fileoutput = os.path.join("Data","MCSimulations", "MC")
        Montecarlo();


    if args.Results:
        if not os.path.exists(os.path.join("Data", "ResultsImages")):
            os.mkdir(os.path.join("Data", "ResultImages"))

        if not args.infile:
            logger.error("INSERISCI UN FILE DA ANALIZZARE USANDO LA FLAG -i")
            raise Exception("NON HAI INSERITO UN FILE DA ANALIZZARE")
        
        fileinput = os.path.join("Data", "Dataroot","TrackingAbstractionData", args.infile)
        fileoutput = os.path.join("Data", "MCSimulations", "MC")

        if not(os.path.isfile(fileinput)):
            logger.error("IL FILE INSERITO NON ESISTE NELLA CARTELLA DEI DATI. SEI SICURO DI AVER INSERITO IL NOME CORRETTO?")
            raise Exception("Il file inserito non esiste.")
        

        if ((not(os.path.isfile(fileoutput + "ZenithExpectedDistribution.root")))|(not (os.path.isfile(fileoutput + "AzimuthExpectedDistribution.root")))):
            logger.error("IL FILE CON LE SIMULAZIONI MC NON ESISTE: GENERA UN SET DI DATI USANDO LA FLAG -MCDist E INSERENDO IL FILE RELATIVO ALLA PRESA DATI CONSIDERATA.")
            raise Exception("La simulazione MC non è stata ancora effettuata.")
        Results()


    if args.TrackSee:
        
        if not args.infile:
            logger.error("INSERISCI UN FILE DA ANALIZZARE USANDO LA FLAG -i")
            raise Exception("NON HAI INSERITO UN FILE DA ANALIZZARE")
        fileinput = os.path.join("Data", "Dataroot","TrackingAbstractionData", args.infile)
        if not(os.path.isfile(fileinput)):
            logger.error("INSERISCI UN FILE ESISTENTE USANDO LA FLAG -i")
            raise Exception("IL FILE INSERITO NON ESISTE.")        
        if((not args.Nx)|(not args.Ny)):
            logger.error("INSERISCI IL NUMERO DI TRACCE CHE VUOI VISUALIZZARE SIA NELLA VISIONE XZ SIA NELLA VISIONE YZ CON LE OPZIONI -xz e -yz")
            raise Exception("NON HAI INSERITO IL NUMERO DI PROIEZIONI CHE VUOI VISUALIZZARE IN UNA O ENTRAMBE LE VISIONI.")
        ROOT.gROOT.LoadMacro(os.path.join("Core", "UtilsMacros", "TrackSee.cpp"))

        ROOT.TrackSee(fileinput, int(args.Nx), int(args.Ny))