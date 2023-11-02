CC   = cc
OBJS = redblacktrees.o

# -Wextra
CFLAGS = -O3 -g3 -Wall -Wextra -Werror=format-security -Werror=implicit-function-declaration \
         -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wwrite-strings \

%.o: %.c
	${CC} $(CFLAGS) -c -o $@ $<

all: test 

test: $(OBJS) test.o
	${CC} -o $@ $^

run: test
	./test

clean:
	rm -f *.o test

redblacktrees.o: redblacktrees.c redblacktrees.h
test.o: redblacktrees.h

