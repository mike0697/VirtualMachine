#P1G113
#Nicola Miotto 873863
#Michele Pizzeghello 875934

#http://makefiletutorial.com/
objects = main.o array.o file_e_controlli.o memoria.o istruzioni.o stampa.o exe.o
eseguibile = main
cflags = -g3 -fsanitize=address -fsanitize=undefined -fsanitize=leak -std=gnu89 -pedantic-errors -Wall -Wextra 
libs = -lm

$(eseguibile) : $(objects)
	gcc $(cflags) -o $(eseguibile) $(objects) $(libs)

clean:
	rm $(eseguibile)  $(objects)

#regole implicite
.c.o :
	gcc -c $(cflags) $< 
.h.c :
	touch $<