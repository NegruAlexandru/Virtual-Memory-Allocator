//Negru Alexandru 314CAb 2023-2024

#include <stdio.h>
#include <errno.h>
#include "lists_structs.h"

#pragma once

#ifndef LISTS_FUNCTIONS_H
#define LISTS_FUNCTIONS_H

#define DIE(assertion, call_description)				\
        do {								\
                if (assertion) {					\
                        fprintf(stderr, "(%s, %d): ",			\
                                        __FILE__, __LINE__);		\
                        perror(call_description);			\
                        exit(errno);					\
                }							\
        } while (0)

dll_t *dll_create(int dataSize);
void dll_create_node(dll_t *list, int position, long address);
void dll_add_by_pos(dll_t *list, node_t *node, int position);
node_t *dll_remove_by_pos(dll_t *list, int position);
node_t *dll_remove_by_addr(dll_t *list, long address);
node_t *dll_get_node_by_addr(aol_t *arrayOfLists, long address);
void dll_sort_by_address(dll_t *list);
void aol_sort(aol_t *aol);
node_t *aol_remove_by_addr(aol_t *arrayOfLists, long address, int *node_size);
node_t *aol_remove_neighbour(node_t *node, aol_t *aol, int *size_of_block, int *size_of_neighbour);
dll_t *aol_get_list_by_size(aol_t *arrayOfLists, int size);
node_t *node_create_with_origin(int data_size, long address, long origin);
int node_get_size_of_block(aol_t *arrayOfLists, long address);
int node_get_size_of_partial_block(aol_t *arrayOfLists, long address);

#endif
