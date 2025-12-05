#include "ts.h"
#include "semantique.h"
/*********************************************************SEMANTIQUE **************************************************/
char* tmp_type = NULL;  // Initialize the pointer to NULL
char* tmp_type2 = NULL;
char* tmp_type3 = NULL;

// divison sur zero avce la cst est un  0
void div_zero_cst(int cst, int nb_ligne, int nb_col, char fileName[])
{
    if (cst == 0)
    {
        printf("\nFile %s| Ligne %d | Colonne %d| Erreur semantique: erreur division sur zero\n", fileName, nb_ligne, nb_col);
        exit(-1);
    }
}
// divison sur zero avce la cst est un  0
void div_zero_cstfloat(float cst, int nb_ligne, int nb_col, char fileName[])
{
    if (cst == 0)
    {
        printf("\nFile %s| Ligne %d | Colonne %d| Erreur semantique: erreur division sur zero\n", fileName, nb_ligne, nb_col);
        exit(-1);
    }
}
// divison sur zero avce l'idf est un  0
void div_zero_idf(char name[], int nb_ligne, int nb_col, char fileName[])
{
    element *elem = searchHashTable(name);
    if (elem != NULL)
    {
        if (elem->state == 1)
        {
            if (strcmp(elem->val, "0") == 0 || strcmp(elem->val, "0.0") == 0)
            {
                printf("\nFile %s | Ligne %d | Colonne %d | Erreur semantique: erreur division sur zero\n", fileName, nb_ligne, nb_col);
                // printf("I AM OUT OF HERE SEMANTIQUE\n");

                return;
                // printf("I AM OUT OF HERE SEMANTIQUE\n");
            }
        }
        else
        {
            printf("File %s | Ligne %d | Colonne %d | Erreur semantique: entité non declarée\n", fileName, nb_ligne, nb_col);
            return;
        }
    }
    else
    {
        printf("File %s | Ligne %d | Colonne %d | Erreur semantique: entité non existante\n", fileName, nb_ligne, nb_col);
        return;
    }
}

// comparaison de type
//-----------------------------
//in comp type i am gonna change it and make it instead of only checking between two 
//types to checking to all the types in l'operation d'affectation,
// for example let's say i have X: INTEGER,
// X= 7 +8.5 +9.33; THIS FUNCTION IS GONNA COMPARE THE TYPE OF X WITH 7 THEN 8.5 THEN 9.33
// SO FOR THIS I AM GONNA CREATE A NEW STRUCTURE THATS GONNA KEEP TRACK OF ALL OF THEM 
//THEN AT THE END TAKES THE LIST THEY ARE STORED IN AND COMPARE AND DISPLAY ALL THE ERRORS

Is_same_type* head_type_list = NULL;
Is_same_type* tail_type_list = NULL;  // To allow fast appending

void Comp_Type(int nbligne, int nbcolonne, Is_same_type* head, char* given_type) {
    Is_same_type* current = head;
    Is_same_type* next;

    while (current != NULL) {
        if (strcmp(given_type, " ")!=0)
        {
            if (strcmp(current->type, given_type) != 0) {
                printf("Erreur semantique: type '%s' incompatible avec '%s' | ligne = %d | colonne = %d\n",
                       current->type, given_type, current->ligne, current->colonne);
            }
        }
        // else printf("\nundefined entity");

        // Save next node before freeing
        next = current->next;
        free(current);
        current = next;
    }

    // Important: reset global pointers after freeing
    head_type_list = NULL;
    tail_type_list = NULL;
}
void add_type_to_list(char* type, int ligne, int colonne) {
    Is_same_type* newNode = malloc(sizeof(Is_same_type));
    if (!newNode) {
        fprintf(stderr, "Erreur d'allocation memoire.\n");
        exit(1);
    }

    // Safe copy (up to 9 chars) and force null-termination
    strncpy(newNode->type, type, 9);
    newNode->type[9] = '\0';

    newNode->ligne = ligne;
    newNode->colonne = colonne;
    newNode->next = NULL;

    if (head_type_list == NULL) {
        head_type_list = newNode;
        tail_type_list = newNode;
    } else {
        tail_type_list->next = newNode;
        tail_type_list = newNode;
    }
}
// THIS IS IN THE CASE OF A UNDEFINED VAR OF THE LEFT PART 
// IN THIS CASE WE SAVED THE TYPES FOR NOTHING SO WE HAVE TO FREE THE LIST 
// void RESRET_types_list() {
//     Is_same_type* current = head_type_list;
//     Is_same_type* next;

