#ifndef STRUCT_H
#define STRUCT_H

/*************************************************************************************************/

/* ******************************* */
/* Struct contient les diff�rentes */
/* structures utilis�es par le jeu */
/* ******************************* */

/*************************************************************************************************/

typedef struct {       /*        Coordonn�es d'une salle        */
    int    x;          /* Coordonn�e en x de la salle           */
    int    y;          /* Coordonn�e en y de la salle           */
    int ** roomBoard;  /* Matrice de la salle                   */
    int    width;      /* Taille horizontale de la salle        */
    int    height;     /* Taille verticale de la salle          */
    int    isCorridor; /* 1 si la salle est un couloir, 0 sinon */
} room;

/*************************************************************************************************/

typedef struct {  /* Coordonn�es et repr�sentation d'un objet */
    int    x;     /* Coordonn�e en x de l'objet               */
    int    y;     /* Coordonn�e en y de l'objet               */
    int    room;  /* Indice de la salle de l'objet            */
    char   c;     /* Repr�sentation graphique de l'objet      */
    char * color;
} object;

typedef struct {       /* Coordonn�es et repr�sentation d'un objet */
    int      type;
    char   * name;     /* Nom de l'objet                           */
    char     c;        /* Repr�sentation graphique de l'objet      */
    int      weight;   /* Poids de l'objet                         */
    char   * desc;     /* Description rapide de l'objet            */
    double   dropRate; /* Taux de drop par un monstre de l'objet   */
} item;

typedef struct elmt {
    item * item;
    struct elmt * next;
} elmt_t;

typedef struct inventaireHash {  /*en gros une table de hachage*/
  elmt_t ** tab;     /*tab de pointeurs d'elmt_t (tab de taille CATEGORIES_MAX) */
}inventaireHash_t;


typedef struct inventaire {

  int taille;
  item ** tab;

}inventaire_t;

typedef struct {
    object       obj;
    inventaire_t inv;
} chest;

/*************************************************************************************************/

typedef struct {     /*    Ensemble de statistiques d'une entit�     */
    int currentHP;   /* Nombre de points de vie actuelle de l'entit� */
    int maxHP;       /* Nombre de points de vie maximum de l'entit�  */
    int strength;    /* Force de l'entit� (d�gats � chaque attaque)  */
} statistics;

typedef struct {
    object         obj;
    statistics     stats;
    inventaire_t * inv;
} entity;

typedef struct {           /*           Structure du h�ros           */
    char   * name;         /* Nom du h�ros                           */
    entity   ent;          /* H�ritage de la structure entit�        */
    int      exp;          /* Nombre de points d'exp�rience du h�ros */
    int      invWeight;    /* Remplissage actuel de l'inventaire     */
    int      invWeightMax; /* Poids max que le h�ros peut porter     */
    int      mode;
} character;

typedef struct { /*      Structure des monstre      */
    entity ent;  /* H�ritage de la structure entit� */
} monster;

/*************************************************************************************************/

typedef struct {           /*      Structure d'un niveau      */
    int       number;      /* Num�ro du niveau                */

    room    * rooms;       /* Liste des salles du niveau      */
    int       nbRoom;      /* Nombre de salles du niveau      */

    int       boardWidth;  /* Taille horizontale de l'�tage   */
    int       boardHeight; /* Taille verticale de l'�tage     */

    monster * monsters;    /* Monstres de l'�tage             */
    int       nbMonsters;  /* Nombre de monstres dans l'�tage */
    double    difficulte;  /* Difficult� du niveau            */

    object    entry;       /* Escaliers d'entr�e de l'�tage   */
    object    exit;        /* Escaliers de sortie de l'�tage  */
} level;

/*************************************************************************************************/

typedef enum itemType {
    ARME = 0,
    CASQUE = 1,
    PLASTRON = 2,
    JAMBIERES = 3,
    BOTTES = 4,
    GANTS = 5,
    SAC = 6,
    CLE = 7,
    POTION = 8
} itemType_t;

/*************************************************************************************************/

#endif
