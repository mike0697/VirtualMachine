/*
P1G113
Nicola Miotto 873863
Michele Pizzeghello 875934
*/
#include "exe.h"

int valid_IP(int p_val, int i_num, err_t *err)
/*Questa funzione si accerta che il dato da leggere si trovi dentro l'array di istruzioni.*/
{
	if (p_val >= i_num || p_val < 0) {
		*err = Missing_argument;
		return 0;
	}
	else
		return 1;
}

err_t esegui(int *array_di_istruzioni, unsigned int instr_num, unsigned int *ip, int *registry, Stack s)
{
	err_t error = None;			/*Variabile che contiene il codice di errore restituito alla fine.*/
	int halt = 0;				/*Variabile che controlla se è stata trovata l'istruzione HALT.*/
	/*int i;*/
	while (*ip < instr_num && !halt && error == None) {
	/*
	Questo ciclo si interrompe:
		se non ci sono più istruzioni da leggere;
		se è stata eseguita l'istruzione "HALT";
		se si sono verificati errori che interrompono l'esecuzione del programma.
	*/
		/*for (i = 0; i < 4; i++)
			printf("Reg[%d]: %d\n", i, registry[i]);
		printf("\n");*/
		(*ip)++;
		switch (array_di_istruzioni[*ip - 1]) {	/*Il comportamento dell'iterazione varia a seconda dell'istruzione letta.*/
			case HALT:
				/*Nel caso in cui venga trovata l'istruzione HALT, bisogna interrompere l'esecuzione del programma.
				Ciò equivale a interrompere il ciclo principale di questa funzione.
				La variabile error rimane impostata a 0: non si sono verififcati errori.*/
				halt = 1;
				break;
			case DISPLAY:
				/*Verificare che ci sia un registro da leggere.*/
				if (valid_IP(*ip, instr_num, &error))
					error = display(array_di_istruzioni[*ip], registry);
				(*ip)++;
				break;
			case PRINT_STACK:
				/*Verificare che ci sia un parametro da leggere.*/
				if (valid_IP(*ip, instr_num, &error))
					print_stack(s, array_di_istruzioni[*ip]);
				(*ip)++;
				break;
			case PUSH:
				/*Verificare che ci sia un registro da leggere.*/
				if (valid_IP(*ip, instr_num, &error)) {
					/*Verificare che il registro sia valido.*/
					if (is_register(array_di_istruzioni[*ip])) {
						if (push(s, registry[array_di_istruzioni[*ip]]))
							/*Questo controllo equivale a "se push non è andata a buon fine..."*/
							error = Stack_overflow;
					}
					else
						error = Register_fail;
				}
				(*ip)++;
				break;
			case POP:
				/*Verificare che ci sia un registro da sovrascrivere.*/
				if (valid_IP(*ip, instr_num, &error)) {
					if (is_register(array_di_istruzioni[*ip])) {
						if (pop(s, &registry[array_di_istruzioni[*ip]]))
							/*Questo controllo equivale a "se pop non è andata a buon fine..."*/
							error = Stack_underflow;
					}
					else
						error = Register_fail;
				}
				(*ip)++;
				break;
			case MOV:
				/*Verificare che ci siano sufficienti argomenti.*/
				(*ip)++;
				if (valid_IP(*ip, instr_num, &error))
					error = move(registry, array_di_istruzioni[*ip - 1], array_di_istruzioni[*ip]);
				(*ip)++;
				break;
			case CALL:
				/*Verificare che ci sia un parametro da leggere.*/
				if (valid_IP(*ip, instr_num, &error)) {
					/*Verificare che non si esegua un salto fuori dall'array di istruzioni.*/
					if (valid_IP(array_di_istruzioni[*ip], instr_num, &error)) {
						if (push(s, *ip + 1))
							error = Stack_overflow;
						else
							*ip = array_di_istruzioni[*ip];
					}
					else
						error = Out_of_bounds;
				}
				break;
			case RET:
				if (pop(s, (int *) ip))
					error = Stack_underflow;
				else if (!valid_IP((int) *ip, instr_num, &error)) {
					error = Out_of_bounds;
				}
				break;
			case JMP:
				/*Verificare che ci sia un parametro da leggere.*/
				if (valid_IP(*ip, instr_num, &error))
					/*Verificare che non si esegua un salto fuori dall'array di istruzioni.*/
					error = jump(ip, array_di_istruzioni[*ip], instr_num);
				break;
			case JZ:
				/*Verificare che ci sia un parametro da leggere.*/
				if (valid_IP(*ip, instr_num, &error)) {
					/*Verificare che nello stack ci sia un elemento da leggere.*/
					if (s -> sp < 1)
						error = Stack_underflow;
					else {
						(s -> sp)--;					/*Istruzione pop a vuoto.*/
						/*Il salto avviene se l'ultimo elemento dello stack è 0.*/
						if (s -> v[s -> sp] == 0)
							error = jump(ip, array_di_istruzioni[*ip], instr_num);
						else
							(*ip)++;
					}
				}
				break;
			case JPOS:
				/*Verificare che ci sia un parametro da leggere.*/
				if (valid_IP(*ip, instr_num, &error)) {
					/*Verificare che nello stack ci sia un elemento da leggere.*/
					if (s -> sp < 1)
						error = Stack_underflow;
					else {
						(s -> sp)--;					/*Istruzione pop a vuoto.*/
						/*Il salto avviene se l'ultimo elemento dello stack è positivo.*/
						if (s -> v[s -> sp] > 0)
							error = jump(ip, array_di_istruzioni[*ip], instr_num);
						else
							(*ip)++;
					}
				}
				break;
			case JNEG:
				/*Verificare che ci sia un parametro da leggere.*/
				if (valid_IP(*ip, instr_num, &error)) {
					/*Verificare che nello stack ci sia un elemento da leggere.*/
					if (s -> sp < 1)
						error = Stack_underflow;
					else {
						(s -> sp)--;					/*Istruzione pop a vuoto.*/
						/*Il salto avviene se l'ultimo elemento dello stack è negativo.*/
						if (s -> v[s -> sp] < 0)
							error = jump(ip, array_di_istruzioni[*ip], instr_num);
						else
							(*ip)++;
					}
				}
				break;
			/*
			Le rimanenti istruzioni sono istruzioni aritmetiche.
			In ciascun caso occorre controllare qui che vi siano sufficienti parametri.
			In caso affermativo, ogni istruzione possiede la propria funzione C.
			Esse effettuano le operazioni e i controlli necessari.
			*/
			case ADD:
				(*ip)++;
				if (valid_IP(*ip, instr_num, &error))
					error = add(registry, array_di_istruzioni[*ip - 1], array_di_istruzioni[*ip], s);
				(*ip)++;
				break;
			case SUB:
				(*ip)++;
				if (valid_IP(*ip, instr_num, &error))
					error = sub(registry, array_di_istruzioni[*ip - 1], array_di_istruzioni[*ip], s);
				(*ip)++;
				break;
			case MUL:
				(*ip)++;
				if (valid_IP(*ip, instr_num, &error))
					error = mul(registry, array_di_istruzioni[*ip - 1], array_di_istruzioni[*ip], s);
				(*ip)++;
				break;
			case DIV:
				(*ip)++;
				if (valid_IP(*ip, instr_num, &error))
					error = div1(registry, array_di_istruzioni[*ip - 1], array_di_istruzioni[*ip], s);
				(*ip)++;
				break;
			default:
				/*
				Se il controllo è arrivato a questo punto, significa che l'istruzione non è riconosciuta.
				In questo caso, bisogna segnalare un errore.
				*/
				error = Invalid_intruction;
		}
	}
	/*
	Se halt è diversa da 0, il programma è stato eseguito correttamente.
	Se error è diversa da None, si è verificato un errore.
	In entrambi i casi, non occorre segnalare che l'istruzione HALT non è stata eseguita.
	Se invece non si sono verificati errori, ma non è nemmeno stata eseguita HALT, bisogna segnalarlo.
	*/
	if (!halt && error == None)
		error = Halt_not_found;
	return error;
	/*
	Se la funzione restituisce None, è andato tutto bene.
	Se la funzione restituisce Halt_not_found, il programma è stato eseguito finché è stato possibile.
	Se la funzione restituisce un altro valore, si è verificato un errore grave che ha interrotto
	l'esecuzione del programma.
	*/
}