%{
#include <stdio.h>    
#include <string.h>  
#include <wchar.h> 
#include "syntaxique.tab.h"
#include "ts.h"
// #include "quad.h"
#include "optimisation.h"
#include "GenerationCodeObjet.h"
extern YYSTYPE yylval;
extern nb_ligne; // extern because shared with syn.y
extern nb_col ;
extern char fileName[20];
%}
lettreM   [A-Z]
lettre    [a-zA-Z]
chiffre   [0-9]
IDF       {lettreM}({lettre}|{chiffre})*
cstInteger {chiffre}+
cstInteger_signed \([+-]?{chiffre}+\)
cstFloat    {chiffre}+\.{chiffre}+
cstFloat_signed \([+-]?{chiffre}+\.{chiffre}+\)
String \"[^"]*\"
char \'[^\']\'
comment §.*

%%

"DATA" {rechercher(yytext,"Mot cle",0,0, 1,"","","","");  nb_col = nb_col + yyleng; printf("DATA\n"); return mc_data; }
"END" {rechercher(yytext,"Mot cle",0,0, 1,"","","","");  nb_col = nb_col + yyleng;  printf("END\n"); return mc_end; }
"CODE" {rechercher(yytext,"Mot cle",0,0, 1,"","","","");  nb_col = nb_col + yyleng; printf("CODE\n"); return mc_code; }
"VECTOR" {rechercher(yytext,"Mot cle",0,0, 1,"","","",""); nb_col = nb_col + yyleng; printf("VECTOR"); return mc_vector; }
"INTEGER" {rechercher(yytext,"Mot cle",0,0, 1,"","","",""); nb_col = nb_col + yyleng; printf(" INTEGER "); return mc_integer; }
"FLOAT" {rechercher(yytext,"Mot cle",0,0, 1,"","","",""); nb_col = nb_col + yyleng; printf(" FLOAT "); return mc_float; }
"CHAR" {rechercher(yytext,"Mot cle",0,0, 1,"","","",""); nb_col = nb_col + yyleng; printf(" CHAR "); return mc_char; }
"STRING" {rechercher(yytext,"Mot cle",0,0, 1,"","","",""); nb_col = nb_col + yyleng; printf(" STRING "); return mc_string; }
"CONST" {rechercher(yytext,"Mot cle",0,0, 1,"","","",""); nb_col = nb_col + yyleng; printf(" CONST "); return mc_const; }
"AND" {rechercher(yytext,"Mot cle",0,0, 1,"","","",""); nb_col = nb_col + yyleng; printf(" AND "); return mc_and; }
"OR" {rechercher(yytext,"Mot cle",0,0, 1,"","","",""); nb_col = nb_col + yyleng; printf(" OR "); return mc_or; }
"G" {rechercher(yytext,"Mot cle",0,0, 1,"","","",""); nb_col = nb_col + yyleng; printf(" G "); return mc_g; }
"GE" {rechercher(yytext,"Mot cle",0,0, 1,"","","",""); nb_col = nb_col + yyleng; printf(" GE "); return mc_ge; }
"LE" {rechercher(yytext,"Mot cle",0,0, 1,"","","",""); nb_col = nb_col + yyleng; printf(" LE "); return mc_le; }
"L" {rechercher(yytext,"Mot cle",0,0, 1,"","","",""); nb_col = nb_col + yyleng; printf(" L "); return mc_l; }
"DL" {rechercher(yytext,"Mot cle",0,0, 1,"","","",""); nb_col = nb_col + yyleng; printf(" DL "); return mc_dl; }
"EQ" {rechercher(yytext,"Mot cle",0,0, 1,"","","",""); nb_col = nb_col + yyleng; printf(" EQ "); return mc_eq; }
"NOT" {rechercher(yytext,"Mot cle",0,0, 1,"","","",""); nb_col = nb_col + yyleng; printf(" NOT "); return mc_not; }
"IF" {rechercher(yytext,"Mot cle",0,0, 1,"","","",""); nb_col = nb_col + yyleng; printf(" IF "); return mc_if; }
"ELSE" {rechercher(yytext,"Mot cle",0,0, 1,"","","",""); nb_col = nb_col + yyleng; printf(" ELSE "); return mc_else; }
"READ" {rechercher(yytext,"Mot cle",0,0, 1,"","","",""); nb_col = nb_col + yyleng; printf(" READ"); return mc_read; }
"DISPLAY" {rechercher(yytext,"Mot cle",0,0, 1,"","","",""); nb_col = nb_col + yyleng; printf("DISPLAY"); return mc_display; }
"FOR" {rechercher(yytext,"Mot cle",0,0, 1,"","","",""); nb_col = nb_col + yyleng; printf(" FOR "); return mc_for; }
"(" {rechercher(yytext,"sep",0,0,2,"","","",""); nb_col = nb_col + yyleng; printf(" ( "); return mc_parover;}
")" {rechercher(yytext,"sep",0,0,2,"","","",""); nb_col = nb_col + yyleng; printf(" ) "); return mc_parferme; }
"+" {rechercher(yytext,"sep",0,0,2,"","","",""); nb_col = nb_col + yyleng; printf(" + "); return mc_plus; }
"=" {rechercher(yytext,"sep",0,0,2,"","","",""); nb_col = nb_col + yyleng; printf(" = "); return mc_egal; }
"-" {rechercher(yytext,"sep",0,0,2,"","","",""); nb_col = nb_col + yyleng; printf(" - "); return mc_moins; }
"*" {rechercher(yytext,"sep",0,0,2,"","","",""); nb_col = nb_col + yyleng; printf(" * "); return mc_mult; }
"/" {rechercher(yytext,"sep",0,0,2,"","","",""); nb_col = nb_col + yyleng; printf(" / "); return mc_div; }
";" {rechercher(yytext,"sep",0,0,2,"","","",""); nb_col = nb_col + yyleng; printf(" ;\n"); return mc_pointvirgule; }
":" {rechercher(yytext,"sep",0,0,2,"","","",""); nb_col = nb_col + yyleng; printf(" : ");  return mc_deuxpoints; }
"|" {rechercher(yytext,"sep",0,0,2,"","","",""); nb_col = nb_col + yyleng; printf(" | "); return mc_pipe; }
"." {rechercher(yytext,"sep",0,0,2,"","","",""); nb_col = nb_col + yyleng; printf(" . "); return mc_dot; }
"[" {rechercher(yytext,"sep",0,0,2,"","","",""); nb_col = nb_col + yyleng; printf(" [ "); return mc_crochetouv; }
"]" {rechercher(yytext,"sep",0,0,2,"","","",""); nb_col = nb_col + yyleng; printf(" ] "); return mc_crochetferme; }
"," {rechercher(yytext,"sep",0,0,2,"","","",""); nb_col = nb_col + yyleng; printf(" , "); return mc_vir; }
"@" {rechercher(yytext,"sep",0,0,2,"","","",""); nb_col = nb_col + yyleng; printf(" @ "); return mc_arobase; }
{IDF} {
    if (yyleng>8) printf ("file: %s, ligne %d, colonne: %d,  WARNING : IDF trop long\n ",fileName ,nb_ligne, nb_col);
    else {
        yylval.str=strdup(yytext); 
	rechercher(yytext,"idf"," ","",0,"","","",""); 
        nb_col= nb_col + strlen(yytext); 
        printf(" %s ",yytext);
    }
    return idf;
}

