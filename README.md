# Misura della Distribuzione degli angoli zenith ed azimuth dei raggi cosmici con il tracker del telescopio Fermi-GLast
Questo codice a partire dai dati in formato .lif restituiti dal tracker fermi-glast esegue l'astrazione dei dati a cluster e a tracce.
Per ora è stato testato su piattaforme Linux-Mint o Fedora, non è garantito il funzionamento su altre piattaforme Unix o Debian based in quanto non testate.
Per usufruire del codice, aprire il terminale nella directory dove si vuole scaricare il codice.

Fare riferimento alla documentazione [qui](https://lapo01.github.io/Glast-Tracker-CosmicRayDistribution/html/index.html) per dependencies e informazioni utili su principali workflow, in particolare alla pagina principale.



Dopo essersi assicurati di aver installato le opportune dipendenze, eseguire:

$ git clone git@github.com:Lapo01/Glast-Tracker-CosmicRayDistribution.git
Porsi nella directory Core ed eseguire: (il path è Glast-Tracker-CosmicRayDistribution/CMEPDA-GLAST/Core)
$ cmake ..
$ make -jN
dove N è il numero di core del PC. (se ne può anche inserire un numero minore, un semplice make bastarebbe.)


Dopo questo, è sufficiente far riferimento ai workflow suggeriti dalla documentazione nella pagina principale per usufruire del codice. 
