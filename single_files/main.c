#include "trie.h"
#include "wordcomplex.h"
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

/*
void restartTEST(){
    int wordSizet=3;
    trie *alberot = trie_init(wordSizet);


    trie_insertWord(alberot,"bza",wordSizet);
    trie_insertWord(alberot,"bzc",wordSizet);

    trie_banLetter(alberot,'a');
    trie_banLetter(alberot,'b');
    // trie_insertWord(albero,"bear",4);
    // trie_insertWord(albero,"bell",4);
    // trie_insertWord(albero,"bore",4);

    printf("Lista degli elementi prima del restart:\n");
    trie_filter(alberot,wordSizet);
    trie_print(alberot,wordSizet);

    printf("Lista degli elementi dopo il restart:\n");
    trie_restart(alberot,wordSizet);
    trie_print(alberot,wordSizet);
    return 0;
}
void trieTEXT(){
    trie *albero = trie_init();
    int wordSize=4;

    trie_insertWord(albero,"aBca",wordSize);
    trie_insertWord(albero,"abxb",wordSize);
    trie_insertWord(albero,"cBBc",wordSize);


    //trie_banLetter(albero,'B');
    // trie_banLetter(albero,'c');
    //trie_setPerfectCountForLetter(albero,'B',1);
    trie_setMinimumCountForLetter(albero,'b',2);
    //trie_setPerfectCountForLetter(albero,'a',2);

    //trie_banLetter(albero,'c');
    // trie_insertWord(albero,"bear",4);
    // trie_insertWord(albero,"bell",4);
    // trie_insertWord(albero,"bore",4);

    trie_filter(albero,wordSize);
    trie_print(albero,wordSize);
}

void wordcomplexTEST(){
    wordcomplex *parola = wordcomplex_init("ghjghgJHGCDSUYJLHhjvjh27i327y8ghdfshjGHJGDSJH72Guydsu7gfjdsag2g23gjvhgJhgv-dsf--fdas-__dfsf272yfdHBHJgyjghfghjgGHVG",NULL);

    wordcomplex_compare(parola,"HJDASEHGJHJGhjgbhjbnBH-Nbmnfdbhjs36r723_-_ru34272yfewjhjhmHJMBNbvhjbHMBJDSBJHKFSDHMNUY3BFDNSG732JBDSHNKhgbdhjgshgfs");
    wordcomplex_printRisComparison(parola);

}

void trieTEST(){
    trie *albero = trie_init();
    int wordSize=4;

    trie_insertWord(albero,"batt",wordSize);
    trie_insertWord(albero,"bear",wordSize);
    trie_insertWord(albero,"stac",wordSize);
    trie_insertWord(albero,"bell",wordSize);
    trie_insertWord(albero,"bore",wordSize);
    trie_insertWord(albero,"stoc",wordSize);
    trie_insertWord(albero,"stot",wordSize);
    trie_insertWord(albero,"ball",wordSize);


   // trie_insertWord(albero,"bear",4);
   // trie_insertWord(albero,"bell",4);
   // trie_insertWord(albero,"bore",4);

    trie_print(albero,wordSize);
}
void listaTEST(){
    //list* lista = list_initEmpty();
    list* lista = list_init(58);
    list_insertAhead(lista,1);
    list_insertAhead(lista,2);

    listNode* n = list_insertAhead(lista,3);
    list_insertAhead(lista,4);
    list_insertAhead(lista,5);
    list_insertAhead(lista,6);

    list_print(lista);

    list_deleteBySearch(lista,4);
    list_print(lista);

    list_delete(lista,n);
    list_print(lista);

    listNode* risS = list_search(lista,20);
}

void tnlistaTEST(){
    tnlist *tnlista = tnlist_initEmpty();


    tnlist_insertByAlphOrder(tnlista,'d');
    tnlist_print(tnlista);

    tnlist_insertByAlphOrder(tnlista,'c');
    tnlist_insertByAlphOrder(tnlista,'b');
    tnlist_insertByAlphOrder(tnlista,'a');

    tnlist_print(tnlista);
}

void tnodoTEST(){
    tNode *tnodo = tNode_newNode('a');

    tNode_addChildOrdered(tnodo,'e');
    tNode_addChildOrdered(tnodo,'c');
    tNode_addChildOrdered(tnodo,'b');
}
*/