/*
P1G113
Nicola Miotto 873863
Michele Pizzeghello 875934
*/
#include "memoria.h"
#include <stdio.h>


/*aggiunge un intero nello stack e incrementa sp*/
int push(Stack s, int elem)
{
      
    if((s->sp) >= STACK_SIZE)
    {
        return -1; 
    }

    s->v[s->sp] = elem;
    s->sp = s->sp+1;
    return 0;
}


/*
void stampa_stack(Stack s)
{
    unsigned int n = 0;
    while(n < (s->sp))
    {
        printf("%d\n" , (s->v)[n]);
        n++;
    }
}
Utilizzata per verificare che le funzioni definite in questo file funzionassero.
*/


/*preleva un elemento dallo stack dopo aver decrementato sp*/
int pop(Stack s, int *val)
{
    if((s->sp) <= 0)
    {
        return -2; /*indica che non ci sono elementi in memoria*/
    }
	else {
    	(s->sp)--;
    	*val = s->v[s->sp];
    	return 0;
	}
}


void destroy(Stack s)
{   
    free(s->v);
    s -> sp = 0;
}

/*inizializza le strutture*/
int bootstrap(Stack s)
{ 
    s->v = (int *)malloc(STACK_SIZE_IN_BYTES);
    s->sp = 0;
    if(s->v == NULL) {
        return -1; 
    }
    else
    	return 0;
}