CC=g++
CFLAGS=-I. -Wall
LIBS=-lcurl -lzip -lZipper
DEPS=curse.h
#DEPS=main.h

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

wow-updater: main.o curse.o
	$(CC) -o wow-updater main.o curse.o $(LIBS)

clean:
	rm -f *.o wow-updater