MYLIBDIR=../mylib
MYLIB=$(MYLIBDIR)/mylib.a
CFLAGS=-I$(MYLIBDIR)

all: s c

s: server.o sessionmain.o
	$(CC) -o $@ $^

c: client.o session.o
	$(CC) -o $@ $^ $(MYLIB) -lcurses

server.o: mtalk2.h sessionmain.h

client.o: mtalk2.h session.h

sessionmain.o session.o: mtalk2.h

.PHONY: clean
clean:
	$(RM) *.o s c *~