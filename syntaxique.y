%{
/* C declarations and headers */
    #include <stdio.h>    
    #include <string.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <ctype.h>
    #include "semantique.h"

    int nb_ligne=1, nb_col=1;
    float resultat;
    float result;
    FILE  *file; 
    int par1;
    char fileName[20];
    extern char ch;
    char type[20];
    extern char sauv_type [30];
    int Fin_if=0,deb_else=0,qc=0,sauv_BZ=0,sauv_BR=0,affect,oper,write,i=0,j=0;
    extern char tmp [90],t[20],sauvTab[30],tmp1 [20],tmp2[20],tmp3[20];
    extern char* tmp_type;
    extern char* tmp_type2;
    extern char* tmp_type3;
    // extern qdr* quad[1000];

%}
%union { 
int   entier;
char*  str;
char car;
float reel;
}
/* Token declarations */
%token <str>mc_data <str>mc_end <str>mc_code <str>mc_vector <str>mc_integer <str>mc_float <str>mc_char <str>mc_string <str>mc_const <str>mc_and <str>mc_or <str>mc_g <str>mc_ge <str>mc_le <str>mc_l <str>mc_dl <str>mc_eq <str>mc_not <str>mc_crochetouv <str>mc_crochetferme <str>mc_vir <str>mc_arobase
%token <str>mc_if <str>mc_else <str>mc_read <str>mc_display <str>mc_for <str>mc_parover <str>mc_parferme <str>mc_plus <str>mc_egal <str>mc_moins <str>mc_mult <str>mc_div <str>mc_pointvirgule <str>cst_char 
%token <str>mc_deuxpoints mc_pipe  mc_dot <str>idf <entier>cst_integer <reel>cst_float <str>string <reel>cst_float_signed <entier>cst_integer_signed erreur

%start S
%%

/* Grammar rules and actions */
S: idf DATA CODE  mc_end  
{
    printf("Syntaxe correct\n");
}
;
/************************************************************************************************************/
DATA: mc_data DECLARATION mc_end 
;
CODE: mc_code List_inst mc_end   
;
/***********************************************************************************************/
DECLARATION: DECLARATION SUITE_DECLARATION
| SUITE_DECLARATION
;

SUITE_DECLARATION: mc_integer mc_deuxpoints VAR_LISTCSTINTEGER mc_pointvirgule
| mc_float mc_deuxpoints VAR_LISTCSTFLOAT mc_pointvirgule
| mc_char mc_deuxpoints VAR_LISTCHAR mc_pointvirgule
| mc_string mc_deuxpoints VAR_LISTSTRING mc_pointvirgule
| mc_const mc_deuxpoints CONST_DECL mc_pointvirgule {}
| mc_vector mc_deuxpoints VECTOR_LIST mc_pointvirgule
;

VAR_LISTCSTINTEGER: idf { add_type($1,"INTEGER", nb_ligne, nb_col, fileName); }
| VAR_LISTCSTINTEGER mc_pipe idf {add_type($3,"INTEGER", nb_ligne, nb_col, fileName);}
;

VAR_LISTCSTFLOAT: idf { add_type($1,"FLOAT", nb_ligne, nb_col, fileName);}
| VAR_LISTCSTFLOAT mc_pipe idf {add_type($3,"FLOAT", nb_ligne, nb_col, fileName);}
;

VAR_LISTCHAR: idf {add_type($1,"CHAR", nb_ligne, nb_col, fileName);}
| VAR_LISTCHAR mc_pipe idf {add_type($3,"CHAR", nb_ligne, nb_col, fileName);}
;

VAR_LISTSTRING: idf {add_type($1,"STR", nb_ligne, nb_col, fileName);}
| VAR_LISTSTRING mc_pipe idf {add_type($3,"STR", nb_ligne, nb_col, fileName);}
;

CONST_DECL: idf mc_egal cst_integer {add_type($1,"INTEGER", nb_ligne, nb_col, fileName);
                                    sprintf(tmp, "%d", $3);
                                    add_value($1, tmp, nb_ligne, nb_col, fileName);
                                    remplir_ts_cst($1, "1");
                                    }
