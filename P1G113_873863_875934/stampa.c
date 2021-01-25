/*
P1G113
Nicola Miotto 873863
Michele Pizzeghello 875934
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "istruzioni.h"
#include "stampa.h"
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KCYN  "\x1B[36m"


void stampa1(int numero_valori, int ip[])
{
	int i = 0;
	int alt =1;
	/*alt = 3 se è stata letta un'istruzione non riconosciuta.*/
	while(i < numero_valori && alt  && alt != 3)
	/*Questo ciclo itera per ogni istruzione.*/
	{
			printf("[%3d] ", i);


			switch(ip[i])
			{
				case HALT:
				printf("HALT\n");
				break;

				case DISPLAY:
				printf("DISPLAY ");
				i = i+1;
				if(i >= numero_valori)
				{
					alt = 0;
				}
				else
				{
					printf("R%-2d\n", ip[i]);	
				}
				break;

				case PRINT_STACK:
				printf("PRINT_STACK " );
				i++;
				if(i >= numero_valori)
					alt = 0;
				else
					printf("%-2d\n", ip[i]);
				break;

				case PUSH:
				printf("PUSH ");
				i++;
				if(i >= numero_valori)
					alt = 0;
				else
					printf("R%-2d\n", ip[i]);
				break;

				case POP:
				printf("POP ");
				i++;
				if(i >= numero_valori)
					alt = 0;
				else
					printf("R%-2d\n", ip[i]);
				break;
				
				case MOV:
				printf("MOV ");
				i++;
				if(i >= numero_valori)
					alt = 0;
				else
				{
					printf("R%-2d ", ip[i]);
					i++;
					if(i >= numero_valori)
						alt = 0;
					else
					{
						printf("%-2d\n", ip[i]);
					}

				}
				break;



				case CALL:
				printf("CALL ");
				i++;
				if(i >= numero_valori)
					alt = 0;
				else
					printf("%-2d\n", ip[i]);
				break;

				case RET:
				printf("RET\n");
				break;

				case JMP:
				printf("JMP ");
				i++;
				if(i >= numero_valori)
					alt = 0;
				else
					printf("%-2d\n", ip[i]);
				break;


				case JZ:
				printf("JZ ");
				i++;
				if(i >= numero_valori)
					alt = 0;
				else
					printf("%-2d\n", ip[i]);
				break;


				case JPOS:
				printf("JPOS ");
				i++;
				if(i >= numero_valori)
					alt = 0;
				else
					printf("%-2d\n", ip[i]);
				break;

				case JNEG:
				printf("JNEG ");
				i++;
				if(i >= numero_valori)
					alt = 0;
				else
					printf("%-2d\n", ip[i]);
				break;



				case ADD:
				printf("ADD ");
				i++;
				if(i >= numero_valori)
					alt = 0;
				else
				{
					printf("R%-2d ", ip[i]);
					i++;
					if(i >= numero_valori)
						alt = 0;
					else
					{
						printf("R%-2d\n", ip[i]);
					}

				}
				break;


				case SUB:
				printf("SUB ");
				i++;
				if(i >= numero_valori)
					alt = 0;
				else
				{
					printf("R%-2d ", ip[i]);
					i++;
					if(i >= numero_valori)
						alt = 0;
					else
					{
						printf("R%-2d\n", ip[i]);
					}

				}
				break;


				case MUL:
				printf("MUL ");
				i++;
				if(i >= numero_valori)
					alt = 0;
				else
				{
					printf("R%-2d ", ip[i]);
					i++;
					if(i >= numero_valori)
						alt = 0;
					else
					{
						printf("R%-2d\n", ip[i]);
					}

				}
				break;

				case DIV:
				printf("DIV ");
				i++;
				if(i >= numero_valori)
					alt = 0;
				else
				{
					printf("R%-2d ", ip[i]);
					i++;
					if(i >= numero_valori)
						alt = 0;
					else
					{
						printf("R%-2d\n", ip[i]);
					}

				}
				break;

				default:
				printf("%sCommand not found.%s\n", KRED, KNRM);
				alt = 3;
				break;
			
			}
			
			i++;
		
	}

	if(alt == 0)
	{
		printf("\n%sAn error in the code was found (missing parameter(s)).%s\n", KRED, KNRM);
	}
	
}