CC = gcc
OFLAGS = -o
WFLAGS = -Wall
extenC = .c

PROG = timerplusplus
REC = timerplusplus_recouvrant

#On compile le main 
main:
	$(CC) $(WFLAGS) $(PROG)$(extenC) $(OFLAGS) $(PROG)
	$(CC) $(WFLAGS) $(REC)$(extenC) $(OFLAGS) $(REC)

#On clean les fichiers exécutables
clean:
	rm $(PROG)
	rm $(REC)