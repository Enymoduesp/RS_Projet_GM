#include <stdio.h>
#include <stdlib.h>

#include "../include/list.h"
#include "../include/person.h"
#include "../include/friendship.h"
#include "../include/graph.h"
#include "../include/set.h"
#include "../include/io.h"
#include "../include/elmlist.h"

int main(int argc, char * argv[]) {

    list_t * Lpers;
    list_t * Lfriends;

    graph_t * G;

    int choix;

    if(argc < 3){
        printf("Usage : %s personnes.txt amities.txt\n", argv[0]);
        return 1;
    }

    /* lecture des fichiers */
    Lpers = stream_2_person_list(argv[1], TEXT);
    Lfriends = stream_2_friendship_list(argv[2], TEXT, Lpers);

    do{

        printf("\n");
        printf("========== KILOGRAM ==========\n");
        printf("1. Afficher les personnes\n");
        printf("2. Afficher les amities\n");
        printf("3. Ajouter une personne\n");
        printf("4. Ajouter une amitie\n");
        printf("5. Supprimer une personne\n");
        printf("6. Supprimer une amitie\n");
        printf("7. Afficher le graphe\n");
        printf("8. Afficher la clique maximale\n");
        printf("0. Quitter\n");

        printf("Choix : ");
        scanf("%d", &choix);

        switch(choix){

            case 1:

                view_list(
                    Lpers,
                    &print_person,
                    "Liste des personnes"
                );

                break;

            case 2:

                view_list(
                    Lfriends,
                    &print_friendship,
                    "Liste des amities"
                );

                break;

            case 3:{

                person_t * P;

                P = scan_person();

                insert_ordered(
                    Lpers,
                    P,
                    &cmp_person
                );

                printf("Personne ajoutee\n");

                break;
            }

            case 4:{

                friendship_t * F;

                F = scan_friendship(Lpers);

                queue(
                    Lfriends,
                    F
                );

                printf("Amitie ajoutee\n");

                break;
            }

            case 5:{

                person_t tmp;
                list_elm_t * E;

                printf("Nom : ");
                scanf("%s", tmp.name);

                printf("Prenom : ");
                scanf("%s", tmp.forename);

                E = Lpers->head;

                while(E){

                    person_t * P = E->datum;

                    if(cmp_person(P, &tmp) == 0){

                        person_t * Ptmp = E->datum;

                        if(E->pred)
                            E->pred->suc = E->suc;
                        else
                            Lpers->head = E->suc;

                        if(E->suc)
                            E->suc->pred = E->pred;
                        else
                            Lpers->tail = E->pred;

                        del_list_elm(&E, NULL);

                        free_person(&Ptmp, false);

                        Lpers->numelm--;

                        printf("Personne supprimee\n");

                        break;
                    }

                    E = E->suc;
                }

                break;
            }

            case 6:{

                list_elm_t * E;

                E = Lfriends->head;

                if(E){

                    friendship_t * Ftmp = E->datum;

                    if(E->pred)
                        E->pred->suc = E->suc;
                    else
                        Lfriends->head = E->suc;

                    if(E->suc)
                        E->suc->pred = E->pred;
                    else
                        Lfriends->tail = E->pred;

                    del_list_elm(&E, NULL);

                    free_friendship(&Ftmp, false);

                    Lfriends->numelm--;

                    printf("Premiere amitie supprimee\n");
                }

                break;
            }

            case 7:

                G = list2matri
                    Lpers,
                    Lfriends
                );

                distance_calculus(G);

                printf_graph(
                    G,
                    "Graphe",
                    true
                );

                del_graph(&G);

                break;

            case 8:{

                set_t * R;
                set_t * P;
                set_t * X;
                set_t * C;

                int i;

                G = list2matriLpers, Lfriends);

                R = new_set(G->num_vertices);
                P = new_set(G->num_vertices);
                X = new_set(G->num_vertices);
                C = new_set(G->num_vertices);

                for(i = 0; i < G->num_vertices; i++)
                    add_set(P, i);

                BronKerbosch(G, R, P, X, C);

                print_set(
                    C,
                    "Clique maximale"
                );

                del_set(&R);
                del_set(&P);
                del_set(&X);
                del_set(&C);

                del_graph(&G);

                break;
            }

            case 0:

                printf("Au revoir\n");

                break;

            default:

                printf("Choix invalide\n");
        }

    }while(choix != 0);

    return 0;
}