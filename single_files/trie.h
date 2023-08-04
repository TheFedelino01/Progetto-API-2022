/*
Class Name: trie.h
Description: Permette di gestire un albero Trie per la memorizzazione delle parole lette in input da WordChecker
Version: 1.0, (01.07.22)
*/

#ifndef TRIE_H_
#define TRIE_H_
#include <stdio.h>
#include <stdlib.h>
#include "array.h"
#include "ban.h"
#include "filteringDetails.h"

/* ----------------- */
/* STRUCT DEFINITION */
/* ----------------- */

typedef struct trie {
    struct trieNode *head;
    ban* bansystem;
    int totalNodes;
    int compatibleWords;
}trie;

/* ----------------- */
/* DECLARATION */
/* ----------------- */
trie* trie_init(int wordLength);

trieNode* trie_newNode(char* key);
trieNode* trie_insertWord(trie* T, trieNode* node, char *word, int wordLength, int insertForInserisci_inizio);
void trie_filter(trie* T, int wordLength);
int trie_fiter_subproc(trie* T, trieNode* node, char* array, int wordLength, int index,wordWithInfo* wi); //Funzione chiamata unicamente da trie_print per implementare la stampa ricorsiva
void trie_print(trie* T, int wordLength);
void trie_print_subproc(trieNode* node, char* array, int index);

void trie_impostaFiltri(trie *T, char* parola, char* risComparison, int wordLength);
int trie_isThisWordPresentNoFiltering(trie *T, char* parola, int wordLength);

void trie_moveAllBanChildToUnBannedChild(trie *T, int wordLength);
void trie_moveAllBanChildToUnBannedChild_subproc(trieNode* node, int wordLength);
void trie_restart(trie *T, int wordLength);


/* ----------------- */
/* IMPLEMENTATION */
/* ----------------- */
trie* trie_init(int wordLength){
    trieNode *a;
    a = trie_newNode("@");//La testa della trie e' un trieNode chiamato '@', i suoi figli sono i nodi

    trie *l = malloc(sizeof(trie)); //linked list
    l->head = a;
    l->totalNodes=1;
    l->bansystem=ban_init(wordLength);

    return l;
}


trieNode* trie_newNode(char* key){
    trieNode *z;
    //z = malloc(sizeof(struct trieNode));
    z = trieNode_init(key);

    return z;
}

trieNode* trie_insertWord(trie* T, trieNode* node, char *word, int wordLength, int insertForInserisci_inizio){
    arnode *figliotmp;
    int posSearch;
    int figlioMatchingChar=0;
    bool foundInBanned=false;

    if(trieNode_getCountChild(node)!=0) {
        //Scorro i nodi del figlio e vedo se ne trovo uno che inizia con la stessa lettera di word
        posSearch = trieNode_searchByFirstLetter(node,word[0],0);
        if(posSearch!=-1){
            figlioMatchingChar=1;
        }else{
            figlioMatchingChar=0;
        }
    }


    if(insertForInserisci_inizio==1) {
        //Se non ho trovato nulla, cerco nei figli bannati
        if (figlioMatchingChar == 0) {
            if (trieNode_getCountBannedChild(node)!=0) {

                //Scorro i nodi del figlio e vedo se ne trovo uno che inizia con la stessa lettera di word
                posSearch = trieNode_searchByFirstLetter(node,word[0],1);
                if(posSearch!=-1){
                    figlioMatchingChar=1;
                    foundInBanned=true;
                }else{
                    figlioMatchingChar=0;
                }
            }
        }
    }
    if(figlioMatchingChar==0){
        //Non ho trovato nessun figlio del node che ha come prima lettera word[0]
        //Quindi, inserisco tutta la parola come mio figlio
        char childName[wordLength+1];
        strcpy(childName,word);
        childName[wordLength]='\0';

        T->totalNodes++;

        return trieNode_addChild(node,childName);

    }else{
        //Ho trovato un figlio che inizia con la stessa lettera di word (es. figlio: "abbb" e devo inserire "accc")
        //Capisco se e' una foglia oppure no
        figliotmp = &node->obj[posSearch];
        if(figliotmp->obj->maxSpace==0){
            //E' una foglia, allora lo diviso
            char preZero = figliotmp->obj->lettere[0];
            char suffix[wordLength];

            for(int i=0; i<wordLength-figlioMatchingChar;i++){
                suffix[i]=figliotmp->obj->lettere[i+figlioMatchingChar];
            }

            suffix[wordLength-figlioMatchingChar]='\0';

            figliotmp->obj->lettere=NULL;
            figliotmp->firstLetter=preZero;
            figliotmp->obj = trieNode__reallocateAndIncreaseSpace(figliotmp->obj, figliotmp->obj->maxSpace);


            figliotmp->obj = trieNode_addChild(figliotmp->obj,suffix);
            T->totalNodes++;

        }
        //else non e' una foglia, quindi non faccio niente
        figliotmp->obj = trie_insertWord(T,figliotmp->obj,word+figlioMatchingChar,wordLength-figlioMatchingChar,insertForInserisci_inizio);

        if(foundInBanned){
            trieNode_moveFromBanToUnban(node,posSearch);
        }
        return node;
    }
}


