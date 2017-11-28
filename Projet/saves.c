#include "saves.h"

/*************************************************************************************************/

char ** getSaves(int * nbSaves)
{
    DIR           *d = opendir(".");
    struct dirent *dir;
    int i = 0;
    char ** saves, * tmp;

    saves = (char **) malloc(sizeof(char *) * 3);

    *nbSaves = 0;

    while (d && (dir = readdir(d)) != NULL)
        if(strrchr(dir->d_name, '.') && !strcmp(strrchr(dir->d_name, '.'), ".pjt"))
        {
            if (*nbSaves < 3) saves[*nbSaves] = (char *) malloc(sizeof(char) * dir->d_namlen);
            strcpy(saves[(*nbSaves)%3], dir->d_name);
            (*nbSaves)++;
        }

    if (*nbSaves > 3)
    {
        *nbSaves = 3;
        while (i < *nbSaves - 1)
        {
            if (strcmp(saves[i], saves[i+1]) < 0)
            {
                tmp = saves[i];
                saves[i] = saves[i+1];
                saves[i+1] = tmp;
                i = 0;
            }
            else i++;
        }
    }

    closedir(d);

    return saves;
}

/*************************************************************************************************/

void loadHeros(char * file, character * heros)
{

    FILE * fp = fopen(file,"rb");
    int i, len;
    int size, type;
    char * s;

    fread(&size,sizeof(size),1,fp);
    heros->name=malloc((size+1)*sizeof(char));
    if (heros->name) {
        fread(heros->name,sizeof(char),size,fp);
        heros->name[size]='\0';
    }

    fread(&(heros->ent.obj.x),sizeof(heros->ent.obj.x),1,fp);
    fread(&(heros->ent.obj.y),sizeof(heros->ent.obj.y),1,fp);
    fread(&(heros->ent.obj.room),sizeof(heros->ent.obj.room),1,fp);
    fread(&(heros->ent.obj.c),sizeof(heros->ent.obj.c),1,fp);
    fread(&size,sizeof(size),1,fp);
    heros->ent.obj.color=malloc((size+1)*sizeof(char));
    if (heros->ent.obj.color)
    {
        fread(heros->ent.obj.color,sizeof(heros->ent.obj.c),size,fp);
        heros->ent.obj.color[size]='\0';
    }

    fread(&(heros->ent.stats.currentHP),sizeof(heros->ent.stats.currentHP),1,fp);
    fread(&(heros->ent.stats.maxHP),sizeof(heros->ent.stats.maxHP),1,fp);
    fread(&(heros->ent.stats.strength),sizeof(heros->ent.stats.strength),1,fp);

    /* load of inv in struct character */
    fread(&size,sizeof(int),1,fp);
    heros->ent.inv=malloc(sizeof(inventaire_t));
    heros->ent.inv->tab=malloc(size*sizeof(item *));
    if(size)
    {
        heros->ent.inv->taille=size;
        for(i = 0; i < size; i++)
        {
            fread(&type,sizeof(int),1,fp);
            fread(&len,sizeof(int),1,fp);
            s=malloc((len+1)*sizeof(char));
            fread(s,sizeof(char),len,fp);
            s[len] = '\0';
            heros->ent.inv->tab[i] = rechercherItem (items, type, s);
            free(s);
        }
    }

	fread(&(heros->exp),sizeof(heros->exp),1,fp);
	fread(&(heros->invWeight),sizeof(heros->invWeight),1,fp);
    fread(&(heros->invWeightMax),sizeof(heros->invWeightMax),1,fp);
	fread(&(heros->mode),sizeof(heros->mode),1,fp);
}

/*************************************************************************************************/

