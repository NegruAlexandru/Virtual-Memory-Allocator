//Negru Alexandru 314CAb 2023-2024

#pragma once

#ifndef LISTS_STRUCTS_H
#define LISTS_STRUCTS_H

typedef struct node_t {
	void *data;
	struct node_t *next, *prev;
} node_t;

typedef struct info_t {
	void *data_info;
	long address;
	int origin;
} info_t;

typedef struct doubly_linked_list_t {
	int size, data_size;
	node_t *head;
} dll_t;

typedef struct array_of_lists_t {
	dll_t **lists;
	int size;
} aol_t;

#endif
