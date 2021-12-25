#include "../../include/radix.h"
#include <time.h>

noeud creation(char a)
{
  noeud retour;
  retour = calloc(1, sizeof(*retour));
  retour->suivant = calloc(26, sizeof(noeud *)); // les 26 lettre de l'alphabet
  retour->fin = 0;                               // pour l'instant ce n'est pas la fin du mot
  retour->lettre = calloc(2, sizeof(char));      // pour l'instant, on lui alloue une seule lettre et le \0
  retour->lettre[0] = a;
  retour->lettre[1] = '\0';
  return retour;
}

void ajout_mot(char *mot, noeud racine)
{
  noeud actuel = racine; // le noeud que l'on va deplacer dans la boucle
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
  racine->lettre = calloc(2, sizeof(char));
  racine->lettre[0] = '$'; // pour pouvoir transformer l'arbre en suffixe

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
  racine->lettre = calloc(2, sizeof(char));
  racine->lettre[0] = '$'; // pour pouvoir transformer l'arbre en suffixe
  /*
  On va lire la premiere ligne dans le but de lancer une boucle while
  Cette ligne sera stockée dans un char appelé stock puis on fera un traitement lettre par lettre
  */
  char stock[28]; // le plus long mot contient 25 lettre donc 26 pour pouvoir contenir le \n ou |0.
  // comme 26 lettres ne suffisaient pas, j'ai mis 28 pour avoir un peu de marge.
  fgets(stock, 28, f_dictio);
  char stock2[28] = {"azertyu"}; // variable necessaire pour detecter la fin du fichier : une ligne apparaissant deux fois
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

int a_fils(noeud feuille, int *nb_fils)
{
  int i = 0;
  int premier_fils = -1;
  while (i < 26)
  {
    // tant qu'on a pas rencontre de fils, on continue de chercher
    // i ne peut pas depasser 25, on sortirait du du dico sinon
    if (feuille->suivant[i] != NULL)
    {                          // si c'est un fils
      *nb_fils = *nb_fils + 1; // on incremente le nb de fils
      if (premier_fils == -1)
        premier_fils = i; // cas ou on a le premier fils
    }

    i++;
  }
  // on renvoie premier fils qui contient -1 si pas de fils et le numero du premier fils s'il y en a un
  return premier_fils;
}

void fusion(noeud *feuille1, noeud *feuille2)
{
  /*
  fusion de feuille 2 dans feuille 1
  on remplace les valeurs de suivant de feuille 1 par les valeurs de suivant de feuille 2
  on ajoute la lettre de feuille 2 dans feuille 1
  si feuille 2 est une fin, feuille 1 le devient aussi
  */
  // on va commencer par preparer feuille1 -> lettre a recevoir une lettre supplementaire
  int longu = strlen((*feuille1)->lettre);               // on recupere la longueur pour pouvoir faire la nouvelle allocution
  char stock[256];                                       // variable pour stocker les lettres puisqu'on va devoir free, afin de reallouer
  strcpy(stock, (*feuille1)->lettre);                    // on stock les lettres
  free((*feuille1)->lettre);                             // on libere la memoire
  (*feuille1)->lettre = calloc(longu + 2, sizeof(char)); // puis on alloue

  // on remplit maintenant feuille1 -> lettre
  strcpy((*feuille1)->lettre, stock);
  (*feuille1)->lettre[longu] = (*feuille2)->lettre[0]; // feuille2 -> lettre ne contient qu'on seule lettre car on parcours l'arbre de bas en haut
  (*feuille1)->lettre[longu + 1] = '\0';

  if ((*feuille2)->fin == 1)
    (*feuille1)->fin = 1; // on s'occupe de la fin de mot

  noeud *a_liberer = (*feuille1)->suivant;
  (*feuille1)->suivant = (*feuille2)->suivant;

  // on doit maintenant s'occuper de la liberation de memoire
  free((*feuille2)->lettre);
  free(*feuille2);
  free(a_liberer);
}

void prefixe_vers_affixe(noeud *dictio)
{
  /*
  On va parcourir tous les noeuds de l'arbre.
  Si un noeud respecte les conditions :
    ce n'est pas une fin
    il a exactement un fils
  alors, il va fusionner avec le fils le suivant pour former un groupement de plusieurs lettres.
  */
  int nb_fils = 0;
  int premier_fils;

  if ((*dictio)->lettre[0] == '$')
  { // on est sur la racine
    // si on est sur la racine, on doit appeler prefexie vers affixe sur les lettre filles
    for (int i = 0; i < 26; i++)
    {
      // on est dans une lettre : on appelle la fonction
      if ((*dictio)->suivant[i] != NULL)
        prefixe_vers_affixe((*dictio)->suivant + i);
    }
  }

  else
  {
    premier_fils = a_fils(*dictio, &nb_fils); // on recup nb fils et premier fils
    if (nb_fils == 1 && (*dictio)->fin != 1)
    { // cas ou on fait la fusion
      fusion(dictio, (*dictio)->suivant + premier_fils); // le premier fils est le seul fils
      prefixe_vers_affixe(dictio); // on rappel la fct
    }
    else if (nb_fils != -1)
    {
      // cas ou on fait pas la fusion : on regarde la fct pour tous les fils
      for (int i = 0; i < 26; i++)
      {
        if ((*dictio)->suivant[i] != NULL)
          prefixe_vers_affixe((*dictio)->suivant + i);
      }
    }
  }
}

void affiche_arbre(noeud dictio)
{
  for (int i = 0; i < 26; i++)
  {
    if (dictio->suivant[i] != NULL)
    {
      affiche_arbre(dictio->suivant[i]);
    }
  }
}

void liberation(noeud *dictio)
{
  int inutile;
  int resultat = a_fils(*dictio, &inutile); // variable servant a stocker les resultats des appels de a_fils

  while (resultat != -1)
  { // si le noeud a un fils : on regarde si ce fils a lui-meme des fils

    liberation(((*dictio)->suivant) + resultat); // on va liberer le fils

    resultat = a_fils(*dictio, &inutile); // on regarde s'il y a un autre fils a liberer
  }
  // une fois qu'on arrive ici, c'est qu'on a libere tous les fils donc on libere le pointeur
  free((*dictio)->suivant); // on commence par liberer suivant
  free((*dictio)->lettre);
  free(*dictio);
  *dictio = NULL; // puis on libere dictio et on le met sur null pour pouvoir utiliser a_fils
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

int lecture(char *mot, noeud dico)
{
  /*
  On va creer un pointeur pour de deplacer dans dico.
  On va se deplacer noeud par noeud dans dico et comparer les lettres contenant dans le noeud et les lettres du mot
  Il faut comparer la longueur du mot et la longueur parcouru:
    si la longueur parcouru est plus grande : alors le mot n'existe pas
    si la longueur parcouru est de la meme taille :  on compare toutes les lettres
    si elle est plus petite : on compare les lettres puis on se deplace sur le neud suivant
  */
  // on commence par enlever tous les mots commencant par autre chose qu'une lettre
  if (mot[0] - 'a' < 0 || mot[0] - 'a' >= 26)
    return 1;

  int longueur = strlen(mot);
  int long_etude;   // variable servant a la longueur de la partie de mot qu'on va etudier
  int compteur = 0; // le compteur de lettre parcourue
  noeud p = dico;
  char let;       // la lettre qu'on etudie
  char etude[30]; // la partie de mot qu'on etudie
  int val;       // permet de convertir une lettre en nb compris entre 0 et 25

  while (compteur <= longueur)
  {
    let = mot[compteur]; // on recupere la lettre a etudier
    val = let - 'a';

    // on regarde si la lettre est present dans suivant
    if (p->suivant[val] == NULL)
      return 0;

    p = p->suivant[val]; // on se deplace la ou on doit etudier
    strcpy(etude, p->lettre);   // on remplit etude
    long_etude = strlen(etude); // on recupere sa longueur

    if (compteur + long_etude > longueur)
      return 0; // c'est trop grand : le mot n'y est pas

    // si on est ici, c'est que on est de meme longueur ou plus petit
    // on va donc faire la comparaison lettre a lettre
    for (int i = 0; i < long_etude; i++)
    {
      if (etude[i] != mot[compteur])
        return 0; // cas ou les lettres different
      // sinon c'est que les deux lettres sont id, auquel cas on va juste comparer les lettres suivantes
      compteur++; // on augmente compteur
    }
    // si on arrive ici c'est que toutes les lettres sont id
    // on va donc regarder si les longeurs sont identiques
    if (compteur == longueur && p->fin == 1)
      return 1; // si elles sont id et que toutes les lettres sont id, alors
    if (compteur == longueur && p->fin == 0)
      return 0; // on a lu tous le mot mais ce n'est pas une fin

    // si on arrive ici, c'est qu'on a compteur plus petit que longueur, il faut donc continuer la boucle
  }
  return 0 ; //inutile
}

int main()
{


  float temps_creation = 0, temps_lecture = 0;
  clock_t t1, t2, t3;

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

  t1 = clock();
  noeud dictionnaire = creation_arbre(f_dictio);
  prefixe_vers_affixe(&dictionnaire);

  t2 = clock();
  verification_texte(f_texte, dictionnaire);
  t3 = clock();
  liberation(&dictionnaire);
  temps_creation += (float)(t2 - t1) / CLOCKS_PER_SEC;
  temps_lecture += (float)(t3 - t2) / CLOCKS_PER_SEC;

  fclose(f_dictio);
  fclose(f_texte);

  printf("temps creation = %f\n", temps_creation);
  printf("temps lecture: %f\n", temps_lecture);

  return 15 ;

}
