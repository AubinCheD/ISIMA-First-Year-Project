/* *************************************** */
/* Main est le module principale contenant */
/* la fonction principale et les fonctions */
/* de gestion et d'initialisation du jeu   */
/* *************************************** */

#include "main.h"

/*************************************************************************************************/

/* *********************************************************** */
/* Fonction principale, initialise le jeu, g�re les ev�nements */
/* clavier et nettoie la m�moire � la fin de la partie.        */
/* *********************************************************** */

int main()
{
    character * hero = (character *) malloc(sizeof(character));
    level     * floor = (level *) malloc(sizeof(level));
    items = creerInventaireHash("items.ipjt");
    int key, quit = 0;

    startGame(floor, hero);

	while (!quit)
    {
        switch(handleKeyboardEvent(floor, hero))
        {
            case 0 : playMonstersTurn(floor, hero);
            case 1 : display(getBoardDisplay(*floor, *hero), NULL); break;
            case 2 : startGame(floor, hero); break;
            case 3 : quit = 1; break;
        }

        if (hero->ent.stats.currentHP <=0)
        {
            display(getBoardDisplay(*floor, *hero), getGameOverDisplay());

            while ((key = getch()) != 13 && key != 27 && key != 82 && key != 114);

            if (key == 82 || key == 114) startGame(floor, hero);
            else quit = 1;
        }
    }

    system("cls");

	freeMemory(floor, hero);

	return 0;
}

/*************************************************************************************************/

void startGame(level * floor, character * hero)
{
    generateLevel(floor, 1);
    generateHero(hero, floor->entry);

    display(getBoardDisplay(*floor, *hero), NULL);
}

/*************************************************************************************************/

/* ******************************************** */
/* G�re les ev�nements claviers pour quitter le */
/* jeu, changer de niveau ou d�placer le h�ros. */
/* ******************************************** */

int handleKeyboardEvent(level * floor, character * hero)
{
    int ret = 0;

    switch(getch())
    {
        /*case 27 : (*hero)->ent.stats.currentHP = 0; break;*/ /* Echap : Quitter le jeu */
        case 27 : ret = loadMenu(floor, hero, getBoardDisplay(*floor, *hero), getMainMenu(*hero)); break;
        case 13 : /* Entr�e */
            if (hero->ent.obj.x == floor->exit.x && hero->ent.obj.y == floor->exit.y) /* Si le h�ros a atteint les escaliers de sortie */
            {
                hero->exp += 50;
                generateLevel(floor, floor->number + 1);
                hero->ent.obj.x    = floor->entry.x;
                hero->ent.obj.y    = floor->entry.y;    /* Place le h�ros sur l'entr�e du nouvel �tage */
                hero->ent.obj.room = floor->entry.room;
            } break;
        case 0 : case 224 : /* Touches fl�ch�es (getch() renvoie deux caract�res pour les touches fl�ch�es -> 1er caract�re : 0 = touches fl�ch�es du pav� num�rique, 224 = touches fl�ch�es */
            switch(getch()) /*                                                                             -> 2e  caract�re : code de la touche fl�ch�e pr�ss�e)                             */
            {
                case 72 : ret = !updateCharacterPos(hero, floor, hero->ent.obj.x,   hero->ent.obj.y-1); break; /* Fl�che du haut   */
                case 75 : ret = !updateCharacterPos(hero, floor, hero->ent.obj.x-1, hero->ent.obj.y  ); break; /* Fl�che de gauche */
                case 80 : ret = !updateCharacterPos(hero, floor, hero->ent.obj.x,   hero->ent.obj.y+1); break; /* Fl�che du bas    */
                case 77 : ret = !updateCharacterPos(hero, floor, hero->ent.obj.x+1, hero->ent.obj.y  ); break; /* Fl�che de droite */
            } break;
        case 90 : case 122 : ret = !updateCharacterPos(hero, floor, hero->ent.obj.x,   hero->ent.obj.y-1); break; /* z ou Z */
        case 81 : case 113 : ret = !updateCharacterPos(hero, floor, hero->ent.obj.x-1, hero->ent.obj.y  ); break; /* q ou Q */
        case 83 : case 115 : ret = !updateCharacterPos(hero, floor, hero->ent.obj.x,   hero->ent.obj.y+1); break; /* s ou S */
        case 68 : case 100 : ret = !updateCharacterPos(hero, floor, hero->ent.obj.x+1, hero->ent.obj.y  ); break; /* d ou D */
    }

    return ret; /* Retourne 1 si l'�cran doit �tre mis � jour, O sinon */
}

