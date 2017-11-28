#ifndef MENUS_H
#define MENUS_H

/*************************************************************************************************/

#include "struct.h"
#include "param.h"
#include "board.h"
#include "entity.h"
#include "saves.h"

/*************************************************************************************************/

typedef struct {
	int    id;
	char * title;
	char   sc;
	char * desc;
	int    selectable;
} menuItem;

typedef struct {
	int         id;
	char     *  title;
	menuItem ** grid;
	int         gridWidth;
	int         gridHeight;
	int         nbItem;
	int 	    cursorId;
} menu;

/*************************************************************************************************/

int loadMenu(level * floor, character * hero, char ** boardDisplay, menu m);
int loadMainMenu(level * floor, character ** hero);
menu getMainMenu(character hero);

menu getLoadMenu ();
int load(char * filename, level * floor, character ** hero);

menuItem * getMenuItemFromId(menu m, int id);
void getMenuItemPosition(menu m, int id, int *i, int *j);

char ** getMenuDisplay(menu m, level floor, character hero);

void save(level floor, character hero);

#endif