void loadAll(char * file, level * l, character * heros)
{
    FILE * fp = fopen(file,"rb");
    int i, j, len;
    int size, type;
    char * s;

    fread(&size,sizeof(size),1,fp);
    heros->name=malloc((size+1)*sizeof(char));
    if (heros->name) {
        fread(heros->name,sizeof(char),size,fp);
        heros->name[size]='\0';
    }

    fread(&(heros->ent.obj.x),sizeof(heros->ent.obj.x),1,fp);
    fread(&(heros->ent.obj.y),sizeof(heros->ent.obj.y),1,fp);
    fread(&(heros->ent.obj.room),sizeof(heros->ent.obj.room),1,fp);
    fread(&(heros->ent.obj.c),sizeof(heros->ent.obj.c),1,fp);
    fread(&size,sizeof(size),1,fp);
    heros->ent.obj.color=malloc((size+1)*sizeof(char));
    if (heros->ent.obj.color)
    {
        fread(heros->ent.obj.color,sizeof(heros->ent.obj.c),size,fp);
        heros->ent.obj.color[size]='\0';
    }

    fread(&(heros->ent.stats.currentHP),sizeof(heros->ent.stats.currentHP),1,fp);
    fread(&(heros->ent.stats.maxHP),sizeof(heros->ent.stats.maxHP),1,fp);
    fread(&(heros->ent.stats.strength),sizeof(heros->ent.stats.strength),1,fp);

    /* load of inv in struct character */
    fread(&size,sizeof(int),1,fp);
    heros->ent.inv=malloc(sizeof(inventaire_t));
    heros->ent.inv->tab=malloc(size*sizeof(item *));
    if(size)
    {
        heros->ent.inv->taille=size;
        for(i = 0; i < size; i++)
        {
            fread(&type,sizeof(int),1,fp);
            fread(&len,sizeof(int),1,fp);
            s=malloc((len+1)*sizeof(char));
            fread(s,sizeof(char),len,fp);
            s[len] = '\0';
            heros->ent.inv->tab[i] = rechercherItem (items, type, s);
            free(s);
        }
    }

	fread(&(heros->exp),sizeof(heros->exp),1,fp);
	fread(&(heros->invWeight),sizeof(heros->invWeight),1,fp);
    fread(&(heros->invWeightMax),sizeof(heros->invWeightMax),1,fp);
	fread(&(heros->mode),sizeof(heros->mode),1,fp);

    fread(&(l->number),sizeof(l->number),1,fp);
    fread(&(l->difficulte),sizeof(l->difficulte),1,fp);
    fread(&(l->nbRoom),sizeof(l->nbRoom),1,fp);
    l->rooms=malloc(l->nbRoom*sizeof(room));
    if(l->rooms)
    {
        for(i = 0; i < l->nbRoom; ++i)
        {
            /* save of struct room in struct level */
            fread(&(l->rooms[i].x),sizeof(l->rooms[i].x),1,fp);
            fread(&(l->rooms[i].y),sizeof(l->rooms[i].y),1,fp);
            fread(&(l->rooms[i].width),sizeof(l->rooms[i].width),1,fp);
            fread(&(l->rooms[i].height),sizeof(l->rooms[i].height),1,fp);
            fread(&(l->rooms[i].isCorridor),sizeof(l->rooms[i].isCorridor),1,fp);
            l->rooms[i].roomBoard=malloc(l->rooms[i].height*sizeof(int*));
            for(j = 0; j < l->rooms[i].height; j++)
            {
                l->rooms[i].roomBoard[j]=malloc(l->rooms[i].width*sizeof(int));
                if (l->rooms[i].roomBoard[j])
                    fread(l->rooms[i].roomBoard[j],sizeof(int),l->rooms[i].width,fp);
            }
        }
    }

    fread(&(l->boardWidth),sizeof(l->boardWidth),1,fp);
    fread(&(l->boardHeight),sizeof(l->boardHeight),1,fp);
    fread(&(l->nbMonsters),sizeof(l->nbMonsters),1,fp);

    l->monsters=malloc(l->nbMonsters*sizeof(monster));
    if(l->monsters)
    {
        for(i=0; i<l->nbMonsters; ++i)
        {
            /* save of struct monster in struct level */
            fread(&(l->monsters[i].ent.obj.x),sizeof(l->monsters[i].ent.obj.x),1,fp);
            fread(&(l->monsters[i].ent.obj.y),sizeof(l->monsters[i].ent.obj.y),1,fp);
            fread(&(l->monsters[i].ent.obj.room),sizeof(l->monsters[i].ent.obj.room),1,fp);
            fread(&(l->monsters[i].ent.obj.c),sizeof(l->monsters[i].ent.obj.c),1,fp);
            fread(&size,sizeof(size),1,fp);
            l->monsters[i].ent.obj.color=malloc((size+1)*sizeof(char));
            if (l->monsters[i].ent.obj.color)
            {
                fread(l->monsters[i].ent.obj.color,sizeof(l->monsters[i].ent.obj.c),size,fp);
                l->monsters[i].ent.obj.color[size]='\0';
            }

            fread(&(l->monsters[i].ent.stats.currentHP),sizeof(l->monsters[i].ent.stats.currentHP),1,fp);
            fread(&(l->monsters[i].ent.stats.maxHP),sizeof(l->monsters[i].ent.stats.maxHP),1,fp);
            fread(&(l->monsters[i].ent.stats.strength),sizeof(l->monsters[i].ent.stats.strength),1,fp);

            /* load of inv in struct entity in struct monster */
            fread(&size,sizeof(int),1,fp);
            l->monsters[i].ent.inv=malloc(sizeof(inventaire_t));
            l->monsters[i].ent.inv->tab=malloc(size*sizeof(item *));
            if(size)
            {
                l->monsters[i].ent.inv->taille=size;
                for(j = 0; j < size; j++)
                {
                    fread(&type,sizeof(int),1,fp);
                    fread(&len,sizeof(int),1,fp);
                    s=malloc((len+1)*sizeof(char));
                    fread(s,sizeof(char),len,fp);
                    s[len] = '\0';
                    l->monsters[i].ent.inv->tab[j] = rechercherItem (items, type, s);
                    free(s);
                }
            }
        }
    }
    /* save of struct object in struct level */
    fread(&(l->entry.x),sizeof(l->entry.x),1,fp);
    fread(&(l->entry.y),sizeof(l->entry.y),1,fp);
    fread(&(l->entry.room),sizeof(l->entry.room),1,fp);
    fread(&(l->entry.c),sizeof(l->entry.c),1,fp);
    fread(&size,sizeof(size),1,fp);
    l->entry.color=malloc((size+1)*sizeof(char));
    if(l->entry.color)
    {
        fread(l->entry.color,sizeof(l->entry.c),size,fp);
        l->entry.color[size]='\0';
    }
    fread(&(l->exit.x),sizeof(l->exit.x),1,fp);
    fread(&(l->exit.y),sizeof(l->exit.y),1,fp);
    fread(&(l->exit.room),sizeof(l->exit.room),1,fp);
    fread(&(l->exit.c),sizeof(l->exit.c),1,fp);
    fread(&size,sizeof(size),1,fp);
    l->exit.color=malloc((size+1)*sizeof(char));
    if (l->exit.color)
    {
        fread(l->exit.color,sizeof(l->exit.c),size,fp);
        l->exit.color[size]='\0';
    }
    fclose(fp);
}

