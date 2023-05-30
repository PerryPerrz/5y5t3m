CC = gcc
OFLAGS = -o
WFLAGS = -Wall
extenC = .c

PROG = exo2serveur
REC = exo2client

#On compile le main 
main:
	$(CC) $(WFLAGS) $(PROG)$(extenC) $(OFLAGS) $(PROG)
	$(CC) $(WFLAGS) $(REC)$(extenC) $(OFLAGS) $(REC)

#On clean les fichiers exécutables
clean:
	rm $(PROG)
	rm $(REC)
