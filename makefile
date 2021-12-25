#Makefile pour generer les executables du projet de Guyot Baptiste et Levy Gabriel

#les raccourcis

#Pour les fichiers executables ou les fichiers .o
TESTDIR=./test

#Pour les fichiers sources .c
SRCDIR=./src

#les options du compilateur
CFLAGS=-g -c

#Le nom du compilateur
CC=gcc

#maintenant : la compilation.
test/radix: $(TESTDIR)/radix.o
	$(CC) -o $@ $^

# $@ c'est pour remplacer test/radix
#$^ c'est pour remplacer l'objet, cad $(BINDIR)/radix.o

test/prefixe: $(TESTDIR)/prefixe.o
	gcc -o $@ $^

test/hashtable: $(TESTDIR)/table.o
	gcc -o $@ $^

test/radix.o: $(SRCDIR)/radix/radix.c
	gcc $(CFLAGS) $^ -o $@

test/prefixe.o: $(SRCDIR)/arbre_prefixe/prefixe.c
	gcc $(CFLAGS) $^ -o $@

test/table.o: $(SRCDIR)/hashtable/table.c
	gcc $(CFLAGS) $^ -o $@

#lancement des executables
hashexe: $(TESTDIR)/hashtable
	$^

prefexe: $(TESTDIR)/prefixe
	$^

radexe: $(TESTDIR)/radix
	$^
