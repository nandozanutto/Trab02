all: main
main: sistemalinear.o utils.o main.o
	gcc -o sistemalinear sistemalinear.o utils.o main.o -lm -Wall
sistemalinear.o: sistemalinear.c
main.o: main.c
utils.o: utils.c
clean:
	rm -rf *.o sistemalinear
