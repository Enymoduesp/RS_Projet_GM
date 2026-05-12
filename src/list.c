#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "../include/elmlist.h"
#include "../include/list.h"

/*********************************
DÉCLARATIONS DES FONCTIONS PRIVÉES
*********************************/
void insert_after ( list_t * L, void * datum, list_elm_t * place );

/**********************************
DÉFINITIONS DES FONCTIONS PUBLIQUES
**********************************/
list_t * new_list () {
  list_t * L = calloc ( 1, sizeof(list_t) );
  assert (L);
  return L;
}

void del_list ( list_t ** ptrL, void (*ptrf) () ){
  assert(ptrL && *ptrL);
  list_t * L = *ptrL;
  if(ptrf){
    for(list_elm_t * E = L->head; E ; ){
      list_elm_t * T = E;
      E = E->suc;
      (*ptrf)(T->datum);
      free(T);
    }
  } else {
    for(list_elm_t * E = L->head; E ; ){
      list_elm_t * T = E;
      E = E->suc;
      free(T);
    }
  }
  free(*ptrL);
  (*ptrL) = NULL;
}

bool empty_list ( list_t * L ) {
  assert (L);
  return L->numelm == 0;
}

list_elm_t * get_head ( list_t * L ) {
  assert (L);
  return L->head;
}

list_elm_t * get_tail ( list_t * L ) {
  assert (L);
  return L->tail;
}

void cons ( list_t * L, void * datum ) {
  list_elm_t * E = new_list_elm(datum);
  assert(L && E);
  if(!empty_list(L)) {
    set_pred(L->head, E);
    set_suc(E, L->head);
  }
  L->head = E;
  set_pred(E, NULL);
  if (empty_list(L))
    L->tail = E;
  L->numelm += 1;
}

void queue ( list_t * L, void * datum ) {
  list_elm_t * E = calloc(1, sizeof(list_elm_t));
  assert(E);
  E->datum = datum;
  E->suc = NULL;
  E->pred = L->tail;
  if(L->numelm == 0){
    L->head = E;
    L->tail = E;
  }
  else{
    L->tail->suc = E;
    L->tail = E;
  }
  L->numelm++;
}

void insert_ordered ( list_t * L, void * datum, int (*cmp_ptrf) () ) {
  assert(L);
  if(empty_list(L)){
    cons(L, datum);
  }
  else if(cmp_ptrf(datum, L->head->datum) <= 0){
    cons(L, datum);
  }
  else if(cmp_ptrf(datum, L->tail->datum) >= 0){
    queue(L, datum);
  }
  else{
    list_elm_t * E = L->head;
    while(E->suc != NULL &&
          cmp_ptrf(datum, E->suc->datum) > 0){
      E = E->suc;
          }
    insert_after(L, datum, E);
  }
}

void * find ( list_t * L, void * datum, int (*ptrFct) () ) {
  assert(L);
  list_elm_t * E = L->head;
  while (E) {
    if (ptrFct(E->datum, datum) == 0)
      return E->datum;
    E = E->suc;
  }
  return NULL;
}

void view_ligne ( int length ) {
  printf ( "\t\t" );
	for(int c=0; c < length; c += 1 ) printf ( "=");
	printf ( "\n" );
}

void view_list ( list_t * L, void (*view_fct_ptr) (), char * entete) {
	int lg = strlen ( entete ) + 5;
	view_ligne ( lg );
  printf ( "\t\t|| %s ||\n", entete );
	view_ligne ( lg );
  if ( empty_list ( L ) ) {
    printf("[ ] //empty list\n");
  }
  else {
    list_elm_t * iterator = L->head;
    while ( iterator ) {
      (*view_fct_ptr) ( iterator->datum );
			printf ( "\n" );
      iterator = iterator->suc;
    }
  }
  view_ligne ( lg );
}

/*******************************************
Définitions des fonctions privées du TA list
*******************************************/

void insert_after ( list_t * L, void * datum, list_elm_t * place) {
  assert (L);
  if ( empty_list ( L ) || !place ) {
    cons ( L, datum );
  }
  else if ( place == L->tail ) {
    queue ( L, datum );
  }
  else {
    list_elm_t * E = new_list_elm( datum );
    E->pred = place;
    E->suc = place->suc;
    place->suc->pred = E;
    place->suc = E;
    L->numelm += 1;
  }
}