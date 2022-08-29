#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct nodo {
 char comando;
 int i1, i2;
 int x;
 char **righeold;
 char **righenew;
 int numerorighe;
 struct nodo *next;
 struct nodo *prec;
};

typedef struct nodo *lista;

char **testo;
char stringa[1028];
char comando[50];
char scelta;
char punto[4];
const char s[2] = ",";
char *token;
int len=100;
int numrighe=0;
int nundo=0;
int p=0;
int nredopossibili=0;
int ncomandi;
int i, n, ind1, ind2;
lista testa = NULL;
lista attuale = NULL;
lista last = NULL;

void token_analize(void);
void change(void);
void delete(void);
void print(void);
void undo(void);
void redo(void);
void aumentalen(void);
void aggiunginodoc(void);
void undochange();
void redochange();
void undodelete();
void redodelete();

int main() {
  testo = malloc(len*sizeof(char*));
  last = malloc(sizeof(struct nodo));
  last -> prec = NULL;
  testa = last;
  attuale = last;

  do{
    fgets(comando, 1028, stdin);
    if(comando[0] == 'q') return 0;
    token_analize();

    switch(scelta) {
         case 'c':
         if(p == 0) ncomandi++;
         else{
           ncomandi = ncomandi - nredopossibili;
           ncomandi++;
           nundo = 0;
           p = 0;
         }
         change();
         break;

         case 'd':
         if(p == 0) ncomandi++;
         else{
           ncomandi = ncomandi - nredopossibili;
           ncomandi++;
           nundo = 0;
           p = 0;
         }
         delete();
         break;

         case 'p': print();  break;


         case 'u':
         if(p == 1){
           nundo = nundo + n;
           if(nundo > ncomandi-nredopossibili) nundo = ncomandi-nredopossibili;
           fgets(comando, 1028, stdin);
           if(comando[0] == 'q') return 0;
           token_analize();
           while(scelta == 'u' || scelta == 'r'){
             if(scelta == 'u'){
               nundo = nundo + n;
               if(nundo > ncomandi-nredopossibili) nundo = ncomandi-nredopossibili;
             }
             if(scelta == 'r'){
               nundo = nundo - n;
               if(nundo < nredopossibili*(-1)){
                 nundo = nredopossibili;
                 nundo = nundo*(-1);
              }
             }

             fgets(comando, 1028, stdin);
             if(comando[0] == 'q') return 0;
             token_analize();
           }

           if(nundo < 0){
             n = nundo;
             n = n*(-1);
             redo();
           }
           else{
             n = nundo;
             undo();
           }

           if(scelta == 'c'){
             if(nundo > 0){
               ncomandi = ncomandi - nredopossibili - nundo;
             }
             else ncomandi = ncomandi - (nredopossibili+nundo);
             nundo = 0;
             nredopossibili=0;
             p=0;
             change();
             ncomandi++;
           }
           if(scelta == 'd'){
             if(nundo > 0){
               ncomandi = ncomandi - nredopossibili - nundo;
             }
             else ncomandi = ncomandi - (nredopossibili+nundo);
             nundo = 0;
             nredopossibili=0;
             p=0;
             delete();
             ncomandi++;
           }
           if(scelta == 'p'){
             print();
             nredopossibili = nredopossibili + nundo;
             if(nredopossibili > ncomandi) nredopossibili = ncomandi;
             else if(nredopossibili < 0) nredopossibili=0;
             nundo = 0;
             p = 1;
           }
         }

         else{
         nundo = nundo + n;
         if(nundo > ncomandi) nundo = ncomandi;

         fgets(comando, 1028, stdin);
         if(comando[0] == 'q') return 0;
         token_analize();
         while(scelta == 'u' || scelta == 'r'){
           if(scelta == 'u'){
             nundo = nundo + n;
             if(nundo > ncomandi) nundo = ncomandi;
           }
           if(scelta == 'r'){
             nundo = nundo - n;
             if(nundo < 0) nundo = 0;

           }

           fgets(comando, 1028, stdin);
           if(comando[0] == 'q') return 0;
           token_analize();
         }

         n = nundo;
         undo();
         if(scelta == 'c'){
           ncomandi = ncomandi - nundo;
           nundo = 0;
           change();
           ncomandi++;
         }
         if(scelta == 'd'){
           ncomandi = ncomandi - nundo;
           nundo = 0;
           delete();
           ncomandi++;
         }
         if(scelta == 'p'){
           print();
           nredopossibili = nundo;
           nundo = 0;
           p = 1;
         }
       }
         break;


         case 'r':
         if(p == 0) redo();
         else{
           redo();
           if(n >= nredopossibili){
             nredopossibili = 0;
             p=0;
           }
           else nredopossibili = nredopossibili - n;
         }
         break;
    }

  }while(1);
}


