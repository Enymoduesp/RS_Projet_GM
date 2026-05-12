#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/list.h"
#include "../include/person.h"
#include "../include/friendship.h"
#include "../include/graph.h"
#include "../include/set.h"
#include "../include/io.h"

// retire de la liste L le noeud dont le datum == ptr (comparaison d'adresses)
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

//si statique, argv[1] : fichier à traiter du graph si dynamique argv[1] = liste person et argv[2] = liste friends
int main(int argc, char * argv[]) {

    int statoudyn;
    int choix;

    if (argc < 2) //si ya pas de fichier qui a été donné à l'appel
    {
        printf("merci de donner un fichier à traiter");
        return 1;
    }

    do //on demande si menu statique ou dynamique
    {
        printf("Mode statique ou dynamique? \n 1: statique\n 2: dynamique\n");
        scanf("%d", &statoudyn);
    }while (statoudyn <1 || statoudyn>2);

    switch (statoudyn)
    {
        case 1: //cas statique
            {
                int binoutxt;
                graph_t * G;
                do
                {
                    printf("lire la matrice d'un fichier txt ou bin?\n 1: txt\n 2: bin\n");
                    scanf("%d", &binoutxt);
                }while (binoutxt <1 || binoutxt>2);
                //on suppose argv[1] = fichier à lire
                if (binoutxt == 1) //si c du text on lit avec la fonction pour txt
                {
                    G = scanTXT_graph(argv[1]);
                }
                else //c du BIN
                {
                    G = scanBIN_graph(argv[1]);
                }
                bool symetric = symmetric_graph(G); //pour savoir si symétrique
                do
                {
                    printf("\n========== KILOGRAM ==========\n");
                    printf("1. Afficher matrice adjacences\n");
                    printf("2. Afficher matrice adjacences et distances\n");
                    printf("3. Ecrire graph dans un BIN\n");
                    printf("4. Ecrire graph dans un TXT\n");
                    if (symetric)
                        printf("5. clique maximale\n");
                    printf("0. Quitter\n");
                    printf("Choix : ");
                    scanf("%d", &choix);

                    switch (choix)
                    {
                        case 1: //afficher adjacences
                            {
                                printf_graph(G, "matrice d'adjacences", false);
                                break;
                            }
                        case 2: //ajdacences + distances
                            {
                                distance_calculus(G); // on caclul les distances d'amitié
                                printf_graph(G, "matrice d'adjacences puis distances", true);
                                break;
                            }

                        case 3: //ecriture dans txt
                            {
                                fprintGraphTXT(G, "../GraphEcriture.txt");
                                break;
                            }

                        case 4: //ecriture dans bin
                            {
                                fprintGraphBIN(G, "../GraphEcriture.bin");
                                break;
                            }

                        case 5:
                            {
                                if (!symetric) //si pas symetric
                                {
                                    printf("mauvais choix, graph pas symetrique");
                                    break;
                                }
                                //graph symetric
                                int nbGens = G->num_vertices;
                                //on fait les ensembles de maximum nbgens
                                set_t * R = new_set(nbGens); //clique en construct donc rien au debut
                                set_t * X = new_set(nbGens); //personne déja traité
                                set_t * C = new_set(nbGens); //clique max
                                set_t * P = new_set(nbGens); //candidat à la clique max (faut mettre tout le monde)
                                int i;
                                for (i = 0; i < nbGens; i++)
                                    add_set(P, i); //on ajoute chaque humain à l'ensemble
                                BronKerbosch (G, R, P, X, C);
                                print_set(C, "La clique maximale :");
                                //on libere tous les ensembles.
                                del_set(&X);
                                del_set(&P);
                                del_set(&R);
                                del_set(&C);
                                break;
                            }
                    }
                }while (choix != 0 && choix < 6);
                del_graph(&G); //on a fini on peut libérer le graph
                break;
            }




        case 2: //cas dynamique
            //mettre tout ce qu'il y a en dessous?
            break;
    }
    if (argc < 3) {
        printf("Usage : %s personnes.txt amities.txt\n", argv[0]);
        return 1;
    }

    list_t * Lpers    = stream_2_person_list(argv[1], TXT);
    list_t * Lfriends = stream_2_friendship_list(argv[2], TXT, Lpers);


    do {
<<<<<<< HEAD
        // on convertit en matrice juste pour savoir si le graphe est symétrique
        // comme ça le menu s'adapte (distances et clique max seulement si réciprocité)
        graph_t * Gtmp = list2matrix(Lpers);
=======
        // on rebuild la matrice à chaque tour pour tester la symétrie
        // le menu s'adapte : distances et clique max seulement si graphe symétrique
        graph_t * Gtmp = list2matrix(Lpers, Lfriends);
>>>>>>> feature/menu
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

                        // on supprime toutes les amitiés liées à P avant de le supprimer
                        // sinon Lfriends et les friends des autres auraient des pointeurs morts
                        list_elm_t * EF = Lfriends->head;
                        while (EF) {
                            friendship_t * F = EF->datum;
                            list_elm_t * next = EF->suc;
                            if (F->A == P || F->B == P) {
                                // si X aimait P, on retire P de la liste d'amis de X
                                if (F->B == P) remove_datum(F->A->friends, P);
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

                        // F->A aimait F->B, donc F->B est dans F->A->friends : on le retire
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
                graph_t * G = list2matrix(Lpers);
                printf_graph(G, "Graphe d'adjacences", false);
                del_graph(&G);
                break;
            }

            case 8: {
                if (!sym) { printf("Choix invalide.\n"); break; }
                graph_t * G = list2matrix(Lpers);
                distance_calculus(G);
                printf_graph(G, "Distances d'amitie", false);
                del_graph(&G);
                break;
            }

            case 9: {
                if (!sym) { printf("Choix invalide.\n"); break; }
                graph_t * G = list2matrix(Lpers);
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

    // libération des amitiés d'abord (pointeurs sur personnes, del_persons=false)
    list_elm_t * EF = Lfriends->head;
    while (EF) {
        friendship_t * F = EF->datum;
        EF = EF->suc;
        free_friendship(&F, false);
    }
    del_list(&Lfriends, NULL);

    // libération des personnes avec leurs listes d'amis
    list_elm_t * EP = Lpers->head;
    while (EP) {
        person_t * P = EP->datum;
        EP = EP->suc;
        free_person(&P, true);
    }
    del_list(&Lpers, NULL);

    return 0;
}
