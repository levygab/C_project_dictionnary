#include "../../include/prefixe.h"
#include <time.h>

noeud creation(char a)
{
  noeud retour;
  retour = calloc(1, sizeof(*retour));
  retour->suivant = calloc(26, sizeof(noeud *)); // les 26 lettre de l'alphabet
  retour->fin = 0;                               // pour l'instant ce n'est pas la fin du mot
  retour->lettre = a;
  return retour;
}

void ajout_mot(char *mot, noeud racine)
{
  noeud actuel = racine; // variable utilisé pour se déplacer dans l'arbre
  int i = 0;             // variable permettant de se deplacer dans stock
  char let = mot[0];     // lettre qu'on utilise
  int valeur;           // pour manier le code ascii de a (le ramener entre 0 et 26)
  while (let != '\n' && let != '\0')
  {
    valeur = let - 'a'; // c'est pour verifier sa presence dans le tableau
    if (actuel->suivant[valeur] != NULL)
    {                                   // cas ou ce n'est pas NULL, let est deja present
      actuel = actuel->suivant[valeur]; // on se deplace sur le nouveau noeud
    }
    else
    {
      noeud ajout = creation(let);     // on cree un noeud contenant la lettre
      actuel->suivant[valeur] = ajout; // on ajoute ce pointeur a la liste contenant les lettres suivante
      actuel = ajout;                  // on deplace le pointeur actuel
    }

    // on a traite les deux cas possible donc
    i++;          // on incremente
    let = mot[i]; // on met let a jour
  }
  actuel->fin = 1; // on est fin de mot, donc on met la varibale a 1
}

noeud creation_arbre(FILE *f_dictio)
{
  noeud racine;                        // le noeud de base, celui de depart
  racine = calloc(1, sizeof(*racine)); // celui la n'utilise pas la fonction creation car il ne contient pas de lettre
  racine->fin = 0;
  racine->suivant = calloc(26, sizeof(noeud *)); // les 26 lettres de l'alphabet

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
    ajout_mot(mot, racine);
  }
  return racine;
}

noeud remplissage(FILE *f_dictio)
{
  noeud racine;                        // le noeud de base, celui de depart
  racine = calloc(1, sizeof(*racine)); // celui la n'utilise pas la fonction creation car il ne contient pas de lettre
  racine->fin = 0;
  racine->suivant = calloc(26, sizeof(noeud *)); // les 26 lettres de l'alphabet

  /*
  On va lire la premiere ligne dans le but de lancer une boucle while
  Cette ligne sera stockée dans un char appelé stock puis on fera un traitement lettre par lettre
  */
  char stock[28]; // le plus long mot contient 25 lettre donc 26 pour pouvoir contenir le \n ou |0.
  // comme 26 lettres ne suffisaient pas, j'ai mis 28 pour avoir un peu de marge.
  fgets(stock, 28, f_dictio);
  char stock2[28] = {"azertyui"}; // variable necessaire pour detecter la fin du fichier : une ligne apparaissant deux fois
  // on entre dans la boucle de lecture
  noeud actuel = racine; // le noeud que l'on va deplacer dans la boucle

  while (strcmp(stock, stock2) != 0)
  {

    /*
    On commence par traiter le mot contenue dans stock.
    On traite ce mot lettre par lettre.
    Pour chaque lettre :
      1) on regarde si elle presente dans suivante ou si c'est un '\0'
      2.1) si elle est dans suivant : on va sur le noeud puis on passe a la lettre suivante de stock
      2.2) si elle n'est pas dans suivante : on l'ajoute a suivante puis on va sur le noeud puis on passe a la lettre suivante de stock
      2.3) Si c'est un '\0' : on passe fin a 1 (on est deja sur le bon noeud) et on passe a la ligne suivante
    */
    int i = 0;           // variable permettant de se deplacer dans stock
    char let = stock[0]; // lettre qu'on utilise
    int valeur;         // pour manier le code ascii de a (le ramener entre 0 et 26)
    while (let != '\n' && let != '\0')
    {
      valeur = let - 'a'; // c'est pour verifier sa presence dans le tableau
      if (actuel->suivant[valeur] != NULL)
      {                                   // cas ou ce n'est pas NULL, let est deja present
        actuel = actuel->suivant[valeur]; // on se deplace sur le nouveau noeud
      }
      else
      {
        noeud ajout = creation(let);     // on cree un noeud contenant la lettre
        actuel->suivant[valeur] = ajout; // on ajoute ce pointeur a la liste contenant les lettres suivante
        actuel = ajout;                  // on deplace le pointeur actuel
      }

      // on a traite les deux cas possible donc
      i++;            // on incremente
      let = stock[i]; // on met let a jour
    }

    // on n'a pas fini de traiter la ligne, il faut traiter la fin de ligne
    // on a atteint le '\0' ou '\n' , pas la peine de faire un test
    actuel->fin = 1;       // on est fin de mot, donc on met la varibale a 1
    actuel = racine;       // on remet actuel au depart de l'arbre
    strcpy(stock2, stock); // on met remplace l'ancienne ligne par la ligne qu'on vient d'etudier
    // il reste a lire la ligne suivante
    fgets(stock, 28, f_dictio);
  }

  // on est arrivé en fin de fichier (stock commence par \0)
  // on renvoie la variable dont on a besoin
  return racine;
}

