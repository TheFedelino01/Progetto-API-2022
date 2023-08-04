/* solution_saccani_AA22.c
-------------------------------------------------------------------------------------------
Autore: Saccani Federico
PoliMi Student

Version: release, (18.07.22) 23:52

Soluzione completa e funzionante con votazione 30L per la Prova Finale di API AA 2021-2022.
File unico come da submit ufficiale, contenente tutto il codice.
-------------------------------------------------------------------------------------------
*/


/*
Class Name: trie.h
Description: Permette di gestire un albero Trie per la memorizzazione delle parole lette in input da WordChecker
Version: 1.0, (01.07.22)
*/

#ifndef TRIE_H_
#define TRIE_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
Class Name: array.h
Description: Permette di gestire un array
Version: 1.0, (14.07.22)
*/

#ifndef ARRAY_H_
#define ARRAY_H_
#define INITIALATION_ARRAY 0
#define CELLS_ALLOCATED 0
typedef struct arnode arnode;
typedef struct trieNode trieNode;

/* ----------------- */
/* STRUCT DEFINITION */
/* ----------------- */
typedef struct trieNode{
    char* lettere;
    arnode *obj;

    char lastChildPos;
    char maxSpace;
    char lastBanChildPos;
}trieNode;


typedef struct arnode {
    trieNode *obj;
    char firstLetter;
}arnode;


/* ----------------- */
/* DECLARATION */
/* ----------------- */

trieNode* trieNode_init(char* lettere);
trieNode* trieNode_initDetailed(char* lettere, int arraySpace);
trieNode*  trieNode_addChild(trieNode *node, char *lettere);
trieNode* trieNode_addChildByNode(trieNode *node, arnode banChild);
trieNode*  trieNode_addBanChildByObj(trieNode *node, arnode banChild);
trieNode* trieNode__reallocateAndIncreaseSpace(trieNode* node, int newArrayDimension);
trieNode* trieNode_moveFromUnBanToBan(trieNode *node, int pos);
trieNode* trieNode_moveFromBanToUnban(trieNode *node, int pos);
int trieNode_searchByFirstLetter(trieNode* node, char lettera, int inBannedChild);
int trieNode_givePosWhereToInsert(trieNode* node, char lettera);

void arnode_insertChildByKey(trieNode *node, char *lettere);
void arnode_insertChildByObj(trieNode *node, arnode child);
void arnode_insertToTail(trieNode* node, arnode banChild);

//EXTRA
void trieNode_print(trieNode *node);
int trieNode_getCountChild(trieNode *node);
int trieNode_getCountBannedChild(trieNode *node);
int trieNode_getNumElement(trieNode *node);

/* ----------------- */
/* IMPLEMENTATION */
/* ----------------- */
trieNode* trieNode_init(char* lettere){
    //Voglio allocare tutto lo spazio di tNode e dell'array di figli e lettere
    //in una posizione continua della memoria per minimizzare miss cache
    // trieNode* nodo = malloc(sizeof(trieNode)+sizeof(arnode*)*INITIALATION_ARRAY);
    return trieNode_initDetailed(lettere, INITIALATION_ARRAY);
}

//Permette di specificare la dimensione dell'array dei figli
trieNode* trieNode_initDetailed(char* lettere, int arraySpace){
    //Voglio allocare tutto lo spazio di tNode e dell'array di figli
    //in una posizione continua della memoria per minimizzare miss cache

    int wordLengthmio=0;
    trieNode* nodo;
    if(lettere!=NULL) {
        wordLengthmio = strlen(lettere);
        nodo = malloc(sizeof(trieNode) + sizeof(arnode) * arraySpace + wordLengthmio * sizeof(char) + sizeof(char));
    }else{
        nodo = malloc(sizeof(trieNode) + sizeof(arnode) * arraySpace);
    }



    nodo->obj=(arnode*)(nodo+1);

    nodo->maxSpace=arraySpace;

    if(lettere!=NULL)
        nodo->lettere=(char*)(nodo->obj+arraySpace);
    else
        nodo->lettere=NULL;

/*
     printf("POS_NODE: %lx\n",nodo);
     printf("POS_NODE_OBJ: %lx\n",nodo->obj);
     printf("arnode_SIZE: %lx\n",sizeof(arnode));
     printf("NODO->LETTERE: %lx\n",nodo->lettere);
*/
    if(lettere!=NULL) {
        for (int i = 0; i < wordLengthmio; i++) {
            nodo->lettere[i] = lettere[i];
        }
        nodo->lettere[wordLengthmio] = '\0';
    }

    //nodo->lettere=lettere;
    nodo->lastBanChildPos=-1;
    nodo->lastChildPos=-1;

    return nodo;
}

