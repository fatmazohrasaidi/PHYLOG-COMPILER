#include <stdio.h>
#include <string.h>
#include <ctype.h> // for isdigit
#include "quad.h"
extern qdr quad[1000];
#define MAX_QUADS 1000

//-----get_affectations-----THIS FUNCTION RETURNS A VECTOR OF ALL LES AFFECTATION IN MY CODE, SAVING THE QC WHERE L'AFFECTATION SIMPLE IS, 
//AND SAVING QC_deb AND QC_FIN OF EACH AFFECTATION COMPLIQUES
//EG: AFFACTATION SIMPLE:  3-( = ,Y , vide , X )... /**X=Y */
//THE CODE KNOWS THAT ITS SIMPLE OR NOT BASED ON THE QUAD BEFORE IT
//EG: AFFECTATION COMPLIQUE:  5-( * ,X , 4 , T1 )
//                            6-( = ,T1 , vide , Z )/**Z= 4*Y; */
//IN THIS EXAMPLE IT SAVES: QC_deb=5, AND QC_FIN=6
//
//so each case in the vector has 3 elements:
//struct...{ int is_Simple;//this is =1 if the AFFACTATION is SIMPLE, else it's 0
//           int QC_deb;
//           int QC_FIN;
//          }
//-------------------------------------------------------------------------------------------------------------
//  First, define the struct for the affectation info:
//-------------------------------------------------------------------------------------------------------------
typedef struct {
    int is_Simple;
    int QC_deb;
    int QC_fin;
    int skip;//expression skipped if related to a branchement!!
} Affectation;

Affectation* get_affectations(int* count, int taille_quad) {
    Affectation* table = malloc(taille_quad * sizeof(Affectation));
    *count = 0;
    int i;

    //go thro all les quadruplet and look for all les explression and save their QCs
    for (i = 0; i < taille_quad; i++) {
        if (strcmp(quad[i].oper, "") == 0) break;

        if (strcmp(quad[i].oper, "=") == 0) {
            Affectation a;
            int qc_deb = i;
            int j = i - 1;

            // Walk backward through intermediate expression quads
            while (j >= 0 &&
                   (strcmp(quad[j].oper, "+") == 0 ||
                    strcmp(quad[j].oper, "-") == 0 ||
                    strcmp(quad[j].oper, "*") == 0 ||
                    strcmp(quad[j].oper, "/") == 0 )
                  ) {
                qc_deb = j;
                j--;
            }

            if (qc_deb == i) {
                // It’s a simple affectation
                a.is_Simple = 1;
                a.QC_deb = i;
                a.QC_fin = i;
                a.skip=0;

                
            } else {
                // It’s a complex affectation
                a.is_Simple = 0;
                a.QC_deb = qc_deb;
                a.QC_fin = i;

                //if this affectation is related to a branchemenet then its skipped in the optimisation part
                if ((quad[(qc_deb)-1].oper)[0]=='B' && (quad[(qc_deb)-1].oper)[1]!='O'  )
                a.skip=1;
                else a.skip=0;
            }

            table[*count] = a;
            (*count)++;
        }
    }

    return table;
}


//--------------------------------------------------------------------------------------------------------------
//                  PART 0 OF OPTIMISATION: PROPAGATION DES CONSTANTES
//--------------------------------------------------------------------------------------------------------------
void propagation_de_constante(qdr quads[], Affectation affectations[], int affect_count, int taille_quad)
{
    int i;
    for (i = 0; i < affect_count; i++) 
    {
        /// Only apply to simple affectations
        if (affectations[i].is_Simple) 
        {
            int line = affectations[i].QC_deb;
            if (is_integer_or_float(quads[line].op1))
            {
                //X=5.. replace all X with 5 in the next quad!! stp when X is assigned something else
                char to_replace[100]; // 5
                char replacement[100]; // X
                strcpy(to_replace, quads[line].op1);
                strcpy(replacement, quads[line].res);
                // Check that 'to_replace' is NOT overwritten before being used
                int safe = 1;
                int j;
                // Do propagation: replace every usage of X with 5
                for (j = line + 1; j < taille_quad && strcmp(quads[j].oper, "") != 0; j++) 
                {
                    // If X is reassigned later  stop propagation
                    if (strcmp(quads[j].res, replacement) == 0)
                    {
                        safe = 0;
                        break;
                    } 
                    if (strcmp(quads[j].op1, replacement) == 0)
                    {
                        strcpy(quads[j].op1, to_replace);
                    }
                    if (strcmp(quads[j].op2, replacement) == 0)
                    {
                        strcpy(quads[j].op2, to_replace);
                    } 
                    if (!safe) continue; // Don't propagate if Y is modified
                }
                

            }
        }
    }
        
}


