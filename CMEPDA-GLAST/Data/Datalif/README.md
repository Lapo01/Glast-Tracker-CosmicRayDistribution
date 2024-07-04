# Info on file format .lif

Il The first line contains the Run ID, the same shown in the DAQ GUI during the data acquisition. Then, each line is an event, with several parameters separated by a whitespace (“ ”).

The even line contains the Evt.ID, the number of hits (“#hits”) followed by the “Hit blocks” and the number of TOTs (“#TOTs”) followed by the “TOT blocks”

Each “Hit Block” contains information about one of the hit associated with the event:

Layer name (i.e. "X0")
Layer end :  a sequential number identifying the layer end, 2 for each layer -> Range [0:19] (10 layer)
Strip number -> Range [0:1535]
Front end chip -> Range [0:23]
Channel of the front end chip -> Range [0:63]

Each “TOT Block” contains information about one of the TOT measurement associated with the event (at most 1 per each layer):
Layer name (i.e. "X0")
Layer end :  a sequential number identifying the layer end, 2 for each layer -> Range [0:19] (10 layer)
TDC counts: raw TDC counts for the TOT measurements
TOT value converted to us



We only use the Hit Block in this analysis, the TOT Block can be used for studies on muon decays, but this esulates from this project. 

