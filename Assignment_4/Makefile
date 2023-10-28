CC   = cc
OBJS = linkedlists.o hash.o hashtable.o

CFLAGS = -O3 -g3 -Wall -Wextra -Werror=format-security -Werror=implicit-function-declaration \
         -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wwrite-strings \

%.o: %.c
	${CC} $(CFLAGS) -c -o $@ $<

all: dictionary

Q1: $(OBJS) Q1.o
	${CC} -o $@ $^

Q2: $(OBJS) Q2.o
	${CC} -o $@ $^

run1: Q1
	./Q1 sp-en-dictionary.txt

run2: Q2
	./Q2 sp-en-mini.txt

clean:
	rm -f *.o Q1 Q2

linkedlists.o: linkedlists.c linkedlists.h
hash.o: hash.c hash.h
hashtable.o: hashtable.c hashtable.h linkedlists.h

