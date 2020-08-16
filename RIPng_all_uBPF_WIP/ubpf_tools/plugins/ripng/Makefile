CC=clang-12
LLC=llc-12

SRC=$(shell find . -name '*.c')
OBJ=$(SRC:.c=.o)
CFLAGS=-I../.. -I../../lib -Wall -Wextra


all: $(SRC) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -fno-stack-protector -O2 -emit-llvm -c $< -o - | $(LLC) -march=bpf -filetype=obj -o $@

clean:
	rm *.o

.PHONY: all clean