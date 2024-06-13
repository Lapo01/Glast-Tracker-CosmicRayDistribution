import ROOT
import argparse
from argparse import RawTextHelpFormatter


parser = argparse.ArgumentParser(prog='Lettura Parallela Codice', 
description ='Interfaccia per astrazione dati ed analisi del tracker Fermi-Glast',formatter_class=RawTextHelpFormatter)

parser.add_argument("infile",
                    help = "Name of the file to be analyzed")

parser.add_argument("--Clustering",
                    help = "Takes raw data file and return abstraction of data in clustering as a tree in a root file.")

parser.add_argument("--Tracking",
                    help = "Takes the root file created in the clustering phase and returns a tree in a root file")

parser.add_argument("--Results",
                    help = "Prints out the Zenith and Azimuth angle Cosmic Ray distributions")

parser.add_argument("--MCDist",
                    help = "Prints out a montecarlo simulation for the zenith and azimuth angle distribution")

    

def Clustering(fileinput):
    ROOT.gROOT.LoadMacro("CreateTree.cpp++O");





if __name__ == "__main__":
    args = parser.parse_args()  

    if args.Clustering: 
        Clustering()
    


