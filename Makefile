CC=g++
CFLAGS=
#DEPS = hellomake.h
OBJ = sha256.o ubsub.o main.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

ubsub: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)