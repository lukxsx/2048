CC=gcc

SOURCES=2048.c
OBJECTS=$(SOURCES:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

2048: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o 2048

.PHONY: clean
clean:
	rm 2048 $(OBJECTS)
