/*
Class Name: tnlist.h
Description: Permette di gestire una Doubly Linked List con nodi di tipo trieNode
Version: 1.0, (01.07.22)
*/

#ifndef TNLIST_H_
#define TNLIST_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CELLS_ALLOCATED 100

//enum COLOR {Red, Black};
/* ----------------- */
/* STRUCT DEFINITION */
/* ----------------- */
typedef struct tNode{ //Oggetto composta da lettere, lista figli e banfigli di tipo tnlist
    char* lettere;
    struct rbtree* figli;
    struct rbtree* banFigli;
}tNode;

typedef struct rbnodetree {
    tNode obj;
    char firstLetter;

    struct rbnodetree *left;
    struct rbnodetree *right;
    struct rbnodetree *p;
    //enum COLOR color;
    char color;
}rbnodetree;

typedef struct rbtree {
    rbnodetree *head;
    int numElement;
}rbtree;

/////////////////////////////
typedef struct myalloc {
    tNode* allocatedNodes;
    rbtree * allocatedLists;
    rbnodetree * allocatedListNodes;
    char* allocatedNASL;

    int indexNodes;
    int indexLists;
    int indexListNodes;
    int indexChars;
}myalloc;

myalloc* myalloc_init(){
    myalloc *m=malloc(sizeof(myalloc));

    m->allocatedNodes=malloc(sizeof(tNode)*CELLS_ALLOCATED);
    m->indexNodes=0;

    m->allocatedLists=malloc(sizeof(rbtree) * CELLS_ALLOCATED);
    m->indexLists=0;

    m->allocatedListNodes=malloc(sizeof(rbnodetree) * CELLS_ALLOCATED);
    m->indexListNodes=0;

    m->allocatedNASL=malloc(sizeof(char) * CELLS_ALLOCATED);
    m->indexChars=0;
    return m;
}

tNode* myalloc_getspaceFortNode(myalloc* alloc){
    if(alloc->indexNodes==CELLS_ALLOCATED){
        //Se ho finito l'array, lo ricreo tanto i puntatori continuano a rimanere
        alloc->allocatedNodes=malloc(sizeof(tNode)*CELLS_ALLOCATED);
        alloc->indexNodes=0;
    }

    return &alloc->allocatedNodes[alloc->indexNodes++];
}
rbtree * myalloc_getspaceForrbTree(myalloc* alloc){
    if(alloc->indexLists==CELLS_ALLOCATED){
        //Se ho finito l'array, lo ricreo tanto i puntatori continuano a rimanere
        alloc->allocatedLists=malloc(sizeof(rbtree) * CELLS_ALLOCATED);
        alloc->indexLists=0;
    }

    return &alloc->allocatedLists[alloc->indexLists++];
}
rbnodetree * myalloc_getspaceForrbnodetree(myalloc* alloc){
    if(alloc->indexListNodes==CELLS_ALLOCATED){
        //Se ho finito l'array, lo ricreo tanto i puntatori continuano a rimanere
        alloc->allocatedListNodes=malloc(sizeof(rbnodetree) * CELLS_ALLOCATED);
        alloc->indexListNodes=0;
    }

    return &alloc->allocatedListNodes[alloc->indexListNodes++];
}

char * myalloc_getspaceForChars(myalloc* alloc, int lunghezza){
    if(alloc->indexChars+lunghezza>CELLS_ALLOCATED){
        //Se ho finito l'array, lo ricreo tanto i puntatori continuano a rimanere
        alloc->allocatedNASL=malloc(sizeof(char) * CELLS_ALLOCATED);
        alloc->indexChars=0;
    }

    char* ris= &alloc->allocatedNASL[alloc->indexChars];
    alloc->indexChars+=lunghezza;
    return ris;
}
////////////////////////////////
myalloc *allocator=NULL;

/* ----------------- */
/* DECLARATION */
/* ----------------- */
rbtree* rbtree_initEmpty();
rbtree* rbtree_init(char* key);
rbnodetree * rbtree_newNode(char* key);
rbnodetree* rbtree_search(rbtree* l, char* key); //Ricerca nella lista in modo lineare
rbnodetree* rbtree__search_subproc(rbnodetree* n, char* key);

