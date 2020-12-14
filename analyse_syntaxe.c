//Alexis Aoun et Killian Allaire; projet grapheur, module : analyseur syntaxique 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "analyse_syntaxe.h"
#include"analyse_lexical.h"



//L'analyseur de syntaxe se divise en trois grande partie :
//1- Tester le placement et l'équilibre des parentheses
//2- Convertir l'expression infix en entrée en une expression postfix (durant cette étape la vérification de la syntaxe sera effectué)
//3- Convertir l'expression postfix obtenue précédement en un arbre binaire


//Dans ce module les piles seront utilisé à plusieurs reprises

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//Les fonctions nécessaires afin de gérer les piles

//fonction empillage pour la pile
void empiller (Pile * pile , typejeton nJeton ){
  ElementPile *nouveau = malloc(sizeof(*nouveau));

  if (pile == NULL || nouveau == NULL){
    exit(EXIT_FAILURE);
  }

  nouveau->jeton = nJeton;
  nouveau->suivant = pile->top_pile;
  pile->top_pile = nouveau;
}


//fonction qui retire l'élément le plus haut dans la pile
void pop (Pile * pile){
  if (pile == NULL){
    exit(EXIT_FAILURE);
  }
  ElementPile *elementDepile = pile->top_pile;

    if (pile != NULL && pile->top_pile != NULL)
    {
        pile->top_pile = elementDepile->suivant;
        free(elementDepile);
    }

}


// fonction d'initialisation de la pile
Pile * init_pile (){
  Pile *pile = malloc(sizeof(*pile));
  ElementPile *element = malloc(sizeof(*element));

  if (pile == NULL || element == NULL)
  {
      exit(EXIT_FAILURE);
  }

  typejeton jeton_fin;
  jeton_fin.lexem = FIN;
  jeton_fin.valeur.reel = 0;

  element->jeton = jeton_fin;
  element->suivant = NULL;

  pile -> top_pile = element;

  return pile;

}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//étape 1

//fonction qui vérifie que les parentheses , bar (absolu) et crochet sont correctement utilisé dans l'expression infix à l'aide d'une pile
//prend en fonction le tableau de token
//si une erreur est trouvé dans l'expression infix la fonction retourne un rapport avec la position et la description de l'erreur
//si il n'ya pas d'erreur retourne rapport avec rapport.pos_debut = -1
Rapport test_parentheses(typejeton * input_token){
  Pile * pile = init_pile();
  int compteur_abs = 0;
  Rapport r;
  r.pos_debut=-1;
  //on parcourt notre tableau de token
  int i;
  for(i = 0 ; input_token[i].lexem != FIN && r.pos_debut == -1 ; i++){

    //printf("jeton actuel : %d\n", input_token[i].lexem );
    //printf("top jeton pile parenthese = %d \n", pile->top_pile->jeton.lexem );
    //test sur les bars de la fonction absolue
    if(input_token[i].lexem == ABSOLU){
      if(compteur_abs == 0){
        empiller(pile , input_token[i]);
        compteur_abs = 1;
      }
      else if (compteur_abs == 1){
        if (pile->top_pile->jeton.lexem == ABSOLU){
          pop(pile);
          compteur_abs = 0;
        }
        else
          r.pos_debut = i;
          strcpy(r.desc, "Erreur aves une/des barres absolue");
      }
    }

    //test sur les parentheses
    else if(input_token[i].lexem == PAR_OUV)
      empiller(pile , input_token[i]);

    else if (input_token[i].lexem == PAR_FERM){
      //printf("top jeton pile parenthese = %d \n", pile->top_pile->jeton.lexem );
      if (pile->top_pile->jeton.lexem == PAR_OUV)
        pop(pile);
      else{
        r.pos_debut = i;
        strcpy(r.desc, "Erreur avec une/des parenthèses");
      }
    }

    //test sur les crochets
    else if(input_token[i].lexem == BAR_OUV)
      empiller(pile , input_token[i]);

    else if (input_token[i].lexem == BAR_FERM){
      if (pile->top_pile->jeton.lexem == BAR_OUV)
        pop(pile);
      else{
        strcpy(r.desc, "Erreur avec un/des crochets");
        r.pos_debut = i;
      }
    }
  }

  //après avoir parcouru le tableau on vérifie que la pile est vide, si elle l'est l'expression est correcte sinon il y a déséquilibre avec les parentheses, bars ou crochets
    if(pile->top_pile->jeton.lexem != FIN && r.pos_debut == -1 ){
      r.pos_debut = 0;
      strcpy(r.desc, "()/[]/|| non équilibrés dans l'expresion");
    }

  return r;

}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//étape 2


