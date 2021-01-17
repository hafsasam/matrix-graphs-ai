#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include "Windows.h"
#include "psapi.h"
#define WINVER 0x0500

#include "graphemat.h"
#include "liregraphe.h"

typedef GrapheMat Graphe;

//pour calculer le temps d'execution de l'algo
struct timeval start,stop;
double sec_profondeur, sec_largeur, sec_coutU, sec_PL, sec_IP, sec_G, sec_A;

int menu () {
  printf("+-------------------+------------------+-----------------+-----------------+---------------+------------+-------------+");

  printf ("\n\n\t\t\t\t\tGRAPHES\n\n");
  printf("+-------------------+------------------+-----------------+-----------------+---------------+------------+-------------+");
  printf ("\n");
  printf ("0 - Fin du programme\n");
  printf ("1 - Creation a partir d'un fichier\n");
  printf("+-------------------+------------------+-----------------+-----------------+---------------+------------+-------------+");

  printf ("\n");
  printf ("2 - Initialisation d'un graphe vide\n");
  printf ("3 - Ajout d'un sommet\n");
  printf ("4 - Ajout d'un arc\n");
  printf ("5 - Liste des sommets et des arcs\n");
  printf ("6 - Destruction du graphe\n");
  printf("+-------------------+------------------+-----------------+-----------------+---------------+------------+-------------+");

  printf ("\n");

  printf ("\n");
  printf ("7 - Parcours avec Cout Unifom d'un graphe\n");
  printf ("8 - Parcours en profondeur d'un graphe\n");
  printf ("9 - Parcours en largeur d'un graphe\n");
  printf ("10 - Parcours en profondeur limitee d'un graphe\n");
  printf ("11 - Parcours en profondeur limitee iterative d'un graphe\n");
  printf ("12 - Parcours avec A* \n");
  printf ("100 - Tous les parcours \n");
  printf("+-------------------+------------------+-----------------+-----------------+---------------+------------+-------------+");
  printf ("\n");
  printf ("Votre choix ? ");
  printf ("\n");
  printf("+-------------------+------------------+-----------------+-----------------+---------------+------------+-------------+");
  printf ("\n");
  int cod; scanf ("%d", &cod); getchar();
  printf ("\n");
  return cod;
}
int main () {
  Graphe* graphe;
  booleen fini = faux;
  while (!fini) {
    switch ( menu() ) {
        case 0:
            fini = vrai;
            break;
        case 1: {  // création à partir d'un fichier
            printf ("Nom du fichier contenant le graphe ? ");
            char nomFe [50];
            scanf  ("%s", nomFe);
            //strcpy (nomFe, "graphe.txt");
            FILE* fe = fopen (nomFe, "r");
            if (fe == NULL) {
                perror (nomFe);
            } else {
                graphe = lireGraphe (fe, 20); // 20 sommets maximum
                fclose (fe);
            }
            break;
       }
       case 2: {             // création d’un graphe vide
            printf ("Nombre maximum de sommets ? ");
            int nMaxSom; scanf  ("%d", &nMaxSom);
            printf ("0) non valué; 1) graphe valué ? ");
            int value; scanf ("%d", &value);
            graphe = creerGrapheMat (nMaxSom, value);
            break;
        }
        case 3: {  // ajouter un sommet
            printf ("Nom du sommet à insérer ? ");
            NomSom somD; scanf  ("%s", somD);
            ajouterUnSommet (graphe, somD);
            break;
        }
        case 4: {  // ajouter un arc
            printf ("Nom du sommet de départ ? ");
            NomSom somD; scanf  ("%s", somD);
            printf ("Nom du sommet d'arrivée ? ");
            NomSom somA; scanf  ("%s", somA);
            int cout;
            if (graphe->value) {
                printf ("Cout de la relation ? ");
                scanf ("%d", &cout);
            } else {
                cout = 0;
            }
            ajouterUnArc (graphe, somD, somA, cout);
            break;
        }
        case 5:
            ecrireGraphe(graphe);
            break;
        case 6:
            detruireGraphe(graphe);
            break;
        case 7:
            printf("\n Exploration a cout uniforme\n");
            parcoursLargeurUniform(graphe,"H");
            break;
        case 8:
            printf("\n Exploration en profondeur d'abord\n");
            parcoursProfond (graphe);
            break;
        case 9:
            printf("\n Exploration en largeur d'abord\n");
            parcoursLargeur(graphe);
            break;
        case 10:
            printf("\n Exploration en profondeur limitee\n");
            profondeurLimitee(graphe, 3, "H");
            break;
        case 11:
            printf("\n Exploration iterative en profondeur\n");
            profondeurLimiteeIt(graphe, "H");
            break;
        case 12:
            printf("Exploration A*\n");
            IA(graphe,"H");
            break;
        case 100 :
            printf("Exploration a cout uniforme\n");
            gettimeofday(&start, NULL);
            parcoursLargeurUniform(graphe,"H");
            gettimeofday(&stop, NULL);
            sec_coutU = (double) (stop.tv_sec*1e6 + stop.tv_usec) - (double)(start.tv_sec*1e6 + start.tv_usec);
            printf("\n");
            printf("Exploration en profondeur d'abord\n");
            gettimeofday(&start, NULL);
            parcoursProfond(graphe);
            gettimeofday(&stop, NULL);
            sec_profondeur = (double) (stop.tv_sec*1e6 + stop.tv_usec) - (double)(start.tv_sec*1e6 + start.tv_usec);
            printf("\n");
            printf("Exploration en largeur d'abord\n");
            gettimeofday(&start, NULL);
            parcoursLargeur(graphe);
            gettimeofday(&stop, NULL);
            sec_largeur = (double) (stop.tv_sec*1e6 + stop.tv_usec)  - (double)(start.tv_sec*1e6 + start.tv_usec);
            printf("\n");
            printf("Exploration en profondeur limitee\n");
            gettimeofday(&start, NULL);
            profondeurLimitee(graphe,3,"H");
            gettimeofday(&stop, NULL);
            sec_PL = (double) (stop.tv_sec*1e6 + stop.tv_usec)  - (double)(start.tv_sec*1e6 + start.tv_usec);
            printf("\n");
            printf("Exploration A*\n");
            gettimeofday(&start, NULL);
            IA(graphe,"H");
            gettimeofday(&stop, NULL);
            sec_A = (double) (stop.tv_sec*1e6 + stop.tv_usec)  - (double)(start.tv_sec*1e6 + start.tv_usec);
            printf("\n");
            printf("\n");
            printf("Exploration iterative en profondeur\n");
            gettimeofday(&start, NULL);
            profondeurLimiteeIt(graphe,"H");
            gettimeofday(&stop, NULL);
            sec_IP = (double) (stop.tv_sec*1e6 + stop.tv_usec)  - (double)(start.tv_sec*1e6 + start.tv_usec);
            /*
            gettimeofday(&start, NULL);
            parcoursGlouton(graphe);
            gettimeofday(&stop, NULL);
            sec_G = (double) ( (stop.tv_usec - start.tv_usec) / 1000000 );
            */
            printf(" \n");printf(" \n");
            printf("Temps d'execution :");
            printf(" \n");printf(" \n");
            printf("+-------------------+------------------+-----------------+-----------------+---------------+------------+");
            printf(" \n");
            printf("|    PROFONDEUR     |      LARGEUR     |    UNIFORME     |    P.LIMITEE    |  ITERATIVE.P  |     A*     |");
            printf(" \n");
            printf("+-------------------+------------------+-----------------+-----------------+---------------+------------+");
            printf(" \n");
            printf("|    %d    |    %d    |   %d    |    %d    |   %d  | %d |",sec_profondeur, sec_largeur, sec_coutU, sec_PL, sec_IP, sec_A);
            printf(" \n");
            printf("+------------------+-------------------+-----------------+-----------------+---------------+------------+");
            printf(" \n");
            printf(" \n");
            break;
    }   // switch
    if (!fini) {
        printf ("\n\nTaper Return pour continuer\n");
        getchar();
    }
  }

return 0;
}
