/*
Class Name: ban.h
Description: Permette di gestire il filtraggio delle parole
Version: 1.0, (02.07.22)
*/

#ifndef BAN_H_
#define BAN_H_
#include <stdio.h>
#include <stdlib.h>
#include "wordcomplex.h"
#include "wordWithInfo.h"
#include "list.h"
#include <stdbool.h>

/* ----------------- */
/* STRUCT DEFINITION */
/* ----------------- */
typedef struct ban {
    //Vettori di booleani, se 1 significa che la lettere e' bannata, se e' 0 non e' bannata
    int minuscoleBan[26];
    int MaiuscoleBan[26];
    int numberBan[10];
    int dashBan; // '-'
    int underscoreBan;// '_'

    //Permettono di capire il numero esatto di volte che la lettere deve comparire nella parola
    //=> se minuscoleNeededCount[0]=3 significa che nelle lettere dalla parola ci devono essere sicuramente 3 'a' minuscole
    //NB: TUTTO VIENE INIZIALIZZATO A 0 MA !NON! SIGNIFICA CHE L'OCCORRENZA DI TALE LETTERA DEVE ESSERE 0, per quello chi pensa minuscoleBan,MaiuscoleBan, etc..
    int minuscoleNeededCount[26];
    int MaiuscoleNeededCount[26];
    int numberNeededCount[10];
    int dashNeededCount; // '-'
    int underscoreNeededCount;// '_'

    //Dicono il numero minimo di occorrenze che ci devono essere per una lettere
    int minuscoleMinimunCount[26];
    int MaiuscoleMinimunCount[26];
    int numberMinimunCount[10];
    int dashMinimunCount; // '-'
    int underscoreMinimunCount;// '_'

    int doWeHaveAtLeatOneMinimumCount;
    int doWeHaveAtLeatOnePerfectCount;//Di default e' a 0, cosi' evito di controllare tutti gli array dei NeededCount per poi scoprire che sono tutti a 0
    int doWeHaveAtLeatOnePerfectPosition;
    int doWeHaveAtLeatOneBanPosition;

    char *perfectPosition;
    bool (*banPosition)[64]; //Matrice [X][64]
    int wordLength;

    list *lettereAttive;
}ban;


/* ----------------- */
/* DECLARATION */
/* ----------------- */
ban* ban_init(int wordLength);
void ban_banletter(ban* b, char lettera);
int* ban_getAdrOfCell_arrayBan(ban *b, char letter);
int* ban_getAdrOfCell_arrayNeededCount(ban *b, char letter);
//int ban_isThisWordOKv2(ban *b, char* wor, int wordLength,wordWithInfo* wi);
int ban_isThisWordOK(ban *b, char* word, int wordLength,wordWithInfo* wi);
void ban_setPerfectCount(ban *b, char letter, int count);
int* ban_getAdrOfCell_arrayMinimumCount(ban *b, char letter);
void ban_setMinimumCount(ban *b, char letter, int count);
void ban_increaseByOneMinimumCount(ban *b, char letter);
void ban_setPerfectPos(ban *b, char lettera, int pos);
void ban_setBanPos(ban *b, char lettera, int pos);
int ban_getCountPerfectPos(ban *b, char lettera);
int ban_isOkToThisLetterToBeHere(ban *b, char lettera,int pos,wordWithInfo* wi);

void ban_restart(ban *b, int wordLength);
void ban_insertInActiveLetter(ban* b, char letter);

/* ----------------- */
/* IMPLEMENTATION */
/* ----------------- */
ban* ban_init(int wordLength){
    ban *b = malloc(sizeof(ban)); //linked list
    b->perfectPosition=malloc(wordLength*sizeof(char));

    b->banPosition=malloc(wordLength*sizeof(*b->banPosition));

    b->lettereAttive=NULL;

    ban_restart(b,wordLength);

    return b;
}

