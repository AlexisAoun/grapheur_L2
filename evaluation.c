

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "analyse_lexical.h"
#include "analyse_syntaxe.h"
#include "evaluation.h"

//--------------------------------------------------------------
/*
LAM Alexandre
BERNARD Thomas

- Analysateur Syntaxique
- Analysateur Lexical
- *EVALUATEUR*
- Interface graphique




//********************  DOCUMENTATION  *************************
//                 Merci de lire la documentation             //
//********************  DOCUMENTATION  *************************




*/
//--------------------------------------------------------------



//--------------------------------------------------------------
//******************  STRUCTURE GENERAL  ***********************
//--------------------------------------------------------------


//--------------------------------------------------------------
/*
Les points non definis de notre fonction s'expriment soit
sous forme de points ou soit sous forme d'intervalles.
Ces caracteristiques sont stockees dans la struct jetonInterval
*/
//--------------------------------------------------------------

struct Stack_interval*  createStack_i(unsigned capacity)
{
    struct Stack_interval*  stack = (struct Stack_interval* )malloc(sizeof(struct Stack_interval));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (float*)malloc(stack->capacity * sizeof(float));
    return stack;
}

int isFull_i(struct Stack_interval* stack)
{
    return stack->top == stack->capacity - 1;
}

void push_i(struct Stack_interval*  stack, float item)
{
    if (isFull_i(stack))
        return;
    stack->array[++stack->top] = item;
}

float pop_i(struct Stack_interval*  stack)
{
    return stack->array[stack->top--];
}

float del_i(struct Stack_interval* stack, int index)
{
    float delValue = stack->array[index];
    for(int i = index; i <= stack->top; i++){
        stack->array[i] = stack->array[i+1];
    }
    stack->top-=1;
    return delValue;
}

//Transfert le contenu de la stackA vers la stackB
void fuse_A_dans_B(struct Stack_interval* stackA, struct Stack_interval* stackB)
{
    while(stackA->top != -1){
        push_i(stackB, pop_i(stackA));
    }
}

void add_i(struct Stack_interval* stack, int index, float item)
{
    stack->top += 1;
    for(int i = stack->top; i >= index; i--)
        stack->array[i] = stack->array[i-1];
    stack->array[index] = item;
}
//--------------------------------------------------------------


//--------------------------------------------------------------
//*****************  FONCTION GENERAL  *************************
//--------------------------------------------------------------

float calculate(Arbre root, float x){
    if(root->jeton.lexem == REEL){
        return root->jeton.valeur.reel;
    }

    else if(root->jeton.lexem == VARIABLE){
        return x;
    }

    else if(root->jeton.lexem == OPERATEUR){
        if(root->jeton.valeur.operateur == PLUS){
            return calculate(root->pjeton_d, x) + calculate(root->pjeton_g, x);
        }

        else if(root->jeton.valeur.operateur == MOINS)
            return calculate(root->pjeton_g, x) - calculate(root->pjeton_d, x);

        else if(root->jeton.valeur.operateur == FOIS)
            return calculate(root->pjeton_d, x) * calculate(root->pjeton_g, x);

        else if(root->jeton.valeur.operateur == DIV)
            return calculate(root->pjeton_g, x) / calculate(root->pjeton_d, x);

        else
            return pow(calculate(root->pjeton_g, x),calculate(root->pjeton_d, x));
    }

    else if(root->jeton.lexem == FONCTION){
        if(root->jeton.valeur.fonction == ABS)
            return fabs(calculate(root->pjeton_g, x));
        else if(root->jeton.valeur.fonction == SIN)
            return sin(calculate(root->pjeton_g, x));
        else if(root->jeton.valeur.fonction == SQRT)
            return sqrt(calculate(root->pjeton_g, x));
        else if(root->jeton.valeur.fonction == LOG)
            return log(calculate(root->pjeton_g, x));
        else if(root->jeton.valeur.fonction == COS)
            return cos(calculate(root->pjeton_g, x));
        else if(root->jeton.valeur.fonction == TAN)
            return tan(calculate(root->pjeton_g, x));
        else if(root->jeton.valeur.fonction == EXP)
            return exp(calculate(root->pjeton_g, x));
        else if(root->jeton.valeur.fonction == VAL_NEG)
            return -(calculate(root->pjeton_g, x));
        else if(root->jeton.valeur.fonction == SINC)
            return sin(calculate(root->pjeton_g, x)) / x;
    }
    else{
    if(root->pjeton_g!=NULL){
        calculate(root->pjeton_g, x);
    }
    if(root->pjeton_d!=NULL){
       calculate(root->pjeton_d, x);
    }
}

}
//--------------------------------------------------------------





//--------------------------------------------------------------
//  (2) Expression qui s'annule
//--------------------------------------------------------------

//L'expression possede une valeur nulle entre deux bornes si le
//signe de l'expression change
int PossedeUneValeurNulle(float borneA, float borneB, float pas, Node* root){
    int res = 0;
    int signe = 1;

    if(calculate(root, borneA)<0)
        signe = -1;

    while(borneB - borneA >  0){
        borneA += pas;
        if(calculate(root, borneA) * signe < 0){
            res = 1;
        }

    }
    return res;
}