//fonction qui determine si operateur 1 est prioritaire par rapport à opérateur 2, retourne 1 si vrai 0 sinon
int estPrioritaire (typejeton op1 , typejeton op2){
  int r = 0;
  int coef1 , coef2;


  coef1 = op1.valeur.operateur;
  coef2 = op2.valeur.operateur;

  if(op1.lexem == FONCTION) // les fonctions sont plus prioritaire que les opérateurs
    coef1 = 100;
  if(op2.lexem == FONCTION)
    coef2 = 100;

  if (coef1 == 1 || coef1 == 3) // on égalise les coeffs +,- et *,/
    coef1--;

  if (coef2 == 1 || coef2 == 3)
    coef2--;

  if(coef1 >coef2 || (coef1 == coef2 && op1.lexem != PUIS))//op1 etant avant op2 est plus prioritaire SAUF en cas de puissance
    r = 1;

  return r;

}


//fonction qui converti une expression infix en une expresison postfix
//si une erreur est trouvé dans l'expression infix la fonction retourne un rapport avec la position et la description de l'erreur
//si il n'ya pas d'erreur retourne rapport avec rapport.pos_debut = -1
Rapport infix_a_postfix (typejeton * infix , typejeton * postfix){
  Pile * pile = init_pile();

  int curseur_postfix = 0;
  int compteur_abs = 0;

  Rapport rapport;
  rapport.pos_debut = -1;
  rapport.pos_fin = -1;
  rapport.lexem = ERREUR;



  int dernier_indice;
  //on parcourt notre tableau de token infix
  int i;
  for(i = 0 ; infix[i].lexem != FIN ; i++){

    //verification de la syntaxe
    //on verifie que le caractere precedent est valide
    if(i == 0 && infix[i].lexem == OPERATEUR){ // on commence par verifier le premier jeton
      rapport.pos_debut = i;
      rapport.lexem = infix[i].lexem;
      strcpy(rapport.desc, "Le premièr élément ne peut pas être un opérateur");
      return rapport;
    }
    if(i>0){
      if(infix[i].lexem == REEL || infix[i].lexem == VARIABLE){ //si on a un reel ou une variable
      if(infix[i-1].lexem == PAR_FERM || infix[i-1].lexem == BAR_FERM || (infix[i-1].lexem == ABSOLU && !compteur_abs) || infix[i-1].lexem == VARIABLE || infix[i-1].lexem == REEL){
        rapport.pos_debut = i;
        rapport.lexem = infix[i].lexem;
        strcpy(rapport.desc, "Elément inattendu ou manquant avant réel / variable");
        return rapport;
        }
      }
      if(infix[i].lexem == OPERATEUR || infix[i].lexem == PAR_FERM || infix[i].lexem == BAR_FERM || (infix[i].lexem == ABSOLU && compteur_abs)){ //si on a un operateur / ')' / ']' / '|' fermante
        if(infix[i-1].lexem == PAR_OUV || infix[i-1].lexem == BAR_OUV || (infix[i-1].lexem == ABSOLU && compteur_abs) || infix[i-1].lexem == FONCTION || infix[i-1].lexem == OPERATEUR){
          rapport.pos_debut = i;
          rapport.lexem = infix[i].lexem;
          strcpy(rapport.desc, "Elément inattendu ou manquant avant opérateur / ')' / ']' / '|'");
          return rapport;
          }
        }
      if(infix[i].lexem == PAR_OUV || infix[i].lexem == BAR_OUV || (infix[i].lexem == ABSOLU && !compteur_abs)){//si on a '(' / '[' / '|' ouvrante
        if(infix[i-1].lexem == PAR_FERM || infix[i-1].lexem == BAR_FERM || (infix[i-1].lexem == ABSOLU && !compteur_abs) || infix[i-1].lexem == VARIABLE || infix[i-1].lexem == REEL){
          rapport.pos_debut = i;
          rapport.lexem = infix[i].lexem;
          strcpy(rapport.desc, "Elément inattendu ou manquant avant '(' / '[' / '|'");
          return rapport;
          }
      }
      if(infix[i].lexem == FONCTION){//si on a une fonction
        if(infix[i-1].lexem == PAR_FERM || infix[i-1].lexem == BAR_FERM || (infix[i-1].lexem == ABSOLU && !compteur_abs) || infix[i-1].lexem == VARIABLE || infix[i-1].lexem == REEL || (infix[i-1].lexem == FONCTION && infix[i-1].valeur.fonction != ABS)){
          rapport.pos_debut = i;
          rapport.lexem = infix[i].lexem;
          strcpy(rapport.desc, "Elément inattendu ou manquant avant fonction");
          return rapport;
          }
      }
    }


    //si on a un reel ou une variable on le place directement dans l'expression postfix
    if(infix[i].lexem == REEL || infix[i].lexem == VARIABLE){
      postfix[curseur_postfix] = infix[i];
      curseur_postfix ++;
    }

    //si on a un operateur ou une fonction
    //on gère le cas des bars de la fonction absolue
    //si on a une barre ouvrante on la remplace par un token fonction absolue
    //et on ajoute dans la pile une parenthese ouverte
    else if(infix[i].lexem == OPERATEUR || infix[i].lexem == FONCTION || (infix[i].lexem == ABSOLU && compteur_abs == 0)){


      int rajouter_par = 0;
      if(infix[i].lexem == ABSOLU){
        infix[i].lexem = FONCTION;
        infix[i].valeur.fonction = ABS;
        rajouter_par = 1;
        compteur_abs = 1;
      }
      //si la pile est vide
      if(pile->top_pile->jeton.lexem != FIN && pile->top_pile->jeton.lexem != PAR_OUV && pile->top_pile->jeton.lexem != BAR_OUV){
        //on compare la priorité de l'opérateur séléctionné et l'opérateur sur le haut de la pile
        //on vide la pile dans l'expression postfix tant que l'opérateur au top de la pile a la priorité, que la pile n'est pas vide et qu on ne rencontre pas de parentheses ouvertes
        while(estPrioritaire(pile->top_pile->jeton, infix[i]) && pile->top_pile->jeton.lexem != FIN && pile->top_pile->jeton.lexem != PAR_OUV && pile->top_pile->jeton.lexem != BAR_OUV && pile->top_pile->jeton.lexem != ABSOLU){
          postfix[curseur_postfix] = pile->top_pile->jeton;
          curseur_postfix ++;
          pop(pile);
        }
        empiller(pile , infix[i]);
      }
      else
        empiller(pile , infix[i]);

      if(rajouter_par){
        typejeton par_ouverte;
        par_ouverte.lexem = PAR_OUV;
        empiller(pile , par_ouverte);
      }

    }

    //si on a une parenthese ou un crochet ouvert
    //on place dans la pile
    else if(infix[i].lexem == PAR_OUV || infix[i].lexem == BAR_OUV){
      empiller(pile , infix[i]);
    }

    //quand on rencontre un crochet ou parenthese ferme on vide la pile dans l'expression postfix jusqu'à arrivé à une parenthese ou crochet ouvert

    //si on a une parenthese fermee
    else if(infix[i].lexem == PAR_FERM || (infix[i].lexem == ABSOLU && compteur_abs == 1)){
      while (pile->top_pile->jeton.lexem != PAR_OUV){
          postfix[curseur_postfix] = pile->top_pile->jeton;
          curseur_postfix++;
          pop(pile);
      }
      pop(pile);

      if(infix[i].lexem == ABSOLU && compteur_abs == 1)
        compteur_abs = 0;
    }

    //si on a un crochet ferme
    else if(infix[i].lexem == BAR_FERM){
      while (pile->top_pile->jeton.lexem != BAR_OUV){
          postfix[curseur_postfix] = pile->top_pile->jeton;
          curseur_postfix++;
          pop(pile);
      }
      pop(pile);
    }


    dernier_indice = i;


  }


  //on verifie à la fin de notre expression que le dernier jeton est valide
  if(infix[dernier_indice].lexem == OPERATEUR || infix[dernier_indice].lexem == FONCTION){
    rapport.pos_debut = dernier_indice;
    rapport.lexem = infix[dernier_indice].lexem;
    strcpy(rapport.desc, "Elément inattendu ou manquant à la fin de l'expression");
    return rapport;

  }

  //on vide notre pile lorsqu'on arrive à la fin de notre expression
  while(pile->top_pile->jeton.lexem != FIN){
    postfix[curseur_postfix] = pile->top_pile->jeton;
    curseur_postfix ++;
    pop(pile);

  }

  //on rajoute un jeton de fin à la fin du tableau de l'expresion postfix
  typejeton jeton_fin;
  jeton_fin.lexem = FIN;

  postfix[curseur_postfix] = jeton_fin;

  return rapport;

}

