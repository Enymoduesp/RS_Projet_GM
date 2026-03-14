#pragma once

typedef struct {
    int nbLig;
    int nbCol;
	double * values;
} matrix_t;

matrix_t * consMatrix ( int nlig, int ncol );
// matrix_t * cpyMatrix( matrix_t * M );
void freeMatrix( matrix_t ** ptrM );
void printMatrix( matrix_t * M, char * entete );
matrix_t * scanMatrix();
// matrix_t * addMatrix ( matrix_t *  A, matrix_t *  B );
// matrix_t * multMatrix ( matrix_t *  A, matrix_t *  B );