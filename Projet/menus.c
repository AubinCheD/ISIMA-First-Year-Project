#include "menus.h"

/*************************************************************************************************/

int loadMenu(level * floor, character * hero, char ** boardDisplay, menu m)
{
    int i, j, key, option = 0, ret = 0;

    while (ret == 0)
    {
        option = -1;
        while (option == -1)
        {
            display(boardDisplay, getMenuDisplay(m, *floor, *hero));
            key = getch();

            for(i=0; i < m.nbItem && option == -1; i++) /* Si un raccourci est utilisé ou si on appuie sur entrée */
                if (key == getMenuItemFromId(m, i)-> sc || key == getMenuItemFromId(m, i)->sc + 32 || (key == 13 && m.cursorId == i))
                    option = i;

            if(option == -1)
                switch(key)
                {                       /* Touches fléchées (getch() renvoie deux caractères pour les touches fléchées -> 1er caractère : 0 = touches fléchées du pavé numérique, 224 = touches fléchées */
                    case 0 : case 224 : /*                                                                             -> 2e  caractère : code de la touche fléchée préssée)                             */
                        getMenuItemPosition(m, m.cursorId, &i, &j);
                        switch(getch())
                        {
                            case 72 : do i = (i == 0) ? m.gridHeight - 1 : i - 1; while (m.grid[i][j].id == -1); break; /* Flèche du haut   */
                            case 75 : do j = (j == 0) ? m.gridWidth  - 1 : j - 1; while (m.grid[i][j].id == -1); break; /* Flèche de gauche */
                            case 80 : do i = (i == m.gridHeight - 1) ? 0 : i + 1; while (m.grid[i][j].id == -1); break; /* Flèche du bas    */
                            case 77 : do j = (j == m.gridWidth  - 1) ? 0 : j + 1; while (m.grid[i][j].id == -1); break; /* Flèche de droite */
                        }
                        m.cursorId = m.grid[i][j].id; break;
                    case 27 : option = 0; break; /* Touche échap */
                }
        }

        switch(m.id)
        {
            case 0 : switch(option)
                     {
                         case 0 : ret = 1; break;
                         case 1 : hero->mode = !hero->mode; getMenuItemFromId(m, option)->title = hero->mode ? "Mode : Attack" : "Mode : Move"; break;
                         case 2 : save(*floor, *hero); break;
                         case 3 : ret = loadMenu(floor, hero, boardDisplay, getLoadMenu()) - 1; break;
                         case 4 : ret = 2; break;
                         case 5 : ret = 3; break;
                     } break;

            case 1 : if (option == m.gridHeight-1) ret = 1;
                     else
                     {
                        loadAll(getSaves(&i)[option], floor, hero);
                        ret = 2;
                     } break;
        }

    }

    return ret;
}

/*************************************************************************************************/

menu getMainMenu(character hero)
{
    int i;
    menu mainMenu;

    mainMenu.id = 0;
    mainMenu.title = "MAIN MENU";
    mainMenu.gridWidth = 2;
    mainMenu.gridHeight = 3;
    mainMenu.grid = (menuItem **) malloc(sizeof(menuItem *) * mainMenu.gridHeight);
    for(i=0; i < mainMenu.gridHeight; i++) mainMenu.grid[i] = (menuItem *) malloc(sizeof(menuItem) * mainMenu.gridWidth);
    mainMenu.nbItem = mainMenu.gridWidth * mainMenu.gridHeight;
    mainMenu.cursorId = 0;

    mainMenu.grid[0][0].id = 0;
    mainMenu.grid[0][0].title = "Continue";
    mainMenu.grid[0][0].sc = 'C';
    mainMenu.grid[0][0].desc = "Press enter to return to the game.";
    mainMenu.grid[0][0].selectable = 1;

    mainMenu.grid[0][1].id = 1;
    mainMenu.grid[0][1].title = hero.mode ? "Mode : Attack" : "Mode : Move";
    mainMenu.grid[0][1].sc = 'M';
    mainMenu.grid[0][1].desc = "Press enter to switch between attack and move.";
    mainMenu.grid[0][1].selectable = 1;

    mainMenu.grid[1][0].id = 2;
    mainMenu.grid[1][0].title = "Save";
    mainMenu.grid[1][0].sc = 'S';
    mainMenu.grid[1][0].desc = "Press enter to save the actual game.";
    mainMenu.grid[1][0].selectable = 1;

    mainMenu.grid[1][1].id = 3;
    mainMenu.grid[1][1].title = "Load";
    mainMenu.grid[1][1].sc = 'L';
    mainMenu.grid[1][1].desc = "Press enter to load a save of an other game.";
    mainMenu.grid[1][1].selectable = 1;

    mainMenu.grid[2][0].id = 4;
    mainMenu.grid[2][0].title = "Reset";
    mainMenu.grid[2][0].sc = 'R';
    mainMenu.grid[2][0].desc = "Press enter to restart the game. Every non-saved progress will be lost.";
    mainMenu.grid[2][0].selectable = 1;

    mainMenu.grid[2][1].id = 5;
    mainMenu.grid[2][1].title = "Exit";
    mainMenu.grid[2][1].sc = 'E';
    mainMenu.grid[2][1].desc = "Press enter to exit the game without saving.";
    mainMenu.grid[2][1].selectable = 1;

    return mainMenu;
}

