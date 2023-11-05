CC   = cc
OBJS = redblacktrees.o searchtrees.o

CFLAGS = -O3 -g3 -Wall -Wextra -Werror=format-security -Werror=implicit-function-declaration \
         -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wwrite-strings -Wno-unused-parameter

%.o: %.c
	${CC} $(CFLAGS) -c -o $@ $<

all: test 

test: $(OBJS) test.o
	${CC} -o $@ $^

run: test
	./test

clean:
	rm -f *.o test
	rm -rf all_attributes_results
	rm -rf some_attributes_results

redblacktrees.o: redblacktrees.c redblacktrees.h
searchtrees.o: searchtrees.c searchtrees.h
test.o: redblacktrees.h

