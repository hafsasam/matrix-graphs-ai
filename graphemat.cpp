#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "liste.h"
#include "graphemat.h"

booleen  grapheDetruit = vrai;


// remise à zéro du tableau de marquage
static void razMarque (GrapheMat* graphe){
    for (int i=0; i<graphe->n; i++) graphe->marque [i] = faux;
}

GrapheMat* creerGrapheMat (int nMax, int value){
    // allocation de graphe
    GrapheMat* graphe  = (GrapheMat*) malloc (sizeof (GrapheMat));
    graphe->n       = 0;
    graphe->nMax    = nMax;
    graphe->value   = value; // value : vrai si le graphe est valué
    graphe->nomS    = (NomSom*)  malloc (sizeof(NomSom)  *nMax);
    graphe->marque  = (booleen*) malloc (sizeof(booleen) *nMax);
    graphe->element = (int*)     malloc (sizeof(int)*nMax*nMax);
    graphe->valeur  = (int*)     malloc (sizeof(int)*nMax*nMax);
    // initialisation par défaut
    for (int i=0; i<nMax; i++){
        for (int j=0; j<nMax; j++){
            graphe->element [i*nMax+j] = faux;
            graphe->valeur  [i*nMax+j] = INFINI;
        }
    }
    razMarque (graphe);
    grapheDetruit = faux;
    return graphe;
}

// désallocation d'un graphe
void detruireGraphe (GrapheMat* graphe){
    free (graphe->nomS);
    free (graphe->marque);
    free (graphe->element);
    free (graphe->valeur);
    free (graphe);
    grapheDetruit = vrai;
}

static int rang (GrapheMat* graphe, NomSom nom){
    int i = 0;
    booleen  trouve = faux;
    while ( (i<graphe->n) && !trouve){
        trouve = strcmp (graphe->nomS [i], nom) == 0;
        if (!trouve) i++;
    }
    return trouve ? i : -1;
}

void ajouterUnSommet (GrapheMat* graphe, NomSom nom){
    if (rang (graphe, nom) == -1){
        if (graphe->n < graphe->nMax){
            strcpy (graphe->nomS [graphe->n++], nom);
        }
        else{
            printf ("\nNombre de sommets > %d\n", graphe->nMax);
        }
    }
    else{
        printf ("\n%s déjà défini\n", nom);
    }
}

void ajouterUnArc (GrapheMat* graphe, NomSom somD, NomSom somA, int cout){
    int nMax = graphe->nMax;
    int rd = rang (graphe, somD);
    int rg = rang (graphe, somA);
    graphe->element [rd*nMax+rg] = vrai;
    graphe->valeur  [rd*nMax+rg] = cout;
}

void ecrireGraphe (GrapheMat* graphe){
    if (grapheDetruit == vrai){
        printf ("Le graphe a ete detruit ou n'est pas encore cree\n");
    }
    else{
        int nMax = graphe->nMax;
        for (int i=0; i<graphe->n; i++) printf ("%s ", graphe->nomS[i]);
        printf (";\n");
        for (int i=0; i<graphe->n; i++){
            printf ("\n%s : ", graphe->nomS[i]);
            for (int j=0; j<graphe->n; j++){
                if (graphe->element [i*nMax+j] == vrai){
                    printf ("%s ", graphe->nomS[j]) ;
                    if (graphe->value){
                        printf (" (%3d) ", graphe->valeur [i*nMax+j] );
                    }
                }
            }
            printf (";");
        }
    }
}

static void profondeur(GrapheMat* graphe, int numSommet){
    int nMax = graphe->nMax;
    graphe->marque [numSommet] = vrai;
    printf ("%s\n", graphe->nomS [numSommet]);
    for (int i=0; i<graphe->n; i++){
        if ( (graphe->element [numSommet*nMax+i] == vrai) && !graphe->marque [i] ){
            profondeur (graphe, i);
        }
    }
}


void parcoursProfond (GrapheMat* graphe){
    razMarque (graphe);
    for (int i=0; i<graphe->n; i++){
        if (!graphe->marque [i]) profondeur (graphe, i);
    }
}

static void ecrireEtape (Matrice a, Matrice p, int k, int ns, int nMax){
    printf ("Passage par le sommet numéro %d\n", k);
    for (int i=0; i<ns; i++){
        for (int j=0; j<ns; j++){
            if (a [i*nMax+j]==INFINI){
                printf (" %3s", "*");
            }
            else{
                printf (" %3d", a [i*nMax+j]);
            }
        }
        printf ("%6s", " ");
        for (int j=0; j<ns; j++){
            printf ("%3d", p [i*nMax+j]);
        }
        printf ("\n");
    }
    printf ("\n");
}

