#ifndef BOARDUTILS_H
#define BOARDUTILS_H

/*************************************************************************************************/

#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "struct.h"
#include "param.h"

/*************************************************************************************************/

/* ********************************************* */
/* BoardUtils contient les fonctions secondaires */
/* utilis�es par le module Board pour la         */
/* g�n�ration et l'affichage des niveaux         */
/* ********************************************* */

/*************************************************************************************************/

int isInsertable(level *floor, room toInsert);                                      /* Recherche si une salle aux dimensions donn�es peut �tre ins�r� sur un niveau donn�       */
int isInsertableToCoord(level *floor, room toInsert);                               /* Recherche si une salle aux dimensions donn�es peut �tre ins�r� � des coordonn�es donn�es */
int isEmpty(int x, int y, int Room, level floor, character hero);                   /* Recherche si une case est une case vide accessible et non occup�e par une entit�         */
int isWall(int c);                                                                  /* Recherche si le caract�re donn� est un mur (ou une porte)                                */
int isCorner(int c);                                                                /* Recherche si le caract�re donn� est un coin de mur                                       */
int isRoomFull(level floor, int nbRoom);
int isWallFull(room Room, int wall);

int getBoardSize(int level);                                                        /* Calcule la taille du niveau en fonction de l'�tage                                       */
int getNbRooms (int level);                                                         /* Calcule un nombre al�atoire de salle en fonction de l'�tage                              */

object* getObjectByCoord(level floor, int x, int y, int room);                      /* Renvoie l'objet aux coordonn�es donn�es s'il existe                                      */

/*************************************************************************************************/

#endif