//---------------------------------------------------------------------------------------
//fonction gerant les nodes des arbres

//ajoute une node sans fils
Arbre nouvelle_node(typejeton nJeton){
	Arbre nouveau = malloc(sizeof(nouveau));
	if (nouveau == NULL){
    		exit(EXIT_FAILURE);
  	}
	nouveau->jeton = nJeton;
	nouveau->pjeton_d = NULL;
	nouveau->pjeton_g = NULL;
}

//ajoute une node avec deux fils
Arbre nouvelle_node_operateur(typejeton nJeton,Arbre gauche, Arbre droite){
	Arbre nouveau = malloc(sizeof(nouveau));
	if (nouveau == NULL){
    		exit(EXIT_FAILURE);
  	}
	nouveau->jeton = nJeton;
	nouveau->pjeton_g = gauche;
	nouveau->pjeton_d = droite;
}

//ajoute une node avec un fils gauche
Arbre nouvelle_node_fonction(typejeton nJeton,Arbre gauche){
	Arbre nouveau = malloc(sizeof(nouveau));
	if (nouveau == NULL){
    		exit(EXIT_FAILURE);
  	}

	nouveau->jeton = nJeton;
	nouveau->pjeton_g = gauche;
	nouveau->pjeton_d = NULL;
}


//fonction d'afficage de l'arbre, utilisé pour le debug
void affichage_arbre(Arbre root, int niveau){
	if(root->jeton.lexem == REEL){
	   printf("niveau %d , %f\n",niveau, root->jeton.valeur.reel);
	    niveau++;
	}
	else if(root->jeton.lexem == OPERATEUR){
	   printf("niveau %d , op = %d\n",niveau ,root->jeton.valeur.operateur);
	    niveau++;
	}
  else if(root->jeton.lexem == FONCTION){
	   printf("niveau %d , f = %d\n",niveau ,root->jeton.valeur.fonction);
	   niveau++;
	}
  else if(root->jeton.lexem == VARIABLE){
	   printf("niveau %d , X\n",niveau);
	   niveau++;
	}


	if(root->pjeton_g!=NULL){
		 affichage_arbre(root->pjeton_g, niveau);
	}
	if(root->pjeton_d!=NULL){
	   affichage_arbre(root->pjeton_d, niveau);
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/* -> Les fonctions nécessaires afin de gérer les piles d'adresses <- */

//fonction empillage pour la pile
void empiller_arbre (Pile_adresse * pile , Arbre nArbre ){
  ElementPile_arbre *nouveau = malloc(sizeof(*nouveau));

  if (pile == NULL || nouveau == NULL){
    exit(EXIT_FAILURE);
  }

  nouveau->arbre = nArbre;
  nouveau->suivant = pile->top_pile;
  pile->top_pile = nouveau;
}


//fonction qui retire l'élément le plus haut dans la pile
void pop_arbre (Pile_adresse * pile){
  if (pile == NULL){
    exit(EXIT_FAILURE);
  }
  ElementPile_arbre *elementDepile = pile->top_pile;

    if (pile != NULL && pile->top_pile != NULL)
    {
        pile->top_pile = elementDepile->suivant;
        free(elementDepile);
    }

}


// fonction d'initialisation de la pile
Pile_adresse * init_pile_arbre(){
  Pile_adresse *pile = malloc(sizeof(*pile));
  ElementPile_arbre *element = malloc(sizeof(*element));

  if (pile == NULL || element == NULL)
  {
      exit(EXIT_FAILURE);
  }

  element->arbre = NULL;
  element->suivant = NULL;

  pile -> top_pile = element;

  return pile;

}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------


Arbre postfix_a_arbre(typejeton * postfix){
	int i;
	Arbre gauche, droite;
	Arbre passation;
	Pile_adresse * pile = init_pile_arbre(); 		//Cree la pile
	for(i=0; postfix[i].lexem != FIN; i++){		//Parcours le tableau postfix

		if(postfix[i].lexem == REEL || postfix[i].lexem == VARIABLE){		//On a un reel qu'on transforme en arbre, on l'ajoute a la pile
			passation = nouvelle_node(postfix[i]);
			empiller_arbre(pile, passation);
		}

		else if(postfix[i].lexem == OPERATEUR){	//On a un operateur, on forme donc un arbre. En prenant comme branche les adresse des deux derniers elements qu'on enleve. On ajoute ensuite notre arbre a la pile.
			droite=pile->top_pile->arbre;
			pop_arbre(pile);
			gauche=pile->top_pile->arbre;
			pop_arbre(pile);
			passation = nouvelle_node_operateur(postfix[i], gauche, droite);
			empiller_arbre(pile, passation);
		}

		else{	//On a une fonction, on forme donc un arbre. En prenant comme branche l'adresse du dernier element qu'on enleve. On ajoute ensuite notre arbre a la pile
			gauche=pile->top_pile->arbre;
			pop_arbre(pile);
			passation = nouvelle_node_fonction(postfix[i], gauche);
			empiller_arbre(pile, passation);
		}

	}
	return pile->top_pile->arbre;

}


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------

//fonction principale
Arbre infix_a_arbre (typejeton * input , Rapport * r , int taille){

  Arbre arbre;
  typejeton postfix [taille];

	*r = test_parentheses(input); //on test les parentheses

	if( r->pos_debut == -1){

		*r = infix_a_postfix(input , postfix); // on fait la conversion infix postfix

		if(r->pos_debut != -1)
			return 0;
	}
	else
    return 0;

  //si il y a pas d'erreur on traduit notre expression postfix en arbre qu on retourne
	arbre = postfix_a_arbre(postfix);

  return arbre;
}