/*************************************************************************************************/

item * getItems()
{
    item * items = (item *) malloc(sizeof(item)*10);

    items[0].name = "Key";
    items[0].c = KEY;
    items[0].weight = 1;
    items[0].desc = "This may open a door...";
    items[0].dropRate = 0.33;

    items[1].name = "Premium Key";
    items[1].c = KEY;
    items[1].weight = 2;
    items[1].desc = "Opens all doors.";
    items[1].dropRate = 0.05;

    items[2].name = "Small Health Potion";
    items[2].c = HEALTHPOTS;
    items[2].weight = 3;
    items[2].desc = "Regenerate 25% of max HP.";
    items[2].dropRate = 0.3;

    items[3].name = "Big Health Potion";
    items[3].c = HEALTHPOTB;
    items[3].weight = 5;
    items[3].desc = "Regenerate 50% of max HP.";
    items[3].dropRate = 0.15;

    items[4].name = "Helmet";
    items[4].c = HELMET;
    items[4].weight = 10;
    items[4].desc = "HP max +5. Should be put on the head.";
    items[4].dropRate = 0.15;

    items[5].name = "Chestplate";
    items[5].c = CHESTPLATE;
    items[5].weight = 25;
    items[5].desc = "HP max +15.";
    items[5].dropRate = 0.05;

    items[6].name = "Leggings";
    items[6].c = LEGGHINGS;
    items[6].weight = 20;
    items[6].desc = "HP max +10.";
    items[6].dropRate = 0.1;

    items[7].name = "Boots";
    items[7].c = BOOTS;
    items[7].weight = 10;
    items[7].desc = "HP max +5. Should be put on the feet.";
    items[7].dropRate = 0.15;

    items[8].name = "Sword";
    items[8].c = SWORD;
    items[8].weight = 30;
    items[8].desc = "Strength x2. Way more efficient than punches.";
    items[8].dropRate = 0.05;

    items[9].name = "Bag";
    items[9].c = BAG;
    items[9].weight = 10;
    items[9].desc = "Inventory size x2.";
    items[9].dropRate = 0.05;

    return items;
}

/*************************************************************************************************/

/* ****************************************************** */
/* Joue le tour de chaque monstre de l'�tage.             */
/* Essaye d'attaquer le h�ros et s'il n'est pas � port�e, */
/* essaye de se d�placer et r�essaye d'attaquer le h�ros. */
/* ****************************************************** */

void playMonstersTurn(level *floor, character *hero)
{
    int i;

    srand(time(NULL));

    for(i=0; i < floor->nbMonsters; i++)
        if (floor->monsters[i].ent.stats.currentHP > 0 && !tryFight(floor->monsters[i], hero)) /* Si le monstre est en vie */
        {
            switch(rand()%4)
            {
                case 0 : if (isEmpty(floor->monsters[i].ent.obj.x,   floor->monsters[i].ent.obj.y-1, floor->monsters[i].ent.obj.room, *floor, *hero)) floor->monsters[i].ent.obj.y--; break; /* D�placement vers le haut   */
                case 1 : if (isEmpty(floor->monsters[i].ent.obj.x+1, floor->monsters[i].ent.obj.y,   floor->monsters[i].ent.obj.room, *floor, *hero)) floor->monsters[i].ent.obj.x++; break; /* D�placement vers la droite */
                case 2 : if (isEmpty(floor->monsters[i].ent.obj.x,   floor->monsters[i].ent.obj.y+1, floor->monsters[i].ent.obj.room, *floor, *hero)) floor->monsters[i].ent.obj.y++; break; /* D�placement vers le bas    */
                case 3 : if (isEmpty(floor->monsters[i].ent.obj.x-1, floor->monsters[i].ent.obj.y,   floor->monsters[i].ent.obj.room, *floor, *hero)) floor->monsters[i].ent.obj.x--; break; /* D�placement vers la gauche */
            }
            tryFight(floor->monsters[i], hero);
        }
}