{cstInteger} { 
    if((atoi(yytext) >= -32768 && atoi(yytext) <= 32767) || (atof(yytext) >= -32768.32768 && atof(yytext) <= 32767.32767)) {
        yylval.entier=atoi(yytext);
        rechercher(yytext,"INTEGER","",intToString(atoi(yytext)),0,"","","",""); 
        nb_col= nb_col + strlen(yytext); 
        printf(" %s ",yytext);
    }
    else printf("file: %s, ligne %d, colonne: %d, Erreur Lexical: Const non valide\n", fileName, nb_ligne, nb_col);
    return cst_integer;
}

{cstInteger_signed} {
    /* Remove parentheses and convert to integer */
    char *temp = strdup(yytext + 1);
    temp[strlen(temp) - 1] = '\0';
    
    /* Check if the value is within valid range */
    if((atoi(temp) >= -32768 && atoi(temp) <= 32767) || (atof(temp) >= -32768.32768 && atof(temp) <= 32767.32767)) {
        yylval.entier = atoi(temp);
        rechercher(temp, "INTEGER", "", intToString(atoi(temp)), 0,"","","","");
        nb_col += strlen(yytext);
        printf(" %s ", temp);
    }
    else {
        printf("file: %s, ligne %d, colonne: %d, Erreur Lexical: Const non valide\n", fileName, nb_ligne, nb_col);
    }

    free(temp);
    return cst_integer_signed;
}

{cstFloat} { 
    if((atof(yytext) >= -32768.0 && atof(yytext) <= 32767.0) || (atof(yytext) >= -32768.32768 && atof(yytext) <= 32767.32767)) {
        yylval.reel = atof(yytext); // Converts string to float/// it was yylval.entier=atoi(yytext);
        rechercher(yytext,"FLOAT","",floatToString(atof(yytext)),0,"","","",""); 
        nb_col= nb_col + strlen(yytext); 
        printf(" %s ",yytext);
    }
    else printf("file: %s, ligne %d, colonne: %d, Erreur Lexical: Const non valide\n", fileName, nb_ligne, nb_col);
    return cst_float;
}

{cstFloat_signed} {
    /* Remove parentheses and convert to float */
    char *temp = strdup(yytext + 1);
    temp[strlen(temp) - 1] = '\0';
    
    /* Check if the value is within valid range */
    if((atof(temp) >= -32768.0 && atof(temp) <= 32767.0) || (atof(temp) >= -32768.32768 && atof(temp) <= 32767.32767)) {
        yylval.reel = atof(temp);  // Converts string to float
        rechercher(temp, "FLOAT", "", floatToString(atof(temp)), 0,"","","","");
        nb_col += strlen(yytext);
        printf(" %s ", temp);
    }
    else {
        printf("file: %s, ligne %d, colonne: %d, Erreur Lexical: Const non valide\n", fileName, nb_ligne, nb_col);
    }

    free(temp);
    return cst_float_signed;
}

{String} {
    yylval.str=strdup(yytext); 
	rechercher(yytext,"","STR","",0,"","","",""); 
    nb_col= nb_col + strlen(yytext); 
    printf(" %s ",yytext);
    return string;
}
{char} {
    yylval.str=strdup(yytext); 
	rechercher(yytext,"","CHAR","",0,"","","",""); 
    nb_col= nb_col + strlen(yytext); 
    printf(" %s ",yytext);
    return cst_char ;
}
{comment} {
    //yylval.str=strdup(yytext); 
    nb_col= nb_col + strlen(yytext);
    //ECHO;  
}

[ \t] {
    
    nb_col=nb_col + strlen(yytext);  
    ECHO;
}

\n {
    nb_col= 1; nb_ligne++; 
    ECHO;
}
. { 
    printf ("Ligne %d | Colonne %d| lexical error: entite non reconnue \n", nb_ligne,nb_col);
    return erreur;
    ECHO;
}//lepoint veut if lfo9 mkch il ya une erreur lexical

%%


