#pragma once
#ifndef LISTS_STRUCTS_H
#define LISTS_STRUCTS_H
typedef struct node_t {
	void *data;
	struct node_t *next, *prev;
	int address;
} node_t;

typedef struct doublyLinkedList_t {
	int size, data_size;
	node_t *head;
} doublyLinkedList_t;
#endif
