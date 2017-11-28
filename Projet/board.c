/* ***************************************** */
/* Board contient les fonctions relatives à  */
/* la génération et à l'affichage des étages */
/* ***************************************** */

#include "board.h"

/*************************************************************************************************/

/* ****************************************************** */
/* Génère un niveau en initialisant les paramètes et en   */
/* générant un étage et les entités au sein de cet étage. */
/* ****************************************************** */

void generateLevel(level *floor, int levelNumber)
{
    room entryRoom, exitRoom;


    srand(time(NULL));
    floor->number = levelNumber;

    generateBoard(floor);

    entryRoom = floor->rooms[floor->entry.room];
    exitRoom  = floor->rooms[floor->exit.room];

    do {
        floor->entry.x = entryRoom.x + 1 + rand()%(entryRoom.width  - 2);
        floor->entry.y = entryRoom.y + 1 + rand()%(entryRoom.height - 2); /* Place l'entrée sur une case libre */
    } while (entryRoom.roomBoard[floor->entry.y - entryRoom.y][floor->entry.x - entryRoom.x] != EMPTYLOC);
    do {
        floor->exit.x = exitRoom.x + 1 + rand()%(exitRoom.width  - 2);
        floor->exit.y = exitRoom.y + 1 + rand()%(exitRoom.height - 2); /* Place la sortie sur une case libre */
    } while(exitRoom.roomBoard[floor->exit.y - exitRoom.y][floor->exit.x - exitRoom.x] != EMPTYLOC ||
           (floor->exit.x == floor->entry.x && floor->exit.y == floor->entry.y));

    generateMonsters2(floor);
}

/*************************************************************************************************/

/* ******************************************************************** */
/* Génère un étage et y place des salles et les couloirs correspondant. */
/* ******************************************************************** */

void generateBoard(level *floor)
{
    free(floor->rooms);

    floor->nbRoom = getNbRooms(floor->number);
    floor->rooms = (room *) malloc(sizeof(room) * (floor->nbRoom*(floor->nbRoom+1)/2));

    floor->boardHeight = getBoardSize(floor->number);
    floor->boardWidth  = getBoardSize(floor->number);

    generateRooms(floor);                  /* Génère ensuite des salles */
    generateCorridors(floor);              /* Place enfin les couloirs  */

    floor->entry.c = STAIRS_U;
    floor->entry.color = GREEN;
    floor->entry.room = rand()%floor->nbRoom;

    floor->exit.c = STAIRS_D;
    floor->exit.color = GREEN;
    floor->exit.room = rand()%floor->nbRoom;
}

/*************************************************************************************************/

/* ******************************************************************* */
/* Génère les salles d'un étage. Pour chaque salles, choisit des       */
/* dimensions aléatoires et essaye de l'inserer à un endroit aléatoire */
/* de l'étage. Renvoie le nombre de salles réellement créées.          */
/* ******************************************************************* */

void generateRooms(level *floor)
{
    int nbRoom = floor->nbRoom;

    srand(time(NULL));

    for(floor->nbRoom=0; floor->nbRoom < nbRoom; floor->nbRoom++)
    {
        do {
            floor->rooms[floor->nbRoom].width  = rand()%((int)(floor->boardWidth  * 0.33) - MIN_ROOM_SIZE) + MIN_ROOM_SIZE; /* Tant qu'on arrive pas à insérer et que la */
            floor->rooms[floor->nbRoom].height = rand()%((int)(floor->boardHeight * 0.33) - MIN_ROOM_SIZE) + MIN_ROOM_SIZE; /* salle n'est pas aux dimmensions minimales */
        } while(!isInsertable(floor, floor->rooms[floor->nbRoom]) &&
                (floor->rooms[floor->nbRoom].width > MIN_ROOM_SIZE || floor->rooms[floor->nbRoom].height > MIN_ROOM_SIZE));

        if(isInsertable(floor, floor->rooms[floor->nbRoom]))
        {
            do {
                floor->rooms[floor->nbRoom].x = rand()%(floor->boardWidth  - floor->rooms[floor->nbRoom].width);
                floor->rooms[floor->nbRoom].y = rand()%(floor->boardHeight - floor->rooms[floor->nbRoom].height);
            } while(!isInsertableToCoord(floor, floor->rooms[floor->nbRoom]));

            floor->rooms[floor->nbRoom].roomBoard  = drawRoom(floor->rooms[floor->nbRoom]);
            floor->rooms[floor->nbRoom].isCorridor = 0;
        }
        else nbRoom = floor->nbRoom;
    }
}
/*************************************************************************************************/

