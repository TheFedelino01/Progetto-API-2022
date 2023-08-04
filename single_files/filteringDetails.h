/*
Class Name: filteringdetails.h
Description: Permette di analizzare una parola stabilendo i vari caratteri speciali
Version: 1.0, (02.07.22)
*/

#ifndef FILTERINGDETAILS_H_
#define FILTERINGDETAILS_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ----------------- */
/* STRUCT DEFINITION */
/* ----------------- */
typedef struct filteringdetails {
    char* parola;
    char* riscomparison;
    int wordLength;

    int plusminuscole[26];
    int verticalminuscole[26];
    int obliqueminuscole[26];

    int plusMaiuscole[26];
    int verticalMaiuscole[26];
    int obliqueMaiuscole[26];

    int plusNumber[10];
    int verticalNumber[10];
    int obliqueNumber[10];

    int plusdash, verticaldash, obliquedash;
    int plusunderscore, verticalunderscore, obliqueunderscore;

}filteringdetails;


/* ----------------- */
/* DECLARATION */
/* ----------------- */
filteringdetails* filteringdetails_init(char* parola, char* riscomparison, int wordLength);
void filteringdetails_initializeArrays(filteringdetails* w);
int* filteringdetails_getPlusAdrOfCell(filteringdetails *w, char letter);
int* filteringdetails_getVerticalAdrOfCell(filteringdetails *w, char letter);
int* filteringdetails_getObliqueAdrOfCell(filteringdetails *w, char letter);

/* ----------------- */
/* IMPLEMENTATION */
/* ----------------- */
filteringdetails* filteringdetails_init(char* parola,char* riscomparison, int wordLength){

    filteringdetails* a = malloc(sizeof(filteringdetails)); //Alloco spazio per oggetto di tipo wordcomplex

    a->wordLength=wordLength;

    a->parola=parola;
    a->riscomparison=riscomparison;

    filteringdetails_initializeArrays(a);

    for(int i=0; i<a->wordLength; i++){
        if(riscomparison[i]=='+'){
            *(filteringdetails_getPlusAdrOfCell(a,parola[i]))+=1;
        }else if(riscomparison[i]=='|'){
            *(filteringdetails_getVerticalAdrOfCell(a,parola[i]))+=1;
        }else if(riscomparison[i]=='/'){
            *(filteringdetails_getObliqueAdrOfCell(a,parola[i]))+=1;
        }
    }

    return a;
}

int* filteringdetails_getPlusAdrOfCell(filteringdetails *w, char letter){
    if(letter>='0' && letter<='9')
        return &w->plusNumber[letter-'0'];
    else if(letter>='A' && letter<='Z')
        return &w->plusMaiuscole[letter-'A'];
    else if(letter>='a' && letter<='z')
        return &w->plusminuscole[letter-'a'];
    else if(letter=='-')
        return &w->plusdash;
    else if(letter=='_')
        return &w->plusunderscore;

    exit(0); //Carattere non riconosciuto
}

int* filteringdetails_getVerticalAdrOfCell(filteringdetails *w, char letter){
    if(letter>='0' && letter<='9')
        return &w->verticalNumber[letter-'0'];
    else if(letter>='A' && letter<='Z')
        return &w->verticalMaiuscole[letter-'A'];
    else if(letter>='a' && letter<='z')
        return &w->verticalminuscole[letter-'a'];
    else if(letter=='-')
        return &w->verticaldash;
    else if(letter=='_')
        return &w->verticalunderscore;

    exit(0); //Carattere non riconosciuto
}
int* filteringdetails_getObliqueAdrOfCell(filteringdetails *w, char letter){
    if(letter>='0' && letter<='9')
        return &w->obliqueNumber[letter-'0'];
    else if(letter>='A' && letter<='Z')
        return &w->obliqueMaiuscole[letter-'A'];
    else if(letter>='a' && letter<='z')
        return &w->obliqueminuscole[letter-'a'];
    else if(letter=='-')
        return &w->obliquedash;
    else if(letter=='_')
        return &w->obliqueunderscore;

    exit(0); //Carattere non riconosciuto
}

void filteringdetails_initializeArrays(filteringdetails* w){
    for(int i=0; i<26; i++){
        w->plusMaiuscole[i]=0;
        w->plusminuscole[i]=0;

        w->verticalMaiuscole[i]=0;
        w->verticalminuscole[i]=0;

        w->obliqueMaiuscole[i]=0;
        w->obliqueminuscole[i]=0;
    }
    for(int i=0; i<10;i++){
        w->plusNumber[i]=0;
        w->verticalNumber[i]=0;
        w->obliqueNumber[i]=0;
    }
    w->plusdash=w->verticaldash=w->obliquedash=0;
    w->plusunderscore=w->verticalunderscore=w->obliqueunderscore=0;
}
#endif