/*************************************************************************************************/

/* **************************************************** */
/* Attaque le h�ros s'il est � port�e du monstre donn�. */
/* Renvoie 1 si le h�ros � �t� attaqu�, 0 sinon.        */
/* **************************************************** */

int tryFight(monster attacker, character *hero)
{
    int fought = 1;

    if (attacker.ent.obj.room == hero->ent.obj.room && ((attacker.ent.obj.x == hero->ent.obj.x && (attacker.ent.obj.y-1 == hero->ent.obj.y || attacker.ent.obj.y+1 == hero->ent.obj.y)) || /* Si le h�ros est sur la case en-dessous ou au-dessus */
                                                        (attacker.ent.obj.y == hero->ent.obj.y && (attacker.ent.obj.x-1 == hero->ent.obj.x || attacker.ent.obj.x+1 == hero->ent.obj.x))))  /* ou si le h�ros est sur la case � gauche ou � droite */
        hero->ent.stats.currentHP -= attacker.ent.stats.strength;
    else fought = 0;

    return fought;
}

/*************************************************************************************************/

char ** getGameOverDisplay()
{
    char ** display   = initDisplay();
    char  * gameOver   = "GAME OVER !";
    char  * pressEnter = "Press enter to continue or R to start a new game";
    int     i;

	strcpy(display[DISPLAY_SIZE/2 - 2], "  ");
	strcat(display[DISPLAY_SIZE/2 - 2], gameOver);
	strcat(display[DISPLAY_SIZE/2 - 2], "  ");

	strcpy(display[DISPLAY_SIZE/2 + 2], "  ");
	strcat(display[DISPLAY_SIZE/2 + 2], pressEnter);
	strcat(display[DISPLAY_SIZE/2 + 2], "  ");

    memset(display[DISPLAY_SIZE/2 - 3], 0, strlen(display[DISPLAY_SIZE/2 - 3]));
	memset(display[DISPLAY_SIZE/2 - 1], 0, strlen(display[DISPLAY_SIZE/2 - 1]));
	frame(display, 2, DISPLAY_SIZE/2 - 2, strlen(gameOver), 1);

	memset(display[DISPLAY_SIZE/2 + 1], 0, strlen(display[DISPLAY_SIZE/2 + 1]));
	memset(display[DISPLAY_SIZE/2 + 3], 0, strlen(display[DISPLAY_SIZE/2 + 3]));
	frame(display, 2, DISPLAY_SIZE/2 + 2, strlen(pressEnter), 1);

	for(i=DISPLAY_SIZE/2 - 3; i <= DISPLAY_SIZE/2 + 3; i++)
        strcpy(display[i], center(display[i], UNSEENLOC, DISPLAY_SIZE*2));

    return display;
}

/*************************************************************************************************/

/* ***************************************************** */
/* Lib�re toute la m�moire allou� au cours du programme. */
/* **************************************************** */

void freeMemory(level *floor, character *hero)
{
    int i, j;

    for(i=0; i < floor->nbRoom; i++)
    {
        for(j=0; j < floor->rooms[i].height; j++) free(floor->rooms[i].roomBoard[j]); /* Lib�re chaque ligne de la matrice de l'�tage */
        free(floor->rooms[i].roomBoard);
    }
	free(floor->rooms);

	for(i=0; i < floor->nbMonsters; i++) libererInventaire(floor->monsters[i].ent.inv);
	free(floor->monsters);

	free(floor);

	libererInventaire(hero->ent.inv);
	free(hero);
}