void token_analize(void){
  char *pass;
  scelta = comando[strlen(comando)-2];

  pass = strstr(comando, s);
  if(pass != NULL){
    token = strtok(comando, s);
    ind1 = atoi(token);
    token = strtok(NULL, s);
    ind2 = atoi(token);
  }
  else{
    n = atoi(comando);
  }
}


void aumentalen(void){
  while(ind2 > len){
    len = len*2;
  }

  testo = realloc(testo, len*sizeof(char*));
}


void change(void){
  if(ind2 > len) aumentalen();

  testa = attuale;                                      //cancello i comandi annulati
  testa -> prec = NULL;

  lista nuovonodo;
  nuovonodo = malloc(sizeof(struct nodo));
  nuovonodo -> comando = scelta;
  nuovonodo -> i1 = ind1;
  nuovonodo -> i2 = ind2;
  nuovonodo -> x = 0;
  nuovonodo -> numerorighe = numrighe;
  nuovonodo -> next = testa;
  nuovonodo -> prec = NULL;
  nuovonodo -> righeold = NULL;
  nuovonodo -> righenew = NULL;
  if (testa != last) testa -> prec = nuovonodo;
  else last -> prec = nuovonodo;
  testa = nuovonodo;
  attuale = testa;


  nuovonodo -> righenew = malloc((ind2-ind1+1)*sizeof(char*));

  if(ind2 > numrighe){
    nuovonodo -> righeold = malloc((numrighe-ind1+1)*sizeof(char*));                    //stai attento, se aggiungi nuove righe, non puoi usare questo puntatore
  }
  else if(ind2 <= numrighe){
    nuovonodo -> righeold = malloc((ind2-ind1+1)*sizeof(char*));
  }

  for(i = ind1; i <= ind2; i++){
    fgets(stringa, 1028, stdin);
    if(i <= numrighe) nuovonodo -> righeold[i-ind1] = testo[i-1];
    testo[i-1] = malloc((strlen(stringa)+1)*sizeof(char));
    strcpy(testo[i-1], stringa);
    nuovonodo -> righenew[i-ind1] = testo[i-1];
  }

  fgets(punto, 4, stdin);
  if(ind2 > numrighe) numrighe = ind2;
}

/*void aggiunginodoc(){
  nuovonodo = malloc(sizeof(struct nodo));
  nuovonodo -> comando = scelta;
  nuovonodo -> i1 = ind1;
  nuovonodo -> i2 = ind2;
  nuovonodo -> numerorighe = numrighe;
  nuovonodo -> next = testa;
  nuovonodo -> prec = NULL;
  nuovonodo -> righeold = NULL;
  nuovonodo -> righenew = NULL;
  if (testa != NULL) testa -> prec = nuovonodo;
  testa = nuovonodo;
  attuale = testa;
}*/


void delete(void){
  int contatore=0;                                      //serve per righeold
  testa = attuale;                                      //cancello i comandi annulati
  testa -> prec = NULL;

  lista nuovonodo;
  nuovonodo = malloc(sizeof(struct nodo));
  nuovonodo -> comando = scelta;
  nuovonodo -> i1 = ind1;
  nuovonodo -> i2 = ind2;
  nuovonodo -> numerorighe = numrighe;
  nuovonodo -> next = testa;
  nuovonodo -> prec = NULL;
  nuovonodo -> righeold = NULL;
  if (testa != last) testa -> prec = nuovonodo;
  else last -> prec = nuovonodo;
  testa = nuovonodo;
  attuale = testa;


  if(ind1 == 0 && ind2 == 0){
    nuovonodo -> x = 1;
    return;
  }

  if(ind1 == 0){
    ind1++;
    nuovonodo -> i1 = ind1;
  }

  if(ind2 < numrighe){
    nuovonodo -> righeold = malloc((ind2-ind1+1)*sizeof(char*));
    nuovonodo -> x = 2;
    i = ind2;
    i++;
    while(i <= numrighe){
      if(contatore < (ind2-ind1+1)) nuovonodo -> righeold[contatore] = testo[i-1-(ind2-ind1+1)];
      testo[i-1-(ind2-ind1+1)] = testo[i-1];
      i++;
      contatore++;
    }

    if(contatore < (ind2-ind1+1)){
      for(int j = ind1+contatore; j <= ind2; j++){
        nuovonodo -> righeold[contatore] = testo[j-1];
        contatore++;
      }
    }

    numrighe = numrighe - (ind2-ind1+1);
    return;
  }


  if(ind2 >= numrighe && ind1 <= numrighe){
    if(ind2 > numrighe){
      ind2 = numrighe;
      nuovonodo -> i2 = ind2;
    }
    nuovonodo -> righeold = malloc((ind2-ind1+1)*sizeof(char*));
    nuovonodo -> x = 3;
    for(i = ind1; i <= ind2; i++){
      nuovonodo -> righeold[i-ind1] = testo[i-1];
    }
    numrighe = ind1;
    numrighe--;
    return;
  }


  if(ind1 > numrighe){
    nuovonodo -> x = 1;
    return;
  }
}