trieNode* trieNode_addChild(trieNode *node, char *lettere){

    if(trieNode_getNumElement(node)+1>node->maxSpace){
        //Non ho piu' spazio per allocare figli, rialloco tutto ed aumento
        //la dimensione dell'array
        trieNode* nuovo = trieNode__reallocateAndIncreaseSpace(node, node->maxSpace!=0? node->maxSpace * 2 : 1);

        return trieNode_addChild(nuovo,lettere);
    }else{
        //Inserimento ordinato
        arnode_insertChildByKey(node, lettere);
        return node;
    }

}
trieNode* trieNode_addChildByNode(trieNode *node, arnode banChild){

    if(trieNode_getNumElement(node)+1>node->maxSpace){
        trieNode* nuovo = trieNode__reallocateAndIncreaseSpace(node, node->maxSpace!=0? node->maxSpace * 2 : 1);
        return trieNode_addChildByNode(nuovo,banChild);
    }else{
        arnode_insertChildByObj(node, banChild);
        return node;
    }

}

trieNode* trieNode_addBanChildByObj(trieNode *node, arnode banChild){

    if(trieNode_getNumElement(node)+1>node->maxSpace){
        trieNode* nuovo = trieNode__reallocateAndIncreaseSpace(node, node->maxSpace!=0? node->maxSpace * 2 : 1);
        return trieNode_addBanChildByObj(nuovo, banChild);
    }else{
        //Inserimento in coda
        arnode_insertToTail(node, banChild);
        return node;
    }
}



void arnode_insertToTail(trieNode* node, arnode banChild){

    if(node->lastBanChildPos==-1){
        node->obj[node->maxSpace-1]=banChild;
        node->lastBanChildPos=node->maxSpace-1;
    }else{
        //Ci sono altri figli bannati
        node->obj[(int)--node->lastBanChildPos]=banChild;
    }

}



trieNode* trieNode__reallocateAndIncreaseSpace(trieNode* node, int newArrayDimension){
    trieNode *nuovo = trieNode_initDetailed(node->lettere, newArrayDimension);
    nuovo->maxSpace=newArrayDimension;

    for(int i=0; i<=node->lastChildPos;i++){
        nuovo->obj[i]=node->obj[i];
    }

    int j=0;
    if(node->lastBanChildPos!=-1) {
        //Scorro array ban child vecchio e lo copio in fondo all'array nuovo
        for (int i = node->maxSpace - 1; i >= node->lastBanChildPos; i--) {
            nuovo->obj[nuovo->maxSpace - 1 - j] = node->obj[i];
            j++;
        }
        nuovo->lastBanChildPos=nuovo->maxSpace - j;
    }else{
        nuovo->lastBanChildPos=-1;
    }


    nuovo->lastChildPos=node->lastChildPos;

    free(node);
    return nuovo;
}

trieNode* trieNode_moveFromUnBanToBan(trieNode *node, int pos){
    arnode ris;

    ris=node->obj[pos];

    //Shift left degli elementi successivi
    for(int j=pos;j<node->lastChildPos;j++){
        node->obj[j]=node->obj[j+1];
    }
    node->lastChildPos--;

   /* if(node->lastChildPos-1==0){
        node->lastChildPos=-1;
    }else{
        node->lastChildPos--;
    }*/

    //Aggiungo il nodo da spostare a quelli bannati
    return trieNode_addBanChildByObj(node, ris);

}

trieNode* trieNode_moveFromBanToUnban(trieNode *node, int pos){
    arnode ris;

    ris=node->obj[pos];
    //Shift right
    for(int j=pos;j>node->lastBanChildPos;j--){
        node->obj[j]=node->obj[j-1];
    }


    if(node->lastBanChildPos+1==node->maxSpace){
        node->lastBanChildPos=-1;
    }else{
        node->lastBanChildPos++;
    }

    return trieNode_addChildByNode(node, ris);

}



