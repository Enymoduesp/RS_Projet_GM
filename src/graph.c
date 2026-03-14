//
// Created by ianpa on 14/03/2026.
//
#include "../include/graph.h"
#include <stdlib.h>
#include <assert.h>
#include "../include/io.h" //juste pour recup le enum BIN TXT

//	A graph is an adjacency matrix with a friendship distance matrix. Others members are optional.
// typedef struct {
//     int num_vertices;
//     int * adjacencies;
//     int * distances;
//     int * gaps;
//     int * connections;
//     int far_from;
// } graph_t;

//nouveau graph
graph_t * new_graph ( const int nb_vertices )
{
    graph_t * g = malloc(sizeof(graph_t));
    assert(g);
    g->num_vertices = nb_vertices;      //nombre d'humains
    g->adjacencies = calloc(nb_vertices * nb_vertices, sizeof(int));//matrice des amitié basique
     assert(g->adjacencies);
    g->distances = calloc(nb_vertices * nb_vertices, sizeof(int)); // matrice des distances d'amitié
    assert(g->distances);
    g->gaps = NULL; //KESAQO???? A REMPLIR
    g->connections = calloc(nb_vertices, sizeof(int)); // g[0] = nombre d'amis direct de humain 0
    assert(g->connections);
    g->far_from = 0; // KESAQO????????? A REMPLIR
    return g;
}

void del_graph ( graph_t ** ptrG )
{
    assert(ptrG && *ptrG);
    free((*ptrG) -> adjacencies);
    free((*ptrG) -> distances);
    free((*ptrG) -> gaps);
    free((*ptrG) -> connections);
    free(*ptrG);
    *ptrG = NULL;
}


// 3
// 1 2 -1
// 0 2 -1
// 0 1 -1
//ce qu'on doit lire: nb personne = 3, p0 ami avec 1 et 2, p1 ami avec 0 et 2, p 3 ami avec 0 et 1. -1 = fin d'ami
//V1.0 marche que si dans l'ordre les data 0, 2 -1 ok mais 2, 1 -1 PAS OK
// graph_t * scanTXT_graph ( const char * filename )
// {
//     FILE * fd = fopen ( filename , "rt");
//     assert (fd);
//     int nbGens;
//     fscanf (fd , " %d", &nbGens);
//     graph_t * g = new_graph(nbGens);
//
//     int numP = 0;
//     int next = 1;
//     int i;
//     int j;
//     for (i = 0; i < nbGens; i++)
//     {
//         for (j = 0; j < nbGens; j ++)
//         {
//
//             if (next == 1)
//                 fscanf (fd, "%d ", &numP);
//             if (numP == -1)
//                 g->adjacencies[i * nbGens + j] = 0;
//             else if ( j == numP)
//             {
//                 g->adjacencies[i * nbGens + j] = 1;
//                 next = 1;
//             }
//             else
//             {
//                 g->adjacencies[i * nbGens + j] = 0;
//                 next = 0;
//             }
//
//
//         }
//     }
//     fclose ( fd ) ;
//     return g;
// }
void iniZero(int * m, int taille)
{
    int i;
    for (i = 0; i < taille; i++)
        m[i] = 0;
}

graph_t * scanTXT_graph ( const char * filename )
{
    FILE * fd = fopen ( filename , "rt");
    assert (fd);
    int nbGens;
    fscanf (fd , " %d", &nbGens);
    graph_t * g = new_graph(nbGens);

    iniZero(g->adjacencies, nbGens * nbGens); //on met tout à 0
    int numP;
    int i;

    for (i = 0; i < nbGens; i++) //on fait chaque ligne
    {
        fscanf (fd , "%d", &numP);
        while (numP != -1) //si ya pas moins -1
        {
            g->adjacencies[i * nbGens + numP] = 1; //on met la case[i][numP] = 1 pour dire que i ami avec numP
            fscanf (fd , "%d", &numP);
        }
    }
    fclose ( fd );
    return g;
}

graph_t * scanBIN_graph ( const char * filename )
{
    FILE * fd = fopen ( filename , "rb");
    assert ( fd );
    int nbGens;
    fread ( &nbGens, sizeof ( int ) , 1, fd ) ;
    graph_t * g = new_graph(nbGens);
    iniZero(g->adjacencies, nbGens * nbGens); //on met tout à 0

    int numP;
    int i;

    for (i = 0; i < nbGens; i++) //on fait chaque ligne
    {
        fread (&numP , sizeof(int), 1, fd);
        while (numP != -1) //si ya pas moins -1
        {
            g->adjacencies[i * nbGens + numP] = 1; //on met la case[i][numP] = 1 pour dire que i ami avec numP
            fread (&numP , sizeof(int), 1, fd);
        }
    }

    fclose ( fd ) ;
    return g;
}




graph_t * fscanf_graph ( const char * filename, const stream_mode_t mode )
{
    if (mode == TXT) //si on veut lire du txt, on lit avec la fonction txt sinon avec la Bin
        return scanTXT_graph ( filename );
    return scanBIN_graph ( filename );
}

bool symmetric_graph( const graph_t * G )
{
    int i, j;
    int nbGens = G->num_vertices;
    for (i = 0; i < nbGens; i++)
    {
        for (j = i + 1; j < nbGens; j++)
//i + 1 pour pas comparer avec lui meme et éviter de recomparer ce qu'on a deja comparé avant
//--> si on a check 1 ami avec 2, on a aussi check 2 ami avec 1 alors on peut commencer sur 2 ami avec 3
        {
            if (G->adjacencies[i * nbGens + j] != G->adjacencies[j * nbGens + i])
                return false;
        }
    }
    return true;
}

void distance_calculus ( graph_t * G );