//--------------------------------------------------------------------------------------------------------------
//                  PART 1 OF OPTIMISATION: PROPAGATION DE COPIE
//--------------------------------------------------------------------------------------------------------------
int starts_with_T_and_numbers(const char *s) {
    if (s == NULL || s[0] != 'T') {
        return 0;
    }
    int i;
    // Check every character after the 'T'
    for (i = 1; s[i] != '\0'; i++) {
        if (!isdigit(s[i])) {
            return 0;
        }
    }

    // If we reach here, it's OK
    return 1;
}

int is_integer_or_float(const char *s) {
    int i = 0;
    int has_dot = 0;

    if (s == NULL || s[0] == '\0') {
        return 0;
    }

    if (s[i] == '+' || s[i] == '-') {
        i++;
    }

    int has_digit = 0;

    for (; s[i] != '\0'; i++) {
        if (isdigit(s[i])) {
            has_digit = 1;
        } else if (s[i] == '.') {
            if (has_dot) {
                // Already had a dot -> invalid
                return 0;
            }
            has_dot = 1;
        } else {
            // Not digit, not dot -> invalid
            return 0;
        }
    }

    // Must have at least one digit
    if (!has_digit) {
        return 0;
    }

    return 1;
}

void propagation_de_copie(qdr quads[], Affectation affectations[], int affect_count, int taille_quad) {
    int i;
    for (i = 0; i < affect_count; i++) {
        // Only apply to simple affectations
        if (affectations[i].is_Simple) 
        {
            int line = affectations[i].QC_deb;

            // Skip constants or temporary vars
            if (starts_with_T_and_numbers(quads[line].op1) || is_integer_or_float(quads[line].op1))
                continue;

            // Variables involved
            char to_replace[100]; // Y
            char replacement[100]; // X
            strcpy(to_replace, quads[line].op1);
            strcpy(replacement, quads[line].res);

            // Check that 'to_replace' is NOT overwritten before being used
            int safe = 1;
            int j;
            

            // Do propagation: replace every usage of Y with X
            for (j = line + 1; j < taille_quad && strcmp(quads[j].oper, "") != 0; j++) 
            {
                // If Y is reassigned later  stop propagation
                //look up to what is the number of affectation this j belongs to:
                int v,is_it_skipped=0;
                for (v = 0; v < affect_count; v++)
                {
                    if ( (affectations[v].QC_deb) <= j && (affectations[v].QC_fin)>=j)
                    is_it_skipped=affectations[v].skip;
                }

                if (strcmp(quads[j].res, to_replace) == 0 && is_it_skipped!=1)
                {
                    safe = 0;
                    break;
                }  
                else 
                {
                    if (strcmp(quads[j].op1, to_replace) == 0)
                    {
                        strcpy(quads[j].op1, replacement);
                        // printf("\nI JUST REPLACED %s with %s in j=%d\n",to_replace,replacement,j );
                    }
                    
                    if (strcmp(quads[j].op2, to_replace) == 0)
                    {
                        strcpy(quads[j].op2, replacement);
                        // printf("\nI JUST REPLACED %s with %s in j)%d\n",to_replace,replacement,j );
                    } 
                    if (is_it_skipped==1 && strcmp(quads[j].res, to_replace)==0)
                    {
                        strcpy(quads[j].res, replacement);
                        // printf("\nI JUST REPLACED %s with %s in j=%d\n",to_replace,replacement,j );
                    }

                }
                if (!safe) continue; // Don't propagate if Y is modified
                  
            }
        }
    }
}