int trieNode_searchByFirstLetter(trieNode* node, char lettera, int inBannedChild){
    int inizio;
    int fine;
    int m;

    if(inBannedChild==0) {
        inizio=0;
        fine = node->lastChildPos;
        //Ricerca Binaria
        while(inizio<=fine){
            m = (fine+inizio)/2;
            if(node->obj[m].firstLetter==lettera){
                return m;
            }else{
                if(node->obj[m].firstLetter>lettera){
                    fine=m-1;
                }else{
                    inizio=m+1;
                }
            }
        }

    }else{
        fine = node->maxSpace-1;
        inizio = node->lastBanChildPos;

        //Ricerca Lineare
        for(int i= inizio; i<=fine; i++){
            if(node->obj[i].firstLetter==lettera)
                return i;
        }
    }

    return -1;
}

int trieNode_givePosWhereToInsert(trieNode* node, char lettera){
    //Restituisce la posizione in cui bisogna inserire l'elemento Child (lavora solo con i figli NON bannati)
    int inizio;
    int fine;

    inizio=0;
    fine = node->lastChildPos;


    int m;

    while(inizio<=fine){
        m = (fine+inizio)/2;
        if(node->obj[m].firstLetter==lettera){
            return m;
        }else{
            if(node->obj[m].firstLetter>lettera){
                fine=m-1;
            }else{
                inizio=m+1;
            }
        }
    }


    return inizio;
}


void trieNode_print(trieNode *node){
    printf("Node: %s | Element: %d, MaxElement:%d\n",node->lettere,trieNode_getNumElement(node),node->maxSpace);
    printf("\tNum Child:%d  |  Num BanChild:%d\n",trieNode_getCountChild(node),trieNode_getCountBannedChild(node));
    for(int i=0; i<trieNode_getNumElement(node);i++){
        printf("\t %s |  %c\n",node->obj[i].obj->lettere,node->obj[i].firstLetter);
    }
}

int trieNode_getCountChild(trieNode *node){
    return node->lastChildPos+1;
}

int trieNode_getCountBannedChild(trieNode *node){
    return node->lastBanChildPos==-1 ? 0 : node->maxSpace - node->lastBanChildPos;
}

int trieNode_getNumElement(trieNode *node){
    return trieNode_getCountChild(node)+trieNode_getCountBannedChild(node);
}

void arnode_insertChildByKey(trieNode *node, char *lettere){
    trieNode* nuovo  = trieNode_init(lettere);
    arnode in;
    in.firstLetter=lettere[0];
    in.obj=nuovo;

    arnode_insertChildByObj(node, in);
}

void arnode_insertChildByObj(trieNode *node, arnode child){
    int fine = node->lastChildPos;

    int pos = trieNode_givePosWhereToInsert(node, child.firstLetter);

    //Shift right
    for(int j=fine;j>=pos;j--){
        node->obj[j+1]=node->obj[j];
    }

   /* printf("POS: %d\n",pos);
    printf("POS_NODE: %lx\n",node);
    printf("POS_NODE_OBJ: %lx\n",node->obj);
    printf("MAX_SIZE: %d\n",node->maxSpace);
    printf("trieNode_SIZE: %lx\n",sizeof(trieNode));*/
    node->obj[pos]=child;
    node->lastChildPos++;
}



#endif

/*
Class Name: ban.h
Description: Permette di gestire il filtraggio delle parole
Version: 1.0, (02.07.22)
*/

#ifndef BAN_H_
#define BAN_H_
/*
Class Name: wordcomplex.h
Description: Permette di trattare parole in modo complesso
Version: 1.0, (02.07.22)
*/

#ifndef WORDCOMPLEX_H_
#define WORDCOMPLEX_H_

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


/*
Class Name: wordwithinfo.h
Description: Permette di memorizzare il numero di occorrenze di ogni lettera in una parola
Version: 1.0, (10.07.22)
*/

#ifndef WORDWITHINFO_H_
#define WORDWITHINFO_H_

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





/*
Class Name: list.h
Description: Permette di gestire una Doubly Linked List
Version: 1.0, (01.07.22)
*/

#ifndef LIST_H_
#define LIST_H_

/* ----------------- */
/* STRUCT DEFINITION */
/* ----------------- */
typedef struct listNode {
    int key;
    struct listNode *next;
    struct listNode *prev;
}listNode;

typedef struct list {
    struct listNode *head;
    int numElement;
}list;


/* ----------------- */
/* DECLARATION */
/* ----------------- */
list* list_initEmpty();
list* list_init(int key);
listNode* list_newNode(int key);
listNode* list_search(list* l, int key);
listNode* list_insertAhead(list *l, int key);
void list_delete(list *l, listNode* toRemove);
void list_deleteBySearch(list *l, int key);
void list_print(list *l);

