#include <stdio.h>
#include <stdlib.h>
#include "lists_structs.h"
#include "lists_functions.h"
#include <string.h>
#include <errno.h>
#include <datatype_functions.h>
#define DIE(assertion, call_description)				\
        do {								\
                if (assertion) {					\
                        fprintf(stderr, "(%s, %d): ",			\
                                        __FILE__, __LINE__);		\
                        perror(call_description);			\
                        exit(errno);					\
                }							\
        } while (0)

void printListLong(doublyLinkedList_t *list) {
	node_t *current = list->head;
	while (current != NULL) {
		printf("%ld ", *(long *)current->data);
		current = current->next;
	}
	printf("\n");
}

void printListChar(doublyLinkedList_t *list) {
	node_t *current = list->head;
	while (current != NULL) {
		printf("%s ", (char *)current->data);
		current = current->next;
	}
	printf("\n");
}

doublyLinkedList_t *createDoublyLinkedList(int dataSize) {
	doublyLinkedList_t *list = (doublyLinkedList_t *) malloc(sizeof(doublyLinkedList_t));

	list->size = 0;
	list->dataSize = dataSize;
	list->head = NULL;

	return list;
}

void addToNthPosition(doublyLinkedList_t *list, int position, void *data, long address) {
	node_t *newNode = (node_t *) malloc(sizeof(node_t));
	DIE(newNode == NULL, "malloc");

	newNode->data = malloc(list->dataSize);
	DIE(newNode->data == NULL, "malloc");

	memcpy(newNode->data, data, list->dataSize);

	newNode->address = address;

	newNode->next = NULL;
	newNode->prev = NULL;

	if (position > list->size) {
		position = list->size;
	}

	if (position == 0) {
		newNode->next = list->head;
		list->head = newNode;
	} else {
		node_t *current = list->head;

		for (int i = 0; i < position - 1; i++) {
			current = current->next;
		}

		newNode->next = current->next;
		newNode->prev = current;
		current->next = newNode;
		if (newNode->next != NULL) {
			newNode->next->prev = newNode;
		}
	}

	list->size++;
}

node_t *removeNthPosition(doublyLinkedList_t *list, int position) {
	node_t *current = list->head;

	if (position >= list->size) {
		position = list->size - 1;
	}

	if (position == 0) {
		list->head = list->head->next;
		list->head->prev = NULL;

		list->size--;

		return current;
	} else {
		for (int i = 0; i < position - 1; i++) {
			current = current->next;
		}

		node_t *toRemove = current->next;
		current->next = toRemove->next;

		if (toRemove->next != NULL) {
			toRemove->next->prev = current;
		}

		list->size--;

		return toRemove;
	}
}

node_t *removeNodeByAddress(doublyLinkedList_t *list, long address) {
	node_t *current = list->head;

	while (current != NULL) {
		if (current->address == address) {
			if (current == list->head) {
				list->head = list->head->next;
				list->head->prev = NULL;
			} else {
				current->prev->next = current->next;
				if (current->next != NULL) {
					current->next->prev = current->prev;
				}
			}
			list->size--;
			return current;
		}
		current = current->next;
	}

	return NULL;
}

void addNewEmptyNode(doublyLinkedList_t *list, int position, long address) {
	node_t *newNode = (node_t *) malloc(sizeof(node_t));
	DIE(newNode == NULL, "malloc");

	newNode->address = address;

	newNode->next = NULL;
	newNode->prev = NULL;

	if (position > list->size) {
		position = list->size;
	}

	if (position == 0) {
		newNode->next = list->head;
		list->head = newNode;
	} else {
		node_t *current = list->head;

		for (int i = 0; i < position - 1; i++) {
			current = current->next;
		}

		newNode->next = current->next;
		newNode->prev = current;
		current->next = newNode;
		if (newNode->next != NULL) {
			newNode->next->prev = newNode;
		}
	}

	list->size++;
}
