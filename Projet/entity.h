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

void        generateHero(character * hero, object entry);                    /* Initialise le h�ros et ses parametres                                       */
int         updateCharacterPos(character *hero, level *floor, int x, int y); /* Met � jour la position du h�ros si le d�placement est autoris�              */
char *      heroColorChooser();
int         updateActualRoom(level floor, character * hero);                 /* Met � jour le noumero de salle dans lequel se situe le joueur               */

int         getNbMonsters(int level);                                        /* Calcule un nombre al�atoire de monstres en fonction de l'�tage              */
monster     generateMonster(level floor);
void        generateMonsters (level *floor);                                 /* G�n�re les monstres de l'�tage                                              */
void        generateMonsters2(level *floor);                                 /* G�n�re les monstre de l'�tage avec prise en compte de la difficult� globale */
void        die(level floor, monster * mon);                                 /* Proc�dure de mort d'un monstre                                              */

/*************************************************************************************************/


#endif
