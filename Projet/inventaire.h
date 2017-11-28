#ifndef INVENTAIRE_H
#define INVENTAIRE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "struct.h"
#include "param.h"

/*************************************************************************************************/

inventaireHash_t * items;

/*************************************************************************************************/

inventaireHash_t * initInventaireHash ();
elmt_t * initElmt ();
inventaire_t * initInventaire (int taille);

inventaireHash_t * creerInventaireHash(char * nomFichier);

void libererInventaireHash(inventaireHash_t * inventaireHash);
void libererInventaire(inventaire_t * inventaire);


void sauvegarderInventaireHash (inventaireHash_t * inventaireHash, char * nomFichier); /*ne sert à rien*/
void sauvegarderInventaire (inventaire_t * inventaire, char * nomFichier);


item * rechercherItem (inventaireHash_t * inventaireHash, int type, char * name);


/*taille de l'inventaire à créer*/
inventaire_t * recupererInventaire(char * nomFichier, inventaireHash_t * inventaireHash, int taille);

#endif