void ban_banletter(ban *b,char letter){
    (*ban_getAdrOfCell_arrayBan(b,letter))=1;
}

int* ban_getAdrOfCell_arrayBan(ban *b, char letter){
    if(letter>='0' && letter<='9')
        return &b->numberBan[letter-'0'];
    else if(letter>='A' && letter<='Z')
        return &b->MaiuscoleBan[letter-'A'];
    else if(letter>='a' && letter<='z')
        return &b->minuscoleBan[letter-'a'];
    else if(letter=='-')
        return &b->dashBan;
    else if(letter=='_')
        return &b->underscoreBan;

    return NULL; //Carattere non riconosciuto
}
int* ban_getAdrOfCell_arrayNeededCount(ban *b, char letter){
    if(letter>='0' && letter<='9')
        return &b->numberNeededCount[letter-'0'];
    else if(letter>='A' && letter<='Z')
        return &b->MaiuscoleNeededCount[letter-'A'];
    else if(letter>='a' && letter<='z')
        return &b->minuscoleNeededCount[letter-'a'];
    else if(letter=='-')
        return &b->dashNeededCount;
    else if(letter=='_')
        return &b->underscoreNeededCount;

    return NULL; //Carattere non riconosciuto
}

int* ban_getAdrOfCell_arrayMinimumCount(ban *b, char letter){
    if(letter>='0' && letter<='9')
        return &b->numberMinimunCount[letter-'0'];
    else if(letter>='A' && letter<='Z')
        return &b->MaiuscoleMinimunCount[letter-'A'];
    else if(letter>='a' && letter<='z')
        return &b->minuscoleMinimunCount[letter-'a'];
    else if(letter=='-')
        return &b->dashMinimunCount;
    else if(letter=='_')
        return &b->underscoreMinimunCount;

    return NULL; //Carattere non riconosciuto
}

 int ban_isThisWordOK(ban *b, char* wor, int wordLength,wordWithInfo* wi){
    //Controllo il numero perfetto di occorrenze che ci devono essere in una parola per ogni lettere
    if(b->doWeHaveAtLeatOneMinimumCount!=0) {
        char letteraAnalysing;
        listNode* n=b->lettereAttive->head;
        int occ;

        //Scorro tutte le lettere "attive" di cui so che c'e' un vincolo su perfectCount o su minimumCount
        while(n!=NULL){
            letteraAnalysing = n->key;
            occ=*ban_getAdrOfCell_arrayMinimumCount(b,letteraAnalysing);
            if((occ!=0 && occ > *wordWithInfo_getAdrOfCell(wi,letteraAnalysing)) ){
                return 0;
            }

            n=n->next;
        }

    }

    return 1;
}





void ban_setPerfectCount(ban *b, char letter, int countd){
    b->doWeHaveAtLeatOnePerfectCount=1;
    int x = *(ban_getAdrOfCell_arrayNeededCount(b,letter));
    if(countd>x)
        *(ban_getAdrOfCell_arrayNeededCount(b,letter)) =countd;
}

void ban_setMinimumCount(ban *b, char letter, int countd){
    b->doWeHaveAtLeatOneMinimumCount=1;
    int x = *(ban_getAdrOfCell_arrayMinimumCount(b,letter));
    if(countd>x)//Aggiorno il minimo solo se trovo un minimo piu' stringente
        *(ban_getAdrOfCell_arrayMinimumCount(b,letter))=countd;
}
void ban_increaseByOneMinimumCount(ban *b, char letter){
    b->doWeHaveAtLeatOneMinimumCount=1;
    *(ban_getAdrOfCell_arrayMinimumCount(b,letter))+=1;
}

void ban_setPerfectPos(ban *b, char lettera, int pos){
    b->perfectPosition[pos]=lettera;
    b->doWeHaveAtLeatOnePerfectPosition=1;
    //ban_increaseByOneMinimumCount(b,lettera);
}
void ban_setBanPos(ban *b, char lettera, int pos){
    b->banPosition[pos][ALFABETO_POS[(int)lettera]]=true; //Associazione con ASCII
    b->doWeHaveAtLeatOneBanPosition=1;
}

