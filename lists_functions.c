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

void printListInt(doublyLinkedList_t *list) {
	node_t *current = list->head;
	while (current != NULL) {
		printf("%d ", *(int *)current->data);
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

doublyLinkedList_t *createDoublyLinkedList(int data_size) {
	doublyLinkedList_t *list = (doublyLinkedList_t *) malloc(sizeof(doublyLinkedList_t));

	list->size = 0;
	list->data_size = data_size;
	list->head = NULL;

	return list;
}

void addToNthPosition(doublyLinkedList_t *list, int position, void *data) {
	node_t *newNode = (node_t *) malloc(sizeof(node_t));
	DIE(newNode == NULL, "malloc");

	newNode->data = malloc(list->data_size);
	DIE(newNode->data == NULL, "malloc");

	memcpy(newNode->data, data, list->data_size);

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
		return toRemove;
	}

	list->size--;
}

void addNewEmptyNode(doublyLinkedList_t *list, int position, int address) {
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