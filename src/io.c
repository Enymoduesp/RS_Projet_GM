#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "../include/io.h"
#include "../include/list.h"
#include "../include/person.h"
#include "../include/friendship.h"

/**
	@note une zone tampon utile aux fichiers binaires
*/
typedef struct {
	char nameA[LG];
	char forenameA[LG];
	char nameB[LG];
	char forenameB[LG];
} buffer_t;

list_t * stream_2_person_list ( char * filename, stream_mode_t mode ) {
	FILE * fd;
	list_t * L = new_list();
	person_t * P;

	if(mode == TEXT)
		fd = fopen(filename, "r");
	else
		fd = fopen(filename, "rb");

	assert(fd);

	while((P = person_from_stream(fd, mode)) != NULL){
		insert_ordered(L, P, &cmp_person);
	}

	fclose(fd);
	return L;
}

list_t * stream_2_friendship_list ( char * filename, stream_mode_t mode, list_t * Lpers ) {
	FILE * fd;
	list_t * L = new_list();
	friendship_t * F;

	if(mode == TEXT)
		fd = fopen(filename, "r");
	else
		fd = fopen(filename, "rb");

	assert(fd);

	while((F = friendship_from_stream(fd, mode, Lpers)) != NULL){
		queue(L, F);
	}

	fclose(fd);
	return L;
}

person_t * person_from_stream ( FILE * stream, stream_mode_t mode ) {
	person_t * P = NULL;

	assert(stream);

	if(mode == TEXT){
		P = new_person();

		if(fscanf(stream, "%s %s %d/%d/%d",
			P->name,
			P->forename,
			&(P->birth_date.day),
			&(P->birth_date.month),
			&(P->birth_date.year)) != 5){
			free_person(&P, false);
			return NULL;
			}
	}
	else{
		P = new_person();

		if(fread(P, sizeof(person_t), 1, stream) != 1){
			free_person(&P, false);
			return NULL;
		}

		P->friends = new_list();
	}

	return P;
}

void person_2_stream(person_t * P, FILE * fd, stream_mode_t mode){

	assert(P);
	assert(fd);

	if(mode == TEXT){

		fprintf(fd, "%s %s %d/%d/%d\n",
			P->name,
			P->forename,
			P->birth_date.day,
			P->birth_date.month,
			P->birth_date.year
		);
	}
	else{

		fwrite(P->name, sizeof(char), LG, fd);
		fwrite(P->forename, sizeof(char), LG, fd);

		fwrite(&(P->birth_date.day), sizeof(int), 1, fd);
		fwrite(&(P->birth_date.month), sizeof(int), 1, fd);
		fwrite(&(P->birth_date.year), sizeof(int), 1, fd);
	}
}

friendship_t * friendship_from_stream(FILE * stream, stream_mode_t mode, list_t * Lpers){
	friendship_t * F = NULL;
	person_t tmpA, tmpB;
	person_t * A;
	person_t * B;
	buffer_t buff;

	assert(stream);
	assert(Lpers);

	if(mode == TEXT){
		if(fscanf(stream, "%s %s %s %s",
			buff.nameA,
			buff.forenameA,
			buff.nameB,
			buff.forenameB) != 4){
			return NULL;
			}
	}
	else{
		if(fread(&buff, sizeof(buffer_t), 1, stream) != 1){
			return NULL;
		}
	}

	strcpy(tmpA.name, buff.nameA);
	strcpy(tmpA.forename, buff.forenameA);

	strcpy(tmpB.name, buff.nameB);
	strcpy(tmpB.forename, buff.forenameB);

	A = find(Lpers, &tmpA, &cmp_person);
	B = find(Lpers, &tmpB, &cmp_person);

	if(!A || !B)
		return NULL;

	F = new_friendship();
	F->A = A;
	F->B = B;

	return F;
}