//     while (current != NULL) {
//         // Save next node before freeing
//         next = current->next;
        
//         // Free the current node
//         free(current);
        
//         // Move to the next node
//         current = next;
//     }

//     // Reset global pointers
//     head_type_list = NULL;
//     tail_type_list = NULL;
// }

//----------------------------------------------------------------------
void search_value(char name[], float *val, int nb_ligne, int nb_col, char fileName[])
{
    element *elem = searchHashTable(name);
    if (elem != NULL)
    {
        if (elem->state == 1)
        {
            if (strcmp(elem->val, "") != 0)
            {
                *val = atof(elem->val);
                if (isnan(*val))
                { // Detect NaN values
                    printf("File %s| Ligne %d | Colonne %d| Erreur: %s contient une valeur non valide (NaN)\n",
                           fileName, nb_ligne, nb_col, name);
                }
                return;
            }
            else
            {
                printf("File %s| Ligne %d | Colonne %d| Erreur semantique: %s non initialise\n",
                       fileName, nb_ligne, nb_col, name);
                return;
            }
        }
        else
        {
            printf("File %s| Ligne %d | Colonne %d| Erreur: %s non declare\n",
                   fileName, nb_ligne, nb_col, name);
            return;
        }
    }
    printf("File %s| Ligne %d | Colonne %d| Erreur: %s non existant\n",
           fileName, nb_ligne, nb_col, name);
}



//THIS FUNCTION WILL ADD LE TYPE OF THE ODF INTO LA TS
void add_type(char name[], char type[], int nb_ligne, int nb_col, char fileName[])
{
    element *elem = searchHashTable(name);
    if (elem != NULL)
    {
        if (elem->state == 0)
        {
            strcpy(elem->type, type);
            elem->state = 1;
            return;
        }
        else
        {
            printf("File %s| Ligne %d | Colonne %d| Erreur semantique: double declaration\n",
                   fileName, nb_ligne, nb_col);
            return;
        }
    }
    printf("File %s| Ligne %d | Colonne %d| Erreur semantique: entite non existante\n", fileName, nb_ligne, nb_col);
}
char *cleanFloatValue(float num)
{
    char *str = (char *)malloc(32 * sizeof(char)); // Allocate memory for the float string
    if (!str)
        return NULL;

    snprintf(str, 32, "%f", num); // Convert float to string

    // Check for parentheses and remove them
    if (str[0] == '(' && str[strlen(str) - 1] == ')')
    {
        memmove(str, str + 1, strlen(str)); // Shift the string left to remove '('
        str[strlen(str) - 1] = '\0';        // Remove the trailing ')'
    }

    return str;
}

