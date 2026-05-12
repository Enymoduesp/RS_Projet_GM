#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

#include "../include/friendship.h"
#include "../include/person.h"
#include "../include/list.h"

friendship_t * new_friendship(){
	friendship_t * F = calloc ( 1, sizeof(friendship_t) );
	assert ( F );
	return F;
}

friendship_t * scan_friendship(list_t * Lpers){
  friendship_t * F = new_friendship ();
	printf ( "Définissez une amitié entre \n" );
	person_t * P = scan_person();
	person_t * P1 = find (Lpers, P, &cmp_person);
	assert ( P1 );
	free_person (&P, true); // P est un temporaire créé par scan_person, on libère tout
	F->A = P1;
	printf(" et \n");
	P = scan_person();
	person_t * P2 = find (Lpers, P, &cmp_person);
	assert(P2);
	free_person(&P, true); // idem
	F->B = P2;
	//on met à jour la liste d'amis de chaque personne
	//A aime B donc B apparait dans les amis de A
	queue(P1->friends, P2);
	return F;
}

void free_friendship(friendship_t ** ptrFriend, bool del_persons){
	assert(ptrFriend && *ptrFriend);
	if (del_persons) {
		free_person(&(*ptrFriend)->A, NULL);
		free_person(&(*ptrFriend)->B, NULL);
	}
	free(*ptrFriend);
	*ptrFriend = NULL;
}

int cmp_friendship(friendship_t * F1, friendship_t * F2){
	if ((((cmp_person(F1->A, F2->A) == 0) && (cmp_person(F1->B, F2->B )== 0))) || (((cmp_person(F1->A, F2->B) == 0) && (cmp_person(F1->B, F2->A)) == 0)))
		return 0;
	return 1;
}

void print_friendship(friendship_t * F){
	printf ( "___________Friendship___________\n" );
	print_person ( F->A );
	printf ( "\n" );
	print_person ( F->B );
	printf ( "\t\t\t\tS'AIMENT D'AMOUR\n" );
	printf ( "________Friendship ended________\n" );
}
