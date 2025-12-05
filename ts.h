#ifndef TS_H
#define TS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*** Step 1: Définition des structures de données ***/
typedef struct
{
    int state;
    char name[20];
    char code[20];
    char type[20];
    char val[100]; // "val" remains a string
    char Is_Vector[2];
    char borne_inf[20];
    char taille[20];
    char Is_cst[2];
} element;

typedef struct
{
    int state;
    char name[20];
    char type[20];
} elt;

#define TABLE_SIZE 50
#define TABLE_SYMB_SIZE 40

typedef struct HashNodeElement
{
    char key[20];
    element value;
    struct HashNodeElement *next;
} HashNodeElement;

typedef struct HashNodeElt
{
    char key[20];
    elt value;
    struct HashNodeElt *next;
} HashNodeElt;

// Déclarations des fonctions
void initHashTable();
void insertHashTable();
void freeHashTable();
element *searchHashTable(char *key);
char *intToString(int num);
char *floatToString(float num);
void rechercher(char entite[], char code[], char type[], char *val, int y, char *Is_Vector, char *borne_inf, char *taille, char *Is_cst);
void afficher();

#endif // TS_H
