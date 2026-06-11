# NO TOCAR / NOT MODIFIED ME ##
CC=gcc
FLAGS=-Wno-implicit-function-declaration
CFLAGS=-I.
###############################

# MODIFIED ME ##

OBJ1 = scripter.o 
OBJ2 = mygrep.o

all: scripter mygrep


scripter: $(OBJ1)
	$(CC) $(FLAGS) -L. -o $@ $< $(LIBS)

mygrep: $(OBJ2)
	$(CC) $(FLAGS) -L. -o $@ $< $(LIBS)

%.o: %.c 
	$(CC) $(FLAGS) -c -o $@ $< $(CFLAGS)

clean:
	rm -f ./scripter.o ./scripter ./mygrep.o ./mygrep
