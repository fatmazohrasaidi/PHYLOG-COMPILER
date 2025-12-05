#ifndef SEMANTIQUE_H
#define SEMANTIQUE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void div_zero_cst(int cst, int nb_ligne, int nb_col, char fileName[]);
void div_zero_cstfloat(float cst, int nb_ligne, int nb_col, char fileName[]);
void div_zero_idf(char name[], int nb_ligne, int nb_col, char fileName[]);
typedef struct is_same_type {
    char type[10];                 // Type string (e.g., "int", "float")
    int ligne;                     // Line number
    int colonne;                   // Column number
    struct is_same_type* next;     // Pointer to the next element
} Is_same_type;
extern Is_same_type* head_type_list;
extern Is_same_type* tail_type_list;

extern char* tmp_type;  // Declare as a pointer
extern char* tmp_type2; 
extern char* tmp_type3; 


void Comp_Type(int nbligne, int nbcolonne, Is_same_type* list, char* given_type);
void add_type_to_list(char* type, int ligne, int colonne);
// void RESRET_types_list();
void search_value(char name[], float *val, int nb_ligne, int nb_col, char fileName[]);
void add_type(char name[], char type[], int nb_ligne, int nb_col, char fileName[]);
char *cleanFloatValue(float num);
void add_value(char name[], char val[], int nb_ligne, int nb_col, char fileName[]);
char *intToString(int num);
char *floatToString(float num);
void read_value(char name[], char type[], int nb_ligne, int nb_col, char fileName[]);
void display_value(char message[], char name[], int nb_ligne, int nb_col, char fileName[]);
void if_condition(int condition, int nb_ligne, int nb_col, char fileName[]);
void for_loop_control(char variable[], int start, int end, int nb_ligne, int nb_col, char fileName[]);
int is_const(char *idf,int nb_ligne, int nb_col, char fileName[]);
void update_ts_vector(char name[], char Is_Vector[], char borne_inf[], char taille[]  );
void remplir_ts_cst(char name[], char Is_cst[]);
void vector_bound_ck(char name[], int to_ck ,int nb_ligne, int nb_col, char fileName[]);
char* get_idf_type(char idf[]);
// int does_exist(char idf[]);
int last_char_ofstring_isValid(char* string);
void ck_display(char* string, char* type_tocompare, int nb_ligne, int nb_col,char fileName[]);
void ck_read(char* string, char* type_tocompare, int nb_ligne, int nb_col,char fileName[]);
//check if the idf is initialised
int is_idf_int(char idf[]);
void ck_sematique_For1(char idf1[],char idf2[],char idf3[]);
void ck_sematique_For2(char idf1[],char idf3[]);
void ck_sematique_For3(char idf1[]);


#endif // TS_H
