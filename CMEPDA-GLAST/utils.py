import numpy as np
import ROOT
import multiprocessing as mp
import os
from loguru import logger
##
#@file utils.py
#
#
#
#
"""
Utils to interface
"""


def function(j, fileinput, fileoutput):
        print(j)
        print(fileinput)
        Stringsave = os.path.join("Data", "Dataroot", fileoutput +str(j)+ ".root")
        print(Stringsave)
        ROOT.Retina(fileinput, Stringsave, j,  os.cpu_count())

def Mind(fileinput, fileoutput):
    
    ROOT.gROOT.LoadMacro("Core/TrackingRetina/Retina.cpp++O")
    ROOT.gROOT.LoadMacro("Core/TrackingRetina/UnifyTree.cpp")
    
    
    
    Inputs = []
    Iterations = np.arange(os.cpu_count())
    for i, item in enumerate(Iterations):
        Inputs.append([item, fileinput, fileoutput])
         
    pool = mp.Pool(processes = os.cpu_count())
    results = pool.starmap_async(function, Inputs)
    results.wait()


    UtilsList = [None]*os.cpu_count()



    for i, item in enumerate(Iterations):
        UtilsList[i] =  os.path.join("Data" , "Dataroot" ,fileoutput +str(item) ,".root")
    
    #ROOT.UnifyTree(os.cpu_count(), fileoutput)

    #for i, item in enumerate(UtilsList):
    #    os.remove(UtilsList[i])





