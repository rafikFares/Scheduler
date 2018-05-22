#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <assert.h>
#include "sched.h"

static pthread_mutex_t mutexPile = PTHREAD_MUTEX_INITIALIZER;

Pile * constructeurPile() {
    Pile * pile = (Pile*) malloc(sizeof(Pile));
    pile->tete = (Element*) malloc(sizeof(Element));
    pile->tete = NULL;
    pile->size = 0;
    if (pile != NULL) {
        return pile;
    }else{
        return NULL;
    }
}


void pilePush(Pile *pile, taskfunc v,void* args) {
    Element *n = malloc(sizeof(Element));
    assert(n != NULL);
    n->valeur = v;
    n->args = args;
    if (pile->size == 0){// elle est vide
        n->elementPrecedent = NULL;
    }else{//pas vide ya deja des elements
        n->elementPrecedent = pile->tete;
    }
    pile->tete = n;
    pile->size++;
}


Element * pilePop(Pile *pile) {
    pile->size--;
    Element *n = pile->tete;
    pile->tete = pile->tete->elementPrecedent;
    return n;
}


long sched_init(int nthreads, int qlen, taskfunc f, void *closure){// int , int , quicksort , quicksort_args
    struct scheduler *shed = (struct scheduler*)malloc(sizeof(struct scheduler));

    if (shed != NULL){

        shed->pile = (Pile*)malloc(sizeof(Pile));
        shed->pile = constructeurPile();
        pilePush(shed->pile,f,closure);
        shed->threadOnWork = 0;
        if(nthreads <= 0)
            shed->nombreThreads = sched_default_threads();
        else
            shed->nombreThreads = nthreads;

        shed->pthread =  malloc(shed->nombreThreads*sizeof(pthread_t));

        for (int i = 0;i<shed->nombreThreads;i++){
            shed->pthread[i] = malloc(sizeof(pthread_t*));
        }

        for (int i = 0;i<shed->nombreThreads;i++){
            if(pthread_create(shed->pthread[i], NULL, touverTaskSharing, shed)) {
                fprintf(stderr, "Error creating thread\n");
                return NULL;
            }
        }

        for (int i = 0;i<shed->nombreThreads;i++){
            pthread_join(*shed->pthread[i],NULL);
        }

        free(shed->pile);
        free(shed);

        return 1;
    }

    return 0;
}


int sched_spawn(taskfunc f, void * closure, struct scheduler *s){// quicksort , quicksort_args , scheduler
    pthread_mutex_lock(&mutexPile);
    pilePush(s->pile,f,closure);
    pthread_mutex_unlock(&mutexPile);
    return 1;
}


void * touverTaskSharing(void * shod){
    struct scheduler * sched = (struct scheduler *) shod;

    while (1) {
        pthread_mutex_lock(&mutexPile);
        if (sched->pile->size == 0) {//alors cest vide ya pas de tache
            pthread_mutex_unlock(&mutexPile);

            if(sched->threadOnWork == 0){

                pthread_exit(NULL);
                break;

            }else {

                sleep(1/4);

            }
        } else {// ya du ttravail dans ma pile
            sched->threadOnWork++;

            Element * element = pilePop(sched->pile);
            pthread_mutex_unlock(&mutexPile);

            if(element != NULL){
                (*element->valeur)(element->args,sched);
                free(element);
            }

            sched->threadOnWork--;

        }
    }
    return NULL;
}








