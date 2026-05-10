#include <stdio.h>

//%nd aligne à droite sur mini n caractère et %-nd à gauche sur mini n caractère
void printMatrix(int * M, int taille)
{
    int i, j;
    printf("    |");
    for (i = 0; i < taille - 1; i++)
        printf("%d | ", i); //partie haute du "tableau"
     printf("%d|\n", i);
    for (i = 0; i < taille; i++)
    {   printf("\n");
        for (j = 0; j < taille -1; j++)
        {
            if (j == 0)
                printf("%2d  [", i); //partie gauche du "tableau"
            printf("%d | ", M[i * taille + j]);
        }
        printf("%d]", M[i * taille + j]);

    }
}