#include <stdio.h>
#include <stdlib.h>

#include "../include/list.h"
#include "../include/person.h"
#include "../include/friendship.h"
#include "../include/io.h"

int main(int argc, char * argv[]) {

    if (argc < 3) {
        printf("Usage : %s personnes.txt amities.txt\n", argv[0]);
        return 1;
    }

    // lecture du fichier de personnes -> construction de la liste triée
    list_t * Lpers = stream_2_person_list(argv[1], TEXT);

    // lecture du fichier d'amitiés -> construction de la liste + mise à jour person->friends
    list_t * Lfriends = stream_2_friendship_list(argv[2], TEXT, Lpers);

    view_list(Lpers,    &print_person,     "Liste des personnes");
    view_list(Lfriends, &print_friendship, "Liste des amities");

    // libération des amitiés en premier (elles pointent sur des personnes)
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