/* ----------------- */
/* IMPLEMENTATION */
/* ----------------- */
list* list_initEmpty(){
    list *l = malloc(sizeof(list)); //linked list
    l->head = NULL;
    l->numElement=0;
    return l;
}

list* list_init(int key){
    listNode *a;
    a = list_newNode(key);

    list *l = malloc(sizeof(list)); //linked list
    l->head = a;
    l->numElement=1;

    return l;
}

listNode* list_newNode(int key){
    listNode *z;
    z = malloc(sizeof(struct listNode));
    z->next = z->prev = NULL;
    z->key=key;

    return z;
}

listNode* list_search(list* l, int key){
    listNode* x=l->head;
    while(x!=NULL && x->key!=key)
        x=x->next;
    return x;
}

listNode* list_insertAhead(list *l, int key) {
    if(l->head !=NULL  && l->head->key==key){
        return NULL;
    }

    listNode * n= list_newNode(key);
    n->next = l->head;
    if(l->head != NULL)
        l->head->prev = n;
    l->head = n;
    n->prev=NULL;
    l->numElement++;

    return n;
}

void list_delete(list *l, listNode* toRemove){
    if(toRemove!=NULL) {
        if (toRemove->prev != NULL)
            toRemove->prev->next = toRemove->next;
        else
            l->head = toRemove->next;
        if (toRemove->next != NULL)
            toRemove->next->prev = toRemove->prev;
        free(toRemove);

        l->numElement--;
    }
}

void list_deleteBySearch(list *l, int key){
    listNode *toRemove = list_search(l,key); //Effettuo la ricerca dell'elemento da cancellare
    if(toRemove!=NULL)
        list_delete(l,toRemove);
}

void list_print(list *l){
    listNode *x = l->head;
    while(x != NULL) {
        printf("%d\t", x->key);
        x = x->next;
    }
    printf("\n");
}
#endif

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
/*
Class Name: filteringdetails.h
Description: Permette di analizzare una parola stabilendo i vari caratteri speciali
Version: 1.0, (02.07.22)
*/

#ifndef FILTERINGDETAILS_H_
#define FILTERINGDETAILS_H_

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

#define SIZE_OF_SPECIAL_REQUEST 20 //Cosi posso leggere anche le parole del tipo "+stampa_filtrate"

int startGame();
void restartGame();
void inserisciNuoveParole(int doINeedToFilterTheNewWords);

trie *albero;
int wordSize;
bool filtraPrimaDiStampare=false;

int main() {
    //begin=clock();

    //Fase preparativa del gioco
    //printf("Inserire lunghezza parole: ");
    int risInputWordSize = scanf("%d",&wordSize);

    albero = trie_init(wordSize);

    char *parolaInput;
    if(wordSize>SIZE_OF_SPECIAL_REQUEST){
        parolaInput = malloc((wordSize+1)*sizeof(char));
    }else{
        parolaInput = malloc(SIZE_OF_SPECIAL_REQUEST*sizeof(char));
    }

    int keepPlaying=1;

    //Inizializzazione del dizionario e del game generale
    //Inserisce le parole basi, poi quando legge "+nuova_partita" inizia la partita

    while(keepPlaying) {
        if (risInputWordSize == 1) {
            //Leggo il vocabolario
            //printf("[+]Parola vocabolario: ");
            if(scanf("%s", parolaInput)!=EOF) {
                if (strcmp( parolaInput, "+nuova_partita") != 0) {
                    if(strcmp( parolaInput, "+inserisci_inizio") != 0) {
                        albero->head = trie_insertWord(albero, albero->head, parolaInput, wordSize, 0);
                    }else{
                        inserisciNuoveParole(0); //Inserisco le nuove parole ma non le filtro perche' la partita e' appena iniziata e non ci sono filtri
                        filtraPrimaDiStampare=true;
                    }
                } else {
                    //Inizia il gioco
                    startGame();

                    //Il gioco e' "terminato"
                    keepPlaying = 0;
                }
            }else{
                exit(0);
            }
        }
    }

    restartGame();
    keepPlaying=1;

    //La prima partita e' finita, ora puo' iniziarne un altra o inserire nuove parole per poi iniziare una partita nuova
    while(keepPlaying) {
        if(scanf("%s", parolaInput) != EOF) {
            if (strcmp((const char *) parolaInput, "+nuova_partita") == 0) {
                keepPlaying= startGame();
                if(keepPlaying==1)
                    restartGame();
            } else if (strcmp((const char *) parolaInput, "+inserisci_inizio") == 0) {
                inserisciNuoveParole(0); //Inserisco le nuove parole ma non le filtro perche' la partita e' appena iniziata e non ci sono filtri
                filtraPrimaDiStampare=true;
            }
        }else{
            exit(0);
        }
    }


    return 0;
}

