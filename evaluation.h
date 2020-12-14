#include "jeton.h"

#pragma once
#ifdef __cplusplus
extern "C"
{
#endif



//--------------------------------------------------------------
//Les valeurs qui s'annulent sont stockes dans des stacks pour
//faciliter leur utilisation.
typedef struct Stack_interval {
    int top;
    unsigned capacity;
    float * array;
}Stack_interval;




//-------------------------------------------------------------------------------------
//prototype de fonction
//-------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------
//Gestion des stacks
int isFull_i(struct Stack_interval* stack);

void push_i(struct Stack_interval*  stack, float item);

float pop_i(struct Stack_interval*  stack);

float del_i(struct Stack_interval* stack, int index);

void add_i(struct Stack_interval* stack, int index, float item);
//-------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------
//Calcule la valeur de y à partir d'une valeur de x de
//manière récursive
float calculate(Arbre root, float x);
//-------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------
//Valeurs uniques qui s'annulent (e.g pour la fonction inverse)
int PossedeUneValeurNulle(float borneA, float borneB, float pas, Node* root);

float calculateZero(float borneA, float borneB, float pas, Node* root);

float estZero(float borneA, float borneB, float pas, Node* root, Stack_interval* stack_i, float push);

void pasDefini(float borneA, float borneB, float pas,  struct Node* root, Stack_interval* stack_i);
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
//Tri par ordre croissant les valeurs de la stack
struct Stack_interval* sort_ensemble(struct Stack_interval* stack_i);

//Retourne une stack contenant les valeurs qui s'annulent triées par ordre croissant
struct Stack_interval* ensembleNonDef(float borneA, float borneB, float pas, Node* root);
//-------------------------------------------------------------------------------------


//Fct d'évaluation
tableau_evaluateur * evaluate(float borneA, float borneB, int nombreDePoints, Node* root, int * tailleOutput);



#ifdef __cplusplus
}
#endif
