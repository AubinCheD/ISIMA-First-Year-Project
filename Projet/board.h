#ifndef BOARD_H
#define BOARD_H

/*************************************************************************************************/

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include "struct.h"
#include "param.h"
#include "boardUtils.h"
#include "entity.h"
#include "graphes.h"
#include "utils.h"

/*************************************************************************************************/

/* ***************************************** */
/* Board contient les fonctions relatives �  */
/* la g�n�ration et � l'affichage des �tages */
/* ***************************************** */

/*************************************************************************************************/

void generateLevel(level *floor, int levelNumber);                                                                    /* G�n�re le niveau en fonction du num�ro d'�tage                            */
void generateBoard(level *floor);                                                                                     /* G�n�re les objets de l'�tages (salles, couloirs, entit�, etc.             */

void generateRooms(level *floor);                                                                                     /* G�n�re les salles de l'�tage                                              */
int ** drawRoom(room toDraw);                                                                                         /* Place une salle donn�e sur l'�tage                                        */

void generateCorridors(level *floor);                                                                                 /* G�n�re les couloirs reliant les salles                                    */
void getNextCorridor(level * floor, int *connectedRooms, int nbConnectedRooms, int *room1, int *room2);               /* Donne les deux prochaines salles � connecter                              */
int  drawCorridor(level *floor, int room1, int room2);                                                                /* Place un couloir donn� sur l'�tage                                        */
void getRoomWalls(room room1, room room2, int *wallRoom1, int *wallRoom2);                                            /* Donne le mur de chacune des salles � connecter sur lequel placer la porte */
int  getDoors(room room1, room room2, int wall1, int wall2, object *door1, object *door2);                            /* Donne la porte de chacune des salles � connecter                          */
room initCorridor(object door1, object door2);                                                                        /* Initialise un couloir                                                     */


char ** getBoardDisplay(level floor, character hero);                                                                 /* Affiche l'�tage actuel en se limitant au champ de vision du h�ros         */
char displayLocation(int x, int y, int** board, int toPrint);                                                         /* Affiche une case donn�e et l'intersection avec la case pr�c�dente         */
void displayRoom (room toDisplay);

/*************************************************************************************************/

#endif