/*************************************************************************************************/

menu getLoadMenu ()
{
    menu loadMenu;
    char ** saves = getSaves(&(loadMenu.gridHeight));
    char tmp[8];
    int i;
    character * tempHero = (character *) malloc(sizeof(character));

    loadMenu.id = 1;
    loadMenu.title = "LOAD MENU";
    loadMenu.gridWidth = 1;
    loadMenu.gridHeight++;
    loadMenu.grid = (menuItem **) malloc(sizeof(menuItem *) * loadMenu.gridHeight);
    for(i=0; i < loadMenu.gridHeight; i++) loadMenu.grid[i] = (menuItem *) malloc(sizeof(menuItem) * loadMenu.gridWidth);
    loadMenu.nbItem = loadMenu.gridWidth * loadMenu.gridHeight;
    loadMenu.cursorId = 0;

    for (i=0; i < loadMenu.nbItem - 1; i++)
    {
        loadHeros(saves[i], tempHero);
        strtok(saves[i], "_");
        strtok(NULL, "_");

        loadMenu.grid[i][0].id = i;
        loadMenu.grid[i][0].sc = i + 48;
        loadMenu.grid[i][0].selectable = 1;

        loadMenu.grid[i][0].title = (char *) malloc(sizeof(char) * 64);
        strcpy(loadMenu.grid[i][0].title, tempHero->name);
        strcat(loadMenu.grid[i][0].title, " - ");
        strcat(loadMenu.grid[i][0].title, strtok(NULL, "_"));
        strcat(loadMenu.grid[i][0].title, "/");
        strcat(loadMenu.grid[i][0].title, strtok(NULL, "_"));
        strcat(loadMenu.grid[i][0].title, "/");
        strcat(loadMenu.grid[i][0].title, strtok(NULL, "_"));
        strcat(loadMenu.grid[i][0].title, " ");
        strcat(loadMenu.grid[i][0].title, strtok(NULL, "_"));
        strcat(loadMenu.grid[i][0].title, ":");
        strcat(loadMenu.grid[i][0].title, strtok(NULL, "_"));

        loadMenu.grid[i][0].desc = (char *) malloc(sizeof(char) * 256);
        strcpy(loadMenu.grid[i][0].desc, "EXP: ");
        strcat(loadMenu.grid[i][0].desc, itoa(tempHero->exp, tmp, 10));
        strcat(loadMenu.grid[i][0].desc, "      HP: ");
        strcat(loadMenu.grid[i][0].desc, itoa(tempHero->ent.stats.currentHP, tmp, 10));
        strcat(loadMenu.grid[i][0].desc, "/");
        strcat(loadMenu.grid[i][0].desc, itoa(tempHero->ent.stats.maxHP, tmp, 10));
        strcat(loadMenu.grid[i][0].desc, "      STR: ");
        strcat(loadMenu.grid[i][0].desc, itoa(tempHero->ent.stats.strength, tmp, 10));
    }

    loadMenu.grid[loadMenu.nbItem - 1][0].id = loadMenu.nbItem - 1;
    loadMenu.grid[loadMenu.nbItem - 1][0].title = "Back";
    loadMenu.grid[loadMenu.nbItem - 1][0].sc = 'B';
    loadMenu.grid[loadMenu.nbItem - 1][0].desc = "Return to main menu.";
    loadMenu.grid[loadMenu.nbItem - 1][0].selectable = 1;


    return loadMenu;
}

/*************************************************************************************************/

menuItem * getMenuItemFromId(menu m, int id)
{
    int i, j;
    menuItem * ret = NULL;

    for(i=0; i < m.gridHeight && ret == NULL; i++)
        for(j=0; j < m.gridWidth && ret == NULL; j++)
            if (m.grid[i][j].id == id) ret = &(m.grid[i][j]);

    return ret;
}