//rbnodetree* tnlist_insertAhead(rbtree *l, char* key);
rbnodetree* rbtree_insertByKey(rbtree *t, char* key);
void rbtree__insertFixup(rbtree *t, rbnodetree *z);
void rbtree_leftrotate(rbtree *t, rbnodetree* x);
void rbtree_rightrotate(rbtree *t, rbnodetree* x);

rbnodetree* rbtree_insertByrbNode(rbtree *t, rbnodetree* z);

rbnodetree* rbtree_minimum(rbnodetree* x);
rbnodetree* rbtree_successor(rbnodetree* x);

rbnodetree* rbtree_delete(rbtree *l, rbnodetree* z);
void rbtree_deleteFixup(rbtree *t, rbnodetree* x, rbnodetree*  nodeParent, int nodeIsLeft);

//void tnlist_deleteBySearch(rbtree *l, char* key);
void rbtree_printInOrderVisit(rbtree *l);
void rbtree__printInOrderVisit_subproc(rbnodetree *x);

//tNode* tnlist_getElement(rbtree *l, int pos);
rbnodetree* rbtree_moveElementFromUnBanToBan(rbtree *unBanlist, rbtree *Banlist, rbnodetree* toMove);
rbnodetree* tnlist_moveElementFromBanToUnBan(rbtree *unBanlist, rbtree *Banlist, rbnodetree* toMove);
rbnodetree* rbtree_searchByFirstLetter(rbtree* T,char lettera);
rbnodetree* rbtree__searchByFirstLetter_subproc(rbnodetree* n,char lettera);

rbnodetree * rbtree_getChild(tNode* node, int pos);
//rbnodetree * rbtree_getElement(rbnodetree* x, int pos);
rbnodetree * rbtree_getBannedChild(tNode* node, int pos);

rbnodetree** rbtree_getAllChilds(tNode* node,rbnodetree** outArray);
void rbtree__getAllChilds_subproc(rbnodetree* x, rbnodetree** outArray);

rbnodetree* rbtree_getAllChildsNoShadow(tNode* node,rbnodetree* outArray);
void rbtree__getAllChildsNoShadow_subproc(rbnodetree* x, rbnodetree* outArray);

rbnodetree** rbtree_getAllBanChilds(tNode* node,rbnodetree** outArray);


tNode* tNode_newNode(char* key);
tNode* tNode_addChildOrdered(tNode* node, char* key);
tNode* tNode_addChildOrderedByrbnodeTree(tNode* node, rbnodetree* toadd);
tNode* tNode_addBanChildOrdered(tNode* node, char* key);
tNode* tNode_searchChild(tNode* node, char* key);
tNode* tNode_searchBanChild(tNode* node, char* key);
tNode* tNode_getChild(tNode* node, int pos);
tNode* tNode_getBannedChild(tNode* node, int pos);

rbnodetree* tNode_moveChildFromUnBanToBan(tNode* father, rbnodetree* childToMove);
rbnodetree* tNode_moveChildFromBanToUnBan(tNode* father, rbnodetree* childToMove);


/* ----------------- */
/* IMPLEMENTATION */
/* ----------------- */
rbtree* rbtree_initEmpty(){
    rbtree *l= myalloc_getspaceForrbTree(allocator);
    //tnlist *l = malloc(sizeof(tnlist)); //linked list
    l->head = NULL;
    l->numElement=0;
    if(allocator==NULL)
        allocator=myalloc_init();
    return l;
}

rbtree* rbtree_init(char* key){
    rbnodetree *a;
    a = rbtree_newNode(key);

    rbtree *l= myalloc_getspaceForrbTree(allocator);
    //tnlist *l = malloc(sizeof(tnlist)); //linked list
    l->head = a;
    l->numElement=1;

    return l;
}

