# Misura della Distribuzione degli angoli zenith ed azimuth dei raggi cosmici con il tracker del telescopio Fermi-GLast
Questo codice a partire dai dati in formato .lif restituiti dal tracker fermi-glast esegue l'astrazione dei dati a cluster e a tracce.

Lo scopo ultimo del progetto è misurare la distribuzione angolare zenith ed azimuth dei raggi cosmici.

Per ora questo progetto è stato testato su piattaforme Linux-Mint o Fedora, non è garantito il funzionamento su altre piattaforme Unix o Debian based in quanto non testate, non si garantisce il funzionamento su MacOS o in particolare su Windows.
Fare riferimento alla documentazione [qui](https://lapo01.github.io/Glast-Tracker-CosmicRayDistribution/html/index.html) per informazioni su dependencies, come installare il codice e informazioni utili su principali workflow, in particolare si faccia riferimento alla pagina principale.


Inoltre, per ulteriori informazioni sul formato dati .lif e sull'apparato, fare riferimento [qui][https://docs.google.com/document/d/1vizlW9KQsAPcLd8c2kGcYXq61NNNFOEo/edit?pli=1]

ATTENZIONE: I PARAMETRI OTTENUTI PER ALLINEARE L'APPARATO VIA SOFTWARE SONO STATI OTTENUTI PER UNA CONFIGURAZIONE PASSATA DELL'APPARATO, E' NECESSARIO RICALCOLARLI SE L'APPARATO VIENE SPOSTATO, IN QUESTO PROGETTO NON E' STATO INCLUSO IL CODICE PER CALCOLARE QUESTI PARAMETRI.
