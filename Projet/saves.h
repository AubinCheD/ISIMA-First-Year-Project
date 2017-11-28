#ifndef SAVES_H
#define SAVES_H

/*************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <windows.h>
#include <time.h>

#include "struct.h"
#include "inventaire.h"

/*************************************************************************************************/

char ** getSaves(int * nbSaves);
void loadHeros(char * file, character * hero);
void loadAll(char * file, level * floor, character * hero);
void save(level l, character hero);

#endif
