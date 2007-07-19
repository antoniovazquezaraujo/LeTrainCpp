CC= g++
CFLAGS= -ggdb -ansi -pedantic -Wall -W -Wendif-labels -Wcast-qual -Wwrite-strings -Wcast-align -Wpointer-arith -Wconversion -Wredundant-decls -Wunreachable-code -Winline -Wlarger-than-256
SRCS= commander.cpp sim.cpp vehicle.cpp graphics.cpp list.cpp node.cpp iterator.cpp rail.cpp cross.cpp point.cpp trainiterator.cpp dir.cpp railmap.cpp finder.cpp logging.cpp persist.cpp charmap.cpp
HDRS= letrain.h graphics.h logging.h test.h

all:letrain 

letrain : $(SRCS) $(HDRS) 
	$(CC) $(CFLAGS) letrain.cpp $(SRCS) -o letrain -lncurses 

.PHONY: clean
clean:
	rm -f *.o
	rm -f *.log
	rm -f a.out
	rm -f letrain_test
	rm -f letrain

print:
	enscript -G2rE *.h *.cpp -pdata.ps
	ps2pdf data.ps
	rm -f data.ps

backup: clean
	mkdir -p ../ooletrainbackup
	tar cvzf ../ooletrainbackup/`date +%F_%H_%S_%M`".tar" *

restore: backup	
	dialog --fselect ../backup/ 15 70 2>respuesta
	tar xvzf `cat respuesta`
	rm -f respuesta
