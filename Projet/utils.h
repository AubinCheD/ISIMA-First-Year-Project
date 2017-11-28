#ifndef UTILS_H
#define UTILS_H

/*************************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "param.h"

/*************************************************************************************************/

/* ***************************************** */
/* Utils contient les fonctions g�n�ralistes */
/* utiles aux autres modules.                */
/* ***************************************** */

/*************************************************************************************************/

char * displayMessage(char *message, int answerSize);                                             /* Affiche un message donn� et renvoie la r�ponse   */
char * getTrimmedText(char * string, int size, int line);
void frame(char ** display, int x, int y, int width, int height);
char * center(char * string, char c, int size);
char * addColor(char * string, char c, char * color, int size);
char ** initDisplay();
void display(char ** board, char ** menu);
int intContains(int *array, int object, int size);                                               /* Recherche si un tableau contient un entier donn� */
double maxTab (double * tab, int taille);
int intMax (int i, int j);                                                                       /* Renvoie la plus grande valeur entre deux entiers */
int intMin (int i, int j);                                                                       /* Renvoie la plus petite valeur entre deux entiers */

/*************************************************************************************************/

#endif