void trie_filter(trie* T, int wordLength) {
    char* array=malloc(sizeof(char)*wordLength+1);
    T->compatibleWords=0;
    wordWithInfo *wi = wordWithInfo_init();

    if(T!=NULL){
        trie_fiter_subproc(T, T->head,array,wordLength,0,wi);
    }

    free(wi);
    free(array);
}


int trie_fiter_subproc(trie* T, trieNode* node, char* array, int wordLength, int index, wordWithInfo* wi) {
    int indexNew = index;
    int returnFunction, stopFor = 0;
    int nodeWordLength;

    //Solo se  ho piu' di un carattere in lettere eseguo questo ciclo
    if (node->lettere!=NULL && node->lettere[0] != '@' && (node->maxSpace==0) ) {
        nodeWordLength = strlen(node->lettere);

        for (stopFor = 1; stopFor < nodeWordLength; stopFor++) {
            if (ban_isOkToThisLetterToBeHere(T->bansystem, node->lettere[stopFor], indexNew, wi) == 0) {
                returnFunction = 0;
                goto resetwi;
            }
            *wordWithInfo_getAdrOfCell(wi, node->lettere[stopFor]) += 1;

            array[index + stopFor-1] = node->lettere[stopFor];
            indexNew++;
        }

    }

    if (node->maxSpace==0) {
        array[indexNew] = '\0';

        if (ban_isThisWordOK(T->bansystem, array, wordLength, wi) == 1) {
            T->compatibleWords += 1;
            returnFunction = 1;
            goto resetwi;
        } else {
            returnFunction = 0;
            goto resetwi;
        }
    }

    int ris;
    arnode figliotmp;

    //CICLO 1
    //Controlla la prima lettera di tutti i figli
    //Cosi' elimino subito i figli che hanno la  prima lettera che non va bene (non me ne accorgo dopo aver fatto la chiamata ricorsiva)
    for(int i=0; i<=node->lastChildPos;i++){
        figliotmp = node->obj[i];
        ris = ban_isOkToThisLetterToBeHere(T->bansystem, figliotmp.firstLetter, indexNew, wi);


        if (ris == 0) {
            if (trieNode_getCountChild(node) != 1) {
                array[indexNew]='\0';
               // printf("Bannato1: %s | %c\n",array, figliotmp.firstLetter);
                trieNode_moveFromUnBanToBan(node, i);
                i--;
            } else {
                returnFunction = 0;
                goto resetwi;
            }
        }

    }


    //CICLO 2
    //Scorro i figli e i figli dei figli (cioe' considero i figli che hanno superato il test sulla prima lettera)
    for(int i=0; i<=node->lastChildPos;i++){
        figliotmp = node->obj[i];

        *wordWithInfo_getAdrOfCell(wi, figliotmp.firstLetter) += 1;
        array[indexNew] = figliotmp.firstLetter;

        ris = trie_fiter_subproc(T, figliotmp.obj, array, wordLength, indexNew+1, wi);

        *wordWithInfo_getAdrOfCell(wi, figliotmp.firstLetter) -= 1;


        if (ris == 0) { //Mi hanno segnalato che questo cammino non va piu' bene
            //Allora se sono un nodo che ha piu' figli, escludo questo cammino quindi tolgo questo nodo dai miei figli
            if (trieNode_getCountChild(node) != 1) {

               // printf("Bannato2: %s | %c\n",array, figliotmp.firstLetter);
                trieNode_moveFromUnBanToBan(node, i);

                i--;
            } else {
                returnFunction = 0;
                goto resetwi;
            }
        }
    }

    returnFunction = 1;

    resetwi:
    if (node->lettere!=NULL && node->lettere[0] != '@' && (node->maxSpace==0)) {
        for (int i = 1; i < stopFor; i++) {
            *wordWithInfo_getAdrOfCell(wi, node->lettere[i]) -= 1;
        }
    }
    return returnFunction;
}

void trie_print(trie* T, int wordLength) {
    char* array=malloc(sizeof(char)*wordLength+1);

    if(T!=NULL){
        trie_print_subproc(T->head,array,0);
    }

    free(array);
}

void trie_print_subproc(trieNode* node, char* array, int index) { //Stampo tutte le parole a partire da questo nodo
    int indexNew=index;
    if(node->lettere!=NULL && strcmp(node->lettere,"@")!=0) {
        for (int i = 1; i < strlen(node->lettere); i++) {
            array[index+i-1] = node->lettere[i];
            indexNew++;
        }
    }
    if(node->maxSpace==0){
        array[indexNew]='\0';
        printf("%s\n",array);
        return;
    }

    int figliNum= trieNode_getCountChild(node);

    for(int i=0; i<figliNum;i++){
        arnode figliotmp = node->obj[i];
        array[indexNew]=figliotmp.firstLetter; //Cosi' riesco a stampare i nodi che contengono un solo char perche' node->lettere sara' NULL
        trie_print_subproc(figliotmp.obj,array,indexNew+1);
    }

}


