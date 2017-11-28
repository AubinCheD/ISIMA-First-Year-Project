#include "graphes.h"


graphe_t * creerGraphe (int n /*nb sommets*/, graphe_type type) {

  graphe_t * graphe = NULL;
  int ** new = NULL;
  int i;

  graphe = (graphe_t *) malloc (sizeof(graphe_t));

  if (graphe) {

    graphe->nb_sommets = n;
    graphe->type = type;

    new = (int **) malloc (n*(sizeof (int*)));

    if (new) {

      for (i=0;i<n;i++) {

	new[i]=NULL;
	new[i]= (int*) calloc (n, (sizeof (int)));   /*calloc remplit chaque tableau avec des 0*/

	if (new[i]==NULL) {printf("l'allocation a échoué \n");}

      }

      graphe->matadj=new;

    }

    else {printf("l'allocation a échoué \n");}

  }

  else {printf("l'allocation a échoué \n");}

  return graphe;

}




void supprimerGraphe (graphe_t * graphe) {

  int i;

  for (i=0;i<graphe->nb_sommets;i++) {
    free(graphe->matadj[i]);
  }

  free (graphe->matadj);
  free (graphe);

}



void ajouterAreteGraphe1 (graphe_t * graphe, int s1, int s2) {

  graphe->matadj[s1][s2] = 1;
  graphe->matadj[s2][s1] = 1;

}


void supprimerAreteGraphe1 (graphe_t * graphe, int s1, int s2) {

  graphe->matadj[s1][s2] = 0;
  graphe->matadj[s2][s1] = 0;

}




void ajouterAreteGraphe2 (graphe_t * graphe, int s1, int s2) {

  int * temp1 = NULL;
  int * temp2 = NULL;

  temp1 = *(graphe->matadj + s1);
  *(temp1 + s2) = 1;

  temp2 = *(graphe->matadj + s2);
  *(temp2 + s1) = 1;

  /* graphe[s1][s2] = 1;*/
  /* graphe[s2][s1] = 1;*/

}


void supprimerAreteGraphe2 (graphe_t * graphe, int s1, int s2) {

  int * temp1 = NULL;
  int * temp2 = NULL;

  temp1 = *(graphe->matadj + s1);
  *(temp1 + s2) = 0;

  temp2 = *(graphe->matadj + s2);
  *(temp2 + s1) = 0;

}




void afficherGraphe (graphe_t * graphe){

  int i,j;

  for (i=0;i<graphe->nb_sommets;i++) {

    for (j=0;j<graphe->nb_sommets;j++) {

      printf("%d ", graphe->matadj[i][j]);

    }

    printf("\n");
  }
}




arete_t * creerListeAretes (graphe_t * graphe) {

  arete_t * listeAretes = NULL;
  arete_t * aux = NULL;
  arete_t * cour = NULL;
  int i,j;

  for (i=0;i<(graphe->nb_sommets-1);i++) {

    for (j=i+1;j<graphe->nb_sommets;j++) {

      if((graphe->matadj[i][j]) == 1) {

	aux = (arete_t *) malloc (sizeof (arete_t));

	if (aux) {

	  aux->s1 = i;
	  aux->s2 = j;
	  aux->next = NULL;

	  if (listeAretes == NULL) {

	    listeAretes = aux;
	    cour = listeAretes;
	  }

	  else {

	    cour->next = aux;
	    cour = cour->next;
	  }
       	}

	else
	  printf("l'allocation a échoué \n");

      }
    }
  }

  return listeAretes;

}






void supprimerListeAretes (arete_t * listeAretes){

  arete_t * temp = NULL;

  while (listeAretes) {

    temp = listeAretes->next;
    free(listeAretes);
    listeAretes = temp;
  }
}




void ajouterArete (arete_t * listeAretes, arete_t * new) {

  arete_t * cour = NULL;

  if (listeAretes == NULL) {

    listeAretes = new;
    cour = listeAretes;
  }

  else {

    cour->next = new;
    cour = cour->next;
  }
}