/* ***************************************** */
/* Construit une salle donnée dans un étage. */
/* ***************************************** */

int ** drawRoom(room toDraw)
{
    int x, y;
    int ** board;

    board = (int **) malloc(sizeof(int *)*toDraw.height);
    for(y = 0; y < toDraw.height; y++) board[y] = (int *) malloc(sizeof(int)*toDraw.width);

    board[0]              [0]              = WALL_C_U_L;
    board[toDraw.height-1][0]              = WALL_C_D_L;
    board[0]              [toDraw.width-1] = WALL_C_U_R;
    board[toDraw.height-1][toDraw.width-1] = WALL_C_D_R;

    for(x = 1; x < toDraw.width-1;  x++) board[0][x] = board[toDraw.height-1][x] = WALL_H;
	for(y = 1; y < toDraw.height-1; y++)
    {
        board[y][0] = board[y][toDraw.width-1] = WALL_V;
        for(x = 1; x < toDraw.width-1; x++) board[y][x] = EMPTYLOC;
    }

    return board;
}

/*************************************************************************************************/

/* **************************************************** */
/* Génère les couloirs reliant les salles entres elles. */
/* **************************************************** */

void generateCorridors(level *floor)
{
    arete_t * corridors = getCorridors(floor->nbRoom), * cour;

    for(cour = corridors; cour != NULL; cour = cour->next) drawCorridor(floor, cour->s1, cour->s2);

    supprimerListeAretes(corridors);
}

/*************************************************************************************************/

/* **************************************************** */
/* Recherche les deux salles les plus proches où il est */
/* impossible d'accéder à une salle depuis l'autre.     */
/* **************************************************** */

void getNextCorridor(level * floor, int *connectedRooms, int nbConnectedRooms, int *room1, int *room2)
{
    int i, j, k;
    double dist, minDist = 1000;

    for (i=0; (nbConnectedRooms && i < nbConnectedRooms) || (!nbConnectedRooms && i < floor->nbRoom); i++) /* On cherche la plus faible distance entre une    */
        for (j=0; j < floor->nbRoom; j++)                                                                  /* pièce déjà connectée et une pièce non connectée */
        {
            k = nbConnectedRooms ? connectedRooms[i] : i;
            if(!intContains(connectedRooms, j, nbConnectedRooms) && k != j && !floor->rooms[j].isCorridor && !floor->rooms[k].isCorridor)
            {
                dist = sqrt(pow(floor->rooms[j].x-floor->rooms[k].x,2) + pow(floor->rooms[j].y-floor->rooms[k].y,2)); /* distance entre A et B = racine((Ax-Bx)^2 + (Ay-By)^2) */
                if (dist < minDist)
                {
                    *room1 = k;
                    *room2 = j;
                    minDist = dist;
                }
            }
        }
}

/*************************************************************************************************/

/* *********************************************************** */
/* Construit un couloir entre deux salles données sur l'étage. */
/* *********************************************************** */
                                                                                                                                                       /* *************** */
