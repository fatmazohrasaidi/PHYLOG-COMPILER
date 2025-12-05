/**************** CREATION DE LA TABLE DES SYMBOLES ******************/
#include "ts.h"

// Ici, tu peux ajouter tes fonctions qui manipulent ces structures.

/*** La fonction hash (sans appliquer de modulo ici) ***/
unsigned int hash(char *key)
{
    unsigned int hashValue = 0;
    while (*key)
    {
        hashValue = (hashValue << 5) + *key++;
    }
    return hashValue; // Le modulo se fait au moment d'utiliser la valeur de hachage.
}

/*** Tables globales ***/
// Pour les identifiants et constantes
HashNodeElement *tab[TABLE_SIZE];
// Pour les mots clés
HashNodeElt *tabm[TABLE_SYMB_SIZE];
// Pour les séparateurs
HashNodeElt *tabs[TABLE_SYMB_SIZE];

/*** Step 2: Initialisation des tables des symboles ***/
void initHashTable()
{
    int i;
    for (i = 0; i < TABLE_SIZE; i++)
    {
        tab[i] = NULL;
    }
    for (i = 0; i < TABLE_SYMB_SIZE; i++)
    {
        tabm[i] = NULL;
        tabs[i] = NULL;
    }
}

/*** Step 3: Insertion des entités lexicales dans les tables des symboles ***/
/* On modifie la signature pour accepter la valeur en tant que chaîne de caractères */
void insertHashTable(char entite[], char code[], char type[], char *val, int y, char *Is_Vector, char *borne_inf, char *taille,char *Is_cst)
{
    if (y == 0 || y == 3)
    { // Insertion dans la table des IDF et CONST: in la table tab: the one we have declared vector size in and vector bounds
        unsigned int index = hash(entite) % TABLE_SIZE;
        HashNodeElement *newNode = (HashNodeElement *)malloc(sizeof(HashNodeElement));
        strcpy(newNode->key, entite);
        newNode->value.state = 0;
        strcpy(newNode->value.name, entite);
        strcpy(newNode->value.code, code);
        strcpy(newNode->value.type, type);
        strcpy(newNode->value.val, val);
        strcpy(newNode->value.Is_Vector, Is_Vector);
        strcpy(newNode->value.borne_inf, borne_inf);
        strcpy(newNode->value.taille, taille);
        strcpy(newNode->value.Is_cst, Is_cst);
        newNode->next = tab[index];
        tab[index] = newNode;
    }
    else if (y == 1)
    { // Insertion dans la table des mots clés
        unsigned int index = hash(entite) % TABLE_SYMB_SIZE;
        HashNodeElt *newNode = (HashNodeElt *)malloc(sizeof(HashNodeElt));
        strcpy(newNode->key, entite);
        newNode->value.state = 1;
        strcpy(newNode->value.name, entite);
        /* Comme la structure "elt" ne contient pas de champ "code",
           on utilise ici "type" pour stocker l'information */
        strcpy(newNode->value.type, code);
        newNode->next = tabm[index];
        tabm[index] = newNode;
    }
    else if (y == 2)
    { // Insertion dans la table des séparateurs
        unsigned int index = hash(entite) % TABLE_SYMB_SIZE;
        HashNodeElt *newNode = (HashNodeElt *)malloc(sizeof(HashNodeElt));
        strcpy(newNode->key, entite);
        newNode->value.state = 1;
        strcpy(newNode->value.name, entite);
        strcpy(newNode->value.type, code);
        newNode->next = tabs[index];
        tabs[index] = newNode;
    }
}