//Trouve la valeur zero de maniere iterative entre deux bornes
//Pour cela, on detecte le changement de signe
float calculateZero(float borneA, float borneB, float pas, Node* root){
    int signe = 1;

    if(calculate(root, borneA)<0)
        signe = -1;

    while(calculate(root, borneA) * signe > 0){
        borneA += pas;
    }
    float valeurZero = borneA-pas;


    return valeurZero;

}

float estZero(float borneA, float borneB, float pas, Node* root, Stack_interval* stack_i, float push){
    if(root->jeton.lexem == VARIABLE){
        if(push){
            push_i(stack_i,0);
        }
        return 0;
    }
    else{
        if(root->jeton.valeur.operateur == FOIS){
            estZero(borneA, borneB,pas, root->pjeton_g, stack_i,push);
            estZero(borneA, borneB,pas, root->pjeton_d, stack_i,push);
        }
        else if(root->jeton.valeur.operateur == DIV){
            estZero(borneA, borneB,pas, root->pjeton_g, stack_i,push);
        }

        else if (root->jeton.valeur.operateur == PLUS || root->jeton.valeur.operateur == MOINS){
            //On ne calcule la valeur zero que si elle existe
            if(PossedeUneValeurNulle(borneA,borneB,pas,root)){
                float valeurZero = calculateZero(borneA, borneB,pas, root);
                if(push){
                    push_i(stack_i,valeurZero);
                }
                return valeurZero;
            }
        }
    }
}
//--------------------------------------------------------------





//--------------------------------------------------------------
//  (4) Union d'intervalles
//--------------------------------------------------------------

void display_interval(struct Stack_interval * stack){
    for(int i = 0; i <= stack->top;i++){
        printf("status : ZERO     x    : %f\n", stack->array[i]);
    }
}


//--------------------------------------------------------------
//  (1) Expression pas definie
//--------------------------------------------------------------
void pasDefini(float borneA, float borneB, float pas,  struct Node* root, Stack_interval* stack_i){
    if(root->jeton.lexem == OPERATEUR){
        if(root->jeton.valeur.operateur == DIV){
            estZero(borneA, borneB,pas, root->pjeton_d, stack_i,1);
        }
        if(root->pjeton_g != NULL)
            pasDefini(borneA, borneB,pas, root->pjeton_g, stack_i);
        if(root->pjeton_d != NULL)
            pasDefini(borneA, borneB,pas, root->pjeton_d, stack_i);

    }
}



//Prend une stack contenant les intervalles et les tries par ordre
//croissant.
//Cela permet de simplifier la generation des valeurs dans la fonction
//Evaluate.
//Ordre croissant = valeur la plus faible / borne inferieur la plus
//faible en premier.
struct Stack_interval* sort_ensemble(struct Stack_interval* stack_i){
    struct Stack_interval* ensemble = createStack_i(100);
    while(stack_i->top >= 0){
        int taille = stack_i->top;
        int index = 0;
        float min = stack_i->array[0];
        for(int i = 0 ; i <= taille ; i++){
            if(stack_i->array[i] < min){
                min = stack_i->array[i];
                index = i;
            }
        }
        push_i(ensemble,del_i(stack_i, index));
    }
    //display_interval(ensemble);
    return ensemble;
}

struct Stack_interval* ensembleDefinition(float borneA, float borneB, float pas, Node* root){
    struct Stack_interval* stack_i = createStack_i(100);
    pasDefini(borneA, borneB, pas, root, stack_i);

    return sort_ensemble(stack_i);
}
//--------------------------------------------------------------





//--------------------------------------------------------------
//**********************  EVALUATION  **************************
//--------------------------------------------------------------
tableau_evaluateur * evaluate(float borneA, float borneB, int nombreDePoints, Node* root, int * tailleOutput){
    float pas = (borneB-borneA)/nombreDePoints;
    struct Stack_interval* ensemble_non_defini = ensembleDefinition(borneA, borneB, pas, root);
    float taille = (borneB - borneA) / pas;
    tableau_evaluateur * res;
    res = (tableau_evaluateur*)malloc(taille * sizeof(tableau_evaluateur));
    tableau_evaluateur coupleValeurs;
    int i;

    float x_;
    float y_;

    int bug = 0;

    for(i = 0, borneA ; borneA <= borneB; borneA += pas){
        x_ = borneA - pas;
        y_ = calculate(root,x_);

        coupleValeurs.x = borneA;
        coupleValeurs.y = calculate(root, coupleValeurs.x);
        if(coupleValeurs.y == coupleValeurs.y){
            if(ensemble_non_defini->top >= 0){
                if((coupleValeurs.x <= ensemble_non_defini->array[0] + (pas / 2)) && (coupleValeurs.x >= ensemble_non_defini->array[0] - (pas / 2) )){
                    //printf("Valeur ZERO  X: %f   Y: %f\n", coupleValeurs.x , coupleValeurs.y);
                    del_i(ensemble_non_defini,0);
                }
                else{
                    if(fabs(coupleValeurs.y - y_) < 5){
                        //printf("i : %d     x: %f   y: %f\n", i, coupleValeurs.x, coupleValeurs.y);
                        res[i] = coupleValeurs;
                        i++;
                    }
                }
            }
            else{
                if(fabs(coupleValeurs.y - y_) < 5){
                    //printf("i : %d     x: %f   y: %f\n", i, coupleValeurs.x, coupleValeurs.y);
                    res[i] = coupleValeurs;
                    i++;
                }
            }
        }
    }
    (*tailleOutput) = i;
    return res;
}
//--------------------------------------------------------------
