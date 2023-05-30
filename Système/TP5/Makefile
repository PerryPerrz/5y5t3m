CC = gcc
OFLAGS = -o
extenC = .c

STAT = main
REC = recouvrant
RECB = recouvrant_bleu
RECV = recouvrant_vert
RECR = recouvrant_rouge

#On compile le main 
main:
	$(CC) $(STAT)$(extenC) $(OFLAGS) $(STAT)
	$(CC) $(REC)$(extenC) $(OFLAGS) $(REC)
	$(CC) $(RECB)$(extenC) $(OFLAGS) $(RECB)
	$(CC) $(RECV)$(extenC) $(OFLAGS) $(RECV)
	$(CC) $(RECR)$(extenC) $(OFLAGS) $(RECR)

#On clean les fichiers exécutables
clean:
	rm $(STAT)
	rm $(REC)
	rm $(RECB)
	rm $(RECV)
	rm $(RECR)