int drawCorridor(level *floor, int room1, int room2)                                                                                                   /*        0        */
{                                                                                                                                                      /*   +---------+   */
    int wallRoom1, wallRoom2;                                                                                                                          /*   |         |   */
    object door1, door2;                                                                                                   /* Numérotation des murs */ /* 3 |  Salle  | 1 */
    int sensH, sensV;                                                                                                                                  /*   |         |   */
    int i, drawn = 0;                                                                                                                                  /*   +---------+   */
                                                                                                                                                       /*        2        */
    getRoomWalls(floor->rooms[room1], floor->rooms[room2], &wallRoom1, &wallRoom2);                /* Recherche les murs des salles à connecter */     /* *************** */
    if(getDoors(floor->rooms[room1], floor->rooms[room2], wallRoom1, wallRoom2, &door1, &door2) && /*  Recherche l'emplacement de chaque porte  */
      (door1.x != door2.x || door1.y != door2.y))
    {
        floor->rooms[room1].roomBoard[door1.y - floor->rooms[room1].y][door1.x - floor->rooms[room1].x] = door1.c;
        floor->rooms[room2].roomBoard[door2.y - floor->rooms[room2].y][door2.x - floor->rooms[room2].x] = door2.c;

        floor->rooms[floor->nbRoom] = initCorridor(door1, door2);
        door1.x -= floor->rooms[floor->nbRoom].x;
        door1.y -= floor->rooms[floor->nbRoom].y;
        door2.x -= floor->rooms[floor->nbRoom].x;
        door2.y -= floor->rooms[floor->nbRoom].y;

        if (wallRoom1%2 == 0) /* Si le mur de la première salle est le mur du haut (0) ou le mur du bas (2) */
        {
            sensV = (wallRoom1 == 0) ? -1 : 1;
            sensH = (wallRoom2 == 1) ? -1 : 1; /* Utilisé qu'en cas de couloir coudé, c'est-à-dire si le mur de la deuxième salle est le mur droite (1) ou gauche (3) */

            floor->rooms[floor->nbRoom].roomBoard[door1.y]      [door1.x-1] = (wallRoom1 == 0) ? WALL_C_D_L : WALL_C_U_L;
            floor->rooms[floor->nbRoom].roomBoard[door1.y]      [door1.x+1] = (wallRoom1 == 0) ? WALL_C_D_R : WALL_C_U_R;

            for (i = door1.y+sensV; i*sensV < (door2.y-sensV)*sensV; i+=sensV) /* On crée la partie verticale du couloir. */
            {                                                                  /* On s'arrête une case avant l'ordonnée de la porte 2 */
                floor->rooms[floor->nbRoom].roomBoard[i][door1.x]   = EMPTYLOC;
                floor->rooms[floor->nbRoom].roomBoard[i][door1.x-1] = floor->rooms[floor->nbRoom].roomBoard[i][door1.x+1] = WALL_V;
            }

            if(wallRoom1%2 == wallRoom2%2) /* Si le couloir n'est pas coudé */
            {
                floor->rooms[floor->nbRoom].roomBoard[door2.y-sensV][door1.x]   = EMPTYLOC;
                floor->rooms[floor->nbRoom].roomBoard[door2.y-sensV][door1.x-1] = WALL_V;
                floor->rooms[floor->nbRoom].roomBoard[door2.y-sensV][door1.x+1] = WALL_V;
                floor->rooms[floor->nbRoom].roomBoard[door2.y]      [door2.x-1] = (wallRoom1 == 0) ? WALL_C_U_L : WALL_C_D_L;
                floor->rooms[floor->nbRoom].roomBoard[door2.y]      [door2.x+1] = (wallRoom1 == 0) ? WALL_C_U_R : WALL_C_D_R;
            }
            else /* Si le couloir est coudé */
            {
                floor->rooms[floor->nbRoom].roomBoard[door2.y-sensV][door1.x]       = EMPTYLOC;
                floor->rooms[floor->nbRoom].roomBoard[door2.y]      [door1.x]       = EMPTYLOC;
                floor->rooms[floor->nbRoom].roomBoard[door2.y]      [door1.x+sensH] = EMPTYLOC;
                floor->rooms[floor->nbRoom].roomBoard[door2.y-sensV][door1.x-sensH] = WALL_V;
                floor->rooms[floor->nbRoom].roomBoard[door2.y]      [door1.x-sensH] = WALL_V;                                                  /* On crée le coude */
                floor->rooms[floor->nbRoom].roomBoard[door2.y+sensV][door1.x]       = WALL_H;
                floor->rooms[floor->nbRoom].roomBoard[door2.y+sensV][door1.x+sensH] = WALL_H;
                floor->rooms[floor->nbRoom].roomBoard[door2.y-sensV][door1.x+sensH] =
                floor->rooms[floor->nbRoom].roomBoard[door2.y+sensV][door1.x-sensH] =
                (wallRoom1 == 0) ? ((wallRoom2 == 1) ? WALL_C_U_R : WALL_C_U_L) : ((wallRoom2 == 1) ? WALL_C_D_R : WALL_C_D_L);

                for(i = door1.x+2*sensH; i*sensH < door2.x*sensH; i += sensH) /* Puis on crée la partie horizontale du couloir */
                {
                    floor->rooms[floor->nbRoom].roomBoard[door2.y]  [i] = EMPTYLOC;
                    floor->rooms[floor->nbRoom].roomBoard[door2.y-1][i] = floor->rooms[floor->nbRoom].roomBoard[door2.y+1][i] = WALL_H;
                }

                floor->rooms[floor->nbRoom].roomBoard[door2.y-1][door2.x] = (wallRoom2 == 1) ? WALL_C_U_L : WALL_C_U_R;
                floor->rooms[floor->nbRoom].roomBoard[door2.y+1][door2.x] = (wallRoom2 == 1) ? WALL_C_D_L : WALL_C_D_R;

                if (door1.x + sensH == door2.x && door1.y + sensV != door2.y)
                {
                    floor->rooms[floor->nbRoom].roomBoard[door2.y-sensV][door1.x+sensH] = WALL_V;
                    floor->rooms[floor->nbRoom].roomBoard[door2.y][door2.x] = door2.c;
                }
                else if (door1.y + sensV == door2.y && door1.x + sensH != door2.x)
                {
                    floor->rooms[floor->nbRoom].roomBoard[door2.y-sensV][door1.x+sensH] = WALL_H;
                    floor->rooms[floor->nbRoom].roomBoard[door1.y][door1.x] = door1.c;
                }
            }
        }
        else /* Si le mur de la première salle est le mur de droite (1) ou le mur de gauche (3) */
        {
            sensV = (wallRoom2 == 0) ? 1 : -1; /* Utilisé qu'en cas de couloir coudé, c'est-à-dire si le mur de la deuxième salle est le mur haut (0) ou bas (2). */
            sensH = (wallRoom1 == 1) ? 1 : -1;

            floor->rooms[floor->nbRoom].roomBoard[door1.y-1][door1.x] = (wallRoom1 == 1) ? WALL_C_U_L : WALL_C_U_R;
            floor->rooms[floor->nbRoom].roomBoard[door1.y+1][door1.x] = (wallRoom1 == 1) ? WALL_C_D_L : WALL_C_D_R;

            for (i = door1.x+sensH; i*sensH < (door2.x-sensH)*sensH; i += sensH)/* On crée la partie horizontale du couloir. */
            {                                                                                     /* On s'arrête une case avant l'abscisse de la porte 2 */
                floor->rooms[floor->nbRoom].roomBoard[door1.y]  [i] = EMPTYLOC;
                floor->rooms[floor->nbRoom].roomBoard[door1.y-1][i] = floor->rooms[floor->nbRoom].roomBoard[door1.y+1][i] = WALL_H;
            }

            if(wallRoom1%2 == wallRoom2%2)
            {
                floor->rooms[floor->nbRoom].roomBoard[door1.y]  [door2.x-sensH] = EMPTYLOC;
                floor->rooms[floor->nbRoom].roomBoard[door1.y-1][door2.x-sensH] = WALL_H;
                floor->rooms[floor->nbRoom].roomBoard[door1.y+1][door2.x-sensH] = WALL_H;
                floor->rooms[floor->nbRoom].roomBoard[door2.y-1][door2.x] = (wallRoom2 == 1) ? WALL_C_U_L : WALL_C_U_R;
                floor->rooms[floor->nbRoom].roomBoard[door2.y+1][door2.x] = (wallRoom2 == 1) ? WALL_C_D_L : WALL_C_D_R;
            }
            else
            {
                floor->rooms[floor->nbRoom].roomBoard[door1.y]      [door2.x-sensH] =
                floor->rooms[floor->nbRoom].roomBoard[door1.y]      [door2.x]       =
                floor->rooms[floor->nbRoom].roomBoard[door1.y+sensV][door2.x]       = EMPTYLOC;
                floor->rooms[floor->nbRoom].roomBoard[door1.y-sensV][door2.x-sensH] =
                floor->rooms[floor->nbRoom].roomBoard[door1.y-sensV][door2.x]       = WALL_H;                                                  /* On crée le coude */
                floor->rooms[floor->nbRoom].roomBoard[door1.y]      [door2.x+sensH] =
                floor->rooms[floor->nbRoom].roomBoard[door1.y+sensV][door2.x+sensH] = WALL_V;
                floor->rooms[floor->nbRoom].roomBoard[door1.y+sensV][door2.x-sensH] =
                floor->rooms[floor->nbRoom].roomBoard[door1.y-sensV][door2.x+sensH] =
                (wallRoom1 == 1) ? ((wallRoom2 == 0) ? WALL_C_U_R : WALL_C_D_R) : ((wallRoom2 == 0) ? WALL_C_U_L : WALL_C_D_L);

                for(i = door1.y+2*sensV; i*sensV < door2.y*sensV; i += sensV) /* Puis on crée la partie verticale du couloir */
                {
                    floor->rooms[floor->nbRoom].roomBoard[i][door2.x]   = EMPTYLOC;
                    floor->rooms[floor->nbRoom].roomBoard[i][door2.x-1] = floor->rooms[floor->nbRoom].roomBoard[i][door2.x+1] = WALL_V;
                }
                floor->rooms[floor->nbRoom].roomBoard[door2.y][door2.x-1] = (wallRoom2 == 0) ? WALL_C_D_L : WALL_C_U_L;
                floor->rooms[floor->nbRoom].roomBoard[door2.y][door2.x+1] = (wallRoom2 == 0) ? WALL_C_D_R : WALL_C_U_R;

                if (door1.x + sensH == door2.x && door1.y + sensV != door2.y)
                {
                    floor->rooms[floor->nbRoom].roomBoard[door1.y+sensV][door2.x-sensH] = WALL_V;
                    floor->rooms[floor->nbRoom].roomBoard[door1.y][door1.x] = door1.c;
                }
                else if (door1.y + sensV == door2.y && door1.x + sensH != door2.x)
                {
                    floor->rooms[floor->nbRoom].roomBoard[door1.y+sensV][door2.x-sensH] = WALL_H;
                    floor->rooms[floor->nbRoom].roomBoard[door2.y][door2.x] = door2.c;
                }
            }
        }

        floor->nbRoom++;
        drawn = 1;
    }

    return drawn;
}

