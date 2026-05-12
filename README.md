# RS_Projet_GM
Si pas de compilateur dans l'IDE (CLION par exemple), utiliser le Makefile:
Dans le terminal --> 
## Si version statique: 
    make run ARGS="NOM_FICHIER_A_LIRE"
## Si version dynamique: 
    make run ARGS="NOM_FICHIER_PERSONNE NOM_FICHIER_AMITIE"

Le projet est le projet Kilogram de gestion de mémoire.
C'est un réseau social, sous forme de graph manipulé avec des lists et des matrices.

Concernant les matrices, voici quelques points importants:\
### la matrice d'adjacence sous la forme


     0   1   2
    [0 | 1 | 1 |
    [1 | 0 | 0 |
    [1 | 0 | 0 |

se lit :\
ici, 0 est ami avec 1 et 2.\
1 ami avec 0\
2 ami avec 0\
(un 1 signifie que ligne ami avec colonne)

### la matrice des distances:
elle fonctionne de la meme manière que la matrice d'adjacances mais 1 = ami direct, 2 = chemin passe par un sommet intermédiaire etc...



### La fonction des distances:
````
int distance2p(graph_t * G, int a, int b, int * exclu)
````
Cette fonction est une fonction récursive qui fonctionne comme ça:\
Cas de base:\
&emsp;Si a = b alors on retourne 0\
&emsp;Si a est ami avec b alors on retourne 1\

recursion:\
    &emsp;exclu est la liste des personne par qui on est déjà passé (pour eviter de tourner en rond ou calculer des chemins inutiles) \
   &emsp; elle est sous la forme d'un tableau. Si exclu[i] = 1 alors on est déja passé par i.\
&emsp;On met donc a et b dans la liste car on est passé par a et qu'on a déjà verifié le lien avec b haut dessus\
&emsp;ensuite pour chaque personne du graph\
&emsp; &emsp;si la personne n'est pas exclu alors\
&emsp; &emsp; &emsp;Si a est ami avec la personne (ya un lien qu'on peut possiblement continuer jusqu'à b)\
&emsp; &emsp; &emsp; &emsp; la distance = 1 (le lien a-->i) + la distance entre i et b\
&emsp; &emsp; &emsp; &emsp; Si d est inferieur à mind (mind initalisé à la valeur max possible + 1) ET que d différent de 0 car d = 0 signifie que pas de chemin entre i et b (car 1 + -1 = 0) \
&emsp; &emsp; &emsp; &emsp; &emsp; alors mind prend la valeur de d pour les prochains test = le chemin le plus cours pour l'instant est de d\
&emsp; sortie de la boucle\
&emsp; on a mind qui a le chemin le plus court si yen a un\
&emsp;  on désexclu a pour éviter probleme lié à la récursion et le fait que exclu est partagé entre les appels\
&emsp;  on désexclu également b car on ne veut pas qu'il soit sauté lors de l'appel dans distance calculus pour un autre lien (par ex c, d)
&emsp; &emsp; (0->1->2->3->4 et 0->2->3->4 (on aurait exclu 2 pdt la recherche sur 1) 2 chemins peuvent avoirs des sommets communs qu'il faut revisiter)\
&emsp; enfin si mind a gardé la valeur de l'initialistion alors pas de chemin, on retourne -1\
&emsp; sinon on retourn la distance minimal entre a et b

On appelle en ensuite cette fonction pour entre chaque personnes et on remplit la matrice des distances

(pas utilisé de BFS car je n'en ai entendu parlé que plus tard et pas envie de tout recommencer...)

### la fonction Bronker-Bauch

````
void BronKerbosch ( const graph_t * G, const set_t * R, set_t * P, set_t * X, set_t * C )
````
encore une fois une fonction récursive pour trouver la clique maximale dans un graphe symétrique\
clique max = l'ensemble de gens le plus grand dans lequel tout le monde est ami direct avec tout le monde

R : clique en cours de construction\
P : candidats restants à ajouter à la clique\
X : sommets déjà traités ou appartenant déjà à une clique maximale.\
C : meilleure clique maximale trouvée jusqu'ici\

Cas de base:\
&emsp;Si il n'y a plus de candidats à ajouter et que aucun sommet adjacents à ceux de R ont déjà été traité = font partie d'une autre clique alors\
&emsp;&emsp; Si cette clique est plus grande que la précédente clique max trouvé, elle devient la nouvelle clique max\
Cas Recursif:\
    Tant que l'ensemble des personnes à traiter n'est pas vide = pour toutes les personnes:\
        &emsp; on créer un ensemble contenant la personne pour manipuler les ensembles\
&emsp; On ajoute la personne dans la clique en cours de construction, le R qu'on donnera dans l'appel recursif\
&emsp; On créer un ensemble contenant tous les amis direct de la personne\
&emsp; On fait le prochain X pour l'appel récursif qui sera les gens amis de la personne qui font déjà partie d'une clique, on va pas reexplorer les clique prenant 1 et 0 si on a déjà fais 0 et 1.\
&emsp; On fait le prochain P pour l'appel récursif qui sera ceux qui sont encore candidats et ami avec la personne\
&emsp; on appel Bronkerbosch avec ces nouveaux parametres qui permettra de calculer la clique max avec dedans le nouveau R et de la mettre dans C si elle est la plus grande jusqu'ici\
&emsp; on libere la memoire des ensembles qu'on a créer pour l'appel récursif (hormis C évidemment communs à tous)
&emsp; on enleve la personnne actuelle des candidats à tester (de P)
&emsp; on l'ajoute à X puisqu'on a caclulé sa clique max, qu'on la déjà traité

Dans le programme principal, le main, on appelle simplement cette fonction avec R, X, C des ensembleVide et P un ensemble qui a toutes les personnes. 
A la sortie, C aura la clique max du graph. 

Evidemment, puisqu'on parle de clique, tout cela ne fonctionne qu'en cas où le graph est symétrique\
(par exemple, quand on va mettre a jour le P pour l'appel récursif, inter(La personne, P) peut contenir des gens qui ne sont pas amis avec La personne, et la clique contiendra un lien qui ne va que dans un sens (donc pas une clique finalement))


### list2matrix
enfin list2matrix, qui permet de passer d'une représentation en liste à celle en matrice. \
On parcours chaque personne de la liste à qui on associe un i. (0 pour le premier, 1 pour le deuxieme...)\
   &emsp; Pour chaque personne on regarde chaque ami de sa liste d'ami\
    &emsp; &emsp; on met dans une variable l'ami\
    &emsp; &emsp; Puis On parcours toutes les personnes de la liste en partant du premier de nouveau (sans toucher au premier parcours) et en associant là aussi un j (0 pour le premier...)\
    &emsp;&emsp;  Pour chacun, on verifie si la personne sur laquelle on est l'ami qu'on cherche. Si c'est lui, alors on met un 1 a la ligne i et colonne j




    