|idf mc_egal cst_integer_signed {
                                    add_type($1,"INTEGER", nb_ligne, nb_col, fileName);
                                    sprintf(tmp, "%d", $3);
                                    add_value($1, tmp, nb_ligne, nb_col, fileName);
                                    remplir_ts_cst($1, "1");
                                }

| idf mc_egal cst_float {
                        add_type($1,"FLOAT", nb_ligne, nb_col, fileName);
                        sprintf(tmp, "%f", $3);
                        add_value($1, tmp, nb_ligne, nb_col, fileName);
                        remplir_ts_cst($1, "1");
                        }
| idf mc_egal cst_float_signed {
                        add_type($1,"FLOAT", nb_ligne, nb_col, fileName);
                        sprintf(tmp, "%f", $3);
                        add_value($1, tmp, nb_ligne, nb_col, fileName);
                        remplir_ts_cst($1, "1");
                        }

| idf mc_egal string {
                        add_type($1,"STR", nb_ligne, nb_col, fileName);
                        add_value($1, $3, nb_ligne, nb_col, fileName);
                        remplir_ts_cst($1, "1");
                        }

|idf mc_egal cst_char {add_type($1,"CHAR", nb_ligne, nb_col, fileName);
add_value($1, $3, nb_ligne, nb_col, fileName);}
;

/* New rules for vector continuation */
VECTOR_LIST: SINGLE_VECTOR
| VECTOR_LIST mc_pipe SINGLE_VECTOR
;

SINGLE_VECTOR: idf mc_crochetouv cst_integer mc_vir cst_integer mc_deuxpoints mc_integer mc_crochetferme 
                                        {strcpy(tmp1,tmp); 
                                        sprintf(tmp1, "%d", $3);
                                        sprintf(tmp2, "%d", $5);
                                        R2($1,tmp1);
                                        quadr("ADEC",$1,"vide","vide"); 
                                        add_type($1,"INTEGER", nb_ligne, nb_col, fileName); 
                                        
                                        update_ts_vector($1, "1", tmp1, tmp2);
                                        }
| idf mc_crochetouv cst_integer mc_vir cst_integer mc_deuxpoints mc_float mc_crochetferme 
                                        {strcpy(tmp1,tmp); 
                                        sprintf(tmp1, "%d", $3);
                                        sprintf(tmp2, "%d", $5);
                                        R2($1,tmp1); 
                                        quadr("ADEC",$1,"vide","vide"); 
                                        add_type($1,"FLOAT", nb_ligne, nb_col, fileName);
                                        
                                        update_ts_vector($1, "1", tmp1, tmp2);
                                        }
| idf mc_crochetouv cst_integer mc_vir cst_integer mc_deuxpoints mc_string mc_crochetferme 
                                        {strcpy(tmp1,tmp); 
                                        sprintf(tmp1, "%d", $3);
                                        sprintf(tmp2, "%d", $5);
                                        R2($1,tmp1); 
                                        quadr("ADEC",$1,"vide","vide"); 
                                        add_type($1,"STR", nb_ligne, nb_col, fileName); 
                                    
                                        update_ts_vector($1, "1", tmp1, tmp2);
                                        }
| idf mc_crochetouv cst_integer mc_vir cst_integer mc_deuxpoints mc_char mc_crochetferme 
                                        {strcpy(tmp1,tmp);
                                        sprintf(tmp1, "%d", $3);
                                        sprintf(tmp2, "%d", $5); 
                                        R2($1,tmp1); 
                                        quadr("ADEC",$1,"vide","vide"); 
                                        add_type($1,"CHAR", nb_ligne, nb_col, fileName);
                                        
                                        update_ts_vector($1, "1", tmp1, tmp2);
                                        }
| idf mc_crochetouv cst_integer mc_vir cst_integer mc_deuxpoints mc_const mc_crochetferme 
                                        {strcpy(tmp1,tmp); 
                                        sprintf(tmp1, "%d", $3);
                                        sprintf(tmp2, "%d", $5);
                                        R2($1,tmp1); 
                                        quadr("ADEC",$1,"vide","vide"); 
                                        add_type($1,"CONST", nb_ligne, nb_col, fileName);
                                      
                                        update_ts_vector($1, "1", tmp1, tmp2);
                                        }
;