/*************************************************************************************************/

/* *********************************************** */
/* Recherche les murs de deux salles à relier par  */
/* un couloirs en fonction de leur positionnement. */
/* *********************************************** */

void getRoomWalls(room room1, room room2, int *wallRoom1, int *wallRoom2)
{
    if(room2.x < room1.x + room1.width - 2)          /* La Salle 2  n'est pas à droite  de la Salle 1 */
    {
        if(room1.x < room2.x + room2.width - 2)      /* La Salle 2  n'est pas à gauche  de la Salle 1 */
        {
            if(room2.y >= room1.y + room1.height)        { *wallRoom1 = 2; *wallRoom2 = 0; } /* salle 1 : Bas;    salle 2 : Haut   */
            else                                         { *wallRoom1 = 0; *wallRoom2 = 2; } /* salle 1 : Haut;   salle 2 : Bas    */
        }
        else                                         /* La Salle 2     est à gauche     de la Salle 1 */
        {
            if(room2.y < room1.y + room1.height - 2) /* La Salle 2 n'est pas en dessous de la Salle 1 */
            {
                if(room1.y < room2.y + room2.height - 2) { *wallRoom1 = 3; *wallRoom2 = 1; } /* salle 1 : Gauche;   salle 2 : Droite */
                else                                 /* La Salle 2     est au dessus    de la Salle 1 */
                {
                    if(rand()%2)                         { *wallRoom1 = 0; *wallRoom2 = 1; } /* salle 1 : Haut;   salle 2 : Droite */
                    else                                 { *wallRoom1 = 3; *wallRoom2 = 2; } /* salle 1 : Gauche; salle 2 : Bas    */
                }
            }
            else                                     /* La Salle 2    est en dessous    de la Salle 1 */
            {
                if(rand()%2)                             { *wallRoom1 = 2; *wallRoom2 = 1; } /* salle 1 : Bas;    salle 2 : Droite */
                else                                     { *wallRoom1 = 3; *wallRoom2 = 0; } /* salle 1 : Gauche; salle 2 : Haut   */
            }
        }
    }
    else                                             /* La Salle 2     est à droite     de la Salle 1 */
    {
        if(room2.y < room1.y + room1.height - 2)     /* La Salle 2 n'est pas en dessous de la Salle 1 */
        {
            if(room1.y < room2.y + room2.height - 2)     { *wallRoom1 = 1; *wallRoom2 = 3; } /* salle 1 : Droite;   salle 2 : Gauche */
            else                                     /* La Salle 2     est au dessus    de la Salle 1 */
            {
                if(rand()%2)                             { *wallRoom1 = 0; *wallRoom2 = 3; } /* salle 1 : Haut;   salle 2 : Gauche */
                else                                     { *wallRoom1 = 1; *wallRoom2 = 2; } /* salle 1 : Droite; salle 2 : Bas    */
            }
        }
        else                                         /* La Salle 2    est en dessous    de la Salle 1 */
        {
            if(rand()%2)                                 { *wallRoom1 = 2; *wallRoom2 = 3; } /* salle 1 : Bas;    salle 2 : Gauche */
            else                                         { *wallRoom1 = 1; *wallRoom2 = 0; } /* salle 1 : Droite; salle 2 : Haut   */
        }
    }
}

