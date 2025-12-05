#include <stdio.h>
#include <string.h>
#include "quad.h"
extern qdr quad[1000];
#define MAX_QUADS 1000


// Structure to store object code instructions
typedef struct {
    char etiq_name[15]; //etiq name, could be empty
    char instruction[10];  // e.g., "MOV", "ADD", "JMP"
    char operand1[50];     // First operand
    char operand2[50];     // Second operand (if any)
    int qc_associated;// this is a hidden parametere, we use it to make the connection between the optimisation and the code objets parts
} obj_code;

// Array to store object code
obj_code object_code[1000];
int indice_code_objet=0;
char ACC[50] = ".";  // Initialize accumulator
//----------------------------------------------------------------------------------------------------------------------------------------
//                                 1. GENERATION DE CODE DES EXPRESSIONS
//----------------------------------------------------------------------------------------------------------------------------------------
void Gen(const char* op, const char* operand, int i) 
{   
    // printf("%s %s\n", op, operand);
    
    strcpy(object_code[indice_code_objet].instruction, op);
    strcpy(object_code[indice_code_objet].operand1, operand);
    object_code[indice_code_objet].qc_associated=i;
    indice_code_objet++;
}

// Modified to take char* instead of char**
void GetInAcc(char* x, char* y, int i) {
    // Case 1: ACC is empty
    if (strcmp(ACC, ".") == 0) {
        Gen("LOAD", x,i);
        strcpy(ACC, x);
    }
    // Case 2: ACC contains y (second operand)
    else if (strcmp(ACC, y) == 0) {
        // PROPER STRING SWAP
        char temp[20];
        strcpy(temp, x);   // Save x to temp
        strcpy(x, y);      // Copy y to x
        strcpy(y, temp);   // Copy original x (from temp) to y
    }
    // Case 3: ACC contains something else
    else if (strcmp(ACC, x) != 0) {
        Gen("STORE", ACC,i);
        Gen("LOAD", x,i);
        strcpy(ACC, x);
    }
}
int save_br_info[20];
int counter=0;
void save_branchement(int etiq_number)
{
    save_br_info[counter]=etiq_number;
    counter=counter+1;
}

///-----------------------------------------
int is_indice_a_br(char indice)
{
int i;
    for (i=0; i<counter; i++)
    {
        if (save_br_info[i]==indice)
        return 1;
    }
    return 0;
}
void Code_objet_espressions( qdr quads[], int qc)
{
    int i;
    for (i=0; i<qc; i++)
{
    // printf("\n*******i am at i=%d:\n", i);
    
    if (strcmp("*", quads[i].oper)==0)
    {
        // printf("\n***\n");
        GetInAcc(quads[i].op1, quads[i].op2,i);
        Gen("MULT", quads[i].op2,i);
        strcpy(ACC, quads[i].res);


        
    }
    else if (strcmp("+", quads[i].oper)==0)
    {
        // printf("\n+++\n");
        GetInAcc(quads[i].op1, quads[i].op2,i);
        Gen("ADD", quads[i].op2,i);
        strcpy(ACC, quads[i].res);

    }
    else if (strcmp("-", quads[i].oper)==0)
    {
        if (strcmp("vide", quads[i].op2)!=0) // Binary minus
        {
            // printf("\n---binaire\n");
            GetInAcc(quads[i].op1, " ",i);
            Gen("SUB", quads[i].op2,i);
            strcpy(ACC, quads[i].res);

        }
        else // Unary minus
        {
            // printf("\n---unaire\n");
            GetInAcc(quads[i].op1, " ",i);
            Gen("CHS", " ",i);
            strcpy(ACC, quads[i].res);


        }
    }
    else if (strcmp("/", quads[i].oper)==0)
    {
        GetInAcc(quads[i].op1, " ",i);
        Gen("DIV", " ",i);
        strcpy(ACC, quads[i].res);

    }
    
    else if (strcmp("=", quads[i].oper)==0)
    {
        //
        Gen("STORE", quads[i].res,i ); 


    }
    else {
        //this is the case where the quad is a branchement:
        if ((quads[i].oper)[0] == 'B' && (quads[i].oper)[1] != 'O')
        {
            // printf("\ni m at i=%d and its a branchement and quads[i].oper=%c\n", i, (quads[i].oper)[1]);
            //Case 1: branchement inconditionnel:
            if (  (quads[i].oper)[1] == 'R')
            {
                Gen("JMP", quads[i].op1,i);
                //save that there is a branchement into this etiq number
                int num = atoi(quads[i].op1);  // Convert string to int
                save_branchement(num);


            }
            else
            //Case 2: branchement conditionnel:
            {
                //1. Load RIGHT operand into ACC (accumulator)
                Gen("LOAD", quads[i].op2,i);
                //2. Compare the right operand WITH RES
                Gen("CMP", quads[i].res,i);
                //3. Use the correct conditional jump...(what is in oper+T) JUMP TO THE QUAD MENTIONED IN OP1
                Gen(quads[i].oper, quads[i].op1,i);  // if not > → jump if <=
                //save that there is a branchement into this etiq number
                int num = atoi(quads[i].op1);  // Convert string to int
                save_branchement(num);
                
            }


            

        }
        
    }
}

}
//----------------------------------------------------------------------------------------------------------------------------------------
//                                 2. GENERATION DE CODE DES BRANCHEMENT
//----------------------------------------------------------------------------------------------------------------------------------------

// I ADDED LES BRANCHEMENT WITH THE EXPRESSIONS TO ONLY DO 1 PARCOUR



void update_etiq_number()
{
    int v;
    for (v=0; v<indice_code_objet;v++)
    {
           if (is_indice_a_br(object_code[v].qc_associated) == 1 )
        {
            char str[20];
            sprintf(str, "%d", object_code[v].qc_associated);  // Convert int to string
            strcat( str, ": ");
            strcpy(object_code[v].etiq_name, str);

            //now that i changed the first one, its enough make, change it so it dont change the rest
            int i;
            for (i=0; i<counter; i++)
            {
                if (save_br_info[i]==object_code[v].qc_associated)
                save_br_info[i]=0;
            }

        }
    }
        
        ///object_code[indice_code_objet].qc_associated

}



//----------------------------------------------------------------------------------------------------------------------------------------
//                                 THIS FUNCTION IS GONNE GENERATE THE CODE OBJECT FOR THE WHOLE CODE
//----------------------------------------------------------------------------------------------------------------------------------------
void genere_code(int qc)
{
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    printf("\n++++++                                      LE CODE OBJET                                          ++++++++\n");
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    Code_objet_espressions( quad, qc);
    update_etiq_number();
    int v;
    for (v=0; v<indice_code_objet;v++)
    {
        printf("%s %s %s %s\n",object_code[v].etiq_name, object_code[v].instruction, object_code[v].operand1, object_code[v].operand2);
    }
}





//using this part to test my functions
// int main() {
//     Quad quads[] = {
//         {"*", "a", "b", "t1"},    
//         {"+", "t1", "c", "t2"}, 
//         {"-", "b", "vide", "t3"},
//         {"*", "c", "t3", "t4"},//this is where une permutation should happen
//         {"-", "t2", "t4", "t5"},
//         {"*", "a", "t5", "t6"},
//         {"=", "t6", "vide", "A"}
//     };

//     int i;
//     int qc=7;
    
//     Code_objet_espressions(quads, qc);

//     int v;
//     for (v=0; v<indice_code_objet;v++)
//     {
//         printf("%s %s %s\n", object_code[v].instruction, object_code[v].operand1, object_code[v].operand2);
//     }
    
    
//     return 0;}


