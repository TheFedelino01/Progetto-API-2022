
/*
Class Name: wordwithinfo.h
Description: Permette di memorizzare il numero di occorrenze di ogni lettera in una parola
Version: 1.0, (10.07.22)
*/

#ifndef WORDWITHINFO_H_
#define WORDWITHINFO_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *ALFABETO = "-0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz";
int ALFABETO_POS[255];

/* ----------------- */
/* STRUCT DEFINITION */
/* ----------------- */
typedef struct wordWithInfo {
    //int minuscoleCount[26];
   // int MaiuscoleCount[26];
   // int number[10];
   // int dash; // '-'
   // int underscore;// '_'
    int count[64];
}wordWithInfo;


/* ----------------- */
/* DECLARATION */
/* ----------------- */
wordWithInfo* wordWithInfo_init();
int* wordWithInfo_getAdrOfCell(wordWithInfo *w, char letter);

/* ----------------- */
/* IMPLEMENTATION */
/* ----------------- */
wordWithInfo* wordWithInfo_init(){

    wordWithInfo* a = malloc(sizeof(wordWithInfo)); //Alloco spazio per oggetto di tipo wordcomplex

    for(int i=0; i<64;i++){
        ALFABETO_POS[(int) ALFABETO[i]]=i;
        a->count[i]=0;
    }
    return a;
}

int* wordWithInfo_getAdrOfCell(wordWithInfo *w, char letter) {
    return &w->count[ALFABETO_POS[(int)letter]];
}

#endif




