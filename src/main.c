#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/list.h"
#include "../include/person.h"
#include "../include/friendship.h"
#include "../include/graph.h"
#include "../include/set.h"
#include "../include/io.h"

// retire de la liste L le noeud dont le datum est exactement le pointeur ptr
// on compare les adresses, pas les valeurs
static void remove_datum(list_t * L, void * ptr) {
    list_elm_t * E = L->head;
    while (E) {
        if (E->datum == ptr) {
            if (E->pred) E->pred->suc = E->suc;
            else         L->head = E->suc;
            if (E->suc)  E->suc->pred = E->pred;
            else         L->tail = E->pred;
            L->numelm--;
            del_list_elm(&E, NULL);
            return;
        }
        E = E->suc;
    }
}

int main(int argc, char * argv[]) {

    if (argc < 3) {
        printf("Usage : %s personnes.txt amities.txt\n", argv[0]);
        return 1;
    }

    list_t * Lpers    = stream_2_person_list(argv[1], TEXT);
    list_t * Lfriends = stream_2_friendship_list(argv[2], TEXT, Lpers);

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
            printf("8. Afficher les distances d'amitie\n");
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
                // scan_friendship cherche les deux personnes dans Lpers avant de créer le lien
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

                        // avant de supprimer P, on purge toutes ses amitiés
                        // sinon Lfriends et les friends des autres pointent vers de la mémoire libérée
                        list_elm_t * EF = Lfriends->head;
                        while (EF) {
                            friendship_t * F = EF->datum;
                            list_elm_t * next = EF->suc;
                            if (F->A == P || F->B == P) {
                                // si quelqu'un aimait P, on retire P de sa liste d'amis
                                if (F->B == P) remove_datum(F->A->friends, P);
                                // si P aimait quelqu'un, sa liste d'amis sera détruite avec P
                                if (EF->pred) EF->pred->suc = EF->suc;
                                else          Lfriends->head = EF->suc;
                                if (EF->suc)  EF->suc->pred = EF->pred;
                                else          Lfriends->tail = EF->pred;
                                Lfriends->numelm--;
                                del_list_elm(&EF, NULL);
                                free_friendship(&F, false);
                            }
                            EF = next;
                        }

                        if (E->pred) E->pred->suc = E->suc;
                        else         Lpers->head  = E->suc;
                        if (E->suc)  E->suc->pred = E->pred;
                        else         Lpers->tail  = E->pred;
                        Lpers->numelm--;
                        del_list_elm(&E, NULL);
                        free_person(&P, true);
                        printf("Personne supprimee.\n");
                        break;
                    }
                    E = E->suc;
                }
                break;
            }

            case 6: {
                // on reconstruit une amitié bidon avec juste les noms pour pouvoir comparer
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

                        // on retire F->B de la liste d'amis de F->A
                        // c'est F->A qui aimait F->B, donc B était dans ses amis
                        remove_datum(F->A->friends, F->B);

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

    // on libère d'abord les amitiés (elles pointent sur des personnes, mais del_persons=false)
    list_elm_t * EF = Lfriends->head;
    while (EF) {
        friendship_t * F = EF->datum;
        EF = EF->suc;
        free_friendship(&F, false);
    }
    del_list(&Lfriends, NULL);

    // ensuite les personnes avec leurs listes d'amis
    list_elm_t * EP = Lpers->head;
    while (EP) {
        person_t * P = EP->datum;
        EP = EP->suc;
        free_person(&P, true);
    }
    del_list(&Lpers, NULL);

    return 0;
}
