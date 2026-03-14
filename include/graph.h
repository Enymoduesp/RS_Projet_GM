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
    int * gaps;
    int * connections;
    int far_from;
} graph_t;

//	A brand new graph
graph_t * new_graph ( const int nb_vertices );

//	Delete graph pointed by ptrG
void del_graph ( graph_t ** ptrG );

//	Read graph G from binary/text file
graph_t * fscanf_graph ( const char * filename, const stream_mode_t mode );

//	Is graphe G symmetric ?
bool symmetric_graph( const graph_t * G );

/** Print graph G :
        (+) Its adjacency matrix
        (+) Optionally its distance matrix	*/
void printf_graph ( const graph_t * G, const char * entete, const bool and_Distances );

//	Calculate friendship distances
void distance_calculus ( graph_t * G );

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
#endif //PROJET_GRAPH_H