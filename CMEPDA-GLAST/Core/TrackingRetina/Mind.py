import numpy as np
import ROOT
import multiprocessing as mp
import argparse
import sys
import os
import time
#parser = argparse.ArgumentParser(prog='Lettura Parallela Codice', 
#description ='Codice per fittare esponenziali + fondo',formatter_class=RawTextHelpFormatter)

#parser.add_argument('infile')
#parser.add_argument('infile2')

##
#@file Mind.py
#
#
#
#
"""
File to enable parallel computing

"""

t1 = time.time()

ROOT.gROOT.LoadMacro("Retina.cpp++O")
ROOT.gROOT.LoadMacro("UnifyTree.cpp")

def function(j):
    stringsave = str(j) + sys.argv[2] 
    ROOT.Retina(sys.argv[1], stringsave, j )


    


iterations = range(os.cpu_count())
print(iterations)

pool = mp.Pool(processes = os.cpu_count())
results = pool.map_async(function,iterations)
results.wait()


c = [None]*os.cpu_count()



for i, item in enumerate(iterations):
    c[i] =  str(item) + sys.argv[2]
    
ROOT.UnifyTree(c[0], c[1],c[2],c[3],c[4] , c[5],c[6] ,c[7],  sys.argv[2])

for i, item in enumerate(c):
    os.remove(c[i]);

t2 = time.time()
print("Measured Time is :")
print(t2-t1)





