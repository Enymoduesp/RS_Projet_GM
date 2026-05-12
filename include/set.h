//
// Created by ianpa on 14/03/2026.
//

#ifndef PROJET_SET_H
#define PROJET_SET_H
#define BUD_END -1 // La fin de l'ensemble
#include <stdbool.h>
//	An integer set is a memory vector of size max_numelm
typedef struct {
    int numelm;
    int max_numelm;
    int * data;
} set_t;

//	A new set created with numelm elements max
set_t * new_set( int max_numelm );

//	Duplicate set S1
void dup_set( const set_t * S1, set_t * S2 );

//	Build a new singleton set
set_t * singleton_set( const int elm );

//	Delete an existing set
void del_set( set_t ** ptrSet );

//	Is set S empty ?
bool empty_set( const set_t * S );

//	Print an existing set
void print_set( const set_t * S, char * entete );



//	Is element e in set S ?
bool in_set( const int e, const set_t * S );

//	Union of 2 sets
set_t * union_set( const set_t * S1, const set_t * S2 );

//	Intersection of 2 sets
set_t * inter_set( const set_t * S1, const set_t * S2 );

//	Extract element e from set S if it is present
void substract_set( set_t * S, const int e );

//	Add element e to set S if it not present
void add_set( set_t * S, const int e );
#endif //PROJET_SET_H