void parcoursLargeur (GrapheMat* graphe){
	int k,i,j,nMax = graphe->nMax;
	razMarque (graphe);
	Liste* Li = creerListe(0,NULL,NULL);
	//printf("%d\n",graphe->n);
	for (i=0; i<graphe->n; i++){
		if (!graphe->marque[i]){
			//printf ("\t %s ", graphe->nomS[i]);
			char*debut= (char*)graphe->nomS[i];
			insererEnFinDeListe(Li,debut);
			while(!listeVide(Li)){
				char * sorti = (char*) extraireEnTeteDeListe(Li);
				//printf("%d\n",k);
				k=rang(graphe,sorti);
				if(!graphe->marque[k]){
					printf("%s\t",graphe->nomS[k]);
					graphe->marque[k] = vrai;
				}
				for (j=0; j<graphe->n; j++){
					if ( (graphe->element[k*nMax+j] == vrai) && graphe->marque[j]== faux){
						//printf (" %s ", graphe->nomS [j]);
						char*temp= (char*)graphe->nomS[j];
						insererEnFinDeListe(Li, temp);
						graphe->marque[j]== vrai;
					}
				}
			}
		}
	}
}


 booleen profondeurLimitee(GrapheMat * graphe, int limite, NomSom objet){
    int nMax = graphe->nMax;
    Liste *li = creerListe();
    int a = 0;
    int *h = hauteur(graphe);
    insererEnTeteDeListe(li, graphe->nomS[0]);
    while (!listeVide(li)){
        char *extrait = (char *)extraireEnFinDeListe(li);
         if (!graphe->marque[rang(graphe, extrait)])
            printf("%s\t", extrait);
         if (strcmp(extrait, objet) == 0){
            printf("Found");
            /* a = 1;
            for (int k = 0; k < graphe->n; k++)
             graphe->marque[k] = vrai;*/
             return vrai;
          }
          else{
              graphe->marque[rang(graphe, extrait)] = vrai;
              for (int j = 0; j < graphe->n; j++){
                if ((graphe->element[(rang(graphe, extrait)) * graphe->nMax + j] == vrai)
                     && !graphe->marque[j] && (*(h + j) <= limite))
                        {
                            insererEnFinDeListe(li, graphe->nomS[j]);
                        }
               }
          }
    }
           if (a == 0){
            	printf("Not found");
            	return faux;
			}
}

int *hauteur(GrapheMat * graphe){
    razMarque(graphe);
    int nMax = graphe->n;
    Liste *li = creerListe();
    int *h = (int *)malloc(sizeof(int) * nMax);
    memset(h, 0, graphe->n);
    int a = 0;
    insererEnFinDeListe(li, graphe->nomS[0]);
    graphe->marque[0] = vrai;
    *(h + 0) = 1;
    while (!listeVide(li)){
        char *extrait = (char *)extraireEnTeteDeListe(li);
        for (int j = 0; j < nMax; j++){
            if ((graphe->element[(rang(graphe, extrait)) * graphe->nMax + j] == vrai)
                 && !graphe->marque[j]){
                insererEnFinDeListe(li, graphe->nomS[j]);
                *(h + j) = *(h + rang(graphe, extrait)) + 1;
                a = *(h + j);
                graphe->marque[j] = vrai;
             }
        }
    }
    for (int i = 0; i < graphe->n; i++){
        if (!graphe->marque[i])
            *(h + i) = a + 1;
    }
    razMarque(graphe);
    return h;
}

void profondeurLimiteeIt(GrapheMat * graphe, NomSom objet){
    for (int i = 1; i <= graphe->n; i++){
        printf("etape %d :", i);
        if(profondeurLimitee(graphe, i, objet)) break;
            printf("\n");
    }
}