//--------------------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------------------
//                  PART 2 OF OPTIMISATION: PROPAGATION DES EXPRESSIONS
//--------------------------------------------------------------------------------------------------------------


//----------- DECALER LES QUAD
void decaler_quad(qdr quad[], int *taille_quad, int j, int nb_decalage) {
    if (*taille_quad + nb_decalage >= MAX_QUADS) {
        printf("Erreur : pas assez d'espace pour décaler.\n");
        return;
    }

    if (j < 0 || j > *taille_quad) {
        printf("Erreur : position d'insertion invalide.\n");
        return;
    }

    // Décalage vers la droite à partir de la fin jusqu'à j
    int i;
    for (i = *taille_quad - 1; i >= j; i--) {
        quad[i + nb_decalage] = quad[i];
    }

    // vider les espaces libres créés: gonna be remplie later
    int k;
    for ( k = 0; k < nb_decalage; k++) {
        strcpy(quad[j + k].oper, "----");
        strcpy(quad[j + k].op1, "----");
        strcpy(quad[j + k].op2, "----");
        strcpy(quad[j + k].res, "----");
    }

    // Mettre à jour la taille
    *taille_quad += nb_decalage;
}

void copier_quads(qdr quad[], int deb, int fin, int j, char chaine[30]) {
    if (deb > fin) {
        printf("Erreur : deb > fin.\n");
        return;
    }

    int count = fin - deb;
    int i;
    for ( i = 0; i < count; i++) {
        quad[j + i] = quad[deb + i];  // Copy each quad one by one
    }
}

void supprimer_quads(qdr quad[], int *taille_quad, int deb, int fin) {
    if (deb < 0 || fin >= *taille_quad || deb > fin) {
        printf("Erreur : indices invalides pour suppression.\n");
        return;
    }

    int nb_suppression = fin - deb + 1;
    int i;
    // Décaler les quads suivants vers la gauche pour écraser ceux supprimés
    for (i = fin + 1; i < *taille_quad; i++) {
        quad[i - nb_suppression] = quad[i];
    }

    // Mettre à jour la taille
    *taille_quad -= nb_suppression;
}


int is_integer(const char *s) {
    int i = 0;

    // handle negative and positive numbers
    if (s[0] == '-' || s[0] == '+') {
        i = 1;
        if (s[1] == '\0') return 0; // Only a '-' is not a valid int
    }

    for (; s[i] != '\0'; i++) {
        if (!isdigit(s[i])) {
            return 0; // Not an integer
        }
    }
    return 1; // It's an integer
}
//this function is used to update les branchement after changing/moving/deleting les quadruplets
void update_les_br(qdr quads[], int taille_quad,int starting_point, int cpt)//DONT UPDATE ALL LES QUAD, UPDATE ONLY THOSE AFTER L'EXP
{
    //
    int f;
    for (f = starting_point; f < taille_quad; f++)
    {
        //first: check if quad.open starts with B, then check if quad[f].op1 is int
        if( (quad[f].oper)[0]=='B' && (quad[f].oper)[1]!='O'&& is_integer(quad[f].op1))
        //making sure to not change BOUNDS
        {
            // printf("\nthe quad f=%d needs to be updated\n",f);
            int num = atoi(quad[f].op1);  // Convert string to int
            num=num+cpt;
            char str[20];
            sprintf(str, "%d", num);  // Convert int to string
            strcpy(quad[f].op1, str);
            // printf("\nwhat i just changed is quad[f].op1 into %s\n", quad[f].op1);
        }
    }
}

