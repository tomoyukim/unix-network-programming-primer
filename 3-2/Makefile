MYLIBDIR	=../mylib
MYLIB			=$(MYLIBDIR)/mylib.a
CFLAGS		=-I$(MYLIBDIR)

all: s c

s: server.o goban.o
	$(CC) -o $@ $^ $(MYLIB)

c: client.o goban.o
	$(CC) -o $@ $^ $(MYLIB)

server.o client.o: goban.h

.PHONY: clean
clean:
	$(RM) s c *.o *~