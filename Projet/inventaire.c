#include "inventaire.h"

inventaireHash_t * initInventaireHash () {
    inventaireHash_t * inventaireHash = NULL;
    int i;

    inventaireHash = (inventaireHash_t *) malloc (sizeof(inventaireHash_t));
    if (inventaireHash) {
      inventaireHash->tab = NULL;

      inventaireHash->tab = (elmt_t **) malloc (CATEGORIES_MAX* sizeof(elmt_t *));

      if (inventaireHash->tab) {
	for(i=0;i<CATEGORIES_MAX;i++)
	  (inventaireHash->tab)[i]=NULL;
      }
    }
    return inventaireHash;

}



elmt_t * initElmt () {

  elmt_t * elmt = NULL;

  elmt = (elmt_t *) malloc (sizeof(elmt_t));
  elmt->next = NULL;

  elmt->item = (item *) malloc (sizeof(item));

  return elmt;
}



inventaire_t * initInventaire (int taille) {

  inventaire_t * inventaire = NULL;
  int i;

  inventaire = (inventaire_t *) malloc ( sizeof(inventaire_t));

  if(inventaire) {
    inventaire->taille = taille;
    inventaire->tab=NULL;

    inventaire->tab = (item **) malloc (taille * sizeof(item *));

    if(inventaire->tab) {

      for(i=0;i<taille;i++)
	(inventaire->tab)[i]=NULL;

    }
  }
  return inventaire;
}



/*crée l'inventaire en dur contenant tous les objets du jeu*/
/*format du fichier: type,name;représentationGraphique;weight;description;droprate*/
inventaireHash_t * creerInventaireHash(char * nomFichier) {

  FILE * fic = fopen(nomFichier,"r");
  inventaireHash_t * inventaireHash = NULL;
  elmt_t * new = NULL;
  int i=0,type;

  char c;
  char name[50];
  char desc[200];
  char nb[100];

  fseek(fic, 0, SEEK_SET);

  if (fic) {

    inventaireHash = initInventaireHash();

    c=fgetc(fic);
    while(c != EOF) {

      new = initElmt ();

      nb[0]=c;            /*lecture du type d'item*/
      nb[1]='\0';
      type = atoi(nb);

      new->next = (inventaireHash->tab)[type]; /*insertion du nouvel item*/
      (inventaireHash->tab)[type]=new;

      new->item->type=type;

      c = fgetc(fic); /*lecture du ';' */
      c = fgetc(fic);
      while (c!=';' && c!=EOF) { /*lecture du nom d'item*/
	name[i]=c;
	c=fgetc(fic);
	i++;
      }
      name[i]='\0';
      i=0;

      new->item->name = (char *) malloc((strlen(name)+1)*sizeof(char));
      strcpy(new->item->name,name);

      c=fgetc(fic);    /*lecture de la représentation graphique*/
      new->item->c=c;

      c=fgetc(fic);    /*lecture du ';' */
      c=fgetc(fic);

      while (c!=';' && c!=EOF) { /*lecture du poids d'item*/
	nb[i]=c;
	c=fgetc(fic);
	i++;
      }
      nb[i]='\0';
      i=0;

      new->item->weight=atoi(nb);

      c=fgetc(fic);

      while (c!=';' && c!=EOF) { /*lecture de la description*/
	desc[i]=c;
	c=fgetc(fic);
	i++;
      }
      desc[i]='\0';
      i=0;

      new->item->desc = (char *) malloc((strlen(desc)+1)*sizeof(char));
      strcpy(new->item->desc,desc);


      if(c!=EOF){
	c=fgetc(fic);
	while (c!='\n' && c!=EOF) {   /*lecture du droprate*/
	  nb[i]=c;
	  c=fgetc(fic);
	  i++;
	}
	nb[i]='\0';

	new->item->dropRate = (double)atof(nb);

	c=fgetc(fic);
      }
    }

    fclose(fic);
  }

  return inventaireHash;
}




void libererInventaireHash (inventaireHash_t * inventaireHash) {

  int i;
  elmt_t * temp = NULL;

  for(i=0;i<CATEGORIES_MAX;i++) {

    while( (inventaireHash->tab)[i] != NULL) {

      temp=(inventaireHash->tab)[i];
      (inventaireHash->tab)[i] = temp->next;

      free(temp->item->name);
      free(temp->item->desc);
      free(temp->item);
      free(temp);
    }
  }

  free(inventaireHash->tab);
  free(inventaireHash);

}



