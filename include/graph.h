//
// Created by ianpa on 14/03/2026.
//

#ifndef PROJET_GRAPH_H
#define PROJET_GRAPH_H
#define BUDDY 1 // friendship token
#include "io.h"
#include "set.h"
//	A graph is an adjacency matrix with a friendship distance matrix. Others members are optional.
typedef struct {
    int num_vertices;
    int * adjacencies;
    int * distances;
} graph_t;

//	A brand new graph
graph_t * new_graph ( const int nb_vertices );

//	Delete graph pointed by ptrG
void del_graph ( graph_t ** ptrG );

//met des 0 partout
void iniZero(int * m, int taille);

//lit un graph depuis fichier txt
graph_t * scanTXT_graph ( const char * filename );

//lit graph depuis fichier bin
graph_t * scanBIN_graph ( const char * filename );

//	Read graph G from binary/text file
graph_t * fscanf_graph ( const char * filename, const stream_mode_t mode );

/**
 *  Ecris le graphique dans un fichier text, sous la meme forme que le graph.dta
 * @param G le graphique à ecrire dans le fichier
 * @param filename  nom du fichier
 */
void fprintGraphTXT ( graph_t * G, const char * filename );

/**
 *  Ecris le graphique dans un fichier binaire, sous la meme forme que le graph.dta
 * @param G le graphique à ecrire dans le fichier
 * @param filename  nom du fichier
 */
void fprintGraphBIN ( graph_t * G , const char * filename );

//	Is graphe G symmetric ?
bool symmetric_graph( const graph_t * G );

/**
 * calcul la distance d'amitié entre a et b. 0 si aucun lien, 1 si ami direct et n si ami par intérmédiaire de n - 1 humain
 * @param G le graph
 * @param a l'humain a pour lequel on veut la distance avec b
 * @param b l'humain b (celui qui est la "cible" à relier avec a)
 * @param exclu la liste des personnes par lesquelles on est déjà passée
 *
 * @return la distance d'amitié a-->b (0 si a = b,  -1 si aucun chemin)
 */
int distance2p(graph_t * G, int a, int b, int * exclu);


//	Calculate friendship distances
/*
 * objectif:
* matrice ami:
1 2 -1
0 2 3 -1
1 -1
-1

--> matrice distances:
    0 1 1 2
    1 0 1 1
    2 1 0 2
    0 0 0 0
 */
void distance_calculus ( graph_t * G );

/** Print graph G :
        (+) Its adjacency matrix
        (+) Optionally its distance matrix	*/
void printf_graph ( const graph_t * G, const char * entete, const bool and_Distances );


//	Who're friends of person NÂ°p ?
set_t * friends( const graph_t * G, const int p );

/**	Coenraad Bron et Joseph Kerbosch algorithm with :
    @param R : clique under construction, initialised with s,
    @param P : edges friends with the last edge added to  R, initialised with N(s),
    @param X : edges of current clique under construction, initialised to empty,
    @param C : la clique maximal clique until now, initialised to empty ;

    @result C : maximal clique in fine.	*/
void BronKerbosch ( const graph_t * G, const set_t * R, set_t * P, set_t * X, set_t * C );

/** Cast list representation to adjacency matrix representation */
graph_t * list2matrix( list_t * LP, list_t * LF );


//rajouter les fonctions d'écriture de FICHIERS à partir du graph.
#endif //PROJET_GRAPH_H