void print(void){
  //printf("numrighe = %d\n", numrighe);
  if(ind1 == 0){
    printf(".\n");
    ind1++;
  }

  for (i = ind1; i <= ind2; i++) {
    if(i > numrighe) printf(".\n");
    else printf("%s", testo[i-1]);
  }
}


void undo(void){
  int cont = 0;
  while(cont < n && attuale != last){
    switch(attuale -> comando) {
         case 'c': undochange(); break;
         case 'd': undodelete();  break;
    }

    attuale = attuale -> next;
    cont++;
  }
}

void redo(void){
  int cont = 0;

  while(cont < n && attuale != NULL){
    if(attuale -> prec != NULL) attuale = attuale -> prec;
    else return;

    switch(attuale -> comando) {
         case 'c': redochange(); break;
         case 'd': redodelete();  break;
    }

    cont++;
  }
}


void undochange(void){
  numrighe = attuale -> numerorighe;
  if(attuale -> i2 <= numrighe){
    for(i = attuale -> i1; i <= attuale -> i2; i++){
      testo[i-1] = attuale -> righeold[i - (attuale -> i1)];
    }
  }

  else if(attuale -> i2 > numrighe){
    for(i = attuale -> i1; i <= numrighe; i++){
      testo[i-1] = attuale -> righeold[i - (attuale -> i1)];
    }
  }
}

void redochange(void){
  for(i = attuale -> i1; i <= attuale -> i2; i++){
    testo[i-1] = attuale -> righenew[i - (attuale -> i1)];
  }

  if(attuale -> i2 > numrighe) numrighe = attuale -> i2;
}


void undodelete(void){
  int j;
  switch(attuale -> x) {
       case 2:
       numrighe = attuale -> numerorighe;
       char **old = malloc((attuale -> i2 - attuale -> i1 + 1)*sizeof(char*));
       for(i = 0; i < attuale -> i2 - attuale -> i1 + 1; i++){
         old[i] = attuale -> righeold[i];
       }
       char *temp;

       i = attuale -> i1;
       while(numrighe-i+1 >= attuale -> i2 - attuale -> i1 + 1){                           //numrighe-i+1 indica quante righe ho ancora da "aggiustare"
         for(j = 0; j < (attuale -> i2 - attuale -> i1 + 1); j++){
           temp = testo[i+j-1];
           testo[i+j-1] = old[j];
           old[j] = temp;
         }
         i = i + attuale -> i2 - attuale -> i1 + 1;
       }
       j = 0;
       while(i <= numrighe){
         testo[i-1] = old[j];
         j++;
         i++;
       }
       break;


       case 3:
       numrighe = attuale -> numerorighe;
       for(i = attuale -> i1; i <= numrighe; i++){
         testo[i-1] = attuale -> righeold[i - (attuale -> i1)];
       }
       break;

       case 1:
       break;
  }
}


void redodelete(void){
  switch(attuale -> x){
    case 1:
    break;

    case 2:
    i = attuale -> i2;
    i++;
    while(i <= numrighe){
      testo[i-1-(attuale -> i2 - attuale -> i1 + 1)] = testo[i-1];
      i++;
    }
    numrighe = numrighe - (attuale -> i2 - attuale -> i1 + 1);
    break;


    case 3:
    numrighe = attuale -> i1;
    numrighe--;
    break;
  }
}
