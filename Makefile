objects = windows.o test.o config.o network.o errors.o
options = -lpanel -lncurses -pthread

all : $(objects)
	gcc -o test $(objects) $(options)

test.o : test.c
windows.o : windows.c windows.h
config.o : config.c config.h
network.o : network.c network.h
errors.o : errors.c errors.h

.PHONY : clean dist all
clean : 
	-rm -f test $(objects)
dist :
	-rm -f $(objects)