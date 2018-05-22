#ifndef SCHED_H
#define SCHED_H

#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>


typedef void (*taskfunc)(void*, struct scheduler *);
typedef struct element_struct Element;
typedef struct pile_struct Pile;
typedef struct scheduler schedulerSharing;


struct pile_struct{
    Element * tete;
    _Atomic int size;
};

struct element_struct{
    taskfunc valeur;
    void * args;
    Element * elementPrecedent;
};

Pile * constructeurPile();

void pilePush(Pile *d, taskfunc v,void* args);

Element * pilePop(Pile *d);

struct scheduler{
    Pile * pile;
    long nombreThreads;
    _Atomic int threadOnWork; // on doit utiliser ca pour savoir si ya un thread qui boss
    pthread_t **pthread; // a utiliser pour faire un join
};

static inline long sched_default_threads(){

    return sysconf(_SC_NPROCESSORS_ONLN);
}

long sched_init(int nthreads, int qlen, taskfunc f, void *closure); // int , int , quicksort , quicksort_args

int sched_spawn(taskfunc f, void *closure, struct scheduler *s);// quicksort , quicksort_args , scheduler

void * touverTaskSharing(void * d);




#endif