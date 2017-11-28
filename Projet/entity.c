#include "entity.h"

/*************************************************************************************************/

/* ************************************* */
/* Initialise le h�ros et ses param�tres */
/* et le place � l'entr�e de l'�tage.    */
/* ************************************* */

void generateHero(character * hero, object entry)
{
    srand(time(NULL));

    hero->exp = 0;
    hero->mode = 0;

    hero->name = displayMessage("What's your name, hero ?", 14);

    if (strcmp(hero->name, "Urun"))
    {
        hero->ent.stats.currentHP = hero->ent.stats.maxHP = rand()%16 + 5; /* Points de vie al�atoires entre 5 et 20 */
        hero->ent.stats.strength  = rand()%5 + 1; /* Force al�atoire entre 1 et 5 */
    }
    else
    {
        hero->ent.stats.currentHP = hero->ent.stats.maxHP = 512; /* Points de vie infini */
        hero->ent.stats.strength  = 64; /* Force infinie */
    }

    hero->ent.inv = initInventaire(0);
    hero->invWeight = 0;
    hero->invWeightMax = HERO_INV_WEIGHT_MAX;

    hero->ent.obj.x     = entry.x;
    hero->ent.obj.y     = entry.y;
    hero->ent.obj.room  = entry.room;
    hero->ent.obj.c     = HERO;
    hero->ent.obj.color = heroColorChooser();
}

/*************************************************************************************************/

char * heroColorChooser()
{
    char * colors[] = {WHITE, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN};
    char * display[3];
    int i, j, cursor = 0, chosen = 0;

    for(i=0; i < 3; i++) display[i] = (char *) malloc(sizeof(char)*(7 + strlen(WHITE) + strlen(RESET))*7);

    do
    {
        for(i=0; i < 3; i++) memset(display[i], 0, (7 + strlen(WHITE) + strlen(RESET))*7);
        for(i=0; i < 7; i++)
        {
            display[0][strlen(display[0])] = display[2][strlen(display[2])] = UNSEENLOC;
            display[0][strlen(display[2])] = WALL_C_U_L;
            display[2][strlen(display[2])] = WALL_C_D_L;
            for(j=0; j < 3 + 2*(i==cursor); j++) display[0][strlen(display[0])] = display[2][strlen(display[2])] = WALL_H;
            display[0][strlen(display[0])] = WALL_C_U_R;
            display[2][strlen(display[2])] = WALL_C_D_R;
            display[1][strlen(display[1])] = UNSEENLOC;
            display[1][strlen(display[1])] = WALL_V;
            display[1][strlen(display[1])] = UNSEENLOC;
            if (i == cursor)
            {
                display[1][strlen(display[1])] = CURSOR;
                display[1][strlen(display[1])] = UNSEENLOC;
            }
            strcat(display[1], colors[i]);
            display[1][strlen(display[1])] = HERO;
            strcat(display[1], RESET);
            display[1][strlen(display[1])] = UNSEENLOC;
            display[1][strlen(display[1])] = WALL_V;
        }

        displayMessage("Please select a character", 0);
        printf("%s\n%s\n%s\n", display[0], display[1], display[2]);
        switch(getch())
        {
            case 0 : case 224 : /* Touches fl�ch�es (getch() renvoie deux caract�res pour les touches fl�ch�es -> 1er caract�re : 0 = touches fl�ch�es du pav� num�rique, 224 = touches fl�ch�es */
                switch(getch()) /*                                                                             -> 2e  caract�re : code de la touche fl�ch�e pr�ss�e)                             */
                {
                    case 75 : cursor = cursor ? cursor-1 : 6; break; /* Fl�che de gauche */
                    case 77 : cursor = (cursor+1)%7;          break; /* Fl�che de droite */
                } break;
            case 13 : chosen = 1; break;
        }
    } while (!chosen);

    return colors[cursor];
}

/*************************************************************************************************/

/* ********************************************************************** */
/* Met � jour la position du h�ros s'il est autoris� � se d�placer sur    */
/* la case de destination. Retourne 1 si le h�ros a �t� d�plac�, 0 sinon. */
/* ********************************************************************** */