/*************************************************************************************************/

/* ***************************************** */
/* Recherche les emplacements des portes de  */
/* deux salles données sur deux murs donnés. */
/* ***************************************** */

int getDoors(room room1, room room2, int wall1, int wall2, object *door1, object *door2)
{
    int placed = 0;

    if(!isWallFull(room1, wall1) && !isWallFull(room2, wall2))
    {
        switch(wall1)   /* On assigne les coordonnées fixes des portes */
        {
            case 0 : door1->y = room1.y;                    door1->c = DOOR_H; break;
            case 1 : door1->x = room1.x + room1.width  - 1; door1->c = DOOR_V; break;
            case 2 : door1->y = room1.y + room1.height - 1; door1->c = DOOR_H; break;
            case 3 : door1->x = room1.x;                    door1->c = DOOR_V; break;
        }
        switch(wall2)
        {
            case 0 : door2->y = room2.y;                    door2->c = DOOR_H; break;
            case 1 : door2->x = room2.x + room2.width  - 1; door2->c = DOOR_V; break;
            case 2 : door2->y = room2.y + room2.height - 1; door2->c = DOOR_H; break;
            case 3 : door2->x = room2.x;                    door2->c = DOOR_V; break;
        }

        if (door1->c == door2->c) /* Si le couloir n'est pas coudé, on aligne les portes */
        {
            if (wall1%2) door2->y = door1->y = intMax(room1.y, room2.y) + 1 + rand()%(intMin(room1.y + room1.height - 1, room2.y + room2.height - 1) - intMax(room1.y, room2.y) - 1);
            else         door2->x = door1->x = intMax(room1.x, room2.x) + 1 + rand()%(intMin(room1.x + room1.width  - 1, room2.x + room2.width  - 1) - intMax(room1.x, room2.x) - 1);
        }
        else /* Un couloir faisant trois cases de large, il faut au moins 2 case de marges de chaque coté de la porte (mur du couloir + coin de la salle) */
        {
            if (wall1 == 1 || wall1 == 3) /* Si le mur de la salle 1 est le mur gauche ou droite, on inverse les deux salles pour avoir un seul cas à gérer*/
            {                                                /* 0  1  7  3 */
                do door2->x = room2.x + 1 + rand()%(room2.width  - 2);
                while (room2.roomBoard[door2->y - room2.y][door2->x - room2.x] == DOOR_H || door2->x == door1->x);
                do door1->y = room1.y + 1 + rand()%(room1.height - 2);
                while (room1.roomBoard[door1->y - room1.y][door1->x - room1.x] == DOOR_V || door1->y == door2->y);
            }
            else
            {
                do door1->x = room1.x + 1 + rand()%(room1.width  - 2);
                while (room1.roomBoard[door1->y - room1.y][door1->x - room1.x] == DOOR_H);
                do door2->y = room2.y + 1 + rand()%(room2.height - 2);
                while (room2.roomBoard[door2->y - room2.y][door2->x - room2.x] == DOOR_V);
            }
        }

        placed = 1;
    }

    return placed;
}

