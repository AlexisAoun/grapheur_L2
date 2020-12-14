//--------------------------------------------------------------
/*
DUMILLY Doreen
BOURGUIGNON Manon

- Analysateur Syntaxique
- Analysateur Lexical
- Evaluateur
- *GRAPHEUR*




//********************  DOCUMENTATION  *************************
//                 Merci de lire la documentation             //
//********************  DOCUMENTATION  *************************


// ! Fichier contient main du projet Grapheur !

*/
//--------------------------------------------------------------


#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "graph.h"
#include "jeton.h"
#include "analyse_lexical.h"
#include "analyse_syntaxe.h"
#include "evaluation.h"

#define MAX_LIMIT 500
#define SPEED 0.5 //Vitesse à laquelle on se déplace sur le grid en px

//****Définition des variables globales****
char input[MAX_LIMIT];

float xmin;
float xmax;
float ymin;
float ymax;
float pas;

int flag;

Arbre arbre;



//fonction qui gère, controle et analyse les inputs
//return 0 si aucune erreure est trouvé
//return 1 sinon

int setup_input(){
  printf("\n\nAppuyez sur la touche entrée pour continuer\n");
  while ((getchar()) != '\n');                                  //cette étape permet de flush le buffer de l'input

  do{
    printf("\n \n" );
    printf("Entrez l'expression, x étant la variable : \n");


    fgets(input, MAX_LIMIT, stdin);                            //on recupère l'input de la console

  }while(input[0] == 10 || input[0] == 32);                    //tant que l'input n'est pas vide


  int taille=strlen(input);
  input[taille-1] = 0;

  typejeton tab_jeton[taille];
  Rapport r;

  r = fncjeton(input,tab_jeton , taille-1);                    //on performe l'analyse lexical, on recupere l'output dans tab_jeton


  if(r.pos_debut != -1){                                      //on verifie si l'analyse lexical a trouvé des erreurs
    printf("Erreur Lexical au caracère de position : %d \n", r.pos_debut+1);
    printf("Description de l'erreur : %s \n", r.desc);
    return 1;
  }
  else{
    arbre = infix_a_arbre(tab_jeton, &r, taille);             //on performe l'analyse syntaxique, on recupere l'arbre



    if(r.pos_debut != -1){                                   ////on verifie si l'analyse syntaxique a trouvé des erreurs
      printf("Erreur syntaxique à l'élément de position : %d \n", r.pos_debut+1);
      printf("Description de l'erreur : %s \n", r.desc);
      return 1;
    }

    do{

      printf("Entrez la borne inférieure : \n");     //on recupere les bornes en veifiant qu on recupere un reel et que la borne inferieure est plus petite que la borne supèrieur
      int input_int1 = scanf("%f", &xmin);
      while (getchar() != '\n');
        while (input_int1 != 1){
          printf("Veuillez rentrer un réel\n");
          input_int1 = scanf("%f", &xmin);
          while(getchar() != '\n');
        }

      printf("Entrez la borne supérieur : \n");
      int input_int2 = scanf("%f", &xmax);
      while (getchar() != '\n');
        while (input_int2 != 1){
          printf("Veuillez rentrer un réel\n");
          input_int2 = scanf("%f", &xmax);
          while(getchar() != '\n');
        }

      if(xmin>=xmax)
        printf("Il faut que la borne inférieur soit strictement plus petit que la borne supérieur\n");

    }while(xmin>=xmax);


    printf("Entrez le pas compris entre 1 et 0.001 : \n"); //on recupere le pas en vérifiant qu on recupere un reel compris entre 1 et 0.001
    int input_int3 = scanf("%f", &pas);
    while (getchar() != '\n');
    while (input_int3 != 1 || pas<0 || (pas > 1 || pas <0.001)){
        printf("Veuillez rentrer un réel compris entre 1 et 0.001 :\n");
        input_int3 = scanf("%f", &pas);
        while(getchar() != '\n');
      }


    ymin = xmin;
    ymax = xmax;

  }



  return 0;
}


