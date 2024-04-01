//Negru Alexandru 314CAb 2023-2024

#include "lists_structs.h"

#define MAX_INPUT_SIZE 600
#define INIT_NR_OF_LISTS_ALLOC 0

#pragma once

#ifndef COMMAND_FUNCTIONS_H
#define COMMAND_FUNCTIONS_H

void command_init(char *command, aol_t **aol_free_memory,
				  aol_t **aol_allocated_memory, int *free_mode);
void command_malloc(char *command, aol_t *aol_free_memory,
					aol_t *aol_allocated_memory, int *nr_mallocs,
					int *nr_fragmentations, long *fragment_group);
void command_free(char *command, aol_t *aol_free_memory,
				  aol_t *aol_allocated_memory, int *nr_frees, int free_mode);
void command_write(char *command, aol_t *aol_free_memory,
				   aol_t *aol_allocated_memory, int nr_mallocs,
				   int nr_frees, int nr_fragmentations);
void command_read(char *command, aol_t *aol_free_memory,
				  aol_t *aol_allocated_memory, int nr_mallocs,
				  int nr_frees, int nr_fragmentations);
void command_destroy(aol_t *aol_free_memory, aol_t *aol_allocated_memory);
void command_dump(aol_t *aol_free_memory, aol_t *aol_allocated_memory,
				  int nr_mallocs, int nr_frees, int nr_fragmentations);

#endif
