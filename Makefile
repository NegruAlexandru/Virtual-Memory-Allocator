# Negru Alexandru-Nicolae 314CAb 2023-2024

CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -g -ggdb3
SOURCES=allocator_functions.c command_functions.c lists_functions.c
LIBS=lists_structs.h allocator_functions.h command_functions.h lists_functions.h

TARGETS = run_sfl

build: $(TARGETS)

run_sfl: main.c
	$(CC) $(CFLAGS) main.c -o sfl $(SOURCES)

pack:
	zip -FSr 314CA_NegruAlexandruNicolae_Tema1.zip README.md Makefile *.c *.h

clean:
	rm -f $(TARGETS)

.PHONY: pack clean
