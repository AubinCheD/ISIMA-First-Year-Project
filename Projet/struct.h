#ifndef STRUCT_H
#define STRUCT_H

/*************************************************************************************************/

/* ******************************* */
/* Struct contient les différentes */
/* structures utilisées par le jeu */
/* ******************************* */

/*************************************************************************************************/

typedef struct {       /*        Coordonnées d'une salle        */
    int    x;          /* Coordonnée en x de la salle           */
    int    y;          /* Coordonnée en y de la salle           */
    int ** roomBoard;  /* Matrice de la salle                   */
    int    width;      /* Taille horizontale de la salle        */
    int    height;     /* Taille verticale de la salle          */
    int    isCorridor; /* 1 si la salle est un couloir, 0 sinon */
} room;

/*************************************************************************************************/

typedef struct {  /* Coordonnées et représentation d'un objet */
    int    x;     /* Coordonnée en x de l'objet               */
    int    y;     /* Coordonnée en y de l'objet               */
    int    room;  /* Indice de la salle de l'objet            */
    char   c;     /* Représentation graphique de l'objet      */
    char * color;
} object;

typedef struct {       /* Coordonnées et représentation d'un objet */
    int      type;
    char   * name;     /* Nom de l'objet                           */
    char     c;        /* Représentation graphique de l'objet      */
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

typedef struct {     /*    Ensemble de statistiques d'une entité     */
    int currentHP;   /* Nombre de points de vie actuelle de l'entité */
    int maxHP;       /* Nombre de points de vie maximum de l'entité  */
    int strength;    /* Force de l'entité (dégats à chaque attaque)  */
} statistics;

typedef struct {
    object         obj;
    statistics     stats;
    inventaire_t * inv;
} entity;

typedef struct {           /*           Structure du héros           */
    char   * name;         /* Nom du héros                           */
    entity   ent;          /* Héritage de la structure entité        */
    int      exp;          /* Nombre de points d'expérience du héros */
    int      invWeight;    /* Remplissage actuel de l'inventaire     */
    int      invWeightMax; /* Poids max que le héros peut porter     */
    int      mode;
} character;

typedef struct { /*      Structure des monstre      */
    entity ent;  /* Héritage de la structure entité */
} monster;

/*************************************************************************************************/

typedef struct {           /*      Structure d'un niveau      */
    int       number;      /* Numéro du niveau                */

    room    * rooms;       /* Liste des salles du niveau      */
    int       nbRoom;      /* Nombre de salles du niveau      */

    int       boardWidth;  /* Taille horizontale de l'étage   */
    int       boardHeight; /* Taille verticale de l'étage     */

    monster * monsters;    /* Monstres de l'étage             */
    int       nbMonsters;  /* Nombre de monstres dans l'étage */
    double    difficulte;  /* Difficulté du niveau            */

    object    entry;       /* Escaliers d'entrée de l'étage   */
    object    exit;        /* Escaliers de sortie de l'étage  */
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
