#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef _PREFIXE_H
#define _PREFIXE_H

typedef struct feuille
{
  /*
  Un noeud contient :
   1) la lettre qu'il represente
   2) Un tableau contenant 26 pointeur sur les structure suivantes (par ordre alphabetique)
      Si une lettre suivant n'est pas dans le dictionnaire, alors ce pointeur aura pour valeur NULL
   3)un char ayant pour valeur 0 ou 1. Ce char vaut 1 si le mot est un mot present dans le dictionnaire, 0 sinon.
  */
  char lettre;
  struct feuille **suivant;
  char fin; // un char prend moins de place qu'un int
} * noeud;

/*
fonction creant un noeud contenant la lettre a.
Cette fonction alloue la memoire de suivant.
*/
noeud creation(char a);

/*Cette fonction ajoute un mot à l'arbre prefixe*/
void ajout_mot(char *mot, noeud racine);

/* Cette fonction créer un arbre préfixe et le rempli avec le dictionnaire */
noeud creation_arbre(FILE *f_dictio);

/*
Cette fonction lit le dictionnaire et remplit l'arbre qu'elle renvoie avec ce dictionnaire
Elle renvoie la racine du dictionnaire
*/
noeud remplissage(FILE *f_dictio);

/*
Cette fonction regarde si un mot est present dans le dictionnaire dico qui est la racine du dictionnaire
Elle renvoie 1 si le mot est present, 0 sinon.
*/
int lecture(char *mot, noeud dico);

/* Fonction parcourant le fichier texte et compte le nombre de mots présent dans le dico*/
void verification_texte(FILE *f_texte, noeud dico);

/*
Fonction parcourant le char* fichier, et verifiant pour tous les mots si celui-ci est present dans dico.
Elle affiche dans le terminal les mots qui ne sont pas présent.
*/
void parcours_affiche(FILE *f_texte, noeud dico);

/*
Fonction renvoyant le numero du 1er fils du noeud feuille.
Si feuille n'a pas de fils, la fonction renvoie 26.
*/
int a_fils(noeud feuille);

/*
Fonction utilisant free pour desallouer le noeud dictio (cad le dictionnaire complet).
Cette fonction est une recursivite sur pluiseurs fils. Elle fonctionne de maniere recursive en
essayant a chaque noeud de liberer ses 26 fils.
*/
void liberation(noeud *dictio);

#endif