void add_value(char name[], char val[], int nb_ligne, int nb_col, char fileName[])
{
    element *elem = searchHashTable(name);
    if (elem != NULL)
    {
        if (elem->state == 1)
        {
            strcpy(elem->val, val);
            return;
        }
        else
        {
            printf("File %s| Ligne %d | Colonne %d| Erreur semantique: entite non declare\n",
                   fileName, nb_ligne, nb_col);
            return;
        }
    }
    printf("File %s| Ligne %d | Colonne %d| Erreur semantique: entite %s non existante\n",
           fileName, nb_ligne, nb_col, name);
}
// Function to cast int to string
char *intToString(int num)
{
    // Allocate enough memory (max 20 chars for an int)
    char *result = (char *)malloc(20);
    if (result == NULL)
    {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    sprintf(result, "%d", num); // Convert int to string directly into result
    return result;              // ✅ Valid dynamically allocated string
}
// Function to cast float to string
char *floatToString(float num)
{
    // Assuming a reasonably large buffer size
    char buffer[20];

    // Use sprintf to convert float to string
    sprintf(buffer, "%f", num);

    // Allocate memory for the string and copy the buffer
    char *result = (char *)malloc(strlen(buffer) + 1); // Cast malloc result to char*
    if (result == NULL)
    {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    strcpy(result, buffer);

    return result;
}

// Vérification des bornes d'un vecteur
/*void vector_bounds_check(char name[], int index, int nb_ligne, int nb_col, char fileName[])
{
    element *elem = searchHashTable(name);
    if (elem != NULL)
    {
        if (elem->state == 1)
        {
            int size = atoi(elem->val); // Supposons que la taille du vecteur est stockée en "val"
            if (index < 0 || index >= size)
            {
                printf("File %s | Ligne %d | Colonne %d | Erreur semantique: index %d hors limites pour le vecteur %s\n",
                       fileName, nb_ligne, nb_col, index, name);
                exit(-1);
            }
        }
        else
        {
            printf("File %s | Ligne %d | Colonne %d | Erreur semantique: vecteur non déclaré\n",
                   fileName, nb_ligne, nb_col);
            exit(-1);
        }
    }
}

// Affectation dans un vecteur
void vector_assignment(char name[], int index, char value[], int nb_ligne, int nb_col, char fileName[])
{
    vector_bounds_check(name, index, nb_ligne, nb_col, fileName);
    printf("Affectation: %s[%d] = %s\n", name, index, value);
}*/

// Lecture d'une valeur et stockage dans la table des symboles
void read_value(char name[], char type[], int nb_ligne, int nb_col, char fileName[])
{
    char input[100];
    printf("Entrez la valeur de %s: ", name);
    scanf("%s", input);
    add_value(name, input, nb_ligne, nb_col, fileName);
}

// Affichage d'une valeur
void display_value(char message[], char name[], int nb_ligne, int nb_col, char fileName[])
{
    element *elem = searchHashTable(name);
    if (elem != NULL && elem->state == 1)
    {
        printf("%s: %s\n", message, elem->val);
    }
    else
    {
        printf("File %s | Ligne %d | Colonne %d | Erreur: variable %s non declaree\n", fileName, nb_ligne, nb_col, name);
    }
}

// Vérification des conditions dans un IF
void if_condition(int condition, int nb_ligne, int nb_col, char fileName[])
{
    if (!condition)
    {
        printf("File %s | Ligne %d | Colonne %d | Avertissement: condition IF toujours fausse\n", fileName, nb_ligne, nb_col);
    }
}

// Vérification du contrôle de boucle FOR
void for_loop_control(char variable[], int start, int end, int nb_ligne, int nb_col, char fileName[])
{
    if (start > end)
    {
        printf("File %s | Ligne %d | Colonne %d | Avertissement: boucle FOR ne s'exécutera jamais (%d > %d)\n", fileName, nb_ligne, nb_col, start, end);
    }
}



//-----------------------------------------------------------------------------------------------------------
//this function takes and idf as an input and look in la ts if its a constant it returs 1, else returns 0
int is_const(char *idf,int nb_ligne, int nb_col, char fileName[]) {
    element *e = searchHashTable(idf);  // Recherche l'identifiant dans la table des symboles
    if (strcmp(e->Is_cst, "1") == 0) {
        printf("File %s | Ligne %d | Colonne %d | l'entite %s est constante, doit reste inchange.\n", fileName, nb_ligne, nb_col,idf);
        return 1;  // L'identifiant est une constante
    }
    return 0;  // Ce n'est pas une constante ou il n'existe pas
}
//this function update la ts, it adds les champs du tableau tab(isvector, borne_inf, taille)
// when calling la fonction add_type we already checked that idf exists, so if not found
// no need to print an error that it doesnt exist
void update_ts_vector(char name[], char Is_Vector[], char borne_inf[], char taille[]  )
{
    element *elem = searchHashTable(name);
    if (elem != NULL)// we used the function that insert l'idf in la ts just before using this one,
    {
        // printf("\n** %s \n", borne_inf);
        // printf("\n** %s \n", taille);
        strcpy(elem->Is_Vector, Is_Vector);
        strcpy(elem->borne_inf, borne_inf);
        strcpy(elem->taille, taille);
    }
}

//this is rempli during the declaration of the constant 
void remplir_ts_cst(char name[], char Is_cst[])
{
    element *elem = searchHashTable(name);
    if (elem != NULL)
    {
        strcpy(elem->Is_cst, Is_cst);
    }
}

//first look name vector if exists
//then take the borne inf and la taille, and calculate : borne sup = taille + borne inf
// now take the integer daclared inside the [] and make sure its between borne inf+ borne sup
// no need to check if this is a vector, cuz we're only calling the function in the VECTOR affectation
void vector_bound_ck(char name[], int to_ck ,int nb_ligne, int nb_col, char fileName[])
{
    element *elem = searchHashTable(name);
    if (elem != NULL)
    {
        int binf =atoi(elem->borne_inf);
        int taille=atoi(elem->taille);
        int bsup = binf + taille;
        
        if (to_ck >= binf && to_ck <= bsup) {
            // ✅ to_ck is within bounds
        } else 
        {
            printf("File %s | Ligne %d | Colonne %d | vector out of bounds\n", fileName, nb_ligne, nb_col);
        }

    }
    else 
    {
        printf("File %s | Ligne %d | Colonne %d | le vecteur %s n'est pas declare\n", fileName, nb_ligne, nb_col,name);
    }
}
// this function takes the idf as input and returns its type
char* get_idf_type(char idf[])
{
    //check this again????
    element *elem = searchHashTable(idf);
    if (elem != NULL && elem->state != 1)
    {
        printf("Erreur: identifiant '%s' non trouve dans la table des symboles.\n", idf);
        return NULL;
    }
    // else printf("not error .. found in la ts\n");

    // if (strlen(elem->type) == 0)
    // {
    //     printf("Erreur: type de l'identifiant '%s' non encore défini.\n", idf);
    //     return NULL;
    // }

    return elem->type;
}

//this will return 1 if the last character of the string is valid, else 0
int last_char_ofstring_isValid(char* string)
{
    char last_char= string[strlen(string) - 2];
    if(last_char=='$' || last_char=='%' || last_char=='#' || last_char=='&')
    {
        return 1;
    }
    return 0;
}

void ck_display(char* string, char* type_tocompare, int nb_ligne, int nb_col,char fileName[])
{
    char* type; 
    // if the last char of the string is valid and the type is not empty:
    if( (last_char_ofstring_isValid(string) ==1) &&  (strcmp(type_tocompare, " ")!=0) )
    {
        char last_char= string[strlen(string) - 2];
        if (last_char=='$') type="INTEGER";
        if (last_char=='%') type="FLOAT";
        if (last_char=='#') type="STR";
        if (last_char=='&') type="CHAR";

        if(strcmp(type_tocompare, type)!=0)
        {printf("File %s | Ligne %d | Colonne %d | Vous devez ajouter le signe de formatage correct pour le type:%s.\n", fileName, nb_ligne, nb_col, type_tocompare);}
    }

    // the case where le signe is missing but the idf is present
    if( (last_char_ofstring_isValid(string) ==0) &&  (strcmp(type_tocompare, " ")!=0) )
    {printf("File %s | Ligne %d | Colonne %d | Vous devez ajouter le signe de formatage a la fin du message d'affichage.\n", fileName, nb_ligne, nb_col);}
}
//this function to check id read in correct
void ck_read(char* string, char* type_tocompare, int nb_ligne, int nb_col,char fileName[])
{
    char* type; 
    // if the last char of the string is valid and the type is not empty:
    if( (last_char_ofstring_isValid(string) ==1) &&  (strcmp(type_tocompare, " ")!=0) )
    {
        char last_char= string[strlen(string) - 2];
        if (last_char=='$') type="INTEGER";
        if (last_char=='%') type="FLOAT";
        if (last_char=='#') type="STR";
        if (last_char=='&') type="CHAR";

        if(strcmp(type_tocompare, type)!=0)
        {printf("File %s | Ligne %d | Colonne %d | Vous devez ajouter le signe de formatage correct pour le type:%s.\n", fileName, nb_ligne, nb_col, type_tocompare);}
    }
}


//------------------------
//check if the idf is initialised
int is_idf_int(char idf[])
{
    element *elem = searchHashTable(idf);
    if (elem != NULL && strcmp(elem->val,"")==0)
    {
        return 0;
    }
    return 1;
}

//---------------------------------------------------
//THIS FUNCTION CHECKS IF THE IDFS OF THE FOR LOOP ARE INTEGERS AND INITIALISED (ALSO INSIDE THE FUNCTION GET IDF TYPE ITS GONNA PRINT AN ERROR IF IT WAS NOT DECLARED)
void ck_sematique_For1(char idf1[],char idf2[],char idf3[])
{
    //first check if the first idf is declared (must be an integer) dosnt matter if not initialised 
    tmp_type = get_idf_type(idf1);//this checks if it exists in la ts, and return its type
    tmp_type2 = get_idf_type(idf2);
    tmp_type3 = get_idf_type(idf3);
    if (tmp_type != NULL)
    {
        if (strcmp(tmp_type, "INTEGER")!=0)//ig it's not an integer, print erreur semantique
        printf("Dans la boucle For,L'idf %s doit etre un entier.\n", idf1);
    }
    if (tmp_type2 != NULL)
    {
        if (strcmp(tmp_type2, "INTEGER")!=0)//ig it's not an integer, print erreur semantique
        printf("Dans la boucle For,L'idf %s doit etre un entier.\n", idf2);
    }
    if (tmp_type3 != NULL)
    {
        if (strcmp(tmp_type3, "INTEGER")!=0)//ig it's not an integer, print erreur semantique
        printf("Dans la boucle For,L'idf %s doit etre un entier.\n", idf3);
    }

    //but the idfs must be initialised.. checking that out here:
    if (is_idf_int(idf1)==0) //if it wasnt initialised print error
    {
         printf("Dans la boucle For, L'idf %s doit etre initialise.\n", idf1);
    }
    if (is_idf_int(idf2)==0) //if it wasnt initialised print error
    {
         printf("Dans la boucle For, L'idf %s doit etre initialise.\n", idf2);
    }

    if (is_idf_int(idf3)==0) //if it wasnt initialised print error
    {
         printf("Dans la boucle For, L'idf %s doit etre initialise.\n", idf3);
    }
    //----------------------
    // now we need to get the idf values of idf2 and idf3 and make sure that idf2<idf3

}

//---------------------------------------------------
//SAME AS PREVIOUS FUNCTION BUT JUST HAS 2 IDFS THIS TIME
void ck_sematique_For2(char idf1[],char idf3[])
{
    ///first check if the first idf is declared (must be an integer) dosnt matter if not initialised 
    tmp_type = get_idf_type(idf1);//this checks if it exists in la ts, and return its type
    tmp_type3 = get_idf_type(idf3);
    if (tmp_type != NULL)
    {
        if (strcmp(tmp_type, "INTEGER")!=0)//ig it's not an integer, print erreur semantique
        printf("Dans la boucle For,L'idf %s doit etre un entier.\n", idf1);
    }
    if (tmp_type3 != NULL)
    {
        if (strcmp(tmp_type3, "INTEGER")!=0)//ig it's not an integer, print erreur semantique
        printf("Dans la boucle For,L'idf %s doit etre un entier.\n", idf3);
    }

    //but the idfs must be initialised.. checking that out here:
    if (is_idf_int(idf1)==0) //if it wasnt initialised print error
    {
         printf("Dans la boucle For, L'idf %s doit etre initialise.\n", idf1);
    }

    if (is_idf_int(idf3)==0) //if it wasnt initialised print error
    {
         printf("Dans la boucle For, L'idf %s doit etre initialise.\n", idf3);
    }
    //----------------------
}

void ck_sematique_For3(char idf1[])
{
    //first check if the first idf is declared (must be an integer) 
    tmp_type = get_idf_type(idf1);//this checks if it exists in la ts, and return its type
    if (tmp_type != NULL)
    {
        if (strcmp(tmp_type, "INTEGER")!=0)//ig it's not an integer, print erreur semantique
        printf("Dans la boucle For, L'idf %s doit etre un entier.\n",  idf1);
    }
    if (is_idf_int(idf1)==0) //if it wasnt initialised print error
    {
         printf("Dans la boucle For, L'idf %s doit etre initialise.\n", idf1);
    }
}