int lecture(char *mot, noeud dico)
{
  /*
  On va créer un pointeur noeud servant a se deplacer dans dico.
  On va se deplacer de lettre en lettre dans mot.
  Pour chaque lettre, on va aller dans le pointeur_dico et on va aller dans le noeud correspondant a cette lettre
  Pour chaque lettre on va comparer le nb de lettre parcouru et la longueur du mot
  Si la longueur du mot correpond au nb de lettre parourue : on regarde la valeur fin
  si fin vaut 1 c'est que le mot est present, sinon, le mot n'est pas present
  */

  // on commence par enlever tous les mots commencant par autre chose qu'une lettre
  if (mot[0] - 'a' < 0 || mot[0] - 'a' >= 26)
    return 1;

  int longueur = strlen(mot);
  int compteur = 0; // le compteur de lettre parcourue
  noeud p = dico;
  char let; // la lettre sur laquelle on va travailler
  int val; // permet de travailler entre 0 et 25
  while (compteur <= longueur)
  { // c'est une securite, un while (1) marche aussi
    let = mot[compteur];
    val = let - 'a';
    // on verifie que cette lettre est bien presente dans suivant.
    // si elle ne l'est pas, on renvoie 0 car cela veut dire que le mot ne sera pas present
    if (p->suivant[val] == NULL)
      return 0;

    p = p->suivant[val]; // on deplace p
    compteur++;          // on incremente le compteur de lettre parcouru

    if (compteur == longueur)
    { // si on a parcouru tout le mot : on regarde s'il est present
      if (p->fin == 1)
        return 1; // le mot est present
      else
        return 0; // le mot n'est pas present
    }
  }
  return 0 ; //inutile
}

void verification_texte(FILE *f_texte, noeud dico)
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
      // printf("%c\n", caract);
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
      if (lecture(mot, dico) == 0 && compt_let != 0)
      {
        nb_mots_absents++;
      }
      compt_let = 0;
    }
  }
  printf("Le nombre de mots dans le texte est de: %d\n", nb_mots);
  printf("Le nombre de mots absents du dictionnaire est de: %d\n", nb_mots_absents);
}

