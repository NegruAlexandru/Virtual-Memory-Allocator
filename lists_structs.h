//Negru Alexandru 314CAb 2023-2024

#pragma once

#ifndef LISTS_STRUCTS_H
#define LISTS_STRUCTS_H

typedef struct node_t {
	void *data;
	struct node_t *next, *prev;
	long address, origin;
} node_t;

typedef struct doublyLinkedList_t {
	int size, data_size;
	node_t *head;
} dll_t;

typedef struct arrayOfLists_t {
	dll_t **lists;
	int size;
} aol_t;

#endif
