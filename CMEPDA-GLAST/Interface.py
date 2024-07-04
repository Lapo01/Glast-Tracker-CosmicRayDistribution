"""! @file Interface.py 
Interface used to call c++ macros. 
Several options for data analysis can be chosen.

"""


import ROOT
import argparse
from argparse import RawTextHelpFormatter
from loguru import logger
import os
import time
import multiprocess as mp
import subprocess


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
    This function performs the first abstractin of data by calling the c++ macros CreateTree.cpp .
    After abstracting the data, a second macro is called to plot the Cluster Number Distribution, the Cluster Position distribution on the layer X2 and the number of Clusters per event distribution
    @param fileinput Name of the input file with extension, the path is not necessary.
    
    @param fileoutput Name of the outputfile with no extension., the output file is put in the folder Dataroot/ClusterAbstractionData with the name fileoutput.root
    """



    ROOT.gROOT.LoadMacro(os.path.join("Core", "CreateTree", "CreateTree.cpp")) #load macro
    ROOT.gROOT.LoadMacro(os.path.join("Core", "CreateTree", "PlotClusterResults.cpp")) #load macro

    ROOT.CreateTree(fileinput, fileoutput) #call macro CreateTree
    logger.info("FATTO! Effettuata prima fase di astrazione dati, il file è stato salvato nel path ")
    logger.info(os.path.join(fileoutput))
    ROOT.PlotClusterResults(fileoutput) #Visualize results.



def Tracking(fileinput, fileoutput) :
    """!Docs for Tracking
    This function performs the second abstractin of data by calling the Retina.cpp , RectifyTracks.cpp and UnifyTree.cpp macros. 
    It subdivides the data into a number of bunches equal to the number of pc cores, then each cores performs data abstraction on different bunches.
    At the end the UnifyTree macro is called and all created tree are merge into a unique tree put into .root file.
    It is advised to not interact with the pc throughout this process as this may cause slow-downs due to deprivation of resources.

    @param fileinput Name of the input file with extension, the path is not necessary.
    
    @param fileoutput Name of the outputfile with no extension, the output file is put in the folder Dataroot/TrackingAbstractionData with the name fileoutput.root
    """

    #load macros
    ROOT.gROOT.LoadMacro(os.path.join("Core", "TrackingRetina", "Retina.cpp++O"))
    ROOT.gROOT.LoadMacro(os.path.join("Core", "TrackingRetina", "UnifyTree.cpp"))
    ROOT.gROOT.LoadMacro(os.path.join("Core", "TrackingRetina", "RectifyTracks.cpp"))
    ROOT.gROOT.LoadMacro(os.path.join("Core", "UtilsMacros", "Plot.cpp"))



    ##################################
    # IMPORTANT COMMENT:
    # This may seem (and i would be very happy to find a solution) a very silly thing to do, this function is defined inside and used multiple times around the code,
    # one would expect it to be rational to define it outside this function. However, there are some problems with loading the macros from pyroot, this solution makes the code work.
    #
    #################################
    def AnalysePartOfData(j):
        logger.info("Inizio processo numero " + str(j))
        Stringsave = os.path.join("Data", "Dataroot","TrackingAbstractionData", fileoutput +str(j)+ ".root")
        ROOT.Retina(fileinput, Stringsave, j,  os.cpu_count())
        logger.info("Fine processo numero " + str(j))


    #warning: might be a slow process....
    if os.cpu_count()<2:
        logger.warning("WARNING: Number of Cores detected on the cpu is less than 2, this process may take up a lot of time, maybe 1-2 hours.")



    #start multicore data analysis: divide the events in bunches each to be analyzed by a single core.
    Iterations = range(os.cpu_count())
    logger.warning("Tracking abstraction phase started: it is advised to not touch the PC as it may slow down this phase. This process may take up some time depending on the pc.")
    pool = mp.Pool(processes = os.cpu_count())
    results = pool.map(AnalysePartOfData, Iterations)
    pool.close() #boh non la chiude da sola! bug DI PYTHON!!!!!!
    UtilsList = [None]*os.cpu_count()


    for i, item in enumerate(Iterations):#list defined to later cancel out the names of the created .root file by each core
        UtilsList[i] =  os.path.join("Data" , "Dataroot" ,"TrackingAbstractionData",fileoutput +str(item) +".root")


    #call macro that unifies all created root files by each core into one.
    ROOT.UnifyTree(os.cpu_count(), os.path.join("Data", "Dataroot", "TrackingAbstractionData" ,fileoutput))

    #remove the root files created by each core
    for i, item in enumerate(UtilsList):
        os.remove(UtilsList[i])


    #correct errors due to relative shifts and rotations between the layers of the tracker and save into the final outputfile.
    ROOT.RectifyTracks(os.path.join("Data", "Dataroot","TrackingAbstractionData",  fileoutput + ".root"), os.path.join("Data", "Dataroot","TrackingAbstractionData",  fileoutput + ".root"))
    strOut = "TAGLIO IN CHI2 EFFETTUATO, IL NOME DEL FILE CREATO E' " + fileoutput + ".root"
    logger.info("FATTO! Effettuata seconda fase di astrazione dati, il file è stato salvato nel path")
    logger.info(os.path.join("Data", "Dataroot", "TrackingAbstractionData", fileoutput + ".root"))
    logger.info(strOut)
    ROOT.Plot(os.path.join("Data","Dataroot","TrackingAbstractionData","oldchi2Dist.root"), os.path.join("Data","Dataroot","TrackingAbstractionData","chi2Dist.root"))


def TestReconstruction():
    """!Docs for testing the tracking algorithm
    This function starts by calling the c++ macro MonteCarloGenerazioneDati.cpp that simulate a data acquisition with only 1 tracks per vision and no noise, the simulated data is fed to the tracking algorithm in the macro Retina.cpp . The generated distribution 
    takes into account only the trigger condition, it is compared with a KolmogGorov Test with the reconstructed distribution by the tracking algorithm. One would expect a p-value of 1 for an 
    optimal reconstruction

    

    The results are saved into the folder Data/MCSimulations/TestingRetinaAlgorithm
    """

    #load Macros
    ROOT.gROOT.LoadMacro(os.path.join("Core", "Montecarlo", "MonteCarloGenerazioneDati.cpp++O")) 
    ROOT.gROOT.LoadMacro(os.path.join("Core", "TrackingRetina", "Retina.cpp++O"))
    ROOT.gROOT.LoadMacro(os.path.join("Core", "TrackingRetina", "UnifyTree.cpp"))
    ROOT.gROOT.LoadMacro(os.path.join("Core", "UtilsMacros", "CompareDistributions.cpp"))
    ROOT.gROOT.LoadMacro(os.path.join("Core", "UtilsMacros", "ExtractDistributions.cpp"))



    #call macro that generates the data
    ROOT.MonteCarloGenerazioneDati(os.path.join("Data", "MCSimulations","TestingRetinaAlgorithm", "MCDist.root"), os.path.join("Data", "MCSimulations","TestingRetinaAlgorithm", "Sample"))

    logger.info("FATTO: FATTA SIMULAZIONE; DISTRIBUZIONI SALVATE NELLA CARTELLA:")
    logger.info(os.path.join("Data", "MCSimulations", "TestingRetinaAlgorithm"))
    ##################################
    # IMPORTANT COMMENT:
    # This may seem (and i would be very happy to find a solution) a very silly thing to do, this function is defined inside and used multiple times around the code,
    # one would expect it to be rational to define it outside this function. However, there are some problems with loading the macros, this solution makes the code work.
    #
    #################################
    def AnalysePartOfData(j):
        logger.info("Start process number " + str(j))
        Stringsave = os.path.join("Data","MCSimulations","TestingRetinaAlgorithm", "MCReconstructed" +str(j)+ ".root")
        ROOT.Retina(os.path.join("Data", "MCSimulations", "TestingRetinaAlgorithm","MCDist.root"), Stringsave, j,  os.cpu_count())
        logger.info("End process number " + str(j))


    #start multicore data analysis: divide the events in bunches each to be analyzed by a single core.
    Iterations = range(os.cpu_count())
    logger.warning("Tracking abstraction phase started: it is advised to not touch the PC as it may slow down this phase.")
    pool = mp.Pool(processes = os.cpu_count())
    results = pool.map(AnalysePartOfData, Iterations)
    pool.close() #boh non la chiude da sola! bug DI PYTHON!!!!!!


    UtilsList = [None]*os.cpu_count()#list defined to later cancel out the names of the created .root file by each core
    for i, item in enumerate(Iterations):
        UtilsList[i] =  os.path.join("Data"  ,"MCSimulations","TestingRetinaAlgorithm","MCReconstructed" +str(item) +".root")
        

    #call macro that unifies all created root files by each core into one.
    ROOT.UnifyTree(os.cpu_count(), os.path.join("Data", "MCSimulations" ,"TestingRetinaAlgorithm","MCReconstructed"))


    #remove the root files created by each core
    for i, item in enumerate(UtilsList):
        os.remove(UtilsList[i])

    #extract zenith, azimuth distributions and the Number of projections per vision distributions. Return Kolmogorov Test results.
    ROOT.ExtractDistributions(os.path.join("Data", "MCSimulations", "TestingRetinaAlgorithm","MCReconstructed.root"), os.path.join("Data", "MCSimulations", "TestingRetinaAlgorithm","Reconstructed"), )
    logger.info("Done. The reconstructed simulations have been saved in the folder:")
    logger.info(os.path.join("Data", "MCSimulations","TestingRetinaAlgorithm" ))
    KolmogorovTestResultTheta = round(ROOT.CompareDistributions(os.path.join("Data","MCSimulations" ,"TestingRetinaAlgorithm","ReconstructedZenithDistribution.root"), os.path.join("Data", "MCSimulations", "TestingRetinaAlgorithm","SampleZenithTest.root"), os.path.join("Data", "MCSimulations", "TestingRetinaAlgorithm","ResultComparisonZenith.pdf")),2)
    KolmogorovTestResultPhi = round(ROOT.CompareDistributions(os.path.join("Data","MCSimulations" ,"TestingRetinaAlgorithm","ReconstructedAzimuthDistribution.root"), os.path.join("Data", "MCSimulations", "TestingRetinaAlgorithm","SampleAzimuthTest.root"),os.path.join("Data", "MCSimulations", "TestingRetinaAlgorithm","ResultComparisonAzimuth.pdf")),2)
    OutText = "Il test di kolmogorov effettuato sulla distribuzione zenith generata e su quella ricostruita ritorna un p-value di "+str(KolmogorovTestResultTheta)
    logger.info(OutText)
    OutText = "Il test di kolmogorov effettuato sulla distribuzione azimuth generata e su quella ricostruita ritorna un p-value di "+str(KolmogorovTestResultPhi)
    logger.info(OutText)
    





def Montecarlo(fileinput,fileoutput):
    """!Docs for doing MC simulation
    This function call the c++ macro MonteCarlo.cpp that runs a MC simulation of 3D tracks that goes through the tracker. The generated numbers are four: (x0,y0) point on the plane of the top layer,
    zenith angle theta and azimuth angle phi. From this four numbers the projections are generated on the vision XZ and YZ, then the hits are generated and if a track satisfy the trigger 
    condition the angles zenith and azimuth are inserted into a histogram. The MC simulation does keep account of muted strips and mean efficiency of the tracker.

    @param fileinput Name of the input file with extension .lif, the path is not necessary. This input is used to keep account of muted strips in the simulation.
    
    @param fileoutput variable used to specify where to save plots and distributions.


    The MC distributions are saved into the Data/MCSimulations folder.
    """

    #load MC macro
    ROOT.gROOT.LoadMacro(os.path.join("Core", "Montecarlo", "MonteCarlo.cpp++O"))



    #run MC macro
    logger.info("Avviata simulazione MC: potrebbe volerci massimo qualche minuto per ottenere le distribuzioni attese.")
    ROOT.MonteCarlo(os.path.join("Data", "Datalif", fileinput), fileoutput)
    strOut = "FATTO: LE DISTRIBUZIONI MONTECARLO GENERATE E LE DISTRIBUZIONI MONTECARLO ATTESE DALLA MISURA SONO STATE SALVATE NELLA CARTELLA " + os.path.join("Data","Dataroot", "MCSimulations") 
    logger.info(strOut)

def Results(fileinput, fileoutput):
    """!Docs function that shows results.
    This function calls the c++ macros CompareDistributions.cpp and ExtractDistributions.cpp to extract the zenith and azimuth angle distribution from the measured data and compare them with the MC simulation.
    It is expected that one had already ran the MC simulation before running the results flag.
    The resuls are then saved as plot in the Data/ResultsImages folder.

    @param fileinput path to the tracking root file that contains the data.

    @param fileoutput path to where the MC simulations are, contains also the initial 2 letters of the names of the files (MC).
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
    logger.info("Le immagini e le distribuzioni misurate sono state salvate nella cartella:")
    logger.info(os.path.join("Data", "ResultsImages"))