/*** Step 4: Fonction de recherche qui vérifie si l'entité existe déjà ***/
void rechercher(char entite[], char code[], char type[], char *val, int y, char *Is_Vector, char *borne_inf, char *taille,char *Is_cst)
{
    if (y == 0 || y == 3)//only in la table de ts de les idf we need these three
    //int Is_Vector, int borne_inf, int taille,, that's why in the other fun call 
    // we initilise them to zero cuz they arent even used
    {
        unsigned int index = hash(entite) % TABLE_SIZE;
        HashNodeElement *node = tab[index];
        while (node)
        {
            if (strcmp(node->value.name, entite) == 0)
            {
                // printf("Entité \"%s\" existe déjà\n", entite);
                return;
            }
            node = node->next;
        }
        // printf("\n++++++++++++++inserting l entite: %s+++\n",entite);
        insertHashTable(entite, code, type, val, y, Is_Vector, borne_inf,taille,Is_cst);
    }
    else if (y == 1)
    {
        unsigned int index = hash(entite) % TABLE_SYMB_SIZE;
        HashNodeElt *node = tabm[index];
        while (node)
        {
            if (strcmp(node->value.name, entite) == 0)
            {
                // printf("Entité \"%s\" existe déjà\n", entite);
                return;
            }
            node = node->next;
        }
        // printf("\n++++++++++++++inserting l entite: %s+++\n",entite);
        insertHashTable(entite, code, type, val, y, Is_Vector, borne_inf,taille,Is_cst);
    }
    else if (y == 2)
    {
        unsigned int index = hash(entite) % TABLE_SYMB_SIZE;
        HashNodeElt *node = tabs[index];
        while (node)
        {
            if (strcmp(node->value.name, entite) == 0)
            {
                // printf("Entité \"%s\" existe déjà\n", entite);
                return;
            }
            node = node->next;
        }
        // printf("\n++++++++++++++inserting l entite: %s+++\n",entite);
        insertHashTable(entite, code, type, val, y, Is_Vector, borne_inf,taille,Is_cst);
    }
}

/*** Recherche dans la table des identifiants et constantes ***/
element *searchHashTable(char *key)
{
    unsigned int index = hash(key) % TABLE_SIZE;
    HashNodeElement *node = tab[index];
    while (node)
    {
        // printf("------im parsing la ts and currently at the element: %s\n", &node->value.name);
        if (strcmp(node->value.name, key) == 0)
        {
            // printf("..... the value FOUND in la ts is: val=%s\n", &node->value.name);
            return &node->value;
        }
            
        node = node->next;
    }
    return NULL;
}

/*** Libération de la mémoire allouée pour les tables ***/
void freeHashTable()
{
    int i;
    for (i = 0; i < TABLE_SIZE; i++)
    {
        HashNodeElement *node = tab[i];
        while (node)
        {
            HashNodeElement *temp = node;
            node = node->next;
            free(temp);
        }
    }
    for (i = 0; i < TABLE_SYMB_SIZE; i++)
    {
        HashNodeElt *node = tabm[i];
        while (node)
        {
            HashNodeElt *temp = node;
            node = node->next;
            free(temp);
        }
        node = tabs[i];
        while (node)
        {
            HashNodeElt *temp = node;
            node = node->next;
            free(temp);
        }
    }
}
/*** Step 5: Affichage du contenu de la table des symboles ***/
void afficher()
{
    int i;
    printf("/*************** Table des symboles IDF *************/\n");
    printf("____________________________________________________________________________________________________\n");
    printf("\t| Nom_Entite|Code_Entite| Type_Entite|  Val_Entite  |IS_VECTOR|borne_inf| taille | is_cst\n");
    printf("____________________________________________________________________________________________________\n");
    for (i = 0; i < TABLE_SIZE; i++)
    {
        HashNodeElement *node = tab[i];
        while (node)
        {
            if (node->value.state == 1)
            {
                printf("\t|%11s|%11s|%12s|%14s|%9s|%9s|%8s|%7s|\n",
                       node->value.name,
                       node->value.code,
                       node->value.type,
                       node->value.val, 
                       node->value.Is_Vector, 
                       node->value.borne_inf, 
                       node->value.taille,
                       node->value.Is_cst
                    );
            }
            node = node->next;
        }
    }

    printf("\n/*************** Table des symboles mots cles *************/\n");
    printf("_____________________________________\n");
    printf("\t| NomEntite |  CodeEntite | \n");
    printf("_____________________________________\n");
    for (i = 0; i < TABLE_SYMB_SIZE; i++)
    {
        HashNodeElt *node = tabm[i];
        while (node)
        {
            if (node->value.state == 1)
            {
                printf("\t|%10s |%12s |\n",
                       node->value.name,
                       node->value.type);
            }
            node = node->next;
        }
    }

    printf("\n/*************** Table des symboles separateurs *************/\n");
    printf("_____________________________________\n");
    printf("\t| NomEntite |  CodeEntite | \n");
    printf("_____________________________________\n");
    for (i = 0; i < TABLE_SYMB_SIZE; i++)
    {
        HashNodeElt *node = tabs[i];
        while (node)
        {
            if (node->value.state == 1)
            {
                printf("\t|%10s |%12s |\n",
                       node->value.name,
                       node->value.type);
            }
            node = node->next;
        }
    }
}
