

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define MAX_LIMIT 150 //taille maximum de la chaine de caractére en entrer

#include "analyse_lexical.h"

int estUnOperateur(char input){ //fonction testant si c'est un operateur
    if(input=='*' || input=='+' || input=='/' || input=='-' || input=='^')
    {return 1;}
    return 0;
}

int estUnEntier(char input){//fonction testant si c'est un entier
    if('0'<=input && '9'>=input )
    {return 1;}
    return 0;
}

int estUneVariable(char input){//fonction testant si c'est une variable
    if('x'==input || 'X'==input)
    {return 1;}
    return 0;
}

int estUneParenthese(char input){//fonction testant si c'est une parenthése,un crochet ou une barre absolue
    if('('==input || ')'==input || '['==input || ']'==input || '|'==input )
    {return 1;}
    return 0;
}

int estUneLettre(char input){//fonction testant si c'est une lettre
    if(('a'<=input && 'z'>=input )||('A'<=input && 'Z'>=input) )
    {return 1;}
    return 0;
}

int delimiteurConversion(char input){//fonction qui identifie les types de delimiteurs
    switch(input){
        case '(': return PAR_OUV;
        case ')': return PAR_FERM;
        case '[': return BAR_OUV;
        case ']': return BAR_FERM;
        case '|': return ABSOLU;
    }
    return -1;
}

int operatorConversion(char ch){//fonction qui identifie les differents opérateurs
    switch(ch){
        case '+': return PLUS;
        case '-': return MOINS;
        case '*': return FOIS;
        case '/': return DIV;
        case '^': return PUIS;
    }
    return -1;
}

int fonctionConversion(char * str){//fonction qui identifie les differentes fonctions mathématique
    if(!strcmp(str,"abs")) return ABS;
    else if(!strcmp(str,"sin")) return SIN;
    else if(!strcmp(str,"sqrt")) return SQRT;
    else if(!strcmp(str,"log")) return LOG;
    else if(!strcmp(str,"cos")) return COS;
    else if(!strcmp(str,"tan")) return TAN;
    else if(!strcmp(str,"exp")) return EXP;
    else if(!strcmp(str,"entier")) return ENTIER;
    else if(!strcmp(str,"_")) return VAL_NEG;
    else if(!strcmp(str,"sinc")) return SINC;
    else if(!strcmp(str,"ABS")) return ABS;
    else if(!strcmp(str,"SIN")) return SIN;
    else if(!strcmp(str,"SQRT")) return SQRT;
    else if(!strcmp(str,"LOG")) return LOG;
    else if(!strcmp(str,"COS")) return COS;
    else if(!strcmp(str,"TAN")) return TAN;
    else if(!strcmp(str,"EXP")) return EXP;
    else if(!strcmp(str,"ENTIER")) return ENTIER;
    else if(!strcmp(str,"SINC")) return SINC;
    return -1;
}


//fonction a retirer peux etre avant le rendus final
void display(typejeton * jetonTab){//fonction qui nous permet de visualiser les valeurs du tableaux de jetons *********
    for(int i = 0; jetonTab[i].lexem !=FIN ;i++){
        if(jetonTab[i].lexem == 0){
            printf("lexem : REEL  |  valeur : %f\n", jetonTab[i].valeur.reel);
        }
        else if(jetonTab[i].lexem == 1){
            printf("lexem : OPERATEUR  |  operateur : %d\n",jetonTab[i].valeur.operateur);
        }
        else if(jetonTab[i].lexem == 2){
            printf("lexem : FONCTION  |  fonction : %d\n",jetonTab[i].valeur.fonction);
        }
        else if(jetonTab[i].lexem == VARIABLE)
            printf("lexem : VARIABLE\n");
        else if(jetonTab[i].lexem == FIN){
            printf("lexem : FIN\n");
            break;
        }
        else{
            printf("lexem : %u \n",jetonTab[i].lexem);
        }
    }
}