def Interface_parse():
    parser = argparse.ArgumentParser(prog='Interfaccia Fermi-Glast Data', 
    description ='Interfaccia per astrazione dati ed analisi del tracker Fermi-Glast',formatter_class=RawTextHelpFormatter)

    ################################################################################################################################################
    #           To understand workflows (how to use the interface) please refer to the mainpage of the documentations at                           #
    #                      https://lapo01.github.io/Glast-Tracker-CosmicRayDistribution/html/index.html                                            #
    #                                                                                                                                              #
    #                                                                                                                                              #
    ################################################################################################################################################
    parser.add_argument("-i","--infile",
                    help = "Name of the file to be analyzed")

    parser.add_argument("-o", "--outfile", default = 'Out', 
                    help = "Name of the output file")

    parser.add_argument("-Clustering", action = "store_true",
                    help = "Takes a raw data file and return abstraction of data in clustering as a tree in a root file. To run it requires an --infile and optionally --outfile")

    parser.add_argument("-Tracking", action = "store_true",
                    help = "Takes a root file created in the clustering phase and returns a tree in a root file. To run it requires an --infile and optionally --outfile")

    parser.add_argument("-Results", action = "store_true",
                    help = "Prints out the Zenith and Azimuth angle Cosmic Ray distributions, compares MC simulated distribution and measured distributions To run it requires an --infile")

    parser.add_argument("-MCDist", action = "store_true",
                    help = "Prints out a montecarlo simulation for the zenith and azimuth angle distribution. To run it requires an --infile")

    parser.add_argument("-TestReconstruction", action = "store_true",
                    help = "Generate a sample of MC distribution and feeds it to the tracking algorithm. It returns a Kolmogorov Smirnov Test p-value for both zenith and azimuth distributions.")

    parser.add_argument("-TrackSee", action = "store_true",
                    help = "Displays a reconstructed event with its tracks on both visions. To run it requires to specify an --infile and the numbers of projections --Nx --Ny ")

    parser.add_argument("-nx", "--Nx",
                    help = "Number of tracks XZ that one want to visualize")

    parser.add_argument("-ny", "--Ny",
                    help = "Number of tracks YZ that one want to visualize")

    args = parser.parse_args()  

    ROOT.gROOT.LoadMacro("rootlogon.C")
    ROOT.rootlogon() #load shared libraries. 
    




    if args.Clustering: 
        #run some checks on the input file and on folders.
        if not (os.path.exists(os.path.join("Data", "Dataroot"))):
            os.mkdir((os.path.join("Data", "Dataroot")))
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
        #run some checks on the input file and on folders.
        if not args.infile:
            logger.error("INSERISCI UN FILE DA ANALIZZARE USANDO LA FLAG -i")
            raise Exception("NON HAI INSERITO UN FILE DA ANALIZZARE.")     
        fileinput = os.path.join("Data", "Dataroot", "ClusterAbstractionData",  args.infile)
        if not (os.path.exists(os.path.join("Data", "Dataroot"))):
            os.mkdir((os.path.join("Data", "Dataroot")))    
        if not(os.path.exists(os.path.join("Data", "Dataroot", "TrackingAbstractionData"))):
            os.mkdir((os.path.join("Data", "Dataroot", "TrackingAbstractionData")))    

        if not(os.path.isfile(fileinput)):
            logger.error("IL FILE INSERITO NON ESISTE NELLA CARTELLA DEI DATI CLUSTER. SEI SICURO DI AVER INSERITO IL NOME CORRETTO?")
            raise Exception("Il file inserito non esiste.")
        fileoutput = args.outfile
        Tracking(fileinput, fileoutput)


    if args.TestReconstruction:
        #run some checks on the input file and on folders.

        if not(os.path.exists(os.path.join("Data", "MCSimulations"))):
            os.mkdir((os.path.join("Data", "MCSimulations")))  
        if not(os.path.exists(os.path.join("Data", "MCSimulations","TestingRetinaAlgorithm",))):
            os.mkdir((os.path.join("Data", "MCSimulations","TestingRetinaAlgorithm",)))  
        TestReconstruction()
         

    if args.MCDist:
        #run some checks on the input file and on folders.
        if not(os.path.exists(os.path.join("Data", "MCSimulations"))):
            os.mkdir((os.path.join("Data", "MCSimulations"))) 
        if not args.infile:
            logger.error("INSERISCI UN FILE DA ANALIZZARE USANDO LA FLAG -i DALLA CARTELLA DEI DATI .LIF, E' FONDAMENTALE PER LA SIMULAZIONE AL FINE DI TROVARE LE STRIP INEFFICIENTI/MORTE/MUTATE")
            raise Exception("NON HAI INSERITO UN FILE DA ANALIZZARE.")
        if not (os.path.isfile(os.path.join("Data","Datalif",args.infile))):
            logger.error("IL FILE INSERITO NON ESISTE NELLA CARTELLA DEI DATI LIF. SEI SICURO DI AVER INSERITO IL NOME CORRETTO?")
            raise Exception("Il file inserito non esiste.")
        fileinput = args.infile
        fileoutput = os.path.join("Data","MCSimulations", "MC")
        Montecarlo(fileinput, fileoutput);


    if args.Results:
        #run some checks on the input file and on folders.
        if not(os.path.exists(os.path.join("Data", "ResultsImages"))):
            os.mkdir(os.path.join("Data", "ResultsImages"))

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
        Results(fileinput, fileoutput)


    if args.TrackSee:
        #run some checks on the input file and on folders.
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


if __name__ == "__main__":
    Interface_parse()

    