//1: Wants to keep playing
//0: stop playing
int startGame(){
    char parolaDaIndovinare[wordSize+1];
    char *risComparison=NULL;

    //La parolaInput, non e' detto che sia lunga esattamente wordSize.. Se inserisce "+inserisci_inizio" o simili, la dim e' diversa
    char *parolaInput;
    if(wordSize>SIZE_OF_SPECIAL_REQUEST){
        parolaInput = malloc((wordSize+1)*sizeof(char));
    }else{
        parolaInput = malloc(SIZE_OF_SPECIAL_REQUEST*sizeof(char));
    }

    int numTentativi;
    int won=0;

   // printf("[!]Parola da indovinare: ");
    if(scanf("%s",parolaDaIndovinare)==EOF){
        exit(0);
    }
    wordcomplex *referenceWord= wordcomplex_init(parolaDaIndovinare, wordSize); //Init della parola complessa che e' quella da indovinare

    //printf("[!]Numero di tentativi: ");
    if(scanf("%d",&numTentativi)==EOF){
        exit(0);
    }

    for(int i=0; i<numTentativi && won!=1;i++){
        //printf("[?]Tentativo #%d: ",i+1);
        if(scanf("%s",parolaInput)==EOF){
            exit(0);
        }
        if(strcmp(parolaInput, "+stampa_filtrate") != 0) {
            if(strcmp(parolaInput, "+inserisci_inizio") != 0) {
                if (trie_isThisWordPresentNoFiltering(albero, (char *) parolaInput, wordSize)) {
                    if (strcmp(parolaInput, referenceWord->parola) != 0) {
                        risComparison = wordcomplex_compare(referenceWord, (char *) parolaInput);
                        wordcomplex_printRisComparison(referenceWord);

                        trie_impostaFiltri(albero, (char *) parolaInput, risComparison, wordSize);
                        trie_filter(albero, wordSize);
                        filtraPrimaDiStampare=false;
                        //printf("Parole ancora compatibili: %d\n", albero->compatibleWords);
                        printf("%d\n", albero->compatibleWords);

                        //trie_print(albero, wordSize);

                    } else {
                        //Ha proprio indovinato la parola
                        printf("ok\n");
                        //clock_t end = clock();
                        //double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
                        //printf("TIME: %f",time_spent);
                        won = 1;
                        free(parolaInput);
                        free(referenceWord);
                        free(risComparison);
                        return 1;
                    }
                } else {
                    printf("not_exists\n");
                    i--;
                }
            }else{
                //Ha richiesto +inserisci_inizio
                inserisciNuoveParole(1);//Inserisco le nuove parole e le filtro
                i--;//Non conto il tentativo
            }
        }else{
            //Ha richiesto "+stampa_filtrate"
            //Visualizzo tutte le parole ancora compatibili
            if(filtraPrimaDiStampare==true){
                trie_filter(albero, wordSize);
            }

            trie_print(albero, wordSize);
            i--;//Non lo conto come tentativo
        }
    }
    if(won==0)
        printf("ko\n");

    free(parolaInput);
    free(referenceWord);
    free(risComparison);
    return 1;
}

void inserisciNuoveParole(int doINeedToFilterTheNewWords){
    //Ha richiesto +inserisci_inizio
    int HaDigitatoInserisciFine=0;

    char *newParInput;
    if(wordSize>SIZE_OF_SPECIAL_REQUEST){
        newParInput = malloc((wordSize+1)*sizeof(char));
    }else{
        newParInput = malloc(SIZE_OF_SPECIAL_REQUEST*sizeof(char));
    }

    while(HaDigitatoInserisciFine==0){
        //printf("[+]Parola vocabolario: ");
        if(scanf("%s", newParInput)==EOF){
            exit(0);
        }

        if (strcmp(newParInput, "+inserisci_fine") != 0) {
            albero->head = trie_insertWord(albero,albero->head, newParInput,wordSize,1);
        } else {
            HaDigitatoInserisciFine=1;
            if(doINeedToFilterTheNewWords==1)
                trie_filter(albero,wordSize);
        }
    }

    free(newParInput);
}
void restartGame(){
    trie_restart(albero,wordSize);
}
