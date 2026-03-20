//
// Created by ianpa on 14/03/2026.
//
#include "../include/graph.h"
#include <stdlib.h>
#include <assert.h>
#include "../include/io.h" //juste pour recup le enum BIN TXT
#include "../include/matrix.h"
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

int distance2p(graph_t * G, int a, int b, int * exclu)
{
    if (a == b)
        return 0; //pas ami avec lui meme (il n'est pas fou)
    int i, d;
    int nbGens = G->num_vertices;
    int minD = nbGens; //on ini au nombre de gens car si ya n personne, ami par lien de max n personne donc n - 1 lien
    if (G->adjacencies[a * nbGens + b] == 1) // cas de base, si a ami avec b on retourne 1, ami direct
        return 1;
    exclu[a] = 1; //on ajoute a dans la liste des exclu
    exclu[b] = 1; //car de toute façon si lien avec b il y a, il est renvoyé haut dessus
    for (i = 0; i < nbGens; i++) //sinon on parcourt chaque humain
    {
        if (exclu[i] == 0) //pour éviter boucle infinie si a ami avec b et b ami avec a
//si 0 ami avec 1, 1 ami avec 0 et 2, on aura d = 1 + distance(1, 2)
//mais distance (1,2) aura au premier tour d = 1 + distance (0, 2)
        {
            if (G->adjacencies[a * nbGens + i] == 1) // si a est ami avec i
            {
                d = 1 + distance2p(G, i, b, exclu); //alors distance d'ami = distance d'ami entre i et b + 1
                if (d < minD && d != 1)   //si le nouveau d est plus petit que le dernier trouvé, et distance(i, b) != 0
                {
                    minD = d;
                }
            }
        }
    }
    exclu[a] = 0; // 0->1->2->3->4 et 0->2->3->4 (on aurait exclu 2 pdt la recherche sur 1) 2 chemins peuvent avoirs des sommets communs qu'il faut revisiter
    if (minD == nbGens) //si on a pas trouvé de minD, de lien indirect entre a et b
        return 0;
    return minD; //sinon...
}

//opti à refaire? bcp de recursivité
void distance_calculus ( graph_t * G )
{
    int i, j, k;
    int nbGens = G->num_vertices;
    int * exclu = calloc (nbGens, sizeof(int)); //on fais le tableau des personnes à exclure pdt recherche
    assert(exclu);
    for (i = 0; i < nbGens; i++)
    {
        for (j = 0; j < nbGens; j++)
        {
            iniZero(exclu, nbGens); //on met tout à 0
            k = i * nbGens + j;
            G->distances[k] = distance2p(G, i, j, exclu); //on appelle la fonction qui trouve la distance entre i et j
        }
    }
    free(exclu);
}

void printf_graph ( const graph_t * G, const char * entete, const bool and_Distances )
{
    int nbGens = G->num_vertices;
    printf("%s \n", entete);
    printMatrix(G->adjacencies, nbGens);
    if (and_Distances)
    {
        printf("\n\n\n affichage de la matrice distance : \n\n");
        printMatrix(G->distances, nbGens);
        printf("\n\n");
    }
}

set_t * friends( const graph_t * G, const int p ) //friends = ami direct théoriquement
{
    int nbGens = G->num_vertices;
    set_t * f = new_set(nbGens - 1); //pas ami avec lui meme donc nbGens-1 max ami
    int j, k;
    for (j = 0; j < nbGens; j++)
    {
        k = p * nbGens + j;
        if (G->adjacencies[k] == 1)
            add_set(f, j);
    }
    return f;
}


/**	Coenraad Bron et Joseph Kerbosch algorithm with :
    @param R : clique under construction, initialised with s,
    @param P : edges friends with the last edge added to  R, initialised with N(s),
    @param X : edges of current clique under construction, initialised to empty,
    @param C : la clique maximal clique until now, initialised to empty ;

    @result C : maximal clique in fine.	*/


/*
 *1,2,3 relié, 3 et 4 sinon
BKB(R=  {/}, p = {1,2,3, 4}, x = {/})
PX vide? NOn
Pour v de 1 à 3
v = 1 :
    BKB(R = {1}, p = {2,3}, x = {/})
    PX vide? NOn
    pour v de 2 à 3
    v = 2:
        BKB(R = {1,2}, p = {3}, x = {/})
        PX vide? Non
        pour v de 3 à 3
        v = 3
            BKB (R = {1,2,3}, p ={/}, x ={/})
            PX vide? OUi C =3
        P / 2
        X  = {2}
    v = 3:
        BKB(R = {1,3}, p ={/} x ={2})
        PX vide?
        P vide, pas x -> pour v allant de rien à rien = stop

    P / 1
    X ={1}

v = 2:
    BKB(R ={2}, p = {3}, x = {1})
        BKB { R ={2, 3}, p = {/}, x = {1}
            p vide pas x on stop
    P / 2
    X = [1,2}

v = 3:
    BKB (R = {3}, p = {4}, X = {1,2}
    PX vide? non
    pour v 1,2,4
    v = 4 :
        BKB { R = {3,4}, p = {/}, x = {1,2}
            p vide, pas x on stop;
    P / 3
    X = {1,2,3}
v = 4
    BKB (R = {4}, p = {/}, x = {1,2,3})
    PX vide OUi
    R < C
    Plus grand C = 3
 */

void BronKerbosch ( const graph_t * G, const set_t * R, set_t * P, set_t * X, set_t * C )
{
    set_t * amiDirect;
    set_t * v;
    set_t * Rrecu;
    set_t * Precu;
    set_t * Xrecu;
    if (empty_set(P) && empty_set(X) && R->numelm > C->numelm) //si P et X vide (plus personne à ajt, personne a déjà été traité, et que la nouvelle clique est plus grande que l'ancienne max
    {
        dup_set(R, C); //dupplique car R est le Rrecu de l'appelant, qu'on free après
    }
    while (!empty_set(P)) //comme pour une liste on laisse l'ensemble "se deplacer vers la gauche" sans bouger
    {
        v = singleton_set(P->data[0]); //singleton avec v, le numero de la personne en tete de l'ensemble
        Rrecu = union_set(R, v); //on ajoute v dans la clique en création
        amiDirect = friends(G, v->data[0]); //ensemble amis direct de v
        Xrecu = inter_set(X, amiDirect); //les amisDirect de v qui sont aussi dans X, on les a deja traité
        Precu = inter_set(P, amiDirect); // les amisDirect de v qui sont encore dans les gens à tester
        BronKerbosch (G, Rrecu, Precu, Xrecu, C);
        del_set(&Rrecu); //liberation de memoire après récursivité
        del_set(&Xrecu);
        del_set(&Precu);
        del_set(&amiDirect);
        substract_set(P, v->data[0]); //on enleve v de P, on a traité ses clique
        add_set(X, v->data[0]); //dans les exclu on ajoute v, on vient de passer par lui, on veut pas retrouver la meme clique {1,2,3} = {3,2,1}
        del_set(&v);
    }
}