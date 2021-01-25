/*
P1G113
Nicola Miotto 873863
Michele Pizzeghello 875934
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STACK_SIZE_IN_BYTES 64*1024			/*64kB, dimensione dello stack in byte-*/
#define STACK_SIZE ((64 *1024)/sizeof(int))	/*Dimensione dello stack in elementi interi.*/
/*#define STACK_SIZE ((sizeof(char) * (1 << 16)) / sizeof(int))*/


struct stack
{
    int *v;
    unsigned int sp;
};
typedef struct stack *Stack;


int push(Stack s, int elem);
int pop(Stack s, int *val);
void destroy(Stack s);

int bootstrap(Stack s);

void stampa_stack(Stack s);