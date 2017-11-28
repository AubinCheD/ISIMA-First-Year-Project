#ifndef PARAM_H
#define PARAM_H

/*************************************************************************************************/

/* ************************************************ */
/* Param contient les paramètres généraux du jeu    */
/* notamment des paramètres de génération et les    */
/* représentations graphiques des différents objets */
/* ************************************************ */

/*************************************************************************************************/

#define LEVEL_INITIAL_SIZE      30 /* Taille du premier étage, conditionnant la taille des étages suivant                                         */
#define ROOM_INITIAL_MAX         6 /* Nombre maximum de pièces au premier étage, conditionnant le nombre maximum de pièces des étages suivant     */
#define CORRIDOR_COEFF           1
#define CATEGORIES_MAX           9
#define INVENTAIRE_JOUEUR_MAX  100
#define INVENTAIRE_MONSTRE_MAX  10
#define INVENTAIRE_COFFRE_MAX   10
#define HERO_INV_WEIGHT_MAX    100
#define MONSTERS_INITIAL_MAX     5 /* Nombre maximum de monstres au premier étage, conditionnant le nombre maximum de monstres des étages suivant */
#define MIN_ROOM_SIZE            6 /* Taille minimum d'une salle                                                                                  */
#define VIEW_SIZE               21 /* Taille du champ de vision                                                                                   */
#define DISPLAY_SIZE            29 /* Taille du champ de vision                                                                                   */
#define LEVEL_WEIGHT            40 /* Importance du niveau dans l'évolution de la taille des étages                                               */
                                /* (une valeur plus grande entrainera une augmentation plus grande de la taille de l'étage à chaque niveau)    */

/*************************************************************************************************/

/*       Représentation graphique des éléments du niveau        */
#define CURSOR       62 /* Curseur pour naviguer dans les menus */

#define NOMANSLAND   32 /* Case inaccessible par les entités    */
#define UNSEENLOC   -1  /* Case en dehors de la vision du héros */
#define EMPTYLOC    -80 /* Case vide accessible par les entités */

#define WALL_H      -51 /* Mur horizontal                       */
#define WALL_V      -70 /* Mur vertical                         */
#define WALL_C_U_L  -55 /* Coin haut-gauche de mur              */
#define WALL_C_U_R  -69 /* Coin haut-droit de mur               */
#define WALL_C_D_L  -56 /* Coin bas-gauche de mur               */
#define WALL_C_D_R  -68 /* Coin bas-droit de mur                */

#define DOOR_H      -60 /* Porte horizontal                     */
#define DOOR_V      -77 /* Porte vertical                       */

#define STAIRS_U    -82 /* Escaliers d'entrée                   */
#define STAIRS_D    -81 /* Escaliers de sortie                  */
#define HERO        -89 /* Hero                                 */
#define MONSTER     -31 /* Monstre                              */
#define CHEST         1 /* Coffre                               */

#define KEY         -99 /* Clé                                  */
#define HELMET      -29 /* Casque                               */
#define CHESTPLATE   35 /* Plastron                             */
#define LEGGHINGS   -22 /* Pantalon                             */
#define BOOTS       -48 /* Chaussures                           */
#define HEALTHPOTS  -94 /* Petite potion de santé               */
#define HEALTHPOTB  -21 /* Grande potion de santé               */
#define SWORD       -41 /* Épée                                 */
#define BAG          38 /* Sac                                  */

/*************************************************************************************************/

#define RED     "\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE    "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN    "\x1B[36m"
#define WHITE   "\x1B[37m"
#define RESET   "\x1B[0m"

/*************************************************************************************************/

#endif
