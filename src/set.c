//
// Created by ianpa on 14/03/2026.
//
#include "../include/set.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

set_t * new_set( int max_numelm )
{
    set_t * s = malloc(sizeof(set_t)); //alloue pour l'ensemble
    assert(s);
    s->max_numelm = max_numelm;
    s->numelm = 0;
    s->data = calloc( s->max_numelm, sizeof(int)); //alloue le vecteur gens
    assert(s->data);
    return s;
}

void dup_set( const set_t * S1, set_t * S2 )
{   //on suppose qu'on a la place dans s2 pour duppliquer.
    int i;
    S2->numelm = S1->numelm; //on met le meme nombre de gens sans toucher à max car ça nous arrange pour union_Set
    for ( i = 0; i < S2->numelm; i++ )
    {
        S2->data[i] = S1->data[i];
    }
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

bool empty_set( const set_t * S )
{
    return S->numelm == 0;
}

void print_set( const set_t * S, char * entete )
{
    int i;
    printf("%s\n", entete);
    if (empty_set(S)) printf("{}\n");
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

bool in_set( const int e, const set_t * S )
{   int i;
    for ( i = 0; i < S->numelm; i++ )
    {
        if( e == S->data[i] )
            return true;
    }
    return false;
}

set_t * union_set( const set_t * S1, const set_t * S2 )
{
    int i;
    set_t * sU = new_set(S1->numelm + S2->numelm); //taille max c'est les deux entiers
    dup_set(S1, sU);

    if (!empty_set(S2))
    {
        for( i = 0; i < S2->numelm; i++ )
        {
            if (!in_set(S2->data[i], sU)) //si la valeur est pas encore présente, on la rajoute
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
    int t;
    if (S1->numelm <= S2->numelm)
        t = S1->numelm;
    else
        t = S2->numelm;
    set_t * sI = new_set(t); //taille max = la taille du plus petit ensemble...
    if (!empty_set(S1) && !empty_set(S2))
    {
        for (i = 0; i < S1->numelm; i++)
        {
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
    int i,j;
    if (!empty_set(S))
    {
        for (i = 0; i < S->numelm; i++)
        {
            if (e == S->data[i])
            {
                S->data[i] = S->data[S->numelm - 1]; //opti, plutot que tout décaler, ordre inutile en ensemble alors on inverse avec le dernier
                S->numelm--;
                break;
            }
        }
    }
}

void add_set( set_t * S, const int e )
{
    if (S->numelm < S->max_numelm)
    {
        if (!in_set(e, S)) //s'il est pas encore dans l'ensemble, on l'ajoute
        {
            S->data[S->numelm] = e;
            S->numelm++;
        }
    }

}