/*************************************************************************************************/

room initCorridor(object door1, object door2)
{
    int x, y;
    room corridor;
    object doorTmp;

    doorTmp = door1.x < door2.x ? door1 : door2;
    corridor.x = doorTmp.x;
    if(doorTmp.c == DOOR_H) corridor.x--;

    doorTmp = door1.x > door2.x ? door1 : door2;
    corridor.width = doorTmp.x - corridor.x + 1;
    if(doorTmp.c == DOOR_H) corridor.width++;

    doorTmp = door1.y < door2.y ? door1 : door2;
    corridor.y = doorTmp.y;
    if(doorTmp.c == DOOR_V) corridor.y--;

    doorTmp = door1.y > door2.y ? door1 : door2;
    corridor.height = doorTmp.y - corridor.y + 1;
    if(doorTmp.c == DOOR_V) corridor.height++;

    corridor.roomBoard = (int **) malloc(sizeof(int*)*corridor.height);
    for (y=0; y < corridor.height; y++)
    {
        corridor.roomBoard[y] = (int *) malloc(sizeof(int)*corridor.width);
        for (x=0; x < corridor.width; x++) corridor.roomBoard[y][x] = UNSEENLOC;
    }

    corridor.roomBoard[door1.y - corridor.y][door1.x - corridor.x] = door1.c;
    corridor.roomBoard[door2.y - corridor.y][door2.x - corridor.x] = door2.c;

    corridor.isCorridor = 1;

    return corridor;
}

