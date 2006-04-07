CC= gcc
CFLAGS= -c -g3 -Wall
OBJS= commander.o sim.o vehicle.o graphics.o list.o rail.o railmap.o finder.o logging.o persist.o charmap.o

all:letrain 

test : libletrain.a letrain_test.c 
	$(CC) -ggdb -Wall letrain_test.c test.c -o letrain_test -L. -lletrain -lncurses 
	./letrain_test

letrain : libletrain.a letrain.c 
	$(CC) -ggdb -Wall letrain.c -o letrain -L. -lletrain -lncurses 

libletrain.a: $(OBJS) 
	rm -f libletrain.a
	ar cru libletrain.a $(OBJS) 
	ranlib libletrain.a

.PHONY: clean
clean:
	rm -f *.o
	rm -f *.log
	rm -f a.out
	rm -f libletrain.a
	rm -f letrain_test
	rm -f letrain

backup: clean
	tar cvzf ../backup/`date +%F_%H_%S_%M`".tar" *

$(OBJS): letrain.h 

