/*
P1G113
Nicola Miotto 873863
Michele Pizzeghello 875934
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "istruzioni.h"
#include <limits.h>


int is_register(int n)
/*
Questa funzione verifica semplicemente che l'argomento sia compreso tra 0 e 31.
In caso contrario, restituisce 0.
*/
{
	return n >= 0 && n <= 31;	
}


void halt(Stack e, int *array_di_istruzioni, FILE *Iptr)
/*
Questa funzione libera la memoria allocata dinamicamente(stack e array di istruzioni) e chiude il file aperto.
*/
{
	printf("Quitting...\n");
	destroy(e);
	free(array_di_istruzioni);
	fclose(Iptr);
	return;
}



err_t display(int numero_letto, int *reg_file)
/*
Questa funzione esegue l'istruzione DISPLAY, ma solo se il registro di cui mostrare il contenuto esiste.
In caso contrario, un errore è segnalato (Register_fail).
*/
{
	err_t err_code;
	if (is_register(numero_letto)) {
		/*Se il registro esiste, ne viene stampato il contenuto.*/
		printf("%d\n", reg_file[numero_letto]);
		err_code = None;
	}
	else
		err_code = Register_fail;
	return err_code;	/*!err_code o (err_code == None) indica che la stampa ha avuto successo.*/
}



void print_stack(Stack s, int n)
/*
Questa funzione esegue l'istruzione PRINT_STACK. Non fallisce mai.
Nel caso in cui il numero di posizioni da stampare sia inferiore al parametro n, stampa un avviso.
Non produce errori.
*/
{
	int i = s -> sp;
	while (n && i) {	/*Stampa al più n posizioni dello stack, se ve ne sono abbastanza.*/
		i--;
		printf("STACK[%d]: %d\n", i, s -> v[i]);
		n--;
	}
	if (!i && n > 0)				/*Se vi sono meno di n elementi nello stack, viene stampato un messaggio.*/
		printf("Note: %d more data could not be displayed because the bottom of the stack has been reached.\n", n);
}


err_t move(int *reg_file, int reg, int n)
/*
Questa funzione sovrascrive il valore n nel registro reg_file[reg].
Se il registro non è reale, la scrittura non avviene e un errore è segnalato (Register_fail).
*/
{
	err_t error = None;
	if (!is_register(reg))		/*Qui avviene il controllo che il registro esista.*/
		error = Register_fail;
	else
		reg_file[reg] = n;		/*Qui avviene la scrittura.*/
	return error;
}



err_t jump(unsigned int *ip, unsigned int destination, int in)
/*
Questa funzione esegue il salto incondizionato, ma è richiamata anche dalle istruzioni di salto condizionato.
Se l'istruzione a cui saltare si trova fuori dall'array di istruzioni, il salto non avviene
e un errore è segnalato (Out_of_bounds).
*/
{
	err_t err_code = None;
	if (destination < (unsigned int) in)			/*Controllo che il nuovo indirizzo appartenga all'array di istruzioni.*/
		*ip = destination;
	else
		err_code = Out_of_bounds;
	return err_code;
}



err_t add(int *reg_file, int r1, int r2, Stack s)
/*
Questa funzione somma i valori contenuti nei registri in input e inserisce il risultato nello stack.
Se almeno un registro non esiste, l'operazione non avviene ed è segnalato un errore (Register_fail).
Se l'operazione produce overflow aritmetico, è segnalato un errore (Overflow).
Se l'operazione è avvenuta, ma lo stack è saturo, è segnalato un errore (Stack_overflow).
*/
{
	err_t err_code = None;
	if (!is_register(r1) || !is_register(r2))	/*Controllo che i registri esistano.*/
		err_code = Register_fail;
	else {
		int a = reg_file[r1], b = reg_file[r2];
		if ((a > 0 && b > 0 && a > INT_MAX - b) || (a < 0 && b < 0 && a < INT_MIN - b))
			err_code = Overflow;	/*Se c'è overflow, la somma non avviene.*/
		else {
			if (push(s, a + b))		/*Verifica che push sia andata a buon fine.*/
				err_code = Stack_overflow;
		}
	}
	return err_code;
}



err_t sub(int *reg_file, int r1, int r2, Stack s)
/*
Questa funzione sottrae i valori contenuti nei registri in input e inserisce il risultato nello stack.
Se almeno un registro non esiste, l'operazione non avviene ed è segnalato un errore (Register_fail).
Se l'operazione produce overflow aritmetico, è segnalato un errore (Overflow).
Se l'operazione è avvenuta, ma lo stack è saturo, è segnalato un errore (Stack_overflow).
*/
{
	err_t err_code = None;
	if (!is_register(r1) || !is_register(r2))	/*Controllo che i registri esistano.*/
		err_code = Register_fail;
	else {
		int a = reg_file[r1], b = reg_file[r2];
		if ((b == INT_MIN) || (a > 0 && b < 0 && a > b + INT_MAX) || (a < 0 && b > 0 && a < INT_MIN + b))
			err_code = Overflow;		/*Se c'è overflow, la sottrazione non avviene.*/
		else {
			if (push(s, a - b))			/*Verifica che push sia andata a buon fine.*/
				err_code = Stack_overflow;
		}
	}
	return err_code;
}



err_t mul(int *reg_file, int r1, int r2, Stack s)
/*
Questa funzione moltiplica i valori contenuti nei registri in input e inserisce il risultato nello stack.
Se almeno un registro non esiste, l'operazione non avviene ed è segnalato un errore (Register_fail).
Se l'operazione produce overflow aritmetico, è segnalato un errore (Overflow).
Se l'operazione è avvenuta, ma lo stack è saturo, è segnalato un errore (Stack_overflow).
*/
{
	err_t err_code = None;
	if (!is_register(r1) || !is_register(r2))	/*Controllo che i registri esistano.*/
		err_code = Register_fail;
	else {
		int a = reg_file[r1], b = reg_file[r2];
		if ((a > 0 && b > 0 && a > (double) INT_MAX / b)		||
			(a < 0 && b < 0 && a < (double) INT_MAX / b)		||
			(a > 0 && b < 0 && a > (double) INT_MIN / b)		||
			(a < 0 && b > 0 && a < (double) INT_MIN / b)		)
			err_code = Overflow;		/*Se c'è overflow, la moltiplicazione non avviene.
			****Nota: tutti i casi in cui si verifica overflow sono stati controllati.	*/
		else {
			if (push(s, a * b))			/*Verifica che push vada a buon fine.*/
				err_code = Stack_overflow;
		}
	}
	return err_code;
}



err_t div1(int *reg_file, int r1, int r2, Stack s)
/*
Questa funzione divide i valori contenuti nei registri in input e inserisce il risultato nello stack.
Se almeno un registro non esiste, l'operazione non avviene ed è segnalato un errore (Register_fail).
Se il dvisore è 0, è segnalato un errore (Zero_division).
Se l'operazione è avvenuta, ma lo stack è saturo, è segnalato un errore (Stack_overflow).
*/
{
	err_t err_code = None;
	if (!is_register(r1) || !is_register(r2))		/*Controllo che i registri esistano.*/
		err_code = Register_fail;
	else {
		int a = reg_file[r1], b = reg_file[r2];
		if (b == 0)
			err_code = Zero_division;		/*Questo passaggio previene la divisione per 0.*/
		else if (a == INT_MIN && b == -1)	/*Questo è l'unico caso in cui la divisione produce overflow.*/
			err_code = Overflow;
		else {
			if (push(s, a / b))			/*Verifica che push sia andata a buon fine.*/
				err_code = Stack_overflow;
		}
	}
	return err_code;
}