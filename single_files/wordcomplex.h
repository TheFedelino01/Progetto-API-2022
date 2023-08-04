/*
Class Name: wordcomplex.h
Description: Permette di trattare parole in modo complesso
Version: 1.0, (02.07.22)
*/

#ifndef WORDCOMPLEX_H_
#define WORDCOMPLEX_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ----------------- */
/* STRUCT DEFINITION */
/* ----------------- */
typedef struct wordcomplex {
    char* parola;
    int wordLength;

    int minuscoleCount[26];
    int MaiuscoleCount[26];
    int number[10];
    int dash; // '-'
    int underscore;// '_'

    char* riscomparison;
}wordcomplex;


/* ----------------- */
/* DECLARATION */
/* ----------------- */
wordcomplex* wordcomplex_init(char* parola, int wordLength);
void wordcomplex_initializeArrays(wordcomplex* w);
int* wordcomplex_getAdrOfCell(wordcomplex *w, char letter);
char* wordcomplex_compare(wordcomplex* w, char* guessing);
void wordcomplex_printRisComparison(wordcomplex *x);

/* ----------------- */
/* IMPLEMENTATION */
/* ----------------- */
wordcomplex* wordcomplex_init(char* parola, int wordLength){

    wordcomplex* a = malloc(sizeof(wordcomplex)); //Alloco spazio per oggetto di tipo wordcomplex

    //Se mi specifica lui la lunghezza mi fido e la imposto
    if(wordLength!=-1)
        a->wordLength=wordLength;
    else
        a->wordLength=(int)strlen(parola); //La calcolo io perche' non me l'ha specificata

    a->riscomparison = malloc(a->wordLength*sizeof(char)); //Alloco spazio per la parola

    a->parola=parola;

    wordcomplex_initializeArrays(a);

    char letter;
    for(int i=0; i<a->wordLength; i++){
        letter=parola[i];
        (*wordcomplex_getAdrOfCell(a,letter))++;
        a->riscomparison[i]=' '; //Inizializzo anche il riscomparison con tutti spazi
    }

    return a;
}

void wordcomplex_initializeArrays(wordcomplex* w){
    for(int i=0; i<26; i++){
        w->minuscoleCount[i]=0;
        w->MaiuscoleCount[i]=0;
    }
    w->dash=w->underscore=0;
    for(int i=0; i<10;i++)
        w->number[i]=0;
}
int* wordcomplex_getAdrOfCell(wordcomplex *w, char letter){
    if(letter>='0' && letter<='9')
        return &w->number[letter-'0'];
    else if(letter>='A' && letter<='Z')
        return &w->MaiuscoleCount[letter-'A'];
    else if(letter>='a' && letter<='z')
        return &w->minuscoleCount[letter-'a'];
    else if(letter=='-')
        return &w->dash;
    else if(letter=='_')
        return &w->underscore;

    exit(0); //Carattere non riconosciuto
}

char* wordcomplex_compare(wordcomplex* w, char* guessing){

    //Controllo per posizionare i '+' quando le lettere sono uguali e nella stessa pos
    for(int i=0; i<w->wordLength; i++){
        if(w->parola[i]==guessing[i])
            w->riscomparison[i]='+';
        else
            w->riscomparison[i]=' ';
    }

    char letter;
    int countBefore,countReal,countOk;
    for(int i=0; i<w->wordLength; i++){
        countBefore=0,countReal=0,countOk=0;
        letter=guessing[i];
        //Per ogni cella non ancora assegnata (quindi per le celle diverse da '+')
        if(w->riscomparison[i]==' ') {
            //Se la lettere non compare in w
            countReal=*wordcomplex_getAdrOfCell(w, letter);
            if (countReal == 0) {
                //Non compare, quindi metto '/'
                w->riscomparison[i] = '/';
            } else {
                //La lettere compare in w
                //Conto il numero di volte che 'letter' compare in w con il segno '+'
                //Nel frattempo conto anche quanti segni != '+' compaiono prima della pos i
                for(int j=0; j<w->wordLength;j++)
                    //Conto i segni '+' per la 'letter'
                    if(w->riscomparison[j]=='+' && guessing[j]==letter) {
                        countOk++;
                    }else if(guessing[j]==letter && j<i){ //Mi segno quanti segni != '+' per la 'letter' compaiono in w prima della pos i
                        countBefore++;
                    }

                if(countBefore<countReal-countOk)
                    w->riscomparison[i] = '|';
                else
                    w->riscomparison[i] = '/';
            }
        }
    }
    return w->riscomparison;

}

void wordcomplex_printRisComparison(wordcomplex *x){
    printf("%.*s\n",x->wordLength,x->riscomparison);
}

#endif