rbnodetree* rbtree_newNode(char* key){
    rbnodetree *z;
    z = myalloc_getspaceForrbnodetree(allocator);
    //z = malloc(sizeof(struct rbnodetree));
    z->left = z->right = NULL;
    z->p=NULL;
    //tNode *att=tNode_newNode(key);
    //z->obj=att;
    z->obj.figli=NULL;
    z->obj.banFigli=NULL;
    z->obj.lettere=key;
    z->firstLetter=key[0];

    z->color=1;
    return z;
}


rbnodetree* rbtree_search(rbtree* l, char* key){
    if(l!=NULL && l->head!=NULL){
        return rbtree__search_subproc(l->head,key);
    }
    return NULL;
}

rbnodetree* rbtree__search_subproc(rbnodetree* n, char* key){
    if(n==NULL || strcmp(key,n->obj.lettere)==0)
        return n;
    if(strcmp(key,n->obj.lettere)<0)
        return rbtree__search_subproc(n->left,key);
    else
        return rbtree__search_subproc(n->right,key);
}

/*
rbnodetree* tnlist_insertAhead(rbtree *l, char* key) {
    rbnodetree * n= tnlist_newNode(key);
    n->next = l->head;
    if(l->head != NULL)
        l->head->prev = n;
    l->head = n;
    n->prev=NULL;
    l->numElement++;

    return n;
}
*/
rbnodetree* rbtree_insertByKey(rbtree *t, char* key){
    rbnodetree* z=rbtree_newNode(key); //Creo il nodo da inserire nel BST
    return rbtree_insertByrbNode(t,z);
}

void rbtree__insertFixup(rbtree *t, rbnodetree *z){
    rbnodetree *x;
    rbnodetree *y;
    if (z == t->head) {
        t->head->color = 0;
    } else {
        x = z->p; // x e' il padre di z
        if (x->color == 1) {
            if (x == x->p->left) { // se x e' figlio sin.
                y = x->p->right; // y e' fratello di x
                if (y != NULL && y->color == 1) {
                    x->color = 0;
                    y->color = 0;
                    x->p->color = 1;
                    rbtree__insertFixup(t, x->p);
                } else {
                    if (z == x->right) {
                        z = x;
                        rbtree_leftrotate(t, z);
                        x = z->p;
                    }
                    x->color = 0;
                    x->p->color = 1;
                    rbtree_rightrotate(t, x->p);
                }
            } else {
                y = x->p->left; // y e' fratello di x
                if (y != NULL && y->color == 1) {
                    x->color = 0;
                    y->color = 0;
                    x->p->color = 1;
                    rbtree__insertFixup(t, x->p);
                } else {
                    if (z == x->left) {
                        z = x;
                        rbtree_rightrotate(t, z);
                        x = z->p;
                    }
                    x->color = 0;
                    x->p->color = 1;
                    rbtree_leftrotate(t, x->p);
                }
            }
        }
    }
}

void rbtree_leftrotate(rbtree *t, rbnodetree* x){
    rbnodetree* y = x->right;
    x->right=y->left;
    if(y->left!=NULL){
        y->left->p=x;
    }
    y->p=x->p;
    if(x->p==NULL){
        t->head=y;
    }
    else if(x==x->p->left)
        x->p->left=y;
    else
        x->p->right=y;
    y->left=x;
    x->p=y;
}

void rbtree_rightrotate(rbtree *t, rbnodetree* x){
    rbnodetree* y = x->left;
    x->left = y->right;
    if(y->right != NULL) {
        y->right->p = x;
    }
    y->p = x->p;
    if(x->p == NULL) {
        t->head = y;
    }
    else if(x == x->p->right) {
        x->p->right = y;
    }
    else {
        x->p->left = y;
    }
    y->right = x;
    x->p = y;

}


rbnodetree* rbtree_insertByrbNode(rbtree *t, rbnodetree* z){
    z->left=z->right=z->p=NULL;
    z->color=1;

    rbnodetree* y=NULL;
    rbnodetree* x=t->head;

    while(x!=NULL) { //Se esiste una radice
        y = x;
        if (z->firstLetter < x->firstLetter)
            x = x->left;
        else
            x = x->right;
    }
    z->p=y;
    if(y==NULL) //Il nodo da aggiungere e' la root
        t->head = z;
    else if(z->firstLetter < y->firstLetter)
        y->left=z;
    else y->right=z;

    z->left=NULL;
    z->right=NULL;
    z->color=1;
    rbtree__insertFixup(t,z);
    t->numElement++;


    return z;
}

