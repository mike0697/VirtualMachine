/*
P1G113
Nicola Miotto 873863
Michele Pizzeghello 875934
*/
#include "istruzioni.h" 
int valid_IP(int p_val, int i_num, err_t *err);
err_t esegui(int *program, unsigned int instr_num, unsigned int *pointer, int *reg_file, Stack s);