Affectation* propagation_des_expressions(qdr quads[], Affectation affectations[], int *affect_count, int *n) {
    int i;
    for (i = 0; i < *affect_count; i++) {
        // Skip simple affectations
        if (affectations[i].is_Simple) continue;

        if ((affectations[i].skip)==1)
        {
            // printf("\nTHIS AFFECTATION IS RELETED TO A BRANCHEMENT SO DONT REPLACE IT SKIP IT i=%d\n",i);
            continue;
        }
         
        //----------------------------------------
        //step 1: look for les expression complique

        int deb = affectations[i].QC_deb;
        int fin = affectations[i].QC_fin;

        char result_var[100];
        strcpy(result_var, quads[fin].res);  // result_var=Z in my example
        //Z= 4 * Y;
        //TRIAL= 7 * Z;
        //-------------->  TRIAL= 7 * 4 * Y; (or X in my case cuz we used propagation des copies before)

        //----------------------------------------
        //step 2: check is this expression was 
        //          ever refrenced in the code
        int j;
        int quad_to_be_deleted_later=0;
        int cpt_decalage_br=0;
        int skip_changes=0;
        for (j = fin + 1; j < *n; j++) 
        {
            int used = 0;int save_j;
            
            //if that same vaiable was assigned something else, stop changing it in the next quads!!!
            if ( strcmp(quads[j].res, result_var) == 0)
            {
                // printf("\nI NEED TO SKIP CHANGING THE NEXT!! result_var=%s AFTER j=%d\n",result_var,j);
                skip_changes=1;
            }

            if (strcmp(quads[j].op1, result_var) == 0 || strcmp(quads[j].op2, result_var) == 0)
                used = 1;

            if (used && skip_changes==0) {
                //----------------------------------------
                //step 3: if yes, then we need to replace 
                //          it with what it was defined
                //              (so add new quads) 
                //          as then delete its definition
                //              (delete old quads)

                // printf("\n-----currently at j=%d--\n", j);
                // printf("----LES QUADRUPLETS at j=%d, deb=%d, fin=%d  before le decalage\n", j, deb, fin);
                quad_to_be_deleted_later=1;//this to remember that at the end of replacing the var with its expression, i need to delete it at the end 
                // int f;
                // for (f = 0; f < *n; f++)
                // {
                //     printf("\n %d-( %s ,%s , %s , %s )\n", f, quad[f].oper, quad[f].op1, quad[f].op2, quad[f].res);
                // }
 
                strcpy(chaine,quads[fin-1].res);
                if (strcmp(quads[j].op1, result_var) == 0)
                {
                    strcpy(quads[j].op1,chaine);
                }else //its op2
                {
                    strcpy(quads[j].op2,chaine);
                }
                //--- This part is gonna add the new quads
                decaler_quad(quad, n, j, fin-deb);
                copier_quads(quad,deb, fin, j, chaine);
                
                //----------------------------------------
                //step 4: now we need to update the table
                //           of les affectation
                affectations = get_affectations(affect_count, *n);
                // int v;
                // for (v = 0; v < affect_count; v++) {
                //     printf("Affectation %d: is_Simple = %d, QC_deb = %d, QC_fin = %d\n",
                //            v, affectations[v].is_Simple, affectations[v].QC_deb, affectations[v].QC_fin);
                // }

                //----------------------------------------
                //step 5: now we need to update the quad
                //           that do jumps 
                //     (cuz since we changed les quads
                //          the jump to qc must change)

                cpt_decalage_br=fin-deb;
                update_les_br(quad,*n,j, cpt_decalage_br);

            }
            if ((j==(*n)-1) && quad_to_be_deleted_later ==1)
            {
                supprimer_quads(quad, n, deb,fin);//fin-()
                update_les_br(quad,*n,deb, -(fin-deb+1));//this subtructs the deleted quad number from les BRanchements
            
                affectations = get_affectations(affect_count, *n);
                quad_to_be_deleted_later=0;
            }

        }
    }
    return affectations;
}

