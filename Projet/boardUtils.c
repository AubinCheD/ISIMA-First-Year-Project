/* ********************************************* */
/* BoardUtils contient les fonctions secondaires */
/* utilisées par le module Board pour la         */
/* génération et l'affichage des niveaux         */
/* ********************************************* */

#include "boardUtils.h"

/*************************************************************************************************/

/* ************************************************* */
/* Recherche si une salle aux dimensions             */
/* données peut être inséré sur un niveau donné.     */
/* Renvoie 1 si la salle peut être insérée, 0 sinon. */
/* ************************************************* */

int isInsertable(level *floor, room toInsert)
{
    int insertable = 0;

    for(toInsert.y=0; toInsert.y <= floor->boardHeight - toInsert.height && !insertable; toInsert.y++)   /* On cherche des coordonnées auxquelles */
        for(toInsert.x=0; toInsert.x <= floor->boardWidth - toInsert.width && !insertable; toInsert.x++) /* la salle peut être insérée            */
            insertable = isInsertableToCoord(floor, toInsert);

    return insertable;
}

/*************************************************************************************************/

/* ******************************************************* */
/* Recherche si une salle données peut être inséré dans un */
/* étage à partir de ses coordonnées et de ses dimentions. */
/* ******************************************************* */

int isInsertableToCoord(level *floor, room toInsert)
{
    int i, insertable = 1;

	for(i=0; i < floor->nbRoom && insertable; i++)
        insertable = toInsert.y > floor->rooms[i].y + floor->rooms[i].height ||
                     toInsert.y + toInsert.height < floor->rooms[i].y        || /* On regarde si une salle est déjà présente à      */
                     toInsert.x > floor->rooms[i].x + floor->rooms[i].width  || /* l'endroit où l'on veut insérer la nouvelle salle */
                     toInsert.x + toInsert.width  < floor->rooms[i].x;

    return insertable;
}

/*************************************************************************************************/

/* ****************************************************** */
/* Recherche si une case est une case vide accessible     */
/* (EMPTYLOC) et si elle n'est occupé par aucune entité.  */
/* Renvoie 1 si ces deux conditions sont réunis, 0 sinon. */
/* ****************************************************** */

int isEmpty(int x, int y, int Room, level floor, character hero)
{
    int i, empty = 1;
    room actualRoom = floor.rooms[Room];

    if ((hero.ent.obj.x == x && hero.ent.obj.y == y) || actualRoom.roomBoard[y-actualRoom.y][x-actualRoom.x] != EMPTYLOC)       empty = 0;
    else for(i=0; i < floor.nbMonsters && empty; i++) if (floor.monsters[i].ent.obj.x == x && floor.monsters[i].ent.obj.y == y) empty = 0;

    return empty;
}

/*************************************************************************************************/

/* **************************************************** */
/* Recherche si un caractère donné est un mur ou une    */
/* porte (les portes étant considérées comme des murs). */
/* Renvoie 1 si le caractère est un mur, 0 sinon.       */
/* **************************************************** */

int isWall(int c)
{
    return (isCorner(c) || c == WALL_V || c == WALL_H ||c == DOOR_V || c == DOOR_H);
}

/*************************************************************************************************/

/* ****************************************************** */
/* Recherche si un caractère donné est un coin de mur.    */
/* Renvoie 1 si le caractère est un coin de mur, 0 sinon. */
/* ****************************************************** */

int isCorner(int c)
{
    return(c == WALL_C_U_L || c == WALL_C_U_R || c == WALL_C_D_R || c == WALL_C_D_L);
}

/*************************************************************************************************/

int isWallFull(room Room, int wall)
{
    int i, wallFull = 1, limit = (wall%2) ? Room.width : Room.height;

    for(i=1; i < limit - 1 && wallFull; i++)
        switch(wall)
        {
            case 0 : wallFull = (Room.roomBoard[0]            [i] == DOOR_H); break;
            case 1 : wallFull = (Room.roomBoard[i] [Room.width-1] == DOOR_V); break;
            case 2 : wallFull = (Room.roomBoard[Room.height-1][i] == DOOR_H); break;
            case 3 : wallFull = (Room.roomBoard[i]            [0] == DOOR_V); break;
        }

    return wallFull;
}

/*************************************************************************************************/

int isRoomFull(level floor, int nbRoom)
{
    int x, y, roomFull = 1;

    for(y=floor.rooms[nbRoom].y + 1; y < floor.rooms[nbRoom].y + floor.rooms[nbRoom].height - 1 && roomFull; y++)
        for(x=floor.rooms[nbRoom].x + 1; x < floor.rooms[nbRoom].x + floor.rooms[nbRoom].width - 1 && roomFull; x++)
            roomFull = (getObjectByCoord(floor, x, y, nbRoom) != NULL);

    return roomFull;
}

/*************************************************************************************************/

/* *************************************************** */
/* Calcule la taille du niveau en fonction de l'étage. */
/* La courbe d'évolution suis la forme de log(x).      */
/* *************************************************** */

int getBoardSize(int level)
{
    return LEVEL_WEIGHT * log(level) + LEVEL_INITIAL_SIZE;
}

/*************************************************************************************************/

/* ******************************************************************************* */
/* Calcule un nombre aléatoire de salle en fonction de l'étage. Calcul un nombre   */
/* de salle maximum en fonction de l'étage puis prend un nombre aléatoire entre le */
/* minimum et le maximum calculé. La courbe d'évolution suis la forme de log(x).   */
/* ******************************************************************************* */

int getNbRooms (int level)
{
    srand(time(NULL));

    return rand()%(int)(LEVEL_WEIGHT/3 * log(level)+ ROOM_INITIAL_MAX) + level;
}

/*************************************************************************************************/

/* ********************************************************* */
/* Recherche un objet parmis les objets et entités de        */
/* l'étage à partir de ses coordonnées. Retourne un pointeur */
/* sur l'objet si la recherche trouve un objet, NULL sinon.  */
/* ********************************************************* */

object* getObjectByCoord(level floor, int x, int y, int room)
{
    int i;
    object *obj = NULL;

    for(i=0; i < floor.nbMonsters && obj == NULL; i++)
        if(floor.monsters[i].ent.obj.room == room && floor.monsters[i].ent.obj.x == x && floor.monsters[i].ent.obj.y == y) obj = &(floor.monsters[i].ent.obj);
    if (    obj == NULL && floor.entry.room == room && floor.entry.x == x && floor.entry.y == y) obj = &(floor.entry); /* Escaliers d'entrée  */
    else if(obj == NULL && floor.exit.room  == room && floor.exit.x  == x && floor.exit.y  == y) obj = &(floor.exit);  /* Escaliers de sortie */

   return obj;
}
