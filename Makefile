progname=programaTrab2

all:
	gcc main.c src/*.c -o $(progname) -Iinc/. -std=c99

run:
	./$(progname)

zip: 
	zip -r $(progname).zip Makefile main.c src/ inc/

