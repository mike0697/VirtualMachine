/*
P1G113
Nicola Miotto 873863
Michele Pizzeghello 875934

Consegnare P1G113_873863_875934.zip
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "exe.h"
#include "stampa.h"
/*colori*/
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KCYN  "\x1B[36m"


int main(int argc, char *argv[])
{
	int controllo_parametri;	/*verifica che i parametri siano corretti*/
	int *array_istruzioni;		/*array contenete tutte le istruzioni*/
	unsigned int ip = 0;		/*istruction pointer*/
	int numero_valori = -1;		/*dimensione array di istruzioni*/
	int inserito;				/*per inizializzare l'array di istruzioni*/
	int controllo;				/*per inizializzare l'array di istruzioni*/
	int valore_valido = 0;		/*per inizializzare l'array di istruzioni*/
	int riempiArray_risultato;	/*controlla l'esito di riempiArrey*/
	/**/

    int registry[32]={0};		/*register file*/
    struct stack t_stack;		/*stack con annesso regitro speciale SP (stack pointer)*/
    Stack e = &t_stack;			/*puntatore a stack*/
    FILE *Iptr;					/*puntatore a file input*/
    err_t risultato = None;		/*esito del comando "esegui"*/
		/*NOTA: il tipo err_t (alias enum error) è definito in istruzioni.h.*/

    int alt = 1;				/*per controllo bootstrap e stampa*/
	
	/*Stringhe costanti rilevanti.*/
	const char *fe = "FATAL ERROR: ";	/*Lo spazio alla fine serve!*/
	const char *of_msg = "Overflow detected while reading file.";
	const char *sep = "----------------------";



    /*controlla i parametri se sono corretti*/
	/*controlla l' estensione del file*/
	/*I controlli vengono fatti in file_e_controlli.c.*/
	controllo_parametri = controllo_argomenti(argc, argv);
	if (controllo_parametri != 0)
		return -1;


	/*fase di bootstrap : inizializza le strutture*/
	/*Richiama memoria.c.*/
    alt = bootstrap(e);
    if (alt != 0)
    {
    	printf("%s%sstack allocation failed (bootstrap error).%s\n",KRED, fe, KNRM);
        return alt;
    }



	/* controllo se esiste il file */
    Iptr = fopen(argv[2], "r");
    if (Iptr == NULL) {
    	destroy(e);
    	printf("%sFile not found.%s\n", KRED , KNRM);
    	return -1;
    }

	/**legge la prima riga finche non trova una riga contenete un numero o un valore 
	se contiene spazi o e un commento salta la riga
	**/

	/*la funzione is_valid controlla se non sono caratteri consentiti*/

    while(valore_valido == 0)
    {
		/*read_line si trova in file_e_controlli.c.*/
	    controllo = read_line(Iptr, &numero_valori, &inserito);
	    if(controllo == 1)
	    {
	    	/*riga valida*/
	    	if(inserito == 0)
	    	{
	    		/*riga valida ma non contine un numero*/
	    		valore_valido = 0;
	    	}
	    	else if (numero_valori <= 0) {
				valore_valido = -1;
				printf("%sInvalid first line: number of instructions must be strictly positive.%s\n", KRED, KNRM);
			}
			else
	    	{
	    		valore_valido = 1;
	    		printf("Allocated instructions: %d\n", numero_valori);
	    	}
	    }
	    else if (controllo == -1)
		{
			printf("%s%s%s\n", KRED, of_msg, KNRM);
			valore_valido = -1;
		}
		else
	    {
	    	printf("%sInvalid first line.%s\n", KRED, KNRM);
	    	valore_valido = -1;
	    }
	}


	/* se contiene errori chiude il progrmma */
	if(valore_valido == -1)
	{
		fclose(Iptr);
		destroy(e);
    	return 1;
	}



	/*creare l' array di dimensione prima riga*/


	array_istruzioni = (int *)malloc(sizeof(int) * numero_valori);

	/*controlla se la malloc non e' andata a buon fine*/
	if(array_istruzioni == NULL)
	{
		/*istruzione halt fa terminare il programma*/
		printf("%s%sAllocation failed.%s\n",KRED, fe, KNRM);
		halt(e, array_istruzioni, Iptr);
		return 0;
	}
	else {
		printf("%sC Virtual Machine succesfully booted.%s\n", KCYN, KNRM);
	}


	/*
	A questo punto, l'array di istruzioni deve essere riempito con le istruzioni
	lette dal file in input.
	*/
	riempiArray_risultato = riempiArrey(array_istruzioni, numero_valori, Iptr);
	if(riempiArray_risultato == -1)			/*Questo controllo verifica che tutte le righe del file siano corrette.*/
	{	
		printf("%sInvalid or missing instruction(s).%s\n", KRED, KNRM);	
		halt(e, array_istruzioni, Iptr);
		return 0;
	}
	else if (riempiArray_risultato == -2) {	/*Questo controllo verifica che nessun intero nel file provochi overflow.*/
		printf("%s%s%s\n", KRED, of_msg, KNRM);
		halt(e, array_istruzioni, Iptr);
		return 0;
	}


	/*Caso in cui il comando argv[1] sia 'esegui'.*/
	if(!strcmp(argv[1], "esegui"))
	{
		printf("OUTPUT\n%s\n", sep);
		risultato = esegui(array_istruzioni, (unsigned int) numero_valori, &ip, registry, e);
			/*NOTA: la funzione esegui è definita in exe.c.*/
		printf("%s\n", sep);
		switch (risultato) {	/*
		Il comportamento di questo pezzo di codice varia a seconda del risultato della chiamata di esegui.
		*/
			case None:
				break;
			case Overflow:
				printf("%s%sarithmetic overflow occurred.%s\n",KRED, fe, KNRM);
				break;
			case Zero_division:
				printf("%s%sattempt to divide by zero.%s\n", KRED, fe, KNRM);
				break;
			case Stack_overflow:
				printf("%s%sexecution stopped due to stack overflow.%s\n", KRED, fe, KNRM);
				break;
			case Stack_underflow:
				printf("%s%sexecution stopped due to stack underflow.%s\n", KRED, fe, KNRM);
				break;
			case Invalid_intruction:
				printf("%s%sinstruction not valid.%s\n", KRED, fe, KNRM);
				break;
			case Missing_argument:
				printf("%s%slast read instruction expected one or two parameters which were not found.%s\n", KRED, fe, KNRM);
				break;
			case Out_of_bounds:
				printf("%s%sattempt to jump to forbidden memory location.%s\n", KRED, fe, KNRM);
				break;
			case Register_fail:
				printf("%s%srequested register does not exist.%s\n", KRED, fe, KNRM);
				break;
			case Halt_not_found:
				printf("%sWARNING: execution stopped with no HALT instruction.%s\n", KRED , KNRM);
				break;
			default:		/*Caso filler: non è previsto.*/
				printf("%sUnknown error.%s\n", KRED , KNRM);
		}
	}

	/*Caso in cui il parametro argv[1] sia 'stampa'.*/
	if(!strcmp(argv[1], "stampa"))
	{
		printf("DISASSEMBLING\n%s\n", sep);
		stampa1(numero_valori, array_istruzioni);
		printf("%s\n", sep);
	}


	/*Questa istruzione libera la memoria allocata dinamicamente e chiude il file aperto.*/
	halt(e, array_istruzioni, Iptr);
    return 0;
}