/*****************************************************LES INSTRUCTIONS*******************************************************************/
List_inst:List_inst INST
|INST
;
INST: AFFECTATION
| IFSTATEMENT 
| READ
| DISPLAY  
| FORSTATEMENT
;
AFFECTATION:idf mc_egal EXP mc_pointvirgule {
                                            if (is_const($1, nb_ligne, nb_col, fileName)==0) 
                                            {add_value($1, tmp, nb_ligne, nb_col, fileName);} 
                                            affect=qc; quadr("=",tmp,"vide",$1);  
                                            ajour_quad(affect,1,tmp); 

                                            // comp_type($1,type, nb_ligne, nb_col, fileName);
                                            //------- this part is about compatibilite de type
                                            
                                            if (get_idf_type($1) != NULL)
                                            Comp_Type(nb_ligne, nb_col, head_type_list, get_idf_type($1));
                                            //------- this part is about compatibilite de type
                                            //WE NEED TO EMPTY THE STACK BEFORE THE NEXT OPERATION
                                            // emptyStack_after_pv();
                                            } 
|idf  mc_egal string mc_pointvirgule { if (is_const($1, nb_ligne, nb_col, fileName)==0) 
                                            {add_value($1, tmp, nb_ligne, nb_col, fileName);}     
                                        quadr("=",$3,"vide",$1);     
                                        // comp_type($1,"STR", nb_ligne, nb_col, fileName); 

                                        //------- this part is about compatibilite de type
                                        add_type_to_list("STR",nb_ligne, nb_col);
                                        if (get_idf_type($1) != NULL)
                                        Comp_Type(nb_ligne, nb_col, head_type_list, get_idf_type($1));
                                        //------- this part is about compatibilite de type

                                        add_value($1, $3, nb_ligne,nb_col, fileName); 
                                        }
| idf  mc_egal cst_char  mc_pointvirgule  {if ( is_const($1, nb_ligne, nb_col, fileName)==0) 
                                            {add_value($1, tmp, nb_ligne, nb_col, fileName);}
                                        quadr("=",$3,"vide",$1);    
                                        // comp_type($1,"CHAR", nb_ligne, nb_col, fileName); 

                                        //------- this part is about compatibilite de type
                                        add_type_to_list("CHAR",nb_ligne, nb_col);
                                        if (get_idf_type($1) != NULL)
                                        Comp_Type(nb_ligne, nb_col, head_type_list, get_idf_type($1));
                                        //------- this part is about compatibilite de type

                                        add_value($1, $3, nb_ligne, nb_col, fileName); 
                                        free(tmp); 
                                        }
| idf mc_crochetouv cst_integer  mc_crochetferme  mc_egal EXP mc_pointvirgule 
                                    { sprintf(tmp1,"%d",$3); 
                                    R2_no_add_quad ($1,tmp1);//this will return sauvtab=tabname()... changed it to another function cuz R2 adds an extra quadruplet which we don't need in the affectation phase
                                    affect=qc;
                                    quadr("=",tmp,"vide",sauvTab);  
                                    ajour_quad(affect,3,sauvTab);  
                                    ajour_quad(affect,1,tmp); 
                                    sprintf(tmp2, "%s", $1);
                                    add_value(tmp2, tmp, nb_ligne, nb_col, fileName);   
                                    // comp_type(tmp2,type, nb_ligne, nb_col, fileName);

                                    //------- this part is about compatibilite de type
                                    if (get_idf_type($1) != NULL)
                                    Comp_Type(nb_ligne, nb_col, head_type_list, get_idf_type($1));
                                    //------- this part is about compatibilite de type


                                    sprintf(tmp1, "%d", $3);
                                    vector_bound_ck($1, $3,nb_ligne, nb_col, fileName); 

                                    //WE NEED TO EMPTY THE STACK BEFORE THE NEXT OPERATION
                                    // emptyStack_after_pv();

                                    }          
