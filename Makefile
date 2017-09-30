CC=g++
CFLAGS=
#DEPS = hellomake.h
OBJ = src/sha256.o src/ubsub.o src/test.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

ubsub: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