int updateCharacterPos(character *hero, level *floor, int x, int y)
{
    int i, moved = 0;
    monster *oponent = NULL;
    room actualRoom  = floor->rooms[hero->ent.obj.room];
    int destination;

    if ((hero->ent.obj.x == actualRoom.x && x <= actualRoom.x) || (hero->ent.obj.x == actualRoom.x + actualRoom.width  - 1 && x >= actualRoom.x + actualRoom.width  - 1) ||
        (hero->ent.obj.y == actualRoom.y && y <= actualRoom.y) || (hero->ent.obj.y == actualRoom.y + actualRoom.height - 1 && y >= actualRoom.y + actualRoom.height - 1))
        moved = updateActualRoom(*floor, hero);

    actualRoom  = floor->rooms[hero->ent.obj.room];
    destination = actualRoom.roomBoard[y-actualRoom.y][x-actualRoom.x];

    for(i=0; i < floor->nbMonsters && oponent == NULL; i++)
        if (floor->monsters[i].ent.obj.x == x && floor->monsters[i].ent.obj.y == y)
            oponent = &(floor->monsters[i]);

    if (oponent != NULL)
    {
        oponent->ent.stats.currentHP -= hero->ent.stats.strength;
        if (oponent->ent.stats.currentHP <= 0)
        {
            die(*floor, oponent);
            hero->exp += oponent->ent.stats.maxHP * oponent->ent.stats.strength;
        }
        moved = 1;
    }
    else
    {
        if(!isWall(destination) || destination == DOOR_H || destination == DOOR_V)
        {
            hero->ent.obj.x = x;
            hero->ent.obj.y = y;
            moved = 1;
        }
    }

    return moved;
}

/*************************************************************************************************/

int updateActualRoom(level floor, character *hero)
{
    int i, oldRoom = hero->ent.obj.room;

    for (i=0; i < floor.nbRoom && hero->ent.obj.room == oldRoom; i++)
    {
        if (hero->ent.obj.x >= floor.rooms[i].x && hero->ent.obj.x <= floor.rooms[i].x + floor.rooms[i].width  - 1 &&
            hero->ent.obj.y >= floor.rooms[i].y && hero->ent.obj.y <= floor.rooms[i].y + floor.rooms[i].height - 1 && i != oldRoom &&
            floor.rooms[i]      .roomBoard[hero->ent.obj.y - floor.rooms[i]      .y][hero->ent.obj.x - floor.rooms[i]      .x] ==
            floor.rooms[oldRoom].roomBoard[hero->ent.obj.y - floor.rooms[oldRoom].y][hero->ent.obj.x - floor.rooms[oldRoom].x])
           hero->ent.obj.room = i;
    }

    return !(hero->ent.obj.room == oldRoom);
}

/*************************************************************************************************/

void die(level floor, monster * mon)
{
    mon->ent.obj.x = mon->ent.obj.y = -1;
}

/*************************************************************************************************/

/* ******************************************************************************** */
/* Calcule un nombre al�atoire de monstres en fonction de l'�tage. Calcul un nombre */
/* de monstres maximum en fonction de l'�tage puis prend un nombre al�atoire entre  */
/* le minimum et le maximum calcul�. La courbe d'�volution suis la forme de log(x). */
/* ******************************************************************************** */

int getNbMonsters(int level)
{
    srand(time(NULL));

    return rand()%(int)(LEVEL_WEIGHT/2 * log(level) + MONSTERS_INITIAL_MAX);
}

/*************************************************************************************************/