/*************************************************************************************************/

/* *********************************************************** */
/* Affiche le champ de vision de l'étage visible par le héros. */
/* *********************************************************** */

char ** getBoardDisplay(level floor, character hero)
{
    int      i          = (DISPLAY_SIZE - VIEW_SIZE)/2 + 1, j, x, y;
    object * obj;
    room     actualRoom = floor.rooms[hero.ent.obj.room];
    char  ** display    = initDisplay();
    char   * line       = (char *) malloc(sizeof(char)*256);
    char     c[4], * color;
    char   * p1, * p2, * tmp   = (char *) malloc(sizeof(char)*256);

    strcpy(line, "LEVEL : ");
    sprintf(c, "%d", floor.number);
    strcat(line, c);
    strcat(line, " | ");
    strcat(line, hero.name);
    strcat(line, " | HP : ");
    sprintf(c, "%d", hero.ent.stats.currentHP);
    strcat(line, c);
    strcat(line, "/");
    sprintf(c, "%d", hero.ent.stats.maxHP);
    strcat(line, c);
    strcat(line, " | EXP : ");
    sprintf(c, "%d", hero.exp);
    strcat(line, c);
    strcat(line, " | STR : ");
    sprintf(c, "%d", hero.ent.stats.strength);
    strcat(line, c);

    line = center(line, UNSEENLOC, DISPLAY_SIZE*2);

    c[0] = '|';
    c[1] = '\0';

    p1 = strstr(line, c);
    strcpy (tmp, CYAN);
    strncat(tmp, line, p1-line);
    strcat (tmp, RESET);

    p2 = strstr(p1+1, c);
    strncat(tmp, p1, p2-p1+1);

    p1 = strstr(p2+1, c);
    strcat (tmp, RED);
    strncat(tmp, p2+1, p1-p2-1);
    strcat (tmp, RESET "|");

    p2 = strstr(p1+1, c);
    strcat (tmp, YELLOW);
    strncat(tmp, p1+1, p2-p1-1);
    strcat (tmp, RESET "|" GREEN);
    strcat (tmp, p2+1);
    strcat (tmp, RESET);

    strcpy(display[(DISPLAY_SIZE - VIEW_SIZE)/2 - 1], tmp);

    memset(line, 0, strlen(line));
    line[0] = WALL_C_U_L;

    for(j=1; j<=VIEW_SIZE*2; j++) line[j] = WALL_H;
	line[VIEW_SIZE*2 + 1] = WALL_C_U_R;

	strcpy(display[(DISPLAY_SIZE - VIEW_SIZE)/2], center(line, UNSEENLOC, DISPLAY_SIZE*2));

	for (y=hero.ent.obj.y-VIEW_SIZE/2; y<=hero.ent.obj.y+VIEW_SIZE/2; y++) /* pour chaque case dans la portée du champs de vision du héros */
    {

        memset(line, 0, strlen(line));
        line[0] = WALL_V;
        j = 1;
        for (x=hero.ent.obj.x-VIEW_SIZE/2; x<=hero.ent.obj.x+VIEW_SIZE/2; x++)
        {
            if(y<actualRoom.y || y >= actualRoom.y + actualRoom.height || x < actualRoom.x || x >= actualRoom.x + actualRoom.width)
                line[j] = line[j+1] = UNSEENLOC;
            else
            {
                obj = getObjectByCoord(floor, x, y, hero.ent.obj.room);

                if(hero.ent.obj.x == x && hero.ent.obj.y == y) line[j+1] = hero.ent.obj.c;
                else if (obj != NULL) { line[j+1] = obj->c; if (obj->c == MONSTER) color = obj->color; }
                else line[j+1] = actualRoom.roomBoard[y - actualRoom.y][x - actualRoom.x]; /* on affiche l'objet ou l'entité présente sur la case ou la case */

                line[j] = displayLocation(x - actualRoom.x, y - actualRoom.y, actualRoom.roomBoard, line[j+1]);
            }
            j += 2;
        }
        line[j] = WALL_V;
        line = center(line, UNSEENLOC, DISPLAY_SIZE*2);
        line = addColor(line, DOOR_H,         YELLOW,             strlen(line));
        line = addColor(line, DOOR_V,         YELLOW,             strlen(line));
        line = addColor(line, floor.entry.c,  floor.entry.color,  strlen(line));
        line = addColor(line, floor.exit.c,   floor.exit.color,   strlen(line));
        line = addColor(line, hero.ent.obj.c, hero.ent.obj.color, strlen(line));
        line = addColor(line, MONSTER,        color,              strlen(line));
        strcpy(display[i], line);
        i++;
    }

    memset(line, 0, strlen(line));
    line[0] = WALL_C_D_L;
	for(j=1; j<=VIEW_SIZE*2; j++) line[j] = WALL_H;
	line[VIEW_SIZE*2 + 1] = WALL_C_D_R;
	strcpy(display[i], center(line, UNSEENLOC, DISPLAY_SIZE*2));

	free(line);

	return display;
}