void trie_impostaFiltri(trie *T, char* parola, char* risComparison, int wordLength){

    // printf("Analizzo parola '%s'\n",parola);
    // printf("Ris comparison '%s\n",risComparison);
    char speciale,lettera;
    int MinimumCount, perfectCount;
    int minimo;
    int numOfPlus=0;
    int numOfVertical=0;
    for(int i=0; i<wordLength; i++) {
        speciale = risComparison[i];
        lettera=parola[i];
        if (speciale == '+') { //Dico che questa lettere ci deve essere
            ban_setPerfectPos(T->bansystem, lettera, i);
            // printf("0. Trovata posizione perfetta per lettere '%c' pos '%d'\n", lettere, i);
        }
    }

    numOfPlus=0;
    numOfVertical=0;
    filteringdetails *fil = filteringdetails_init(parola,risComparison,wordLength);

    for(int i=0; i<wordLength; i++){
        speciale = risComparison[i];
        lettera=parola[i];


        MinimumCount = *ban_getAdrOfCell_arrayMinimumCount(T->bansystem, lettera);
        perfectCount = ban_getCountPerfectPos(T->bansystem, lettera);
        numOfPlus= *filteringdetails_getPlusAdrOfCell(fil,lettera);
        numOfVertical=*filteringdetails_getVerticalAdrOfCell(fil,lettera);
        //6670235
        //15
        //7370777 /|++///

        if (speciale == '/') {
            if (MinimumCount == 0 && perfectCount == 0) { //Se la lettere non deve essere presente in assoluto e se la lettere non e' in una pos perfetta, significa che non ci deve essere
                ban_banletter(T->bansystem, lettera);


                //printf("1. Trovata lettere '%c' da bannare\n", lettere);
            }else{
                //Ho trovato il numero esatto per quella lettere

                //Inserisco nella lista solo se non l'ho gia' inserito
                ban_insertInActiveLetter(T->bansystem, lettera);

                ban_setPerfectCount(T->bansystem,lettera,numOfPlus+numOfVertical);
                ban_setMinimumCount(T->bansystem,lettera,numOfPlus+numOfVertical);

                ban_setBanPos(T->bansystem,lettera,i);
                //printf("2. Trovato numero esatto per lettere '%c' numEsatto: '%d'\n",lettere,numOfPlus+numOfVertical);
            }
        }else if(speciale=='|'){
            ban_setBanPos(T->bansystem,lettera,i);
            minimo=numOfPlus+numOfVertical;

            //Inserisco nella lista solo se non l'ho gia' inserito
            ban_insertInActiveLetter(T->bansystem, lettera);

            ban_setMinimumCount(T->bansystem,lettera,minimo);
        }

    }
    free(fil);
}


void trie_strcopy(char* dest, char* src, int startingFrom, int numChars){
    int i;
    for(i=0; i<numChars;i++){
        dest[i]=src[startingFrom+i];
    }
    dest[i]='\0';
}


//0: non e' presente la parola
//1: la parola e' presente
int trie_isThisWordPresentNoFiltering(trie *T, char* parola, int wordLength){
    trieNode* x = T->head;
    int searchPos;


    int offset=0, fine=0;
    if(T!=NULL && T->head!=NULL) {
        for (int i = 0;i<wordLength && fine==0;i++){

            searchPos = trieNode_searchByFirstLetter(x, parola[offset],0);
            if(searchPos!=-1){
                x=x->obj[searchPos].obj;
                offset++;
            }else{
                searchPos = trieNode_searchByFirstLetter(x, parola[offset],1);
                if(searchPos!=-1){
                    x=x->obj[searchPos].obj;
                    offset++;
                }else{
                    if(x->lettere!=NULL && strcmp(parola+offset,x->lettere+1)==0){
                        return 1;
                    }else{
                        return 0;
                    }
                }
            }

        }

    }
    return 1;
}

void trie_moveAllBanChildToUnBannedChild(trie* T, int wordLength) {
    trie_moveAllBanChildToUnBannedChild_subproc(T->head,wordLength);
}

void trie_moveAllBanChildToUnBannedChild_subproc(trieNode* node, int wordLength) {
    if(trieNode_getCountBannedChild(node)!=0) {
        for(int i=node->lastBanChildPos; i<node->maxSpace;i++){
            trieNode_moveFromBanToUnban(node, i);

        }
    }

    //Adesso, per ogni mio figli, sbanno eventuali sotto figli

    if(trieNode_getCountChild(node)!=0) {
        for(int i=0; i<=node->lastChildPos; i++){

            trie_moveAllBanChildToUnBannedChild_subproc(node->obj[i].obj, wordLength);
        }
    }

}

void trie_restart(trie *T, int wordLength){
    ban_restart(T->bansystem, wordLength);
    trie_moveAllBanChildToUnBannedChild(T,wordLength);
}





#endif
