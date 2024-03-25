#pragma once
#ifndef LISTS_STRUCTS_H
#define LISTS_STRUCTS_H

typedef struct node_t {
	void *data;
	struct node_t *next, *prev;
	long address, origin;
} node_t;

typedef struct doublyLinkedList_t {
	int size, dataSize;
	node_t *head;
} doublyLinkedList_t;

typedef struct arrayOfLists_t {
	doublyLinkedList_t **lists;
	int number, memorySize;
} arrayOfLists_t;

#endif