| idf mc_crochetouv cst_integer  mc_crochetferme mc_egal string mc_pointvirgule {
                                                                                // strcpy(sauvTab, $1);
                                                                                // printf("\n**********%s\n", sauvTab);
                                                                                add_value(sauvTab, $6, nb_ligne, nb_col, fileName); 
                                                                                strcpy(tmp2,tmp); 
                                                                                Rpop(); 
                                                                                strcpy(tmp1,tmp);   
                                                                                R7($1,tmp1,tmp2); affect=qc; 
                                                                                quadr("=",$6,"vide",sauvTab);  
                                                                                ajour_quad(affect,3,sauvTab);  
                                                                                // comp_type(sauvTab,"STR", nb_ligne, nb_col, fileName); 

                                                                                //------- this part is about compatibilite de type
                                                                                add_type_to_list("STRING",nb_ligne, nb_col);
                                                                                if (get_idf_type($1) != NULL)
                                                                                Comp_Type(nb_ligne, nb_col, head_type_list, get_idf_type($1));
                                                                                //------- this part is about compatibilite de type

                                                                                vector_bound_ck($1, $3,nb_ligne, nb_col, fileName);
                                                                                }
|idf mc_crochetouv cst_integer  mc_crochetferme mc_egal cst_char mc_pointvirgule {add_value(sauvTab, $6, nb_ligne, nb_col, fileName); 
                                                                                strcpy(tmp2,tmp); 
                                                                                Rpop(); 
                                                                                strcpy(tmp1,tmp);   
                                                                                R7($1,tmp1,tmp2); affect=qc; 
                                                                                quadr("=",$6,"vide",sauvTab);  
                                                                                ajour_quad(affect,3,sauvTab);  
                                                                                // comp_type(sauvTab,"CHAR", nb_ligne, nb_col, fileName);

                                                                                //------- this part is about compatibilite de type
                                                                                add_type_to_list("CHAR",nb_ligne, nb_col);
                                                                                if (get_idf_type($1) != NULL)
                                                                                Comp_Type(nb_ligne, nb_col, head_type_list, get_idf_type($1));
                                                                                //------- this part is about compatibilite de type

                                                                                vector_bound_ck($1, $3,nb_ligne, nb_col, fileName);
                                                                                }
;

READ:mc_read READSUITE mc_pointvirgule 
;
READSUITE:mc_parover  string   mc_deuxpoints mc_arobase idf mc_parferme {
                                                                            if (get_idf_type($5) != NULL)
                                                                            ck_read($2, get_idf_type($5), nb_ligne,nb_col,fileName);
                                                                        }
;
DISPLAY:mc_display MESSAGE mc_pointvirgule
;
MESSAGE : mc_parover string   mc_deuxpoints  idf mc_parferme {
                                                                if (get_idf_type($4) != NULL)
                                                                ck_display($2, get_idf_type($4), nb_ligne, nb_col,fileName);
                                                            }
;
/*****************************************************LES BOUCLES*******************************************************************/
FORSTATEMENT: FOR_BEGIN  List_inst  mc_end {
                                            int updating=pop_qc_debut();
                                            sprintf(tmp2, "%d",updating );
                                            quadr("BR",tmp2,"vide","vide");
                                            //ajour the num of the quad deb here 
                                            sprintf(tmp3, "%d",qc );
                                            ajour_quad(updating, 1, tmp3);
                                            }
;

FOR_BEGIN:mc_for mc_parover idf mc_deuxpoints idf mc_deuxpoints idf mc_parferme
                                                            {
                                                                ck_sematique_For1($3,$5,$7); //the case where: FOR(A:B:C), b<c
                                                                quadr("BG","go_end",$3,$7);
                                                                routine_FOR(qc-1);//this is used to save qc de debut de for
                                                                //now change the value of the $3 in the next iteration .. add le pas ($5) to $3
                                                                update_cpt_FOR($3,$5);
                                                            }
|mc_for mc_parover idf mc_deuxpoints cst_integer mc_deuxpoints idf mc_parferme 
                                                            {
                                                                ck_sematique_For2($3,$7);//the case where: FOR(A:5:C)
                                                                quadr("BG","go_end",$3,$7);
                                                                routine_FOR(qc-1);
                                                                //now change the value of the $3 in the next iteration .. add le pas ($5) to $3
                                                                sprintf(tmp3, "%d", $5);
                                                                update_cpt_FOR($3,tmp3);
                                                            }
