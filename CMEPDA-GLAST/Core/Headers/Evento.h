#pragma once
#include <vector>
/**
 *  * \file Evento.h
 *	Header file for the object Evento
 *
 * 
 *
 *
 */


class Evento
{
public:
	int NEventi; /**< ID dell'evento */
	int NHit;		/**< Numero di strip colpite  */
	std::vector<int> Strip; /**< Vettore contenente il numero della strip colpita 1 */
	std::vector<int> Layer;	/**< Vettore contenente l'ID dei layer delle strip colpite */
	std::vector<int> ClusterDimension;/**< Vettore contenente la dimensione dei cluster */
	std::vector<int> InitialStrip;/**< Vettore contenente la strip iniziale dei cluster */
	std::vector<int> ClusterLayer;/**< Vettore contenente l'ID dei layer dei cluster */
	std::vector<double> ClusterPosizione;/**< Vettore contente la posizione del cluster*/
	bool Flags[4] = {1,1,1,1};/**< Vettore contenente Flags utili per categorizzare eventi: Prima Flag indica evento con cluster analizzabile, seconda flag indica la presenza di cluster di dimensione maggiore di 1 strip ,Terza Flag indica potenziale evento ad una traccia per visione, Quarta Flag indica potenziale evento a due tracce per visione   */
};
