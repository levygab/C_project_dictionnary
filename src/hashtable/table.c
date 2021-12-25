#include "../../include/table.h"
#include <time.h>

unsigned int hash(char *chaine)
{
  // variables utiles
  unsigned int code = 0; // la valeur que l'on renvoie
  int i = 0;             // compteur dans la boucle
  int a = 31;            // coefficient de multiplication

  while (chaine[i] != '\n' && chaine[i] != '\0')
  {
    code = a * code + chaine[i]; // on multiplie par a, puis on ajoute le nouveau terme
    i++;
  }
  code = code % M; // on applique le modulo
  return code;
}

void rempli_cellule(char *chaine, cellule *table)
{

  // variables utiles
  int longu = strlen(chaine);       // la longueur du mot
  unsigned int code = hash(chaine); // le code du mot
  cellule *cell;                    // variable pour se deplacer dans la table

  // deux cas possible : ou bien la cellule avec le hashcode est deja occupe, ou bien elle ne l'est pas

  // si elle ne l'est pas
  if (table[code].mot == NULL)
  {
    // on commence par allouer le mot
    table[code].mot = calloc(longu + 1, sizeof(char)); // le +1, c'est pour le \0
    strcpy(table[code].mot, chaine);                   // on copie maintenant le mot
  }

  // cas ou le hashcode est deja occupe
  else
  {

    // on ajoute le mot en 2eme position dans la liste chainée
    cell = calloc(1, sizeof(cellule));
    cell->next = (table + code)->next;
    cell->mot = calloc(longu + 1, sizeof(char));
    strcpy(cell->mot, chaine); // copie du mot
    (table + code)->next = cell;
  }
}

void aff_table(cellule *table)
{
  for (int i = 0; i < M; i++)
  {

    cellule *cell = table + i;
    while (cell != NULL)
    {
      if (cell->mot != NULL)
        printf("table[%d].mot = %s\n", i, cell->mot);
      cell = cell->next;
    }
  }
}

cellule *creation_table(FILE *f_dictio)
{
  // on crée la cellule de départ
  cellule *table = calloc(M, sizeof(cellule));

  // on récupère le premier mot du dictionnaire
  char mot[28];

  while (fgets(mot, 28, f_dictio) != NULL)
  {
    // on s'assure qu'il n'y a pas de caractere différent d'une lettre dans le mot
    for (int i = 0; i < 28; i++)
    {
      char caractere = mot[i];
      if (caractere - 'a' < 0 || caractere - 'a' >= 26)
      {
        mot[i] = '\0';
        i = 28;
      }
    }
    rempli_cellule(mot, table);
  }
  return table;
}

void verification_texte(FILE *f_texte, cellule *table_dico)
{
  char mot[26]; // variable contenant les mots du texte
  char caract = 0;
  int nb_mots = 0;
  int nb_mots_absents = 0;

  int compt_let = 0; // compteur de lettre dans le mot

  // on traite chaque caractere 1 par 1 du fichier afin de reformer les mots et les chercher dans la table
  while (caract != EOF)
  {
    caract = fgetc(f_texte);

    // on force le caractere en minuscule
    caract = tolower(caract);

    // test si le caractere est une lettre
    if (caract - 'a' >= 0 && caract - 'a' < 26)
    {
      mot[compt_let] = caract;
      compt_let++;
    }

    // si le caractere n'est pas une lettre
    else
    {
      // le mot est fini
      mot[compt_let] = '\0';

      if (compt_let != 0)
        nb_mots++;

      // si compt_let = 0 alors ce n'est pas un mot
      if (lecture(mot, table_dico) == 0 && compt_let != 0)
      {
        nb_mots_absents++;
      }
      compt_let = 0;
    }
  }
  printf("Le nombre de mots dans le texte est de: %d\n", nb_mots);
  printf("Le nombre de mots absents du dictionnaire est de: %d\n", nb_mots_absents);
}

void parcours_affiche(FILE *f_texte, cellule *table_dico)
{

  printf("Ces mots ne sont pas present dans le dictionnaire : \n");

  char mot[26];
  char caract = 0;
  int i = 0;
  int j = 0;

  int compt_let = 0; // compteur de lettre dans le mot

  // on traite chaque caractere 1 par 1 du fichier afin de reformer les mots et les chercher dans la table
  while (caract != EOF)
  {
    caract = fgetc(f_texte);

    // on force le caractere en minuscule
    caract = tolower(caract);

    // test si le caractere est une lettre
    if (caract - 'a' >= 0 && caract - 'a' < 26)
    {
      mot[compt_let] = caract;
      // printf("%c\n", caract);
      compt_let++;
    }

    // si le caractere n'est pas une lettre
    else
    {
      // le mot est fini
      mot[compt_let] = '\0';

      if (compt_let != 0)
        j++;

      // si compt_let = 0 alors ce n'est pas un mot
      if (lecture(mot, table_dico) == 0 && compt_let != 0)
      {
        // le mot n'est pas présent
        printf("   -  %s\n", mot);
        i++;
      }
      compt_let = 0;
    }
  }
  printf("nb mots pas dictionnaire:%d\n", i);
  printf("nb mots total: %d\n", j);
}

int lecture(char *mot, cellule *table_dico)
{
  cellule *cell = table_dico + hash(mot);

  while (cell != NULL)
  {
    if (cell->mot != NULL)
      if (strcmp(cell->mot, mot) == 0)
      {
        return 1;
      }
    cell = cell->next;
  }
  return 0;
}

void liberation(cellule *table)
{
  /*
  On va parcourir la table de hachage qui est une liste de cellule.
  Chaque cellule est une liste chainee qu'on va liberer, puis on va avancer dans le tableau afin de liberer la cellule suivante
  */
  cellule *cell = NULL;
  cellule *stock = NULL;
  int j = 0;
  for (int i = 0; i < M; i++)
  {
    j = 0;
    cell = table + i;
    while (cell != NULL)
    {
      stock = cell;
      cell = cell->next;
      free(stock->mot);
      if (j > 0)
        free(stock);
      j++;
    }
  }
  free(table);
}

int main()
{

  FILE *f_texte = NULL;
  f_texte = fopen("a_la_recherche_du_temps_perdu.txt", "rt"); // en read car on ne modifie pas le fichier
  // test d'erreur d'ouverture
  if (f_texte == NULL)
  {
    printf("erreur ouverture texte\n");
    return EXIT_FAILURE;
  }

  
  // on va lire le fichier
  FILE *f_dictio = NULL;
  f_dictio = fopen("FR.txt", "rt"); // en read car on ne modifie pas le fichier
  // test d'erreur d'ouverture
  if (f_dictio == NULL)
  {
    printf("erreur ouverture dictio\n");
    return EXIT_FAILURE;
  }

  float temps_creation, temps_lecture;
  clock_t t1, t2, t3;

  t1 = clock();
  cellule *table = creation_table(f_dictio);
  t2 = clock();
  verification_texte(f_texte, table);
  t3 = clock();

  temps_creation = (float)(t2 - t1) / CLOCKS_PER_SEC;
  temps_lecture = (float)(t3 - t2) / CLOCKS_PER_SEC;

  printf("temps creation = %f\n", temps_creation);
  printf("temps lecture: %f\n", temps_lecture);



  fclose(f_dictio);
  fclose(f_texte);

  liberation(table);

  return 15;

}