void afficherListe (arete_t ** listeAretes){

  arete_t * cour = *listeAretes;

  while (cour) {
    printf("%d ", cour->s1);
    printf("%d\n", cour->s2);
    cour=cour->next;
  }
}



void sauvegarderGraphe (arete_t ** listeAretes, char * nomFichier) {

  FILE* fichier = NULL;
  arete_t * cour = *listeAretes;
  char sommet[4];

  fichier = fopen(nomFichier, "w");

  if (fichier) {

    while (cour) {

      sprintf(sommet, "%d", cour->s1);
      fputs(sommet,fichier);
      fputc(' ', fichier);

      sprintf(sommet, "%d", cour->s2);
      fputs(sommet,fichier);
      fputc('\n', fichier);

      cour = cour->next;
    }

      fclose(fichier);
    }
}


arete_t * recupererSauvegardeGraphe (char * nomFichier) {

  FILE * fichier = NULL;
  char sommet[4];
  int i;
  arete_t * listeAretes = NULL;
  arete_t * cour = NULL;
  arete_t * new = NULL;

  fichier = fopen(nomFichier, "r");

  if (fichier) {

    while (fgetc(fichier) != EOF) {

      fseek(fichier,-1,SEEK_CUR);
      i=0;

      new = (arete_t *) malloc (sizeof(arete_t));

      if (new) {

	while (fgetc(fichier) != ' ') {

	  fseek(fichier,-1,SEEK_CUR);
	  sommet[i] = fgetc(fichier);
	  i++;
	}
	sommet[i]='\0';

	new->s1 = atoi(sommet);
	i=0;

	while (fgetc(fichier) != '\n') {

	  fseek(fichier,-1,SEEK_CUR);
	  sommet[i] = fgetc(fichier);
	  i++;
	}
	sommet[i]='\0';

	new->s2 = atoi(sommet);
	new->next = NULL;

	if (listeAretes == NULL) {

	    listeAretes = new;
	    cour = listeAretes;
	}

	else {

	  cour->next = new;
	  cour = cour->next;
	}

      }
    }
    fclose(fichier);
  }

  return listeAretes;
}




int * auxConnexite (int taille) {

  int * tab = NULL;
  int i, alea, temp;

  tab = (int *) malloc (taille * sizeof(int));

  if (tab) {

    srand(time(NULL));

    for (i=0;i<taille;i++)
      tab[i] = i;

    for (i=0;i<taille;i++) {

      alea = rand()%taille;
      temp = tab[alea];
      tab[alea] = tab[i];
      tab[i]=temp;
    }

  }

  else
    exit(EXIT_FAILURE);

  return tab;

}


void auxAfficherTab (int * tab, int taille) {

  int i;

  for (i=0;i<taille;i++)
    printf("%d ",tab[i]);

  printf("\n");
}


/*niveau commence à 1*/
graphe_t * genererGraphe (int nbSalles) {

  graphe_t * graphe = NULL;
  int nb_aretes, i, s1, s2;
  int * tab;

  nb_aretes = (int) CORRIDOR_COEFF*nbSalles;   /*nombre de sommets à rajouter (en plus des (nbSalles-1))*/

  graphe = creerGraphe (nbSalles, NON_ORIENTE);

  tab = auxConnexite(nbSalles);

  for (i=0;i<nbSalles-1;i++)
    ajouterAreteGraphe1(graphe,tab[i],tab[i+1]);

  srand(time(NULL));

  for(i=0;i<nb_aretes;i++) {

    s1 = rand()%nbSalles;
    s2 = rand()%nbSalles;

    ajouterAreteGraphe1(graphe,s1,s2);

  }
  return graphe;
}

arete_t * getCorridors (int nbRoom)
{
    graphe_t * g = genererGraphe(nbRoom);
    arete_t  * a = creerListeAretes(genererGraphe(nbRoom));

    supprimerGraphe(g);

    return a;
}
