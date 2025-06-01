flags=-O2 -Wall -std=c2x
ldflags=

all: clean cache22

tree.o: tree.c
        cc ${flags} -c $^

cache22: cache22.o tree.o
        cc ${flags} $^ -o $@ ${ldflags}

cache22.o: cache22.c
        cc ${flags} -c $^

clean:
        rm -f *.o cache22
