
#ifndef ANALYSE_LEXICAL_H
#define ANALYSE_LEXICAL_H

#include "jeton.h"

#pragma once
#ifdef __cplusplus
extern "C"
{
#endif


//fonction qui test si un caratère est un opérateur : input (caractère) output(booléen)
int estUnOperateur(char input);

//fonction qui test si un caratère est un entier : input (caractère) output(booléen)
int estUnEntier(char input);

//fonction qui test si un caratère est une variable : input (caractère) output(booléen)
int estUneVariable(char input);

//fonction qui test si un caratère est un parenthèse : input (caractère) output(booléen)
int estUneParenthese(char input);

//fonction qui test si un caratère est une lettre : input (caractère) output(booléen)
int estUneLettre(char input);

//fonction qui identifie les ()[]|| en lexem : input (caractère) output(entier)
int delimiteurConversion(char ch);

//fonction qui convertie les opèrateur en lexem : input (caractère) output(entier)
int operatorConversion(char ch);

//fonction qui convertie les fonction en lexem : input (string) output(entier)
int fonctionConversion(char * str);

//fonction qui affiche le tableau de lexems : input (tabeau de jeton)
void display(typejeton * jetonTab);

//fonction qui identifie une fonction dans l'input : input (tableau de jeton,string,entier, pointeur d'entier, structure de rapport) output(structure Rapport)
Rapport fncfonction(typejeton* jeton, char* input,int indice,int *k,Rapport *rapport);

//fonction qui identifie un réel dans l'input : input (tableau de jeton,string,entier, pointeur d'entier, structure de rapport) output(structure Rapport)
Rapport fncreel(typejeton* jeton, char* input,int indice,int *k,Rapport *rapport);

//fonction qui change un input string et le convertie en tableau de lexems  : input (tableau de jeton,string,entier) output(structure Rapport)
Rapport fncjeton(char* input ,typejeton* jeton , int taille);




#ifdef __cplusplus
}
#endif

#endif
