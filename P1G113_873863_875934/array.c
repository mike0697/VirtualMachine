/*
P1G113
Nicola Miotto 873863
Michele Pizzeghello 875934
*/
#include <stdio.h>
#include <stdlib.h>
#include "file_e_controlli.h"

int riempiArrey(int *p, int size, FILE *Iptr)
{
	int i = 0;
	int valore, inserito;
	int controllo = 0;
	int valido = 0;
	/*
	valido == -1 se la riga letta non è sintatticamente corretta.
	valido == -2 se la lettura del numero ha generato overflow.
	valido == 1 se la riga è sintatticamente corretta.
	*/
	while(i<size && valido != -1 && valido != -2)
	/*Questo ciclo itera per ogni elemento dell'array di istruzioni.*/
	{
		valido = 0;
		inserito = 0;
		
			while(valido == 0)
			/*Questo ciclo itera finché non viene trovata una riga non vuota.*/
		    {
			    controllo = read_line(Iptr, &valore, &inserito);

				
			    if(controllo == 1)	/*Controllo che la riga sia corretta.*/
			    {
			    	if(inserito == 0)
			    	{
			    		valido = 0;
			    	}
			    	else
			    		valido = 1;
			    }
			    else
			    {
			    	valido = (controllo == -1) ? -2 : -1;
			    }
			}
			if(inserito == 1)	/*Controllo se la riga corretta conteneva effettivamente un numero.*/
			{
				p[i] = valore;
				i++;
			}
	}
	

	return valido;

}