/*************************************************************************************************/

void save(level l, character heros)
{
    FILE * fp;
    char * filename = (char *) malloc(sizeof(char) * 32),
           tmp[8];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int i, j, size;

    strcpy(filename, "project_save_");
    strcat(filename, itoa(tm.tm_mday, tmp, 10));
    strcat(filename, "_");
    strcat(filename, itoa(tm.tm_mon+1,  tmp, 10));
    strcat(filename, "_");
    strcat(filename, itoa(1900+tm.tm_year, tmp, 10));
    strcat(filename, "_");
    strcat(filename, itoa(tm.tm_hour, tmp, 10));
    strcat(filename, "_");
    strcat(filename, itoa(tm.tm_min,  tmp, 10));
    strcat(filename, "_");
    strcat(filename, itoa(tm.tm_sec, tmp, 10));
    strcat(filename, ".pjt");

    fp = fopen(filename,"wb");

    /* save of struct character */

    size=strlen(heros.name);
    fwrite(&size,sizeof(size),1,fp);
    fwrite(heros.name,sizeof(char),size,fp);

    fwrite(&(heros.ent.obj.x),sizeof(heros.ent.obj.x),1,fp);
    fwrite(&(heros.ent.obj.y),sizeof(heros.ent.obj.y),1,fp);
    fwrite(&(heros.ent.obj.room),sizeof(heros.ent.obj.room),1,fp);
    fwrite(&(heros.ent.obj.c),sizeof(heros.ent.obj.c),1,fp);
    size=strlen(heros.ent.obj.color);
    fwrite(&size,sizeof(size),1,fp);
    fwrite(heros.ent.obj.color,sizeof(heros.ent.obj.c),size,fp);
    fwrite(&(heros.ent.stats.currentHP),sizeof(heros.ent.stats.currentHP),1,fp);
    fwrite(&(heros.ent.stats.maxHP),sizeof(heros.ent.stats.maxHP),1,fp);
    fwrite(&(heros.ent.stats.strength),sizeof(heros.ent.stats.strength),1,fp);

    /* save of inv in struct entity in struct monster */
    if(heros.ent.inv)
    {
        fwrite(&(heros.ent.inv->taille),sizeof(int),1,fp);

        for(j = 0; j < heros.ent.inv->taille; j++)
        {
            fwrite(&((*(heros.ent.inv->tab))->type),sizeof(int),1,fp);
            size=strlen((*heros.ent.inv->tab)->name);
            fwrite(&size,sizeof(int),1,fp);
            fwrite((*heros.ent.inv->tab)->name,sizeof(char),size,fp);
        }
    }
    else
    {
        size=0;
        fwrite(&(size),sizeof(int),1,fp);
    }

    fwrite(&(heros.exp),sizeof(heros.exp),1,fp);
    fwrite(&(heros.invWeight),sizeof(heros.invWeight),1,fp);
    fwrite(&(heros.invWeightMax),sizeof(heros.invWeightMax),1,fp);
    fwrite(&(heros.mode),sizeof(heros.mode),1,fp);

    fwrite(&(l.number),sizeof(l.number),1,fp);
    fwrite(&(l.difficulte),sizeof(l.difficulte),1,fp);
    fwrite(&(l.nbRoom),sizeof(l.nbRoom),1,fp);

    for(i = 0; i < l.nbRoom; ++i)
    {
        /* save of struct room in struct level */
        fwrite(&(l.rooms[i].x),sizeof(l.rooms[i].x),1,fp);
        fwrite(&(l.rooms[i].y),sizeof(l.rooms[i].y),1,fp);
        fwrite(&(l.rooms[i].width),sizeof(l.rooms[i].width),1,fp);
        fwrite(&(l.rooms[i].height),sizeof(l.rooms[i].height),1,fp);
        fwrite(&(l.rooms[i].isCorridor),sizeof(l.rooms[i].isCorridor),1,fp);
        for(j=0; j<l.rooms[i].height; j++)
        {
            fwrite(l.rooms[i].roomBoard[j],sizeof(int),l.rooms[i].width,fp);
        }
    }

    fwrite(&(l.boardWidth),sizeof(l.boardWidth),1,fp);
    fwrite(&(l.boardHeight),sizeof(l.boardHeight),1,fp);
    fwrite(&(l.nbMonsters),sizeof(l.nbMonsters),1,fp);

    for(i=0; i<l.nbMonsters; ++i)
    {
        /* save of struct monster in struct level */
        fwrite(&(l.monsters[i].ent.obj.x),sizeof(l.monsters[i].ent.obj.x),1,fp);
        fwrite(&(l.monsters[i].ent.obj.y),sizeof(l.monsters[i].ent.obj.y),1,fp);
        fwrite(&(l.monsters[i].ent.obj.room),sizeof(l.monsters[i].ent.obj.room),1,fp);
        fwrite(&(l.monsters[i].ent.obj.c),sizeof(l.monsters[i].ent.obj.c),1,fp);
        size=strlen(l.monsters[i].ent.obj.color);
        fwrite(&size,sizeof(size),1,fp);
        fwrite(l.monsters[i].ent.obj.color,sizeof(l.monsters[i].ent.obj.c),size,fp);
        fwrite(&(l.monsters[i].ent.stats.currentHP),sizeof(l.monsters[i].ent.stats.currentHP),1,fp);
        fwrite(&(l.monsters[i].ent.stats.maxHP),sizeof(l.monsters[i].ent.stats.maxHP),1,fp);
        fwrite(&(l.monsters[i].ent.stats.strength),sizeof(l.monsters[i].ent.stats.strength),1,fp);

        /* save of inv in struct entity in struct monster */
        if(l.monsters[i].ent.inv)
        {
            fwrite(&(l.monsters[i].ent.inv->taille),sizeof(int),1,fp);
            for(j = 0; j < l.monsters[i].ent.inv->taille; j++)
            {
                fwrite(&((*l.monsters[i].ent.inv->tab)->type),sizeof(int),1,fp);
                size=strlen((*l.monsters[i].ent.inv->tab)->name);
                fwrite(&size,sizeof(int),1,fp);
                fwrite((*l.monsters[i].ent.inv->tab)->name,sizeof(char),size,fp);
            }
        }
        else
        {
            size=0;
            fwrite(&(size),sizeof(int),1,fp);
        }
    }

    /* save of struct object in struct level */
    fwrite(&(l.entry.x),sizeof(l.entry.x),1,fp);
    fwrite(&(l.entry.y),sizeof(l.entry.y),1,fp);
    fwrite(&(l.entry.room),sizeof(l.entry.room),1,fp);
    fwrite(&(l.entry.c),sizeof(l.entry.c),1,fp);
    size=strlen(l.entry.color);
    fwrite(&size,sizeof(size),1,fp);
    fwrite(l.entry.color,sizeof(l.entry.c),size,fp);
    fwrite(&(l.exit.x),sizeof(l.exit.x),1,fp);
    fwrite(&(l.exit.y),sizeof(l.exit.y),1,fp);
    fwrite(&(l.exit.room),sizeof(l.exit.room),1,fp);
    fwrite(&(l.exit.c),sizeof(l.exit.c),1,fp);
    size=strlen(l.exit.color);
    fwrite(&size,sizeof(size),1,fp);
    fwrite(l.exit.color,sizeof(l.exit.c),size,fp);

    /* save of struct character */
    fclose(fp);
}

/*************************************************************************************************/
