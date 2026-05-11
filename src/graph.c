//
// Created by ianpa on 14/03/2026.
//
#include "../include/graph.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "../include/io.h"
#include "../include/list.h"
#include "../include/person.h"

// affiche une matrice carrée n*n aplatie en 1D
static void printMatrix(const int * M, int n)
{
    int i, j;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
            printf("%4d", M[i * n + j]);
        printf("\n");
    }
}

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
    g->num_vertices = nb_vertices;
    // n*n cases pour couvrir toutes les paires de personnes possibles
    g->adjacencies = calloc(nb_vertices * nb_vertices, sizeof(int));
    assert(g->adjacencies);
    g->distances = calloc(nb_vertices * nb_vertices, sizeof(int));
    assert(g->distances);
    g->gaps = NULL;
    // une case par personne pour stocker son nombre de connexions directes
    g->connections = calloc(nb_vertices, sizeof(int));
    assert(g->connections);
    g->far_from = 0;
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
        m[i] = 0; // on remet tout à 0 avant de remplir, sinon on risque de lire des valeurs aléatoires
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

    for (i = 0; i < nbGens; i++) // une itération = une ligne de la matrice = une personne
    {
        fscanf (fd , "%d", &numP);
        while (numP != -1) // -1 dans le fichier signale la fin de la liste d'amis de cette personne
        {
            // adjacencies[i][numP] en notation 2D, aplati en 1D avec i*n+j
            g->adjacencies[i * nbGens + numP] = 1;
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

    for (i = 0; i < nbGens; i++)
    {
        fread (&numP , sizeof(int), 1, fd);
        // même logique que le txt mais on lit des entiers bruts en binaire
        while (numP != -1)
        {
            g->adjacencies[i * nbGens + numP] = 1;
            fread (&numP , sizeof(int), 1, fd);
        }
    }

    fclose ( fd ) ;
    return g;
}




graph_t * fscanf_graph ( const char * filename, const stream_mode_t mode )
{
    if (mode == TEXT) // si on veut lire du txt, on lit avec la fonction txt sinon avec la bin
        return scanTXT_graph ( filename );
    return scanBIN_graph ( filename );
}

bool symmetric_graph( const graph_t * G )
{
    int i, j;
    int nbGens = G->num_vertices;
    for (i = 0; i < nbGens; i++)
    {
        // j part de i+1 : on évite de comparer une case avec elle-même
        // et on évite de tester deux fois la même paire (i,j) et (j,i)
        for (j = i + 1; j < nbGens; j++)
        {
            // si A aime B mais pas l'inverse, le graphe est asymétrique
            if (G->adjacencies[i * nbGens + j] != G->adjacencies[j * nbGens + i])
                return false;
        }
    }
    return true;
}

int distance2p(graph_t * G, int a, int b, int * exclu)
{
    if (a == b)
        return 0; // distance d'une personne à elle-même = 0
    int i, d;
    int nbGens = G->num_vertices;
    // nbGens est la borne max : dans un graphe de n sommets,
    // le plus long chemin simple fait au plus n-1 arêtes
    int minD = nbGens;

    if (G->adjacencies[a * nbGens + b] == 1)
        return 1; // amis directs, pas besoin d'aller plus loin

    exclu[a] = 1; // on marque a pour ne pas y revenir dans les appels récursifs
    exclu[b] = 1; // idem pour b : si on le croise en chemin c'est forcément distance 1, déjà traité au-dessus

    for (i = 0; i < nbGens; i++)
    {
        // si i n'est pas dans les exclus, on peut passer par lui
        // sans ça on bouclerait en faisant a->b->a->b...
        if (exclu[i] == 0)
        {
            if (G->adjacencies[a * nbGens + i] == 1) // a est ami avec i
            {
                d = 1 + distance2p(G, i, b, exclu); // +1 pour le saut a->i, puis on cherche i->b
                // d != 1 signifie que distance2p(i,b) != 0, donc i et b sont bien connectés
                if (d < minD && d != 1)
                    minD = d;
            }
        }
    }
    // on démarque a pour les autres chemins qui passeraient par lui
    // ex: chemin 0->1->3 et 0->2->1->3 ont le sommet 1 en commun, il faut pouvoir le revisiter
    exclu[a] = 0;

    if (minD == nbGens) // aucun chemin trouvé entre a et b
        return 0;
    return minD;
}

void distance_calculus ( graph_t * G )
{
    int i, j, k;
    int nbGens = G->num_vertices;
    // exclu est partagé entre tous les appels récursifs de distance2p
    // on l'alloue une seule fois ici et on le remet à zéro à chaque paire
    int * exclu = calloc (nbGens, sizeof(int));
    assert(exclu);
    for (i = 0; i < nbGens; i++)
    {
        for (j = 0; j < nbGens; j++)
        {
            iniZero(exclu, nbGens); // remise à zéro obligatoire entre deux paires
            k = i * nbGens + j;
            G->distances[k] = distance2p(G, i, j, exclu);
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

set_t * friends( const graph_t * G, const int p )
{
    int nbGens = G->num_vertices;
    // au maximum nbGens-1 amis (on ne compte pas la personne elle-même)
    set_t * f = new_set(nbGens - 1);
    int j, k;
    for (j = 0; j < nbGens; j++)
    {
        k = p * nbGens + j; // case (p, j) dans la matrice aplatie
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

    // P et X vides = on a construit une clique maximale
    // on la garde seulement si elle est plus grande que la meilleure trouvée jusqu'ici
    if (numelm_set(P) == 0 && numelm_set(X) == 0 && R->numelm > C->numelm)
    {
        // dup et pas affectation directe : R appartient à l'appelant qui va le libérer
        dup_set(R, C);
    }

    while (numelm_set(P) != 0)
    {
        v = singleton_set(P->data[0]); // on prend le premier sommet de P
        Rrecu = union_set(R, v);       // on l'ajoute à la clique en cours de construction
        amiDirect = friends(G, v->data[0]);
        // P ∩ N(v) : les candidats restants qui sont aussi voisins de v
        Precu = inter_set(P, amiDirect);
        // X ∩ N(v) : les déjà-visités qui sont aussi voisins de v
        Xrecu = inter_set(X, amiDirect);

        BronKerbosch(G, Rrecu, Precu, Xrecu, C);

        // on libère ce qu'on a créé pour cet appel récursif
        del_set(&Rrecu);
        del_set(&Xrecu);
        del_set(&Precu);
        del_set(&amiDirect);

        // v est traité : on le retire de P et on le met dans X
        // comme ça les prochains appels ne reconstruiront pas les mêmes cliques par un autre chemin
        substract_set(P, v->data[0]);
        add_set(X, v->data[0]);
        del_set(&v);
    }
}

graph_t * list2matrix( list_t * LP, list_t * LF )
{
    int nbGens = LP->numelm;
    int i = 0, j, k;
    list_elm_t * Pielm = LP->head;
    graph_t * G = new_graph(nbGens);
    iniZero(G->adjacencies, nbGens * nbGens);
    person_t * Pi;
    list_elm_t * Felm;
    person_t * F;
    list_elm_t * Pjelm;
    person_t * Pj;

    // on parcourt la liste des personnes : chaque personne = une ligne i dans la matrice
    while (Pielm != NULL)
    {
        Pi = Pielm->datum;
        Felm = Pi->friends->head; // on lit la liste d'amis de Pi

        while (Felm != NULL)
        {
            F = Felm->datum; // F est un ami de Pi

            // pour trouver l'indice j de F dans LP, on refait un parcours
            // c'est pas le plus efficace mais ça reste simple à comprendre
            Pjelm = LP->head;
            j = 0;
            while (Pjelm != NULL)
            {
                Pj = Pjelm->datum;
                if (cmp_person(Pj, F) == 0)
                {
                    k = i * nbGens + j; // case (i,j) aplatie
                    G->adjacencies[k] = 1;
                    break;
                }
                j++;
                Pjelm = Pjelm->suc;
            }
            Felm = Felm->suc;
        }
        Pielm = Pielm->suc;
        i++;
    }
    return G;
}