/*************************************************************************************************/

/* *********************************************************** */
/* Affiche une case et l'intersection avec la case précédente. */
/* *********************************************************** */

char displayLocation(int x, int y, int** board, int toPrint)
{
   return (x == 0)                   ? UNSEENLOC :
          (board[y][x] == EMPTYLOC)  ? EMPTYLOC  :
          (board[y][x] == UNSEENLOC || board[y][x-1] == UNSEENLOC)  ? UNSEENLOC :
          (board[y][x] == DOOR_H    || board[y][x-1] == DOOR_H)     ? DOOR_H :
          (board[y][x] == WALL_H    || board[y][x-1] == WALL_H)     ? WALL_H :
          (board[y][x] == WALL_C_D_R|| board[y][x-1] == WALL_C_U_R) ? WALL_H :
          (board[y][x] == WALL_C_U_R|| board[y][x-1] == WALL_C_D_R) ? WALL_H :
          (isWall(board[y][x])      || isWall(board[y][x-1]))       ? EMPTYLOC :
          (toPrint == NOMANSLAND    || board[y][x-1] == NOMANSLAND) ? NOMANSLAND : EMPTYLOC; /* Un caractère étant environ deux fois plus haut que large, on affiche deux caractères en largeur pour une case */
}

/*************************************************************************************************/

void displayRoom (room toDisplay)
{
    int x, y;

    printf("Origin : [%d;%d]\nSize : %d x %d\nCorridor : %d\nBoard :\n", toDisplay.x, toDisplay.y, toDisplay.width, toDisplay.height, toDisplay.isCorridor);

    for(y=0; y < toDisplay.height; y++)
    {
        for(x=0; x < toDisplay.width; x++)
            printf("%c", toDisplay.roomBoard[y][x]);
        printf("\n");
    }
}
