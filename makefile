assembler: assembler.o first_read.o second_read.o other1.o other2.o
	gcc -g -ansi -Wall -pedantic assembler.o first_read.o second_read.o other1.o other2.o -o assembler
assembler.o: assembler.c assembler.h info.h
	gcc -c -ansi -Wall -pedantic assembler.c -o assembler.o
first_read.o: first_read.c info.h info2.h
	gcc -c -ansi -Wall -pedantic first_read.c -o first_read.o
second_read.o: second_read.c info.h info2.h
	gcc -c -ansi -Wall -pedantic second_read.c -o second_read.o
other1.o: other1.c info.h info2.h
	gcc -c -ansi -Wall -pedantic other1.c -o other1.o
other2.o: other2.c info.h 
	gcc -c -ansi -Wall -pedantic other2.c -o other2.o