/**
* myKey
*
* Gestion des touches du clavier
*
* @parma c code ascci definissant une touche du clavier
*
*/
void myKey(int c)
{
  switch(c)
  {
    case 27 :  //touche esc pour quitter
      exit(0);
      break;

    case 'n': //touche n pour un nouveau graphe
      flag = 1;
      while(flag)
      flag = setup_input();
      break;

    case  'z': //touche z pour aller vers le haut
      ymax += SPEED;
      ymin += SPEED;
      break;

    case  's': //touche s pour aller vers le bas
      ymax -= SPEED;
      ymin -= SPEED;
      break;

    case  'q': //touche q pour aller vers la gauche
      xmax -= SPEED;
      xmin -= SPEED;
      break;

    case  'd': //touche d pour aller vers la droite
      xmax += SPEED;
      xmin += SPEED;
      break;

    case 'a': //touche a pour zoomer
      if(((xmax - xmin) < 6) == 0){
        xmin += SPEED;
        xmax += -SPEED;
        ymin += SPEED;
        ymax += -SPEED;
      }
      break;

    case 'e': //touche e pour dezoomer
      xmin += -SPEED;
      xmax += SPEED;
      ymin += -SPEED;
      ymax += SPEED;
      break;
  }
}


/*
* char* itoa()
* Fonction qui de base appartient à la librairie
* stdlib.h.
* Comme elle est pas détectée, elle est réimplémentée ici.
*
* Transforme un entier en une chaîne de caratères.
*/
char* itoa(int value, char* result, int base) {
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}


/*
* float normalize(float value, float max_v, float min_v)
* -Normalisation des valeurs
* -Prend une valeur entre deux bornes (ici max_v et min_v) et retourne cette
*  mme valeur entre 0 et 1
*/
float normalize(float value, float max_v, float min_v){
    return -1*((2 * ((value-min_v) / (max_v - min_v))) - 1);
}


/*
* Génération des axes, de la grille et de la numérotation des axes
*/
void grid(float pas, tableau_evaluateur * tab_eva){
//Generation du grid

    //Lignes verticales
    for(int i = round(xmin)-1 ; i < xmax ; i ++){
      setcolor(150, 164, 173);
      line(normalize(i,xmin, xmax), -1 , normalize(i, xmin, xmax) , 1);
      char abscisse2String[10]; //pour convertir les int en string, on suppose qu'on n'aura pas de nombres à plus de dix chiffres pour les abs
      itoa(i, abscisse2String, 10);
      setcolor(178, 217, 206 );
      outtextxy(normalize(i,xmin, xmax),normalize(0,ymin, ymax),abscisse2String, 1);
    }

    //Lignes horizontales
    for(int i = round(ymin)-1 ; i < ymax ; i ++){
      setcolor(150, 164, 173);
      line(1, normalize(i,ymin, ymax), -1 , normalize(i, ymin,ymax));
      char ord2String[10]; //pour convertir les int en string, on suppose qu'on n'aura pas de nombres à plus de dix chiffres pour les ord
      itoa(i, ord2String, 10);
      setcolor(178, 217, 206 );
      outtextxy(normalize(0,xmin, xmax),normalize(i,ymin, ymax),ord2String, 1);
    }

    //Génération des axes principaux
    setcolor(242, 210, 221);
    line(1,normalize(0,ymin, ymax),-1,normalize(0,ymin, ymax));
    line(normalize(0,xmin, xmax),-1,normalize(0,xmin, xmax),1);

}



