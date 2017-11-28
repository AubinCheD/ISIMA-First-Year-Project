#ifndef GRAPHES_H
#define GRAPHES_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "param.h"

typedef enum {NON_ORIENTE=0,ORIENTE} graphe_type;


typedef struct graphe {
  int nb_sommets;         /*numérotés de 0 à n-1*/
  graphe_type type;
  int ** matadj;
} graphe_t;


typedef struct arete {
  int s1;
  int s2;
  struct arete * next;
} arete_t;



graphe_t * creerGraphe (int n /*nb sommets*/, graphe_type type);

void supprimerGraphe (graphe_t * graphe);

void ajouterAreteGraphe1 (graphe_t * graphe, int s1, int s2);

void supprimerAreteGraphe1 (graphe_t * graphe, int s1, int s2);



void ajouterAreteGraphe2 (graphe_t * graphe, int s1, int s2);/*ajouterArete revient à ajouter 1 a mat[s1][s2]et [s2][s1]*/
                                                      /*On peut rajouter un paramètre pour ajouter la valeur de l'arc*/

void supprimerAreteGraphe2 (graphe_t * graphe, int s1, int s2); /*sommet 1, sommet 2*/

void afficherGraphe (graphe_t * graphe);


arete_t * creerListeAretes (graphe_t * graphe);

void supprimerListeAretes (arete_t * listeAretes);

void afficherListe (arete_t ** listeAretes);


void sauvegarderGraphe (arete_t ** listeAretes, char * nomFichier);

arete_t * recupererSauvegardeGraphe (char * nomFichier);

graphe_t * genererGraphe (int nb_sommets); /*est ce qu'on ajoute une salle par niveau ?*/

arete_t * getCorridors (int nbRoom);

int * auxConnexite (int taille);

void auxAfficherTab (int * tab, int taille);


#endif