rbnodetree* rbtree_minimum(rbnodetree* x){
    while(x->left!=NULL)
        x=x->left;
    return x;
}

rbnodetree* rbtree_successor(rbnodetree* x){
    if(x->right!=NULL)
        return rbtree_minimum(x->right);
    rbnodetree* y = x->p;
    while(y!=NULL && x==y->right) {
        x = y;
        y=y->p;
    }
    return y;

}

rbnodetree* succ_private=NULL;
rbnodetree* nodeAddedinBan_private=NULL;

rbnodetree* rbtree_delete(rbtree *t, rbnodetree* z){
    rbnodetree *y,*x;
    if(z->left == NULL || z->right == NULL)
        y = z;
    else {
        y = rbtree_successor(z);
        succ_private=z;
    }

    if(y->left != NULL)
        x = y->left;
    else
        x = y->right;

    if(x!=NULL)
        x->p = y->p;

    rbnodetree* xParent = y->p;
    int yIsLeft = 0;
    if(y->p == NULL)
        t->head = x;
    else if(y == y->p->left) {
        y->p->left = x;
        yIsLeft = 1;
    } else {
        y->p->right = x;
        yIsLeft = 0;
    }

    if(y != z) {
        z->obj = y->obj;
        z->firstLetter=y->firstLetter;
    }

    if(y->color == 0)
        rbtree_deleteFixup(t, x, xParent, yIsLeft);

    t->numElement--;
    return y;
}

void rbtree_deleteFixup(rbtree *t, rbnodetree* node, rbnodetree*  nodeParent, int nodeIsLeft) {
    while (node != t->head && (node == NULL || node->color==0)) {
        rbnodetree *w;
        if(nodeIsLeft){
            w = nodeParent->right; // w e' fratello di x
            if(w->color == 1) {// Caso 1
                w->color = 0;
                nodeParent->color = 1;
                rbtree_leftrotate(t, nodeParent);
                w = nodeParent->right;
            }

            if((w->left == NULL || w->left->color == 0) && (w->right == NULL || w->right->color == 0)) {// Caso 2
                w->color = 1;
                node = nodeParent;
                nodeParent = node->p;
                nodeIsLeft = nodeParent != NULL &&(node == nodeParent->left);
            } else {
                if(w->right == NULL || w->right->color == 0) { // Caso 3
                    w->left->color = 0;
                    w->color = 1;
                    rbtree_rightrotate(t, w);
                    w = nodeParent->right;
                }

                w->color = nodeParent->color; // Caso 4
                nodeParent->color = 0;
                if (w->right != NULL) {
                    w->right->color = 0;
                }
                rbtree_leftrotate(t, nodeParent);
                node = t->head;
                nodeParent = NULL;
            }
        } else { /* nodeIsLeft == false */
            w = nodeParent->left; // w e' fratello di x
            if(w->color == 1) {// Caso 1
                w->color = 0;
                nodeParent->color = 1;
                rbtree_rightrotate(t, nodeParent);
                w = nodeParent->left;
            }

            if((w->right == NULL || w->right->color == 0) && (w->left == NULL || w->left->color == 0)) {// Caso 2
                w->color = 1;
                node = nodeParent;
                nodeParent = node->p;
                nodeIsLeft = nodeParent != NULL && (node == nodeParent->left);
            } else {
                if(w->left == NULL || w->left->color == 0) { // Caso 3
                    w->right->color = 0;
                    w->color = 1;
                    rbtree_leftrotate(t, w);
                    w = nodeParent->left;
                }

                w->color = nodeParent->color; // Caso 4
                nodeParent->color = 0;
                if (w->left != NULL) {
                    w->left->color = 0;
                }
                rbtree_rightrotate(t, nodeParent);
                node = t->head;
                nodeParent = NULL;
            }
        }
    }

    if(node != NULL) {
        node->color = 0;
    }
}
/*
void tnlist_deleteBySearch(rbtree *l, char* key){
    rbnodetree *toRemove = tnlist_search(l, key); //Effettuo la ricerca dell'elemento da cancellare
    if(toRemove!=NULL)
        tnlist_delete(l,toRemove);
}
*/
void rbtree_printInOrderVisit(rbtree *l){
    if(l->head!=NULL){
        rbtree__printInOrderVisit_subproc(l->head);
    }
}
void rbtree__printInOrderVisit_subproc(rbnodetree* x){
    if(x!=NULL){
        rbtree__printInOrderVisit_subproc(x->left);
        printf("%s",x->obj.lettere);
        rbtree__printInOrderVisit_subproc(x->right);
    }
}
/*
tNode* tnlist_getElement(rbtree *l, int pos){
    rbnodetree * x=l->head;
    int i=0;
    while(x!=NULL && i<pos) {
        x = x->next;
        i++;
    }
    return x->obj;
}
*/
rbnodetree* rbtree_moveElementFromUnBanToBan(rbtree *unBanlist, rbtree *Banlist, rbnodetree* toMove){
    succ_private=NULL;
    nodeAddedinBan_private=NULL;
    tNode tmp = toMove->obj;
    char first = toMove->firstLetter;
    rbnodetree * n = rbtree_delete(unBanlist,toMove);

    nodeAddedinBan_private=n;

    n->obj=tmp;
    n->firstLetter=first;
    rbtree_insertByrbNode(Banlist,n);

    return succ_private;
}