void libererInventaire(inventaire_t * inventaire) {

  free(inventaire->tab);
  free(inventaire);
}





/*format du fichier: type,name;représentationGraphique;weight;description;droprate*/
void sauvegarderInventaireHash (inventaireHash_t * inventaireHash, char * nomFichier) {

  FILE * fic = fopen(nomFichier, "w");

  elmt_t * cour = NULL;
  char nb[100];
  int i;


  if (fic) {

    for (i=0;i<CATEGORIES_MAX;i++) {

      cour = (inventaireHash->tab)[i];

      while (cour) {

	sprintf(nb, "%d", cour->item->type);  /*type*/
	fputs(nb,fic);
	fputc(';', fic);

	fputs(cour->item->name,fic); /*name*/
	fputc(';',fic);

	fputc(cour->item->c,fic); /*c*/
	fputc(';',fic);

	sprintf(nb, "%d", cour->item->weight); /*weight*/
	fputs(nb,fic);
	fputc(';', fic);

	fputs(cour->item->desc,fic); /*desc*/
	fputc(';',fic);

	sprintf(nb, "%f", cour->item->dropRate); /*weight*/
	fputs(nb,fic);
	fputc('\n', fic);

	cour = cour->next;
      }

    }
    fclose(fic);
  }

  else (exit(EXIT_FAILURE));
}














  /*format du fichier: type,name;représentationGraphique;weight;description;droprate*/
void sauvegarderInventaire (inventaire_t * inventaire, char * nomFichier) {

  FILE * fic = fopen(nomFichier, "w");

  item * cour = NULL;
  char nb[100];
  int i;


  if (fic) {

    for (i=0;i<inventaire->taille;i++) {

      cour = (inventaire->tab)[i];

      if (cour) {
	sprintf(nb, "%d", cour->type);  /*type*/
	fputs(nb,fic);
	fputc(';', fic);

	fputs(cour->name,fic); /*name*/
	fputc(';',fic);

	fputc(cour->c,fic); /*c*/
	fputc(';',fic);

	sprintf(nb, "%d", cour->weight); /*weight*/
	fputs(nb,fic);
	fputc(';', fic);

	fputs(cour->desc,fic); /*desc*/
	fputc(';',fic);

	sprintf(nb, "%f", cour->dropRate); /*weight*/
	fputs(nb,fic);
	fputc('\n', fic);

      }
    }
    fclose(fic);
  }

  else (exit(EXIT_FAILURE));
}








/*renvoie l'adresse de l'item recherché*/
item * rechercherItem (inventaireHash_t * inventaireHash, int type, char * name) {

  elmt_t * cour = (inventaireHash->tab)[type];
  item * item = NULL;

  while (cour && cour->item->name != name) {

    item = cour->item;
    cour=cour->next;
  }

  return item;
}












inventaire_t * recupererInventaire(char * nomFichier, inventaireHash_t * inventaireHash, int taille) {

  FILE * fic = fopen(nomFichier,"r");
  inventaire_t * inventaire = NULL;
  int i=0,j=0,type;

  char c;
  char name[50];
  char nb[10];

  fseek(fic, 0, SEEK_SET);

  if (fic) {

    inventaire = initInventaire(taille);

    c=fgetc(fic);
    while(c != EOF) {

      nb[0]=c;            /*lecture du type d'item*/
      nb[1]='\0';
      type = atoi(nb);

      c = fgetc(fic); /*lecture du ';' */
      c = fgetc(fic);
      while (c!=';' && c!=EOF) { /*lecture du nom d'item*/
	name[i]=c;
	c=fgetc(fic);
	i++;
      }
      name[i]='\0';
      i=0;

      c=fgetc(fic);    /*lecture de la représentation graphique*/

      c=fgetc(fic);    /*lecture du ';' */
      c=fgetc(fic);

      while (c!=';' && c!=EOF) { /*lecture du poids d'item*/
	c=fgetc(fic);
     }

      c=fgetc(fic);

      while (c!=';' && c!=EOF) { /*lecture de la description*/
	c=fgetc(fic);
      }

      if(c!=EOF){
	c=fgetc(fic);
	while (c!='\n' && c!=EOF) {   /*lecture du droprate*/
	  c=fgetc(fic);
	}

	c=fgetc(fic);
      }

      (inventaire->tab)[j] = rechercherItem(inventaireHash, type, name);

    }

    fclose(fic);
  }

  else (exit(EXIT_FAILURE));

  return inventaire;

}
