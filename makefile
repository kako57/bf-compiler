all: com

CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic -g

%.o: %.c
	gcc -c -o $@ $<

com: main.o parser.o lexer.o token.o optimum_pride.o
	gcc -o $@ $^

clean:
	rm -f *.o com .depend

.depend: $(wildcard *.c)
	gcc -MM $^ > $@

include .depend

.PHONY: all clean
