/*
Class Name: array.h
Description: Permette di gestire un array
Version: 1.0, (14.07.22)
*/

#ifndef ARRAY_H_
#define ARRAY_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
