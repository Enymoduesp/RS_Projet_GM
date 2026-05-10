#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/list.h"
#include "../include/person.h"
#include "../include/friendship.h"
#include "../include/graph.h"
#include "../include/set.h"
#include "../include/io.h"

int main(int argc, char * argv[]) {

    if (argc < 3) {
        printf("Usage : %s personnes.txt amities.txt\n", argv[0]);
        return 1;
    }

    list_t * Lpers    = stream_2_person_list(argv[1], TXT);
    list_t * Lfriends = stream_2_friendship_list(argv[2], TXT, Lpers);

    int choix;

    do {
        // on convertit en matrice juste pour savoir si le graphe est symétrique
        // comme ça le menu s'adapte (distances et clique max seulement si réciprocité)
        graph_t * Gtmp = list2matrix(Lpers, Lfriends);
        int sym = symmetric_graph(Gtmp);
        del_graph(&Gtmp);

        printf("\n========== KILOGRAM ==========\n");
        printf("1. Afficher les personnes\n");
        printf("2. Afficher les amities\n");
        printf("3. Ajouter une personne\n");
        printf("4. Ajouter une amitie\n");
        printf("5. Supprimer une personne\n");
        printf("6. Supprimer une amitie\n");
        printf("7. Afficher le graphe\n");
        if (sym) {
            printf("8. Afficher les distances d'amitie\n"); //on peut le faire tout le temps mais le sujet demande uniquement si symétrique?
            printf("9. Afficher la clique maximale\n");
        }
        printf("0. Quitter\n");
        printf("Choix : ");
        scanf("%d", &choix);

        switch (choix) {

            case 1:
                view_list(Lpers, &print_person, "Liste des personnes");
                break;

            case 2:
                view_list(Lfriends, &print_friendship, "Liste des amities");
                break;

            case 3: {
                person_t * P = scan_person();
                insert_ordered(Lpers, P, &cmp_person);
                printf("Personne ajoutee.\n");
                break;
            }

            case 4: {
                friendship_t * F = scan_friendship(Lpers);
                if (F) {
                    queue(Lfriends, F);
                    printf("Amitie ajoutee.\n");
                } else {
                    printf("Personne(s) introuvable(s).\n");
                }
                break;
            }

            case 5: {
                person_t tmp;
                printf("Nom : ");    scanf("%s", tmp.name);
                printf("Prenom : "); scanf("%s", tmp.forename);

                list_elm_t * E = Lpers->head;
                while (E) {
                    if (cmp_person(E->datum, &tmp) == 0) {
                        person_t * P = E->datum;
                        if (E->pred) E->pred->suc = E->suc;
                        else         Lpers->head  = E->suc;
                        if (E->suc)  E->suc->pred = E->pred;
                        else         Lpers->tail  = E->pred;
                        Lpers->numelm--;
                        del_list_elm(&E, NULL);
                        free_person(&P, false);
                        printf("Personne supprimee.\n");
                        break;
                    }
                    E = E->suc;
                }
                break;
            }

            case 6: {
                // on crée une amitié temporaire avec juste les noms pour la comparaison
                person_t tmpA, tmpB;
                printf("Nom A : ");    scanf("%s", tmpA.name);
                printf("Prenom A : "); scanf("%s", tmpA.forename);
                printf("Nom B : ");    scanf("%s", tmpB.name);
                printf("Prenom B : "); scanf("%s", tmpB.forename);

                friendship_t cible;
                cible.A = &tmpA;
                cible.B = &tmpB;

                list_elm_t * E = Lfriends->head;
                while (E) {
                    if (cmp_friendship(E->datum, &cible) == 0) {
                        friendship_t * F = E->datum;
                        if (E->pred) E->pred->suc = E->suc;
                        else         Lfriends->head = E->suc;
                        if (E->suc)  E->suc->pred  = E->pred;
                        else         Lfriends->tail = E->pred;
                        Lfriends->numelm--;
                        del_list_elm(&E, NULL);
                        free_friendship(&F, false);
                        printf("Amitie supprimee.\n");
                        break;
                    }
                    E = E->suc;
                }
                break;
            }

            case 7: {
                graph_t * G = list2matrix(Lpers, Lfriends);
                printf_graph(G, "Graphe d'adjacences", false);
                del_graph(&G);
                break;
            }

            case 8: {
                if (!sym) { printf("Choix invalide.\n"); break; }
                graph_t * G = list2matrix(Lpers, Lfriends);
                distance_calculus(G);
                printf_graph(G, "Distances d'amitie", true);
                del_graph(&G);
                break;
            }

            case 9: {
                if (!sym) { printf("Choix invalide.\n"); break; }
                graph_t * G = list2matrix(Lpers, Lfriends);
                int n = G->num_vertices;
                set_t * R = new_set(n);
                set_t * P = new_set(n);
                set_t * X = new_set(n);
                set_t * C = new_set(n);
                for (int i = 0; i < n; i++) add_set(P, i);
                BronKerbosch(G, R, P, X, C);
                print_set(C, "Clique maximale");
                del_set(&R); del_set(&P); del_set(&X); del_set(&C);
                del_graph(&G);
                break;
            }

            case 0:
                printf("Au revoir !\n");
                break;

            default:
                printf("Choix invalide.\n");
        }

    } while (choix != 0);

    del_list(&Lpers, NULL);
    del_list(&Lfriends, NULL);
    return 0;
}