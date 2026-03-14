#include <stdio.h>
#include <stdlib.h>
#include "../include/matrix.h"
#include <assert.h>

matrix_t * consMatrix ( int nlig, int ncol ){
    matrix_t * M = calloc(1,sizeof(matrix_t));
    (*M).nbLig = nlig;
    M->nbCol = ncol;
    M->values = calloc(nlig*ncol,sizeof(double));
    return M;
}
void printMatrix( matrix_t * M, char * entete ){
    printf("%s (%d,%d\n)",entete,M->nbLig,M->nbCol);
    for(int l=0; l<M->nbLig; l++){
        for(int c=0; c<M->nbCol; c++){
            int k = l*M->nbCol+c;
            printf("%lf\t",M->values[k]);
        }
        printf("\n");
    }
}
matrix_t * scanMatrix(){
    int NL, NC;
    printf("NL, NC =");
    scanf(" %lf %lf",&NL,&NC);
    matrix_t * M = consMatrix(NL,NC);
    for(int l=0; l<NL; l++){
        for(int c=0; c<NC; c++){
            int k=l*NC+c;
            printf("M[%d,%d] =",l,c);
            scanf(" %lf", M->values+k);// l'adresse de la kième cellule de values
        }
    }
    return M;
}
void freeMatrix( matrix_t ** ptrM ){
    assert(ptrM && *ptrM);
    free((*ptrM)->values);
    free(*ptrM);
    *ptrM = NULL;
}
