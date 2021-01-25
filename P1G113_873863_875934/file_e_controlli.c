/*
P1G113
Nicola Miotto 873863
Michele Pizzeghello 875934
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_e_controlli.h"
#include <limits.h>
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KCYN  "\x1B[36m"


int is_valid(char line)
{
	return line == ';' || line == '\r' || line == '\n';
}

int read_line(FILE *fin, int *numero2, int *inserito)
{
	/*
	Questa funzione restituisce il risultato dell'analisi di una riga.
	Se la funzione restituisce 1, vuol dire che la riga è sintatticamente corretta.
		Inolte, se la variabile a cui punta inserito è pari a 1, la funzione ha rilevato un numero.
		In caso contrario, tale variabile è impostata a 0.
	Se la funzione restituisce -1, vuol dire che si è tentato di leggere un intero che provoca overflow.
		Si noti che questa funzione segnala overflow se il modulo del numero letto supera INT_MAX.
		Per questa ragione, il numero INT_MIN non può essere letto da questa funzione.
		INT_MAX e INT_MIN sono definiti in limits.h.
	Se la funzione restituisce 0, vuol dire che la riga esaminata ha una sintassi errata.
	*/
	int overflow = 0;
	char error = 0;			/*Valore restituito alla fine.*/
	char *line = NULL;		/*Stringa che contiene la riga letta.*/
	size_t n = 0;			/*Per getline.*/
	if (getline(&line, &n, fin) == -1)
		error = EOF;		/*Il file è terminato.*/
	else {
		int i = 0, parsed = 0;		/*i scorre la stringa, parsed sarà il numero letto.*/
		int sign = 1;				/*Questo è il segno del numero letto.*/
		*inserito = 0;

		while(line[i] == ' ' || line[i] == '\t')
			i++;	/*Questo serve perché una riga può cominciare con un numero arbitrario di spazi o tabulazioni.*/


		if (is_valid(line[i]))		/*Questo controllo verifica se una riga è vuota a meno di commenti.*/
			error = line[i];
		else {
			/*
			Controllo del segno del numero da leggere. Se e solo se viene trovato un carattere '-',
			il segno diventa negativo. Ciò significa che il carattere '+' può essere omesso.
			*/
			if (line[i] == '+')
				i++;
			else if (line[i] == '-') {
				sign = -1;
				i++;
			}
			do {/*
				Lettura del numero e verifca che non vi siano caratteri estranei.
				Un numero può cominciare con un numero arbitrario di '0'.
				Si interrompe in caso di overflow.
				*/
				if (line[i] >= '0' && line[i] <= '9') {
					if (parsed > (double) INT_MAX / 10)
						overflow = 1;
					else {
						parsed *= 10;
						if (parsed > INT_MAX - line[i] + '0')
							overflow = 1;
						else
							parsed += line[i] - '0';
					}
				}
				else
					error = line[i];		/*Ho trovato un carattere estraneo.*/
				i++;
			} while(!error && !overflow);
			/*Assicurarsi che tra il numero e ';' ci siano solo spazi o tab.*/
			if (!overflow) {
				while (error == ' ' || error == '\t')
					error = line[i++];
				if (is_valid(error)) {
					*numero2 = parsed * sign;
					*inserito = 1;
				}
			}
		}
	}
	free(line);
	if (overflow)
		return -1;
	else
		return is_valid(error) ? 1 : 0;
}


int controllo_argomenti(int i, char **args)
{
	/*controlla i parametri se sono corretti*/

	if (i < 3) {
		printf("%sToo few parameters. Expected 2.%s\n", KRED, KNRM);
		return -1;
	}
	else if (i > 3) {
		printf("%sToo many parameters. Expected 2.%s\n", KRED, KNRM);
		return -1;
	}
	if (strcmp(args[1], "esegui") && strcmp(args[1], "stampa")) {
		printf("%sInvalid parameter.%s\n",KRED ,KNRM);
		return -1;
	}
	/*controlla l' estensione del file*/
	if (!correct_ext(args[2]))
	{
		printf("%sFile extension is not .cvm%s\n", KRED, KNRM);
		return -1;
	}
	return 0;
}


/*La seguente funzione serve per accertarsi che il file da leggere abbia effettivamente
estensione '.cvm'.*/
int correct_ext(char *name)
{
	/*Non è possibile che il file abbia un nome vuoto, il seguente funge solo da caso base.*/
	if (*name == '\0')
		return 0;
	else
		/*Controllare per ogni carattere se esso corrisponde all'inizio dell'estensione '.cvm'.*/
		return !strcmp(name, ".cvm") || correct_ext(name + 1);
		/*1 giusto e 0 sbagliata*/
}