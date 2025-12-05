#ifndef QUAD_H
#define QUAD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
typedef struct
{
    char oper[1000];
    char op1[2000];
    char op2[2000];
    char res[2000];

} qdr;

qdr quad[1000];
extern int qc;
extern float oper, resultat;
float oper3 = 1;
char oper1[60], oper2[60], str[60], str1[60], str2[60], str3[30], chaine[30];
char t[20], tmp[70], sauvTab[30], tmp1[20], tmp2[20], tmp3[20], chainee[20];
int T = 0;

/*****************************************************************************************************************/
#define max_val 30

typedef struct cell5 *stack; // pointeur smato stack
typedef struct cell5
{
    char info[max_val];
    stack suiv;
} py;

stack quadruplet = NULL;

stack sauv_deb_FOR = NULL;//this will save les position de debut de la boucle FOR, the reson why its a list is because sometimes its a for inside for....

int isEmpty(stack s)
{
    return s == NULL;
}

const char *peek(stack s)
{
    if (!isEmpty(s))
    {
        return s->info;
    }
    else
    {
        printf("La pile est vide\n");
        exit(EXIT_FAILURE);
    }
}

stack pushin(stack *tete, char info[])
{
    stack p = malloc(sizeof(py));
    if (p == NULL)
    {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    strcpy(p->info, info);
    p->suiv = *tete;
    *tete = p;

    return *tete;
}

char *popping(stack *tete)
{
    if (*tete == NULL)
    {
        printf("Underflow: Cannot pop from an empty stack.\n");
        return NULL;
    }

    stack p = *tete;
    *tete = (*tete)->suiv;

    // Allocate memory for the chaine to be returned
    char *c = malloc(strlen(p->info) + 1);
    if (c == NULL)
    {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    strcpy(c, p->info);
    free(p);

    return c;
}

void display_LIFO(stack p)
{
    while (p != NULL)
    {
        printf("%s   ", p->info);
        p = p->suiv;
    }
}
/****************************************************************************************************/
void quadr(char opr[], char op1[], char op2[], char res[])
{
    strcpy(quad[qc].oper, opr);
    strcpy(quad[qc].op1, op1);
    strcpy(quad[qc].op2, op2);
    strcpy(quad[qc].res, res);

    qc++;
}

void ajour_quad(int num_quad, int colon_quad, char val[])
{
    if (colon_quad == 0)
    {
        strcpy(quad[num_quad].oper, val);
    }
    else if (colon_quad == 1)
    {
        strcpy(quad[num_quad].op1, val);
    }
    else if (colon_quad == 2)
    {
        strcpy(quad[num_quad].op2, val);
    }
    else if (colon_quad == 3)
    {
        strcpy(quad[num_quad].res, val);
    }
}
void afficher_qdr()
{
    printf("*********************LES QUADRUPLETS*************************\n");
    int i;
    for (i = 0; i < qc; i++)
    {
        printf("\n %d-( %s ,%s , %s , %s )\n", i, quad[i].oper, quad[i].op1, quad[i].op2, quad[i].res);
        printf("-------------------------------------------------------------\n");
    }
}
/**********************************************************************************************************/
bool isFloat(const char *str)
{
    char *endptr;
    strtod(str, &endptr);

    // Check if the entire chaine was consumed during the conversion
    return *endptr == '\0';
}





//------------ 
void emptyStack(stack *p)
{
    stack tmp;
    while (*p != NULL)
    {
        tmp = *p;
        *p = (*p)->suiv;
        free(tmp);
    }
}
void emptyStack_after_pv()
{
    emptyStack(&quadruplet);
}

//-----------------------
void R1(char valeur[])
{
    pushin(&quadruplet, valeur);
}
char *Rpeek()
{
    strcpy(tmp, popping(&quadruplet));
    if (!(isFloat(tmp)))
    {
        popping(&quadruplet);
    }
    return tmp;
}
char *Rpop()
{
    strcpy(tmp, popping(&quadruplet));
    return tmp;
}
char *quadcond(char oper[30])
{
    strcpy(str1, popping(&quadruplet));
    strcpy(chaine, oper);
    strcpy(str2, popping(&quadruplet));
    quadr(chaine, str1, str2, "vide");
    T++;
    sprintf(str3, "%d", T);
    strcpy(chaine, "T");
    strcat(chaine, str3);
    strcpy(tmp, chaine);
    R1(tmp);
    return tmp;
}
char *R2(char idf[30], char cst1[30], char valeur[])
{
    strcpy(sauvTab, idf);
    strcat(sauvTab, "(");
    strcat(sauvTab, tmp1);
    strcat(sauvTab, ")");
    quadr("BOUNDS", tmp1, tmp2, "vide");
    return sauvTab;
}
//this one is same as R2 but it just doesn't add a quadrpuplet to the quad table, cuz we need to return tab(5) but not add a new quad
char *R2_no_add_quad(char idf[30], char cst1[30], char valeur[])
{
    strcpy(sauvTab, idf);
    strcat(sauvTab, "(");
    strcat(sauvTab, tmp1);
    strcat(sauvTab, ")");
    return sauvTab;
}
////



char *R7(char idf[30], char cst1, char cst2, char valeur[])
{
    strcpy(sauvTab, idf);
    strcat(sauvTab, "(");
    strcat(sauvTab, tmp1);
    strcat(sauvTab, ",");
    strcat(sauvTab, tmp2);
    strcat(sauvTab, ")");
    quadr("BOUNDS", tmp1, tmp2, "vide");
    return sauvTab;
}
char *R6()
{
    resultat = 0;
    strcpy(str1, popping(&quadruplet));

    if (isFloat(str1))
    {
        oper = atof(str1);
        resultat = resultat + oper;
    }
    else
    {
        oper = atof(popping(&quadruplet));
        resultat = resultat + oper;
    }
    strcpy(str2, popping(&quadruplet));
    if (isFloat(str2))
    {
        oper = atof(str2);
        resultat = resultat + oper;
    }
    else
    {
        oper = atof(popping(&quadruplet));
        resultat = resultat + oper;
    }
    // function to compare types
    T++;
    sprintf(str3, "%d", T);
    strcpy(chaine, "T");
    strcat(chaine, str3);
    quadr("+", str1, str2, chaine);
    strcpy(tmp, chaine);
    R1(tmp);
    return tmp;
}
char *R5()
{
    resultat = 0;
    strcpy(str1, popping(&quadruplet));

    if (isFloat(str1))
    {
        oper = atof(str1);
        resultat = resultat - (oper);
    }
    else
    { // idf
        oper = atof(popping(&quadruplet));
        resultat = resultat - (oper);
    }
    strcpy(str2, popping(&quadruplet));
    if (isFloat(str2))
    {
        oper = atof(str2);
        resultat = resultat - (oper);
    }
    else
    { // idf
        oper = atof(popping(&quadruplet));
        resultat = resultat - (oper);
    }
    // function to compare types
    T++;
    sprintf(str3, "%d", T);
    strcpy(chaine, "T");
    strcat(chaine, str3);
    quadr("-", str1, str2, chaine);
    strcpy(tmp, chaine);
    R1(tmp);
    return tmp;
}
char *R4()
{
    resultat = 1;
    strcpy(str1, popping(&quadruplet));
    if (isFloat(str1))
    {

        oper3 = atof(str1);
        resultat = resultat * oper3;
    }
    else
    { // idf
        oper3 = atof(popping(&quadruplet));
        resultat = resultat * oper3;
    }
    strcpy(str2, popping(&quadruplet));
    if (isFloat(str2))
    {
        oper3 = atof(str2);
        resultat = resultat * oper3;
    }
    else
    { // idf

        oper3 = atof(popping(&quadruplet));
        resultat = resultat * oper3;
    }
    // function to compare types
    T++;
    sprintf(str3, "%d", T);
    strcpy(chaine, "T");
    strcat(chaine, str3);
    quadr("*", str1, str2, chaine);
    strcpy(tmp, chaine);
    R1(tmp);
    return tmp;
}
char *R3(char t[])
{
    resultat = 1;
    strcpy(str1, t);

    if (isFloat(str1))
    {
        oper = atof(str1);
        resultat = oper / resultat;
    }
    else
    { // idf
        oper = atof(popping(&quadruplet));
        resultat = oper / resultat;
    }
    strcpy(str2, popping(&quadruplet));

    if (isFloat(str2))
    {
        oper = atof(str2);
        resultat = oper / resultat;
    }
    else
    { // idf

        oper = atof(popping(&quadruplet));
        resultat = oper / resultat;
    }
    // function to compare types
    T++;
    sprintf(str3, "%d", T);
    strcpy(chaine, "T");
    strcat(chaine, str3);
    quadr("/", str1, str2, chaine);
    strcpy(tmp, chaine);
    R1(tmp);
    return tmp;
}

/****************************************************************************************************************/
//this function is used to save the number qc of the debut of debut od la boucle for
void routine_FOR(int qc)
{
    //valeur must be a string
    char valeur[5];
    sprintf(valeur, "%d", qc);
    pushin(&sauv_deb_FOR, valeur);
    // display_LIFO(sauv_deb_FOR);
}

int pop_qc_debut()
{
    char *popped_value = popping(&sauv_deb_FOR);
    // if (popped_value == NULL)
    // {
    //     printf("Cannot pop: Stack is empty.\n");
    //     return -1; // or handle error
    // }

    int tampo_int = atoi(popped_value); // Use atoi() to convert string to int

    free(popped_value); // FREE the memory allocated in popping()

    return tampo_int;
}
void update_cpt_FOR(char idf1[],char idf2[])
{
    T++;
    sprintf(str3, "%d", T);
    strcpy(chaine, "T");
    strcat(chaine, str3);
    quadr("+", idf1, idf2, chaine);//should be saved in a temp here!!!!!
    quadr("=", chaine,"vide", idf1);
}

#endif // QUAD_H
