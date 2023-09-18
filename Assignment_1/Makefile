CC   = cc
OBJS = util.o

CFLAGS = -O3 -g3 -Wall -Wextra -Werror=format-security -Werror=implicit-function-declaration \
         -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wwrite-strings \

%.o: %.c
	${CC} $(CFLAGS) -c -o $@ $<

merge_sort: $(OBJS) merge_sort.o
	${CC} -o $@ $^
	./merge_sort

k_minima: $(OBJS) k_minima.o
	${CC} -o $@ $^
	./k_minima

functional: $(OBJS) functional.o
	${CC} -o $@ $^
	./functional

multiply: $(OBJS) multiply.o
	${CC} -o $@ $^
	./multiply

clean:
	rm -f *.o merge_sort k_minima functional multiply

util.o: util.c util.h
functional.o: functional.c util.h
k_minima.o: k_minima.c util.h
merge_sort.o: merge_sort.c util.h
multiply.o: multiply.c util.h