int ban_getCountPerfectPos(ban *b, char lettera){
    int ris=0;
    for(int i=0; i<b->wordLength;i++)
        if(b->perfectPosition[i]==lettera)
            ris++;
    return ris;
}
int ban_isOkToThisLetterToBeHere(ban *b, char lettera, int pos,wordWithInfo* wi){
    //Controllo se la lettera puo' comparire
    if((*ban_getAdrOfCell_arrayBan(b,lettera))!=0) {

        //printf("Bannata Lettera1: %c\n",lettera);
        return 0;
    }

    //Controllo sulla posizione BAN
    if(b->doWeHaveAtLeatOneBanPosition!=0){
        if(b->banPosition[pos][ALFABETO_POS[(int)lettera]]==true){
          //  printf("Bannata Lettera2: %c\n",lettera);
            return 0;
        }
    }


    //Controllo sulla posizione PERFECT
    if(b->doWeHaveAtLeatOnePerfectPosition!=0){
        if(b->perfectPosition[pos]!=' ' && b->perfectPosition[pos]!=lettera){
           // printf("Bannata Lettera3: %c\n",lettera);
            return 0;
        }
    }

    //Se ho gia' un conteggio esatto
    if(b->doWeHaveAtLeatOnePerfectCount!=0){
        int occEsatte= *ban_getAdrOfCell_arrayNeededCount(b,lettera);
        //Se ho piu' lettere del numero di quelle esatte allora non ha senso continuare
        if(occEsatte!=0 && *wordWithInfo_getAdrOfCell(wi,lettera)+1>occEsatte){
            //printf("Bannata Lettera4: %c\n",lettera);
            return 0;
        }
    }

    return 1;
}

void ban_restart(ban* b, int wordLength){
    b->wordLength=wordLength;
    b->doWeHaveAtLeatOnePerfectPosition=0;
    b->doWeHaveAtLeatOneBanPosition=0;

    for(int i=0; i<wordLength;i++) { //Inizialmente non ho nessuna lettere in pos perfetta e nessuna lettere e' in pos ban
        b->perfectPosition[i] = ' ';
        for(int j=0; j<64;j++){
            b->banPosition[i][j]=false;
            ALFABETO_POS[(int) ALFABETO[j]]=j;
        }
    }


    for(int i=0; i<26; i++){
        b->MaiuscoleBan[i]=0;
        b->minuscoleBan[i]=0;
        b->MaiuscoleNeededCount[i]=0;
        b->minuscoleNeededCount[i]=0;
        b->MaiuscoleMinimunCount[i]=0;
        b->minuscoleMinimunCount[i]=0;
    }
    for(int i=0; i<10; i++){
        b->numberBan[i]=0;
        b->numberNeededCount[i]=0;
        b->numberMinimunCount[i]=0;
    }
    b->dashBan=b->underscoreBan=0;
    b->dashNeededCount=b->underscoreNeededCount=0;
    b->dashMinimunCount=b->underscoreMinimunCount=0;

    b->doWeHaveAtLeatOnePerfectCount=0;
    b->doWeHaveAtLeatOneMinimumCount=0;

    if(b->lettereAttive!=NULL){
        listNode *x= b->lettereAttive->head;
        listNode *tmp;
        while(x!=NULL){
            tmp = x->next;
            free(x);
            x=tmp;
        }
        free(b->lettereAttive);
        b->lettereAttive=NULL;
    }
}

void ban_insertInActiveLetter(ban* b, char letter){
    if(b->lettereAttive!=NULL){
        list_insertAhead(b->lettereAttive,letter);
    }else{
        b->lettereAttive= list_init(letter);
    }
}
#endif