/*************************************************************************************************/

void getMenuItemPosition(menu m, int id, int *i, int *j)
{
    int found = 0;

    for(*i=-1; *i+1 < m.gridHeight && !found; (*i)++)
        for(*j=-1; *j+1 < m.gridWidth && !found; (*j)++)
            found = (m.grid[*i+1][*j+1].id == id);
}

/*************************************************************************************************/

char ** getMenuDisplay(menu m, level floor, character hero)
{
    char ** display = initDisplay();
    char  * line    = (char *) malloc(sizeof(char)*256);
    int i, j, k;
    char c[2];

    c[1] = '\0';
    strcpy(display[(DISPLAY_SIZE - VIEW_SIZE)/2 - 1], center(m.title, UNSEENLOC, DISPLAY_SIZE*2));

	for(i=0; i < m.gridHeight; i++)
    {
        memset(line, 0, strlen(line));

        for(j=0; j < m.gridWidth; j++)
            if(m.grid[i][j].id != -1)
            {
                strcat(line, "    ");
                if(m.cursorId == m.grid[i][j].id)
                {
                    line[strlen(line)] = CURSOR;
                    line[strlen(line)] = UNSEENLOC;
                }
                strcat(line, m.grid[i][j].title);
                strcat(line, " (");
                c[0] = m.grid[i][j].sc;
                strcat(line, c);
                strcat(line, ") ");
            }
        if(strlen(line))
        {
            while (strlen(line) <= VIEW_SIZE*2 + 1) line[strlen(line)] = UNSEENLOC;
            strcpy(display[(DISPLAY_SIZE - VIEW_SIZE)/2 + 3*i + 2], center(line, UNSEENLOC, DISPLAY_SIZE*2));
            k = DISPLAY_SIZE - VIEW_SIZE + 3;
            for(j=0; j < m.gridWidth; j++)
                if(m.grid[i][j].id != -1)
                {
                    frame(display, k, (DISPLAY_SIZE - VIEW_SIZE)/2 + 3*i + 2, strlen(m.grid[i][j].title) + 4 + 2*(m.cursorId == m.grid[i][j].id), 1);
                    k += strlen(m.grid[i][j].title) + 4 + 2*(m.cursorId == m.grid[i][j].id) + 5;
                }
        }
    }

    memset(line, 0, strlen(line));
    strcpy(line, "          ");
    line[4] = getMenuItemFromId(m, m.cursorId)->sc;
    strcat(line, getMenuItemFromId(m, m.cursorId)->title);
    while(strlen(line) <= VIEW_SIZE*2 + 1) line[strlen(line)] = UNSEENLOC;
    strcpy(display[VIEW_SIZE - (DISPLAY_SIZE - VIEW_SIZE)/2 + 4], center(line, UNSEENLOC, DISPLAY_SIZE*2));
    frame(display, DISPLAY_SIZE - VIEW_SIZE + 3, VIEW_SIZE - (DISPLAY_SIZE - VIEW_SIZE)/2 + 4, 1, 1);

    memset(line, 0, strlen(line));
    strcpy(line, "  ");
    strcat(line, getTrimmedText(getMenuItemFromId(m, m.cursorId)->desc, VIEW_SIZE*2 - 2, 1));
    while(strlen(line) <= VIEW_SIZE*2 + 1) line[strlen(line)] = UNSEENLOC;
    strcpy(display[VIEW_SIZE - (DISPLAY_SIZE - VIEW_SIZE)/2 + 7], center(line, UNSEENLOC, DISPLAY_SIZE*2));

    memset(line, 0, strlen(line));
    strcpy(line, "  ");
    strcat(line, getTrimmedText(getMenuItemFromId(m, m.cursorId)->desc, VIEW_SIZE*2 - 2, 2));
    while(strlen(line) <= VIEW_SIZE*2 + 1) line[strlen(line)] = UNSEENLOC;
    strcpy(display[VIEW_SIZE - (DISPLAY_SIZE - VIEW_SIZE)/2 + 8], center(line, UNSEENLOC, DISPLAY_SIZE*2));

    frame(display, DISPLAY_SIZE - VIEW_SIZE + 1, (DISPLAY_SIZE - VIEW_SIZE)/2   + 1, VIEW_SIZE*2 - 2, ((VIEW_SIZE-3)/3)*2);
    frame(display, DISPLAY_SIZE - VIEW_SIZE + 1, (3*VIEW_SIZE - DISPLAY_SIZE)/2 + 3, VIEW_SIZE*2 - 2,  (VIEW_SIZE-3)/3);

	free(line);

	return display;
}
