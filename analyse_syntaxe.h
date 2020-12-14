#include "jeton.h"

#pragma once
#ifdef __cplusplus
extern "C"
{
#endif

//pile utilisé pour le test parenthese et infix a postfix
typedef struct ElementPile ElementPile;
struct ElementPile{
  typejeton jeton;
  ElementPile * suivant;
};


typedef struct{
  ElementPile * top_pile;
}Pile;

//pile utilisé pour postfix_a_arbre
typedef struct ElementPile_arbre ElementPile_arbre;
struct ElementPile_arbre{
  Arbre arbre;
  ElementPile_arbre * suivant;
};

typedef struct{
  ElementPile_arbre * top_pile;
}Pile_adresse;


//------------------------------------------------------------------------------
// les prototypes de fonction
// les fonctions utilisées pour gérer les piles
void empiller (Pile *pile, typejeton jeton);

void pop(Pile *pile);

Pile * init_pile ();

void empiller_arbre (Pile_adresse * pile , Arbre nArbre );

void pop_arbre (Pile_adresse * pile);

Pile_adresse * init_pile_arbre();

//------------------------------------------------------------------------------
// les fonctions pour gérer les arbres

Arbre nouvelle_node(typejeton nJeton);

Arbre nouvelle_node_operateur(typejeton nJeton, Arbre gauche, Arbre droite);

Arbre nouvelle_node_fonction(typejeton nJeton,Arbre gauche);

void affichage_arbre(Arbre root, int niveau);



//------------------------------------------------------------------------------
// les fonctions clés

//argument : tableau de token infix à vérifier
//return : rapport, si rapport.pos_debut == -1 pas d'erreur
Rapport test_parentheses(typejeton * input_token);

//argument : tableau de token input infix et le tableau de token output postfix
//return : rapport, si rapport.pos_debut == -1 pas d'erreur
Rapport infix_a_postfix (typejeton * infix , typejeton * postfix);

//argument : tableau de token postfix
//return : pointeur de la racine de l'arbre bianire d'expression
Arbre postfix_a_arbre(typejeton * postfix);

//argument : tableau de token infix , pointeur du rapoort (si rapport.pos_debut == -1 pas d'erreur) , int taille du tableau infix
//return : pointeur de la racine de l'arbre bianire d'expression
Arbre infix_a_arbre (typejeton * input , Rapport * r , int taille);

#ifdef __cplusplus
}
#endif
