CC=gcc
CFLAGS=-O2 -std=c99 -Wall -Werror -pedantic
SOURCES=2048.c
OBJECTS=$(SOURCES:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

2048: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o 2048

.PHONY: clean
clean:
	rm 2048 $(OBJECTS)
