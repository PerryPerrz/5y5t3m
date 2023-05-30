CC = gcc
OFLAGS = -o
WFLAGS = -Wall
CFLAGS = -c
extenC = .c
extenO = .o

PROG = exo1pere
REC = mes_semaphores
FIC = monfichierexecutablefinal

#On compile le main 
main:
	$(CC) $(WFLAGS) $(CFLAGS) $(PROG)$(extenC) $(OFLAGS) $(PROG)$(extenO)
	$(CC) $(WFLAGS) $(CFLAGS) $(REC)$(extenC) $(OFLAGS) $(REC)$(extenO)
	$(CC) $(WFLAGS) $(PROG)$(extenO) $(REC)$(extenO) $(OFLAGS) $(FIC)

#On clean les fichiers exécutables
clean:
	rm $(PROG)$(extenO)
	rm $(REC)$(extenO)
	rm $(FIC)
