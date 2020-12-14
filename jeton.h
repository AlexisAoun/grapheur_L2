#ifndef JETON_H
#define JETON_H

#define MAX_DESC_ERREUR 500


//énumération des différents types de lexems
typedef enum
    {
    REEL,OPERATEUR,FONCTION,ERREUR,FIN,PAR_OUV,PAR_FERM,VARIABLE,BAR_OUV,BAR_FERM,ABSOLU
    }typelexem;

//énumération des diff types d'opérateurs
typedef enum
    {
    PLUS,MOINS,FOIS,DIV,PUIS
    }typeoperateur;

//énumération des diff types de fonctions
typedef enum
    {
    ABS,SIN,SQRT,LOG,COS,TAN,EXP,ENTIER,VAL_NEG,SINC
    }typefonction;

//énumération des diff types de valeurs
typedef union
    {
    float reel;
    typefonction fonction;
    typeoperateur operateur;
    }typevaleur;

//énumération des diff types de jetons
typedef struct
    {
    typelexem lexem;
    typevaleur valeur;
    }typejeton;

//déclaration de l'arbre
typedef struct Node
    {
    typejeton jeton;
    struct Node *pjeton_g;
    struct Node *pjeton_d;
    } Node;

typedef Node *Arbre;

typedef struct
  {
    typelexem lexem ;
    int pos_debut;
    int pos_fin;
    char desc [MAX_DESC_ERREUR];
  }Rapport;

  typedef struct tableau_evaluateur tableau_evaluateur;
  struct tableau_evaluateur{
      float x;
      float y;
  };

#endif
