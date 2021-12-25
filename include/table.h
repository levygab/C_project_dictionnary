#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef _TABLE_H
#define _TABLE_H

#define M 500000

typedef struct cellule
{
  /*
  une cellule (d'une hashtable) contient :
    un char* qui contient le mot
    un next, permettant d'avoir le mot suivant si plusieurs mot ont le meme hashcode
  */
  char *mot;
  struct cellule *next;
} cellule;

/*
Fonction prenant en argument une chaine de caractere et renvoyant
en sortie son hashcode.
*/
unsigned int hash(char *chaine);

/*
Fonction remplissant la table de hachage.
Cette fonction prend en argument une chaine qu'elle ajoute a sa place dans la table
Elle fait une disjonction de cas : soit le hashcode est nouveau et le mot est directement ajoute a la table
soit le hashcode est deja apparu et il est ajoute a la table mais dans une cellule next.
*/
void rempli_cellule(char *chaine, cellule *table);

/*
Fonction affichant tous les mots contenus dans la table
pour l'istant elle n'affiche que les mots qui sont dans la premiere partie de la table ou dans le premier next
cette fonction est tres utile pour les test.
*/
void aff_table(cellule *table);

/*
Fonction créant une table de hachage avec les mots du dictionnaire
*/
cellule *creation_table(FILE *f_dictio);

/* Fonction parcourant le fichier texte et compte le nombre de mots présent dans le dico*/
void verification_texte(FILE *f_texte, cellule *table_dico);

/*
Fonction parcourant le char* fichier, et verifiant pour tous les mots si celui-ci est present dans dico.
Elle affiche dans le terminal les mots qui ne sont pas présent.
*/
void parcours_affiche(FILE *f_texte, cellule *table_dico);

/*
Cette fonction regarde si un mot est present dans la table dictionnaire
Elle renvoie 1 si le mot est present, 0 sinon.
*/
int lecture(char *mot, cellule *table_dico);

/*
Fonction libérant la mémoire utilisé par la table
*/
void liberation(cellule *table);

#endif
