/*
P1G113
Nicola Miotto 873863
Michele Pizzeghello 875934
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int correct_ext(char *name);
int controllo_argomenti(int i, char **args);
int read_line(FILE *fin, int *numero2, int *inserito);
int is_valid(char line);
int riempiArrey(int *p, int size, FILE *Iptr);