|mc_for mc_parover idf mc_deuxpoints cst_integer mc_deuxpoints cst_integer mc_parferme
                                                            {
                                                                ck_sematique_For3($3);//the case where: FOR(A:5:7)
                                                                sprintf(tmp2, "%d", $7);
                                                                quadr("BG","go_end",$3,tmp2);
                                                                routine_FOR(qc-1);
                                                                //now change the value of the $3 in the next iteration .. add le pas ($5) to $3
                                                                sprintf(tmp3, "%d", $5);
                                                                update_cpt_FOR($3,tmp3);
                                                            }
|mc_for mc_parover idf mc_deuxpoints idf mc_deuxpoints cst_integer mc_parferme
                                                            {
                                                                ck_sematique_For2($3,$5);//the case where: FOR(A:B:7)
                                                                sprintf(tmp2, "%d", $7);
                                                                quadr("BG","go_end",$3,tmp2);
                                                                routine_FOR(qc-1);
                                                                //now change the value of the $3 in the next iteration .. add le pas ($5) to $3
                                                                update_cpt_FOR($3,$5);
                                                            }
;
/*****************************************************IF - ELSE *******************************************************************/
IFSTATEMENT: IFSTAT1 mc_else mc_deuxpoints List_inst  mc_end { sprintf(tmp,"%d",qc);  ajour_quad(Fin_if,1,tmp);}
|IFSTAT2 List_inst mc_end {sprintf(tmp,"%d",qc);  ajour_quad(deb_else,1,tmp);}
;
IFSTAT1:IFSTAT2 List_inst    {  Fin_if=qc; quadr("BR"," ","vide", "vide"); sprintf(tmp,"%d",qc); ajour_quad(deb_else,1,tmp); }
;
IFSTAT2:mc_if mc_parover COND mc_parferme mc_deuxpoints  { deb_else=qc; quadr("BZ","","temp_cond", "vide"); } 
;
/***************************************************EXPRESSION******************************************************/
EXP:  EXP mc_moins EXP1 { R5(); } 
| EXP mc_plus EXP1 { R6(); }
| EXP1 
;
EXP1: EXP1 mc_mult EXP2 { R4(); }
| EXP1 mc_div mc_parover EXP mc_parferme  
| EXP1 mc_div idf {
                div_zero_idf($3, nb_ligne, nb_col, fileName);  R3($3); 
                tmp_type = get_idf_type($3);
                if (get_idf_type($3) != NULL)
                add_type_to_list(tmp_type, nb_ligne, nb_col);
                }   
| EXP1 mc_div cst_integer {
                div_zero_cst($3, nb_ligne, nb_col, fileName);  sprintf(t,"%d",$3);   R3(t); 
                add_type_to_list("INTEGER", nb_ligne, nb_col);
                } 
| EXP1 mc_div cst_float 
            {div_zero_cstfloat($3, nb_ligne, nb_col, fileName); sprintf(t,"%f",$3);   R3(t);
            add_type_to_list("FLOAT", nb_ligne, nb_col);
            }
//F HAD LE CAS WIN RANA NDIRO DIVISION SUR 0 AVEC LES TABLEAUX IL N'EST PAS ENTRAIN DE DECLARER L'EXISTANCE DE L'ERREUR SEMANTIQUE
| EXP1 mc_div  idf mc_crochetouv cst_integer mc_crochetferme 
                    {Rpop(); strcpy(tmp2,tmp); Rpop(); strcpy(tmp1,tmp);R7($3,tmp1,tmp2); 
                    search_value(sauvTab,&result,nb_ligne, nb_col, fileName); sprintf(tmp,"%.2f",result); 
                    div_zero_idf(tmp, nb_ligne, nb_col, fileName); R3(sauvTab); 

                    tmp_type = get_idf_type($3);
                    if (get_idf_type($3) != NULL)
                    add_type_to_list(tmp_type, nb_ligne, nb_col);
                    }
| EXP2  
;
EXP2: mc_parover EXP mc_parferme 
| idf       { search_value($1,&result,nb_ligne, nb_col, fileName); 
            sprintf(tmp,"%.2f",result); R1(tmp);    
            strcpy(tmp,$1);  R1(tmp);  
            tmp_type = get_idf_type($1);
            if (tmp_type != NULL)  strcpy(type, tmp_type);
            else  strcpy(type, ""); 
            add_type_to_list(tmp_type, nb_ligne, nb_col); 
            }
