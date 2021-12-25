#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef _RADIX_H
#define _RADIX_H

typedef struct feuille
{
  /*
  Un noeud contient :
   1) la lettre qu'il represente
   2) Un tableau contenant 26 pointeur sur les structure suivantes (par ordre alphabetique)
      Si une lettre suivant n'est pas dans le dictionnaire, alors ce pointeur aura pour valeur NULL
   3)un char ayant pour valeur 0 ou 1. Ce char vaut 1 si le mot est un mot present dans le dictionnaire, 0 sinon.
  */
  char *lettre;
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
Fonction renvoyant le numero du 1er fils du noeud feuille.
Si feuille n'a pas de fils, la fonction renvoie -1.
La fct modifi egalement nb_fils de maniere a ce que cette variable contienne le nb de fils du noeud
*/
int a_fils(noeud feuille, int *nb_fils);

/*
fonction servant a fusionner feuille1 et feuille2.
on va garder feuille1 en lui inserant les variables utiles et supprimer feuille2
*/
void fusion(noeud *feuille1, noeud *feuille2);

/*
fonction servant transformer un arbre radix en arbre prefixe
Cette fonction transforme les noeuds ayant un seul fils et n'etant pas une fin de mots en noeud contenant plusieurs lettres
*/
void prefixe_vers_affixe(noeud *dictio);

/*
fonction servant pour les test et pour voir ce qu'il se passe.
elle affiche de maniere recursive l'arbre
si vous voulez pouvoir comprendre, il faut limiter le nombres de mots lus dans le dictionnaire
*/
void affiche_arbre(noeud dictio);

/* Fonction parcourant le fichier texte et compte le nombre de mots présent dans le dico*/
void verification_texte(FILE *f_texte, noeud dico);

/*
Fonction utilisant free pour desallouer le noeud dictio (cad le dictionnaire complet).
Cette fonction est une recursivite sur pluiseurs fils. Elle fonctionne de maniere recursive en
essayant a chaque noeud de liberer ses 26 fils.
*/
void liberation(noeud *dictio);

/*
fonction de lecture d'un mot dans l'arbre radix. On regarde si ce mot est bien present
Cette fonction est inspire de la fonction lecture de l'abre prefixe.
elle renvoie 1 si le mot est present, 0 sinon
*/
int lecture(char *mot, noeud dico);

#endif
