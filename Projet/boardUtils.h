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
/* utilisées par le module Board pour la         */
/* génération et l'affichage des niveaux         */
/* ********************************************* */

/*************************************************************************************************/

int isInsertable(level *floor, room toInsert);                                      /* Recherche si une salle aux dimensions données peut être inséré sur un niveau donné       */
int isInsertableToCoord(level *floor, room toInsert);                               /* Recherche si une salle aux dimensions données peut être inséré à des coordonnées données */
int isEmpty(int x, int y, int Room, level floor, character hero);                   /* Recherche si une case est une case vide accessible et non occupée par une entité         */
int isWall(int c);                                                                  /* Recherche si le caractère donné est un mur (ou une porte)                                */
int isCorner(int c);                                                                /* Recherche si le caractère donné est un coin de mur                                       */
int isRoomFull(level floor, int nbRoom);
int isWallFull(room Room, int wall);

int getBoardSize(int level);                                                        /* Calcule la taille du niveau en fonction de l'étage                                       */
int getNbRooms (int level);                                                         /* Calcule un nombre aléatoire de salle en fonction de l'étage                              */

object* getObjectByCoord(level floor, int x, int y, int room);                      /* Renvoie l'objet aux coordonnées données s'il existe                                      */

/*************************************************************************************************/

#endif