monster generateMonster(level floor)
{
    monster mon;

    do mon.ent.obj.room = rand()%floor.nbRoom; while (isRoomFull(floor, mon.ent.obj.room));

    mon.ent.stats.currentHP = mon.ent.stats.maxHP = rand()%(5*floor.number) + floor.number;
    mon.ent.stats.strength = rand()%(2*floor.number) + floor.number;
    mon.ent.obj.c = MONSTER;
    mon.ent.obj.color = (mon.ent.stats.maxHP*2 + mon.ent.stats.strength < 6*floor.number) ? GREEN :
                        (mon.ent.stats.maxHP*2 + mon.ent.stats.strength > 9*floor.number) ? RED   : YELLOW;
    mon.ent.inv = initInventaire(0);
    do {
        mon.ent.obj.x = floor.rooms[mon.ent.obj.room].x + 1 + rand()%(floor.rooms[mon.ent.obj.room].width  - 1);
        mon.ent.obj.y = floor.rooms[mon.ent.obj.room].y + 1 + rand()%(floor.rooms[mon.ent.obj.room].height - 1);
    } while (floor.rooms[mon.ent.obj.room].roomBoard[mon.ent.obj.y - floor.rooms[mon.ent.obj.room].y][mon.ent.obj.x - floor.rooms[mon.ent.obj.room].x] != EMPTYLOC || /* Recherche une case libre */
             getObjectByCoord(floor, mon.ent.obj.x, mon.ent.obj.y, mon.ent.obj.room) != NULL);                                                                          /*   o� placer le monstre   */

    return mon;
}

/*************************************************************************************************/

/* ***************************************************************** */
/* Recherche le nombre de monstres � g�n�rer sur l'�tage, initialise */
/* chaque monstre et le place sur une case libre de l'�tage.         */
/* ***************************************************************** */

void generateMonsters(level *floor)
{
    int nbMonsters = getNbMonsters(floor->number); /* R�cup�re le nombre de monstres � g�n�rer */

    srand(time(NULL));

    floor->monsters = (monster *) malloc(sizeof(monster)*nbMonsters);
    for(floor->nbMonsters = 0; floor->nbMonsters < nbMonsters; floor->nbMonsters++)
        floor->monsters[floor->nbMonsters] = generateMonster(*floor);
}

/*************************************************************************************************/

double difficulteSup2 (room * room, int nbRoom,  monster * mon, int nbMonsters) {  /*on connait le nombre de salle mais pas le nombre de montres*/

    int i;
    int * nbMobsParSalle = NULL;
    double * difficulteParSalle = NULL;

    double difficulte;

    nbMobsParSalle = (int *) malloc (nbRoom * sizeof(int));
    difficulteParSalle = (double *) malloc (nbRoom * sizeof(double));


    for(i=0;i<nbRoom;i++) {
        nbMobsParSalle[i] = 0;
        difficulteParSalle[i] = 0.0;
    }

    for (i=0;i<nbMonsters;i++) {
        nbMobsParSalle[mon[i].ent.obj.room]++;
    }

    for (i=0;i<nbRoom;i++) {

        difficulteParSalle[i] = ( (double) (nbMobsParSalle[i]) / (double)(room[i].width * room[i].height));

    }

    difficulte = maxTab(difficulteParSalle,nbRoom);

    free(nbMobsParSalle);
    free(difficulteParSalle);

    return difficulte;
}

/*************************************************************************************************/

void generateMonsters2(level *floor) {
    int nbMonsters = getNbMonsters(floor->number); /* R�cup�re le nombre de monstres � g�n�rer */
    monster * listeMonstres = NULL;
    int i;

    srand(time(NULL));

    if (floor->number==1) {
        floor->monsters = (monster *) malloc(sizeof(monster)*nbMonsters);
        for(floor->nbMonsters = 0; floor->nbMonsters < nbMonsters; floor->nbMonsters++)
            floor->monsters[floor->nbMonsters] = generateMonster(*floor);
        floor->difficulte = difficulteSup2(floor->rooms,floor->nbRoom,floor->monsters,floor->nbMonsters);
    }

    else {
        listeMonstres = (monster *) malloc(sizeof(monster)*100);                            /*100 pour �tre large*/
        for(floor->nbMonsters = 0; floor->nbMonsters < nbMonsters; floor->nbMonsters++)
            listeMonstres[floor->nbMonsters] = generateMonster(*floor);


        while (difficulteSup2(floor->rooms,floor->nbRoom, listeMonstres, floor->nbMonsters) < floor->difficulte && floor->nbMonsters<100) {  /*on rajoute des monstres*/
            listeMonstres[floor->nbMonsters] = generateMonster(*floor);
            floor->nbMonsters++;
        }

        floor->monsters = (monster *) malloc(sizeof(monster)*floor->nbMonsters);
        for (i=0;i<floor->nbMonsters;i++)
            floor->monsters[i]=listeMonstres[i];

        free(listeMonstres);
    }
}