void parcours_affiche(FILE *f_texte, noeud dico)
{
  /*
  Cette fonction va parcourir le fichier ligne par ligne, puis decouper chaque ligne en mot en faisant un traitement charactere par charactere.
  Un mot est fini quand on atteint :
    -une virgule,
    -un '\n' sauf s'il y a un tiret avant.
    -un espace,
    -un point
    -un tiret
    -un '\0', sauf s'il y a un tiret avant
    -autre option : un mot s'arrete si le code ascii n'est pas celui d'une lettre (attention a prendre en compte les majuscules)

  Des qu'on a trouve un mot : on utilise lecture pour verifier que ce mot est bien dans le dictinnaire
  Si le mot est juste un espace, la fonction lecture renvoie 1, meme si l'espace n'est pas dans le dico
  */
  // varaiable pour traiter la ligne
  char stock[512];  // varibale qu'on va utiliser pour stocker les charactere de chaque ligne
  char stock2[512]; // varibale qu'on utilise pour verifier que l'on est pas a la fin
  int c_stock = 0;  // compteur, pour permettre de se deplacer dans stock

  // variable pour traiter le mot
  char mot[26];  // varibale remplit au fur et a mesure de chaque lettre
  int c_mot = 0; // compteur pour se deplacer dans mot
  int test;      // pour le retour de la fct lecture

  // varibale pour traiter la lettre
  char let;     // pour traiter chaque lettre
  char val_min; // pour ramener la lettre ne 0 et 26 des minuscules
  char val_maj; // pour ramener la lettre ne 0 et 26 des majuscule

  int i = 0;

  // lecture de la premiere ligne
  fgets(stock, 512, f_texte);

  // boucle de lecture du fichier

  while (strcmp(stock, stock2) != 0)
  {
    // on va faire le traitement ligne a ligne

    while (stock[c_stock] != '\n' && stock[c_stock] != '\0')
    {

      let = stock[c_stock]; // on prend le charactere a etudier
      val_min = let - 'a';
      val_maj = let - 'A';

      // test si le charactere est une lettre :
      if ( (val_min >= 0 && val_min < 26) || (val_maj >= 0 && val_maj < 26))
      {
        // on est ici donc c'est une lettre : on l'ajoute a mot, en minuscule
        let = tolower(let);
        mot[c_mot] = let;

        // on incremente les deux compteur
        c_mot++;
        c_stock++;
      }

      else
      {
        // dans le cas ou ce n'est pas une lettre
        // peu importe le charactere, si ce n'est pas une lettre, le mot est fini.

        mot[c_mot] = '\0';         // le mot est fini, on lui met le charactere de fin
        test = lecture(mot, dico); // on fait le test
        if (test == 0)
        {
          i++;
        }

        // que le mot soit present ou non, on continue le traitement :
        c_mot = 0; // on remet le compteur mot a 0
        c_stock++; // on incremente le compteur de stock
      }
    }
    // c'est qu'on est en fin de ligne : il faut mettre le compteur de stock a 0 et lire une nouvelle ligne
    c_stock = 0;
    strcpy(stock2, stock); // permet d'arreter la boucle while
    fgets(stock, 512, f_texte);
  }
}

int a_fils(noeud feuille)
{
  int i = 0;
  while (i < 26 && feuille->suivant[i] == NULL)
  {
    // tant qu'on a pas rencontre de fils, on continue de chercher
    // i ne peut pas depasser 25, on sortirait du du dico sinon
    i++;
  }
  // on renvoie i car soit il a une valeur inferieur a 26 et c'est un fils,
  // soit il a une valeur superieur a 26 et noeud n'a pas de fils

  return i;
}

void liberation(noeud *dictio)
{
  int resultat = a_fils(*dictio); // variable servant a stocker les resultats des appels de a_fils

  while (resultat != 26)
  { // si le noeud a un fils : on regarde si ce fils a lui-meme des fils

    liberation(((*dictio)->suivant) + resultat); // on va liberer le fils

    resultat = a_fils(*dictio); // on regarde s'il y a un autre fils a liberer
  }
  // une fois qu'on arrive ici, c'est qu'on a libere tous les fils donc on libere le pointeur
  free((*dictio)->suivant); // on commence par liberer suivant
  free(*dictio);
  *dictio = NULL; // puis on libere dictio et on le met sur null pour pouvoir utiliser a_fils
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

  noeud dictionnaire = creation_arbre(f_dictio);

  t2 = clock();

  verification_texte(f_texte, dictionnaire);
  t3 = clock();

  temps_creation = (float)(t2 - t1) / CLOCKS_PER_SEC;
  temps_lecture = (float)(t3 - t2) / CLOCKS_PER_SEC;

  printf("temps creation = %f\n", temps_creation);
  printf("temps lecture: %f\n", temps_lecture);

  liberation(&dictionnaire);
  fclose(f_dictio);
  fclose(f_texte);

  return 15 ;
}
