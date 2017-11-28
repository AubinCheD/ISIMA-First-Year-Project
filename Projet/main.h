#ifndef MAIN_H
#define MAIN_H

/*************************************************************************************************/

#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <string.h>

#include "struct.h"
#include "inventaire.h"
#include "board.h"
#include "entity.h"
#include "menus.h"
#include "param.h"

/*************************************************************************************************/

/* *************************************** */
/* Main est le module principale contenant */
/* la fonction principale et les fonctions */
/* de gestion et d'initialisation du jeu   */
/* *************************************** */

/*************************************************************************************************/

int main();                                                /* Fonction principale, initialise et g�re le jeu       */
void startGame         (level * floor, character * hero);
int handleKeyboardEvent(level * floor, character * hero); /* G�re les ev�nements clavier                          */
item * getItems();                                         /* Cr�e la liste des objets lootable                    */
void playMonstersTurn(level * floor, character * hero);    /* Joue le tour de chaque monstre de l'�tage            */
int tryFight(monster attacker, character * hero);          /* Essaye d'attaque le h�ros s'il est � port�e          */
char ** getGameOverDisplay();
void freeMemory(level *floor, character * hero);           /* Lib�re toute la m�moire allou� au cours du programme */

/*************************************************************************************************/

#endif