void IA(GrapheMat* graphe,char* but){
    int* depart=(int*) malloc(sizeof(int));
    *depart=0;
    Liste* listParcourt=creerListe();
    insererEnOrdre(listParcourt,depart);
    razMarque (graphe);
    graphe->marque [0] = vrai;
    //vecteur des sommets
    int* intSommet=(int*)malloc(sizeof(int)*graphe->n);
    for(int j=0;j<graphe->n;j++){
        *(intSommet+j)=j;
    }
    //vecteur cout cumulee
    int* intCout=(int*)malloc(sizeof(int)*graphe->n);
    for(int j=0;j<graphe->n;j++){
        *(intCout+j)=0;
    }
    // vecteur fn
    int* fn=(int*)malloc(sizeof(int)*graphe->n);
    for(int j=0;j<graphe->n;j++){
        *(fn+j)=0;
    }
    //vecteur des valeurs de h
    int* h=(int*)malloc(sizeof(int)*graphe->n);
    *(h)=366;
    *(h+1)=253;
    *(h+2)=329;
    *(h+3)=374;
    *(h+4)=176;
    *(h+5)=380;
    *(h+6)=193;
    *(h+7)=0;
    *(h+8)=160;
    *(h+9)=100;
    //vecteur precedent
    int* intPrec=(int*)malloc(sizeof(int)*graphe->n);
    for(int j=0;j<graphe->n;j++){
        *(intPrec+j)=0;
    }
    while(!listeVide(listParcourt)){
        int* ex=(int*)extraireEnTeteDeListe(listParcourt);
        printf("(%s,%d)\t", graphe->nomS[*ex],fn[*ex]);
        if(strcmp(but,graphe->nomS[*ex])==0)  break;
        graphe->marque[*ex] = vrai;
        for(int j=0;j<graphe->n;j++){
            if ( (graphe->element [(*ex)*graphe->nMax+j] == vrai) && !graphe->marque [j] ){
                    if(intCout[j]==0){
                        intCout[j]=intCout[*ex]+graphe->valeur[(*ex)*graphe->nMax+j];
                        fn[j]=intCout[j]+h[j];
                        insererEnOrdreCout(listParcourt,intSommet+j,fn);
                        intPrec[j]=*ex;
                    }
                    else if(intCout[j]>=intCout[*ex]+graphe->valeur[(*ex)*graphe->nMax+j]){
                        intCout[j]=intCout[*ex]+graphe->valeur[(*ex)*graphe->nMax+j];
                        fn[j]=intCout[j]+h[j];
                        insererEnOrdreCout(listParcourt,intSommet+j,fn);
                        intPrec[j]=*ex;
                    }
                }
        }
    }
    int j=rang(graphe, but);
    printf("\n");
    while(j!=0){
        printf("%2s<-",graphe->nomS[j]);
        j= *(intPrec+j) ;
    }
    printf("%2s",graphe->nomS[0]);
}

void parcoursLargeurUniform(GrapheMat* graphe, char* but){
    int* depart=(int*) malloc(sizeof(int));
    *depart=0;
    Liste* listParcourt=creerListe();
    insererEnOrdre(listParcourt,depart);
    razMarque(graphe);
    graphe->marque[0] = vrai;
    //vecteur des sommets
    int* indiceSommet=(int*)malloc(sizeof(int)*graphe->n);
    for(int j=0;j<graphe->n;j++){
        *(indiceSommet+j)=j;
    }
    //vecteur cout cumulee
    int* intCout=(int*)malloc(sizeof(int)*graphe->n);
    for(int j=0;j<graphe->n;j++){
        *(intCout+j)=0;
    }
    //vecteur precedent
    int* intPrec=(int*)malloc(sizeof(int)*graphe->n);
    for(int j=0;j<graphe->n;j++){
        *(intPrec+j)=0;
    }

    while(!listeVide(listParcourt)){
        int* ex=(int*)extraireEnTeteDeListe(listParcourt);
        graphe->marque [*ex] = vrai;
        printf("(%s,%d)\t", graphe->nomS[*ex],intCout[*ex]);
        //test sur le but si on est en arrive ou pas non
        if(strcmp(but,graphe->nomS[*ex])==0)  break;

        for(int j=0;j<graphe->n;j++){
            if ( (graphe->element [(*ex)*graphe->nMax+j] == vrai) && !graphe->marque[j] ){
                if(intCout[j]==0){
                    intCout[j]=intCout[*ex]+graphe->valeur[(*ex)*graphe->nMax+j];
                    insererEnOrdreCout(listParcourt,indiceSommet+j,intCout);
                    //graphe->marque [j] = vrai;
                    intPrec[j]=*ex;
                }
                else if(intCout[j]>intCout[*ex]+graphe->valeur[(*ex)*graphe->nMax+j]){
                    intCout[j]=intCout[*ex]+graphe->valeur[(*ex)*graphe->nMax+j];
                    insererEnOrdreCout(listParcourt,indiceSommet+j,intCout);
                    //graphe->marque [j] = vrai;
                    intPrec[j]=*ex;
                }
            }
        }
    }
    int j=rang (graphe, but);
     printf("\n");
    while(j!=0){
        printf("%2s<-",graphe->nomS[j]);
        j= *(intPrec+j) ;
    }
    printf("%2s",graphe->nomS[0]);
}

