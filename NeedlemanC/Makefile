CC = gcc
CFLAGS = -W -Wall
EXE = main
OBJ = main.o
all: $(EXE)
main: $(OBJ)
	$(CC) -o $@ $^
%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)
main.o: main.c
	$(CC) -o $@ -c $< $(CFLAGS)
.PHONY: clean cleanall
clean: 
	rm -rf *.o
cleanall: clean
	rm -rf $(EXE)