/**
* myDraw
*
* Proc�dure
*
*/
void myDraw(void)
{
  /*
  colors:
   - beige : 240, 232, 218
   - pale rose : 242, 210, 221
   - rose / red : 201, 132, 146
   - blue / green : 131, 181, 177
   - pale green : 178, 217, 206
   - grey / black : 72, 78, 85
   - light grey : 150, 164, 173
  */

  //Mise en place du fond
  setcolor(72, 78, 85);
  bar(-1.0F,-1.0F,1.0F,1.0F);

  //Génération des variables
  int i = 0;
  int nbDepoints = int((xmax - xmin)/pas);

  float treshold = 1.05*pas; //marge pour les erreurs dues au float
  int nbDePtsGenere;


  //init tableau valeur
  tableau_evaluateur * tab_eva = evaluate(xmin,xmax,nbDepoints,arbre, &nbDePtsGenere);

  //Affichage du grid
  grid(pas, tab_eva);


  //Trace de la courbe
  setcolor(255,255,255);

  while(i<nbDePtsGenere - 1){
    //Valeurs qui ne sont pas définies
    if(((tab_eva[i+1].x < tab_eva[i].x + treshold) == 0)){
      //Solution pour les lignes infinis, on simule les infinis avec -1000 et 1000 valeurs suffisamment grandes
      if(tab_eva[i].y < tab_eva[i-1].y) 
      {
        line(normalize(tab_eva[i].x , xmin,xmax), -1000, normalize(tab_eva[i].x, xmin, xmax),normalize(tab_eva[i].y, ymin, ymax));
        i+=2;
        if(tab_eva[i].y < tab_eva[i-1].y) //si point plus petit que celui le précédant, alors droite vers le bas
          line(normalize(tab_eva[i].x , xmin,xmax), 1000, normalize(tab_eva[i].x, xmin, xmax),normalize(tab_eva[i].y, ymin, ymax));
        else //et inversement, droite vers le haut
          line(normalize(tab_eva[i].x , xmin,xmax), -1000, normalize(tab_eva[i].x, xmin, xmax),normalize(tab_eva[i].y, ymin, ymax));


      }
      else
      {
        line(normalize(tab_eva[i].x, xmin,xmax), 1000, normalize(tab_eva[i].x, xmin, xmax),normalize(tab_eva[i].y, ymin, ymax));
        i+=2;
        if(tab_eva[i].y < tab_eva[i-1].y) //si point plus petit que celui le précédant, alors droite vers le bas
          line(normalize(tab_eva[i].x , xmin,xmax), 1000, normalize(tab_eva[i].x, xmin, xmax),normalize(tab_eva[i].y, ymin, ymax));
        else //et inversement, droite vers le haut
          line(normalize(tab_eva[i].x , xmin,xmax), -1000, normalize(tab_eva[i].x, xmin, xmax),normalize(tab_eva[i].y, ymin, ymax));

      }
    }

    //Trace de la courbe
    line(normalize(tab_eva[i].x, xmin, xmax), normalize(tab_eva[i].y, ymin, ymax), normalize(tab_eva[i+1].x, xmin, xmax),normalize(tab_eva[i+1].y, ymin, ymax));
    i++;
  }


  // Trace un titre
    setcolor(242, 210, 221);
    bar(-0.7F,0.95F,0.7F,0.85F);
    setcolor(0,0,0);
    outtextxy(-0.05,0.88,input, 0);


}




/**
* main
*
* La fonction principale avec deux arguments permettant de r�cup�rer les �l�ments en ligne de comment.
*
* Dans  cet  exemple  les  fonctions  (dites  callback)  myDraw  et  myKey  sont  install�es  ici  par
* l'appel  InitGraph  en  tant  que fonctions  r�agissantes  aux  �v�nements  de  "re-dessinage"  (pour  myDraw)
* et  aux  �v�nements  d'appui  sur  une  touche  du
* clavier (myKey).
�
* @parma ac : nombre de parametres
* @parma av : tableau contenant les parametres
*
*/
int main(int ac,char *av[])
{
  printf("\n \n" );
  printf("GRAPHEUR 0.2.3\n \n");
  printf("Comamande : \nz,q,s,d : déplacement du graphe\n" );
  printf("a,e : zoom/dézoom \n" );
  printf("n : nouveau graphe\n" );
  printf("esc : quitter le programme\n" );
  printf("\n");
  printf("NOTICE : Il convient de rajouter entre parenthèses les paramètres des fonctions. \nPar exemple : sin(3+x), exp(2*x) \n");
  printf("La fonction valeur négative est représentée par un underscore '_',\net doit être utilisée comme une fonction avec des parenthèses\n");
  printf("Exemple : _(x) pour -x\n");


  flag = 1;
  while(flag)
  flag = setup_input();

  InitGraph(ac,av,"MADOGEBRA",1280,720,myDraw,myKey);


  return 0;

}
