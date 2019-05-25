CC = gcc
NAME = main
FLAGS = -w -fsanitize=address

all: archive

main.o: main.c archive.h
	$(CC) $(FLAGS) -c -o main.o main.c

myarchive.o: archive.c archive.h
	$(CC) $(FLAGS) -c -o archive.o archive.c

archive: main.o archive.o
	$(CC) $(FLAGS) -o $(NAME) main.o archive.o

clean:
	rm *.o
	rm $(NAME)