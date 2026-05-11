//
// Created by ianpa on 14/03/2026.
//
#include "../include/set.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

set_t * new_set( int max_numelm )
{
    set_t * s = malloc(sizeof(set_t));
    assert(s);
    s->max_numelm = max_numelm;
    s->numelm = 0; // l'ensemble est vide au départ
    s->data = calloc( s->max_numelm, sizeof(int)); // le vecteur qui stockera les entiers de l'ensemble
    assert(s->data);
    return s;
}

void dup_set( const set_t * S1, set_t * S2 )
{
    // pré-condition implicite : S2 a été alloué avec au moins S1->numelm cases
    int i;
    S2->numelm = S1->numelm;
    // on ne touche pas à max_numelm de S2 sinon on casserait union_set
    // qui compte sur max_numelm = S1->numelm + S2->numelm
    for ( i = 0; i < S2->numelm; i++ )
        S2->data[i] = S1->data[i];
}

set_t * singleton_set( const int elm )
{
    set_t * s = new_set( 1 );
    s->data[0] = elm;
    s->numelm = 1;
    return s;
}

void del_set( set_t ** ptrSet )
{
    assert( ptrSet && *ptrSet );
    free( (*ptrSet)->data );
    free( *ptrSet );
    *ptrSet = NULL;
}

// retourne le nombre d'éléments, 0 si vide
int numelm_set( const set_t * S )
{
    return S->numelm;
}

void print_set( const set_t * S, char * entete )
{
    int i;
    printf("%s\n", entete);
    if (numelm_set(S) == 0) printf("{}\n");
    else
    {
        printf("{ ");
        for ( i = 0; i < S->numelm - 1; i++ )
        {
            printf("%d, ", S->data[i]);
        }
        printf("%d }\n", S->data[S->numelm - 1]);
    }

}

// retourne 1 si e est dans S, 0 sinon
int in_set( const int e, const set_t * S )
{
    int i;
    for ( i = 0; i < S->numelm; i++ )
    {
        if( e == S->data[i] )
            return 1;
    }
    return 0; // parcouru tout sans trouver
}

set_t * union_set( const set_t * S1, const set_t * S2 )
{
    int i;
    // la taille max possible c'est |S1| + |S2| si les deux sont disjoints
    int total = S1->numelm + S2->numelm;
    set_t * sU = new_set(total == 0 ? 1 : total); // au moins 1 pour éviter calloc(0)
    dup_set(S1, sU); // on commence avec tous les éléments de S1

    if (numelm_set(S2) != 0)
    {
        for( i = 0; i < S2->numelm; i++ )
        {
            // on n'ajoute que si l'élément n'est pas déjà là, sinon c'est plus un ensemble
            if (!in_set(S2->data[i], sU))
            {
                sU->data[sU->numelm] = S2->data[i];
                sU->numelm++;
            }
        }
    }
    return sU;
}

set_t * inter_set( const set_t * S1, const set_t * S2 )
{
    int i;
    // l'intersection ne peut pas être plus grande que le plus petit des deux ensembles
    int t = (S1->numelm <= S2->numelm) ? S1->numelm : S2->numelm;
    set_t * sI = new_set(t == 0 ? 1 : t); // au moins 1 pour éviter calloc(0)

    if (numelm_set(S1) != 0 && numelm_set(S2) != 0)
    {
        for (i = 0; i < S1->numelm; i++)
        {
            // un élément est dans l'intersection s'il est dans les deux ensembles
            if (in_set(S1->data[i], S2))
            {
                sI->data[sI->numelm] = S1->data[i];
                sI->numelm++;
            }
        }
    }
    return sI;
}

void substract_set( set_t * S, const int e )
{
    int i, j;
    if (numelm_set(S) != 0)
    {
        for (i = 0; i < S->numelm; i++)
        {
            if (e == S->data[i])
            {
                // on décale tout ce qui est après vers la gauche pour boucher le trou
                // on aurait pu juste échanger avec le dernier (plus rapide) mais
                // on préfère garder l'ordre d'insertion pour la lisibilité du débogage
                for ( j = i; j < S->numelm - 1; j++)
                    S->data[j] = S->data[j + 1];

                S->numelm--;
                break; // e est unique dans l'ensemble, inutile de continuer
            }
        }
    }
}

void add_set( set_t * S, const int e )
{
    // on ne dépasse pas la capacité allouée et on n'ajoute pas de doublons
    if (S->numelm < S->max_numelm && !in_set(e, S))
    {
        S->data[S->numelm] = e;
        S->numelm++;
    }
}
