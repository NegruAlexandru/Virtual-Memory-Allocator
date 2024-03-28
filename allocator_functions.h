#include "lists_structs.h"

#pragma once

#ifndef ALLOCATOR_FUNCTIONS_H
#define ALLOCATOR_FUNCTIONS_H

aol_t *aol_create_increasing_size(long address, int number, int memory_size);
void print_memory_dump(aol_t *aol_free_memory, aol_t *aol_allocated_memory, int nr_of_mallocs, int nr_of_frees, int nr_of_fragmentations);
node_t *node_get_ideal_block(aol_t *aol, int size_needed, int *size_of_block);
node_t *node_get_available_block(aol_t *aol, int size_needed, int *size_block);
void move_block_to_aol(aol_t *aol, node_t *block, int size);
int malloc_function(aol_t *aol_free_memory, aol_t *aol_allocated_memory, long *fragment_group);
int free_function(aol_t *aol_allocated_memory, aol_t *aol_free_memory, long address, int free_mode);
void delete_aol(aol_t *aol);
int is_space_to_write(aol_t *aol, long address, int size_to_write);
void write_to_allocated_memory(aol_t *aol, char *data, long address, int size_to_write);
int is_requested_memory_allocated(aol_t *aol, long address, int size_to_read);
char *read_from_allocated_memory(aol_t *aol, long address, int size_to_read);

#endif