rbnodetree* tnlist_moveElementFromBanToUnBan(rbtree *unBanlist, rbtree *Banlist, rbnodetree* toMove){
    succ_private=NULL;
    nodeAddedinBan_private=NULL;
    tNode tmp = toMove->obj;
    char first = toMove->firstLetter;
    rbnodetree * n = rbtree_delete(Banlist,toMove);

    nodeAddedinBan_private=n;

    n->obj=tmp;
    n->firstLetter=first;
    rbtree_insertByrbNode(unBanlist,n);
    return succ_private;
}


rbnodetree* rbtree_searchByFirstLetter(rbtree* T,char lettera){
    if(T!=NULL && T->head!=NULL){
        return rbtree__searchByFirstLetter_subproc(T->head,lettera);
    }
    return NULL;
}
rbnodetree* rbtree__searchByFirstLetter_subproc(rbnodetree* n,char lettera){
    if(n==NULL || lettera==n->firstLetter)
        return n;

    if(lettera<n->firstLetter)
        return rbtree__searchByFirstLetter_subproc(n->left,lettera);
    else
        return rbtree__searchByFirstLetter_subproc(n->right,lettera);
}

int index_child_private;
rbnodetree* rbtree_getAllChildsNoShadow(tNode* node,rbnodetree* outArray){

    if(node->figli!=NULL) {
        index_child_private=0;
        rbtree__getAllChildsNoShadow_subproc(node->figli->head,outArray);
    }
    return outArray;
}

void rbtree__getAllChildsNoShadow_subproc(rbnodetree* x, rbnodetree* outArray){
    if(x!=NULL){
        rbtree__getAllChildsNoShadow_subproc(x->left, outArray);
        outArray[index_child_private++]=*x;
        rbtree__getAllChildsNoShadow_subproc(x->right,outArray);
    }
}


rbnodetree** rbtree_getAllChilds(tNode* node,rbnodetree** outArray){

    if(node->figli!=NULL) {
        index_child_private=0;
        rbtree__getAllChilds_subproc(node->figli->head,outArray);
    }
    return outArray;
}

void rbtree__getAllChilds_subproc(rbnodetree* x, rbnodetree** outArray){
    if(x!=NULL){
        rbtree__getAllChilds_subproc(x->left, outArray);
        outArray[index_child_private++]=x;
        rbtree__getAllChilds_subproc(x->right,outArray);
    }
}

