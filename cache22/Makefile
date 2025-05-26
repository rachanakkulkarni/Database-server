# Define flags
flags = -O2 -Wall -std=c2x
ldflags =

# Default target
all: htable

hatable: htable.o
        cc ${flags} $^ -o $@ ${ldflags}

htable.o: htable.c
        cc ${flags} -c $^

# Build tree binary
tree: tree.o
        cc ${flags} $^ -o $@ ${ldflags}

tree.o: tree.c
        cc ${flags} -c $^

# Build cache22 binary
cache22: cache22.o
        cc ${flags} $^ -o $@ ${ldflags}

cache22.o: cache22.c
        cc ${flags} -c $^

# Clean rule
clean:
        rm -f *.o cache22 tree 