//C function that takes an integer index and searches the Affectation table to find which 
//affectation's range (QC_deb to QC_fin) contains that index.
int find_qc_range(int index, Affectation affectations[], int count, int *qc_deb, int *qc_fin) {
    int i;
    for (i = 0; i < count; i++) {
        if (index >= affectations[i].QC_deb && index <= affectations[i].QC_fin) {
            *qc_deb = affectations[i].QC_deb;
            *qc_fin = affectations[i].QC_fin;
            return 1; // found
        }
    }
    return 0; // not found
}
//--------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------
//                  PART 3 OF OPTIMISATION: SIMPLIFACATION ALGEBRIQUE (MULTIPLICATION)
//--------------------------------------------------------------------------------------------------------------
void Simplification_Algebrique_multiplication(qdr quads[], Affectation affectations[], int affect_count) {
    int i;
    for (i = 0; i < affect_count; i++) {
        if (affectations[i].is_Simple) continue;

        int deb = affectations[i].QC_deb;
        int fin = affectations[i].QC_fin;

        int j;
        for (j = deb; j <= fin; j++) {
            if (strcmp(quads[j].oper, "*") == 0) {
                // Check if result = op1 * const or result = const * op2
                char* res = quads[j].res;
                char* op1 = quads[j].op1;
                char* op2 = quads[j].op2;

                if (is_integer(op2) ||  is_integer(op1) ){
                    // printf("Found X = X * C pattern at quad %d: %s = %s * %s\n",j, res, op1, op2);
                    //only if the constant is 2 we change this quad, cuz if it's bigger then the multiplication is gonna be a better choice
                    if (is_integer(op2) && (atoi(quads[j].op2)==2    )  )
                    {
                        strcpy(quads[j].oper, "+");
                        strcpy(quads[j].op2, quads[j].op1);
                    }
                    if (is_integer(op1) && (atoi(quads[j].op1)==2    )  )
                    {
                        strcpy(quads[j].oper, "+");
                        strcpy(quads[j].op1, quads[j].op2);
                    }
                }
            }
        }
    }
}







//-------------------------------------------------------------------------------------------------------------------
//              THE CALL OF THE GLOBAL OPTIMISATION FUNCTION IS DONE HERE
//-------------------------------------------------------------------------------------------------------------------
//NOTES TO ADD/CHANGE:
// ----1- in propagation_de_copie maybe i should delete the quad after having it replaced??
// ----2- in propagation_des_expressions tables needs to change since les quads changed, so maybe return its value also(DONE!!)
// ----3- in propagation_des_expressions add the 5th step: update the brachement (jumps) since we are changing les quads(DONE!!)
// ----4- in propagation_des_expressions IF THE EXP IS CHANGED THEN DONT REPLACE IT (DONE!!)
// ----5- in propagation_des_expressions dont change les expressions related to a branchement (DONE!!)
// ----6- in propagation_des_expressions sometimes problem with the update of les branchement when there is a expression inside the FOR loop!!!(DONE!!)

void Optimisation(int *taille_quad)
{
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    printf("\n++++++                         LES QUADRUPLETS APRES L'OPTIMISATION                                ++++++++\n");
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    int affect_count;
    Affectation* table = get_affectations(&affect_count, *taille_quad);


    propagation_de_constante(quad, table, affect_count, *taille_quad);
    propagation_de_copie(quad, table, affect_count, *taille_quad);
    table =propagation_des_expressions(quad, table, &affect_count, taille_quad); 
    Simplification_Algebrique_multiplication(quad, table,  affect_count) ;

    //keep in mind that the table is changing inside the propagation_des_expressions functions so maybe i should also return it!!!!!!
    free(table); 
}





// //TESTING IF THE FUNCTIONS ARE WORKING WELL
// void trial(int taille_quad)
// {
//     int n,i;
//     Affectation* table = get_affectations(&n,taille_quad);

//     for (i = 0; i < n; i++) {
//         printf("Affectation %d: is_Simple = %d, QC_deb = %d, QC_fin = %d\n",
//                i, table[i].is_Simple, table[i].QC_deb, table[i].QC_fin);
//     }

//     free(table); // Always free dynamic memory
// }
