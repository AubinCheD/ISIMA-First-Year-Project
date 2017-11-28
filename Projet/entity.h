#ifndef ENTITY_H
#define ENTITY_H

/*************************************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <conio.h>

#include "struct.h"
#include "param.h"
#include "utils.h"
#include "boardUtils.h"
#include "inventaire.h"

/*************************************************************************************************/

void        generateHero(character * hero, object entry);                    /* Initialise le héros et ses parametres                                       */
int         updateCharacterPos(character *hero, level *floor, int x, int y); /* Met à jour la position du héros si le déplacement est autorisé              */
char *      heroColorChooser();
int         updateActualRoom(level floor, character * hero);                 /* Met à jour le noumero de salle dans lequel se situe le joueur               */

int         getNbMonsters(int level);                                        /* Calcule un nombre aléatoire de monstres en fonction de l'étage              */
monster     generateMonster(level floor);
void        generateMonsters (level *floor);                                 /* Génère les monstres de l'étage                                              */
void        generateMonsters2(level *floor);                                 /* Génère les monstre de l'étage avec prise en compte de la difficulté globale */
void        die(level floor, monster * mon);                                 /* Procédure de mort d'un monstre                                              */

/*************************************************************************************************/


#endif