| cst_integer{ sprintf(tmp,"%d",$1); strcpy(type, "INTEGER");  R1(tmp); 
                 add_type_to_list("INTEGER", nb_ligne, nb_col);
                }
| cst_integer_signed{ sprintf(tmp,"%d",$1); strcpy(type, "INTEGER");  R1(tmp); 
                 add_type_to_list("INTEGER", nb_ligne, nb_col);
                    }
| cst_float { // les () should be elimiated before processing
    char * cleanedtmp = cleanFloatValue($1); // Convert and clean float value
    //printf("TMP after cleaning: %s\n", tmp); // Fix printf format
    strcpy(tmp, cleanedtmp);
    strcpy(type, "FLOAT");
    R1(tmp);
    add_type_to_list("FLOAT", nb_ligne, nb_col);
}
| cst_float_signed { // les () should be elimiated before processing
    char * cleanedtmp = cleanFloatValue($1); // Convert and clean float value
    //printf("TMP after cleaning: %s\n", tmp); // Fix printf format
    strcpy(tmp, cleanedtmp);
    strcpy(type, "FLOAT");
    R1(tmp);
    add_type_to_list("FLOAT", nb_ligne, nb_col);

}
|idf mc_crochetouv cst_integer mc_crochetferme 
                                {
                                    if (get_idf_type($1) != NULL)
                                    add_type_to_list(get_idf_type($1), nb_ligne, nb_col);
                                    vector_bound_ck($1, $3,nb_ligne, nb_col, fileName); 
                                }
//{Rpop(); strcpy(tmp2,tmp); Rpop(); strcpy(tmp1,tmp); R7($1,tmp1,tmp2); search_value(sauvTab,&result,nb_ligne, nb_col, fileName); sprintf(tmp,"%.2f",result); R1(tmp); strcpy(tmp,sauvTab); R1(tmp);  }
;
/***************************************************CONDITION****************************************************/

COND: COND  mc_or  COND_1   {quadcond("OR");  } 
| COND_1
;
COND_1:  COND_1  mc_and  COND_2{quadcond("AND");  } 
| COND_2
;
COND_2:   EXPR_BOOL
| COND3
;
COND3 : mc_not EXPR_BOOL {quadcond("NOR");  } 
|EXPR_BOOL  mc_dot mc_ge   mc_dot EXPR_BOOL{quadcond("BG");  } 
| EXPR_BOOL  mc_dot mc_l  mc_dot EXPR_BOOL  {quadcond("BL");  } 
|EXPR_BOOL  mc_dot mc_g mc_dot EXPR_BOOL       {quadcond("BGE");  } 
|EXPR_BOOL  mc_dot mc_le   mc_dot EXPR_BOOL    {quadcond("BLE");  } 
|EXPR_BOOL  mc_dot mc_eq   mc_dot EXPR_BOOL     {quadcond("BE");  } 
|EXPR_BOOL  mc_dot mc_dl  mc_dot EXPR_BOOL      {quadcond("BNE");  } 
;
EXPR_BOOL : EXP    
| mc_parover COND  mc_or    COND_1 mc_parferme  {quadcond("OR");  } 
| mc_parover  COND_1  mc_and   COND_2 mc_parferme {quadcond("AND");  } 
| mc_parover COND3 mc_parferme  
;
/**********************************************************************************************************************/
%%

int main(int argc, char* argv[])
{
    FILE *file = fopen(argv[1], "r");
    strcpy(fileName, argv[1]);
    if (argc != 2 && !file) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier.\n");
        exit(EXIT_FAILURE);
    }
    initHashTable();
    yyrestart(file);
    //initialisation();    
    yyparse();
    afficher(); 
    afficher_qdr();  
    freeHashTable();
    Optimisation(&qc);
    /* trial(qc); */
    printf("\nqc=%d\n", qc);
    //afficher les quadruplets apres l'optimisation
    afficher_qdr(); 
    //affiche le code objet
    genere_code(qc);
    return 0;
}
int yyerror(char *msg)
{
    printf(" Ligne %d | Colonne %d| %s\n", nb_ligne,nb_col, msg);
}
yywrap()
{
    return 1;
}