Rapport fncfonction(typejeton* jeton, char* input,int indice,int *k,Rapport *rapport){//fonction qui permet creer un jeton de fonction
    int i=*(k);
    int debut=i;
    int fin=i;
    int virg=0;

    jeton[indice].lexem = FONCTION;
    i++; //c'est l'emplacement dans le input

    
    while(estUneLettre(input[i]) && !( (input[i] == 'x' || input[i] == 'X') && !(input[i-1] == 'e' || input[i-1] == 'E')) && input[i-1] != '_' ) {// permet d'identifier l'intervale a isoler dans l'input  *******

        fin++;
        i++;
    }

    int size=fin-debut+1;//taille de l'inverval

    char *fonct=malloc(size*sizeof(char));
    memset(fonct, 0, sizeof(fonct));

    int j=0; // emplacmeent pour la fonction
    for(int v=debut;v<=fin;v++){//copie de la fonction de l'input dans un nouveau tableau fonct
        fonct[j]=input[v];
        j++;
    }
    if (-1!=fonctionConversion(fonct)){
        jeton[indice].lexem=FONCTION;
        jeton[indice].valeur.fonction=fonctionConversion(fonct);
    }
    else{
        rapport->lexem = FONCTION;
        rapport->pos_debut = debut;
        rapport->pos_fin = fin;
        strcpy(rapport->desc,"la fonction '");
        strcat(rapport->desc, fonct);
        strcat(rapport->desc, "' n'existe pas");
        return *rapport;
    }
    *(k)=i;//mise a jour de l'indice de l'input
    return *rapport;
}

Rapport fncreel(typejeton* jeton, char* input,int indice,int *k, Rapport* rapport){// fonction qui permet de creer un jeton de réel
    int i=*(k);
    int debut=i;
    int fin=i;
    int virg=0;
    while(('0'<=input[i] && '9'>=input[i] )|| input[i]==',' || input[i]=='.'){// permet d'identifier l'intervale a isoler dans l'input


        if(virg==2){
            rapport->lexem = REEL;
            rapport->pos_debut = i;
            rapport->pos_fin = i;// permet de savoir si il y a une erreur de virgule
            strcpy(rapport->desc,"le réel a plusieurs virgules");
            *(k)=i;
            return *rapport;
        }

        if(input[i]==','){//remplace les virgules par des points
            input[i]='.';
        }

        if(input[i]=='.'){//Conteur du nombre de virgules
            virg++;
        }

        fin++;
        i++;
    int size=fin-debut+1;
        char *reel=malloc(size*sizeof(char));
        int j=0; // emplacmeent pour le reel
        for(int v=debut;v<=fin;v++){
            reel[j]=input[v];
            j++;
        }
        float value = atof(reel);//changement du type de variable de caractères à réel
        jeton[indice].lexem=REEL;
        jeton[indice].valeur.reel= value;
        *(k)=i;

    }
        return *rapport;
}


Rapport fncjeton(char* input ,typejeton* jeton , int taille){//fonction qui change le string input en tableau de jeton
	int i=0;//indice du string input
	int indice=0;//indice du tableau de jeton
    Rapport rapport;
    rapport.pos_debut = -1;
	while(i<taille && indice<taille && rapport.pos_debut == -1){

		if(input[i]==' '){
            i++;
			while(input[i]==' '){
				i++;
			}
            indice--; // Pour supprimer la case vide du tableau de jeton qui correspond à l'espace
		}
        else if(estUnOperateur(input[i])){
            jeton[indice].lexem=OPERATEUR;
            jeton[indice].valeur.operateur=operatorConversion(input[i]);
            ++i;
        }
		else if(estUnEntier(input[i])){
			rapport=fncreel(jeton,input,indice,&(i),&(rapport));
		}
		else if(estUneParenthese(input[i])){
            jeton[indice].lexem=delimiteurConversion(input[i]);
            ++i;
		}
        else if(estUneVariable(input[i]) && !estUneLettre(input[i+1])){
            jeton[indice].lexem = VARIABLE;
            ++i;
        }
		else if(estUneLettre(input[i]) || input[i] == '_'){
            rapport=fncfonction(jeton,input,indice,&(i),&(rapport));

		}
		else{

			      rapport.lexem = ERREUR;
            rapport.pos_debut = i;
            rapport.pos_fin = i;
            char c = input[i];
            strcpy(rapport.desc,"le caractère '");
            strncat(rapport.desc,&c,1);
            strcat(rapport.desc, "' n'est pas autorisé dans la fonction");


		}
        indice++;

    }

    jeton[indice].lexem=FIN;
    taille= indice+1;

	return rapport;
}
