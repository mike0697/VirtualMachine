/*
P1G113
Nicola Miotto 873863
Michele Pizzeghello 875934
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_e_controlli.h"
#include "memoria.h"

#define HALT 0
#define DISPLAY 1
#define PRINT_STACK 2
#define PUSH 10
#define POP 11
#define MOV 12
#define CALL 20
#define RET 21
#define JMP 22
#define JZ 23
#define JPOS 24
#define JNEG 25
#define ADD 30
#define SUB 31
#define MUL 32
#define DIV 33



enum error {
	None,					/*Non ci sono errori.*/
	Overflow,				/*Si è verificato overflow aritmetico.*/
	Zero_division,			/*Si è tentato di eseguire la divisione per 0.*/
	Stack_overflow,			/*Non è stato possibile effettuare push causa stack saturo.*/
	Stack_underflow,		/*Non è stato possibile effettuare prelievo causa stack vuoto.*/
	Invalid_intruction,		/*L'istruzione da eseguire non è una vera istruzione.*/
	Missing_argument,		/*L'ultima posizione dell'array di istruzioni richiede parametri che sono assenti.*/
	Out_of_bounds,			/*Un'istruzione di jump richiede che si salti fuori dall'array di istruzioni.*/
	Register_fail,			/*Il registro da utilizzare non esiste perché non è compreso tra R0 e R31.*/
	Halt_not_found			/*La lettura dell'array di istruzioni è terminata senza aver eseguito HALT.*/
};
typedef enum error err_t;

/*Funzione che verifica che ad un intero corrisponda un registro.*/
int is_register(int n);

void halt(Stack e, int *p, FILE *Iptr);

/*Funzioni di stampa.*/
err_t display(int ip, int *reg_file);
void print_stack(Stack s, int n);

err_t move(int *reg_file, int reg, int n);

err_t jump(unsigned int *ip, unsigned int destination, int in);

/*Funzioni per eseuguire istruzioni aritmetiche.*/
err_t add(int *reg_file, int r1, int r2, Stack s);
err_t sub(int *reg_file, int r1, int r2, Stack s);
err_t mul(int *reg_file, int r1, int r2, Stack s);
err_t div1(int *reg_file, int r1, int r2, Stack s);