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
/* Board contient les fonctions relatives à  */
/* la génération et à l'affichage des étages */
/* ***************************************** */

/*************************************************************************************************/

void generateLevel(level *floor, int levelNumber);                                                                    /* Génère le niveau en fonction du numéro d'étage                            */
void generateBoard(level *floor);                                                                                     /* Génère les objets de l'étages (salles, couloirs, entité, etc.             */

void generateRooms(level *floor);                                                                                     /* Génère les salles de l'étage                                              */
int ** drawRoom(room toDraw);                                                                                         /* Place une salle donnée sur l'étage                                        */

void generateCorridors(level *floor);                                                                                 /* Génère les couloirs reliant les salles                                    */
void getNextCorridor(level * floor, int *connectedRooms, int nbConnectedRooms, int *room1, int *room2);               /* Donne les deux prochaines salles à connecter                              */
int  drawCorridor(level *floor, int room1, int room2);                                                                /* Place un couloir donné sur l'étage                                        */
void getRoomWalls(room room1, room room2, int *wallRoom1, int *wallRoom2);                                            /* Donne le mur de chacune des salles à connecter sur lequel placer la porte */
int  getDoors(room room1, room room2, int wall1, int wall2, object *door1, object *door2);                            /* Donne la porte de chacune des salles à connecter                          */
room initCorridor(object door1, object door2);                                                                        /* Initialise un couloir                                                     */


char ** getBoardDisplay(level floor, character hero);                                                                 /* Affiche l'étage actuel en se limitant au champ de vision du héros         */
char displayLocation(int x, int y, int** board, int toPrint);                                                         /* Affiche une case donnée et l'intersection avec la case précédente         */
void displayRoom (room toDisplay);

/*************************************************************************************************/

#endif
