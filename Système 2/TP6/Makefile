CC = gcc
OFLAGS = -o
WFLAGS = -Wall
CFLAGS = -c
extenC = .c
extenO = .o

PROG = serveur
REC = client

#On compile le main
main:
	$(CC) $(WFLAGS) $(CFLAGS) $(PROG)$(extenC) $(OFLAGS) $(PROG)
	$(CC) $(WFLAGS) $(CFLAGS) $(REC)$(extenC) $(OFLAGS) $(REC)

#On clean les fichiers exécutables
clean:
	rm $(PROG)
	rm $(REC)