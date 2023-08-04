/*
Class Name: list.h
Description: Permette di gestire una Doubly Linked List
Version: 1.0, (01.07.22)
*/

#ifndef LIST_H_
#define LIST_H_
#include <stdio.h>
#include <stdlib.h>

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