rbnodetree** rbtree_getAllBanChilds(tNode* node,rbnodetree** outArray){

    if(node->banFigli!=NULL) {
        index_child_private=0;
        rbtree__getAllChilds_subproc(node->banFigli->head,outArray);
    }
    return outArray;
}



/*
int count_private=0;
rbnodetree * rbtree_getChild(tNode* node, int pos){
    //Voglio prendere il figlio pos-esimo presente nel BST di node->figli
    rbnodetree *x=NULL;
    if(node->figli!=NULL) {
        count_private=0;
        x=rbtree_getElement(node->figli->head,pos);
    }
    return x;
}

rbnodetree * rbtree_getElement(rbnodetree* x, int pos){
    if(x==NULL)
        return NULL;

    rbnodetree* sx = rbtree_getElement(x->left,pos);

    if(sx!=NULL)
        return sx;

    count_private++;
    if(count_private==pos+1)
        return x;

    return rbtree_getElement(x->right,pos);
}

rbnodetree * rbtree_getBannedChild(tNode* node, int pos){
    //Voglio prendere il figlio bannato pos-esimo presente nel BST di node->banfigli
    rbnodetree *x=NULL;
    if(node->banFigli!=NULL) {
        count_private=0;
        x=rbtree_getElement(node->banFigli->head,pos);
    }
    return x;
}
*/

tNode* tNode_newNode(char* key){
    tNode *att;
    att = myalloc_getspaceFortNode(allocator);
    //att = malloc(sizeof(struct tNode));
   // att->figli=tnlist_initEmpty();
    //att->banFigli=tnlist_initEmpty();

    att->figli=NULL;
    att->banFigli=NULL;

    //att->lettere =malloc(sizeof(char)* strlen(key));
    att->lettere=key;

    return att;
}

tNode* tNode_addChildOrdered(tNode* node, char* key){
    if(node->figli==NULL){
        node->figli=rbtree_initEmpty();
    }

    return &rbtree_insertByKey(node->figli, key)->obj;
}
tNode* tNode_addChildOrderedByrbnodeTree(tNode* node, rbnodetree* toadd){
    if(node->figli==NULL){
        node->figli=rbtree_initEmpty();
    }
    return &rbtree_insertByrbNode(node->figli,toadd)->obj;
}

tNode* tNode_addBanChildOrdered(tNode* node, char* key){
    if(node->banFigli==NULL){
        node->banFigli=rbtree_initEmpty();
    }
    return &rbtree_insertByKey(node->banFigli, key)->obj;
}



tNode* tNode_searchChild(tNode* node, char* key){
    if(node->figli!=NULL) {
        rbnodetree *ris = rbtree_search(node->figli, key);
        if (ris != NULL)
            return &ris->obj;
    }
    return NULL;
}

tNode* tNode_searchBanChild(tNode* node, char* key){
    if(node->banFigli!=NULL) {
        rbnodetree *ris = rbtree_search(node->banFigli, key);
        if (ris != NULL)
            return &ris->obj;
    }
    return NULL;
}
/*
tNode* tNode_getChild(tNode* node, int pos){
    return rbtree_getChild(node, pos)->obj;
}
tNode* tNode_getBannedChild(tNode* node, int pos){
    return rbtree_getBannedChild(node, pos)->obj;
}
*/
rbnodetree* tNode_moveChildFromUnBanToBan(tNode* father, rbnodetree* childToMove){
    if(father->figli==NULL){
        father->figli=rbtree_initEmpty();
    }
    if(father->banFigli==NULL){
        father->banFigli=rbtree_initEmpty();
    }
    return rbtree_moveElementFromUnBanToBan(father->figli, father->banFigli, childToMove);
}

rbnodetree* tNode_moveChildFromBanToUnBan(tNode* father, rbnodetree* childToMove){
    if(father->figli==NULL){
        father->figli=rbtree_initEmpty();
    }
    if(father->banFigli==NULL){
        father->banFigli=rbtree_initEmpty();
    }
    return tnlist_moveElementFromBanToUnBan(father->figli, father->banFigli, childToMove);
}




#endif
