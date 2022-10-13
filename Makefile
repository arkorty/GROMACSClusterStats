CC=gcc
CFLAGS=
PROG=gromacs-mod

$(PROG): $(PROG).c
	$(CC) $(CFLAGS) $(PROG).c -o $(PROG)

run:
	./$(PROG) set-a.txt set-b.txt

clean:
	rm $(PROG)