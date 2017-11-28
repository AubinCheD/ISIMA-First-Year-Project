/* ***************************************** */
/* Utils contient les fonctions généralistes */
/* utiles aux autres modules.                */
/* ***************************************** */

#include "utils.h"

/*************************************************************************************************/

/* ******************************************************************* */
/* Affiche à l'utilisateur, dans un encadré, un message donné.         */
/* Renvoie sa réponse si une réponse est attendue, sinon renvoie NULL. */
/* ******************************************************************* */

char * displayMessage(char *message, int answerSize)
{
    unsigned int i;
    char * answer = NULL;

    system("cls");

    printf("\n\t%c", WALL_C_U_L);
    for(i=0; i < strlen(message)+2; i++) printf("%c", WALL_H);                       /* Bordure supérieure */
	printf("%c\n\t%c %s %c\n\t%c", WALL_C_U_R, WALL_V, message, WALL_V, WALL_C_D_L); /* Ligne du message   */
    for(i=0; i < strlen(message)+2; i++) printf("%c", WALL_H);                       /* Bordure inférieure */
	printf("%c\n\n", WALL_C_D_R);

	if(answerSize)
    {
        answer = (char *) malloc(sizeof(char)*answerSize);
        printf("\t%c ", CURSOR);
        fgets(answer, answerSize, stdin);
        answer[strlen(answer)-1] = '\0'; /* Supprime le retour chariot */
    }

    return answer;
}

/*************************************************************************************************/

char * getTrimmedText(char * string, int size, int line)
{
    char * trimmed = (char *) malloc(sizeof(char) * (size + 1));
    char * cursor;

    if(strlen(string) > size * (line - 1))
    {
        cursor = string + size * (line - 1);
        if (line > 1) while (*(cursor - 1) != ' ') cursor--;
        strncpy(trimmed, cursor, size);
        if(strlen(string) > size * line) while (*(trimmed + strlen(trimmed) - 1) != ' ') trimmed[strlen(trimmed) - 1] = '\0';
    }
    else strcpy(trimmed, "");

    return trimmed;
}

/*************************************************************************************************/

void frame(char ** display, int x, int y, int width, int height)
{
    int i;

    display[y-1][x-2] = WALL_C_U_L;
    display[y-1][x+width+1] = WALL_C_U_R;
    for(i=x-1; i <= x + width; i++) display[y-1][i] = display[y+height][i] = WALL_H;
    display[y+height][x-2] = WALL_C_D_L;
    display[y+height][x+width+1] = WALL_C_D_R;
    for(i=y; i < y + height; i++) display[i][x-2] = display[i][x+width+1] = WALL_V;
}

/*************************************************************************************************/

char * center(char * string, char c, int size)
{
    char * centered = (char *) malloc(sizeof(char) * (size + 1));
    int i;

    memset(centered, 0, size+1);

    for(i=0; i < (size - strlen(string)) / 2; i++) centered[strlen(centered)] = c;
    strcat(centered, string);
    for(i=0; i < (size - strlen(string)) / 2 + (size - strlen(string))%2; i++) centered[strlen(centered)] = c;
    centered[size] = '\0';

    return centered;
}

/*************************************************************************************************/

char * addColor(char * string, char c, char * color, int size)
{
    char * buffer = (char *) malloc(sizeof(char) * 256);
    char * p;
    char   s[2];

    s[0] = c;
    s[1] = '\0';

    if (!(p = strstr(string, s)) || size - (p-string) <= 0) strcpy(buffer, string);
    else
    {
        strncpy(buffer, string, p-string);
        strcat (buffer, color);
        strncat(buffer, p, 1);
        strcat (buffer, RESET);
        strcat (buffer, addColor(p+1, c, color, size - (p-string + 1)));
    }

    return buffer;
}

/*************************************************************************************************/

char ** initDisplay()
{
    int i;

    char ** display = (char **) malloc(sizeof(char *) * DISPLAY_SIZE);

    for(i=0; i < DISPLAY_SIZE; i++)
    {
        display[i] = (char *) malloc(sizeof(char) * 256);
        memset(display[i], UNSEENLOC, DISPLAY_SIZE*2);
        display[i][DISPLAY_SIZE*2] = '\0';
    }

    return display;
}

/*************************************************************************************************/

void display(char ** leftScreen, char ** rightScreen)
{
    int i;

    system("cls");

    for(i=0; i < DISPLAY_SIZE; i++)
    {
        printf("%s", leftScreen[i]);
        if (rightScreen != NULL) printf("%s", rightScreen[i]);
        printf("\n");
    }
}

/*************************************************************************************************/

/* ********************************************************** */
/* Recherche si un tableau d'entier contient un entier donné. */
/* Renvoie 1 si l'entier est trouvé dans le tableau, 0 sinon. */
/* ********************************************************** */

int intContains (int *array, int object, int size)
{
    int i;

    for (i=0; i < size; i++) if(array[i] == object) return 1;

    return 0;
}

/*************************************************************************************************/

double maxTab (double * tab, int taille) {

    int i;
    double max = tab[0];

    for (i=0;i<taille;i++)
        if (tab[i]>max)
            max = tab[i];

    return max;
}

/*************************************************************************************************/

/* ******************************************************** */
/* Compare deux entier et renvoie la valeur la plus grande. */
/* ******************************************************** */

int intMax (int i, int j)
{
    return (i>j) ? i : j;
}

/*************************************************************************************************/

/* ******************************************************** */
/* Compare deux entier et renvoie la valeur la plus petite. */
/* ******************************************************** */

int intMin (int i, int j)
{
    return (i<j) ? i : j;
}
