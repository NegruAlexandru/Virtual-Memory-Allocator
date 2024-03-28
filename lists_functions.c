#include <stdio.h>
#include <stdlib.h>
#include "lists_structs.h"
#include "lists_functions.h"
#include <string.h>
#include <errno.h>
#include "datatype_functions.h"
#define DIE(assertion, call_description)				\
        do {								\
                if (assertion) {					\
                        fprintf(stderr, "(%s, %d): ",			\
                                        __FILE__, __LINE__);		\
                        perror(call_description);			\
                        exit(errno);					\
                }							\
        } while (0)

doublyLinkedList_t *createDoublyLinkedList(int dataSize) {
	doublyLinkedList_t *list = (doublyLinkedList_t *) malloc(sizeof(doublyLinkedList_t));

	list->size = 0;
	list->data_size = dataSize;
	list->head = NULL;

	return list;
}

void createNodeNthPosition(doublyLinkedList_t *list, int position, long address) {
	node_t *newNode = (node_t *) malloc(sizeof(node_t));
	DIE(newNode == NULL, "malloc");

	newNode->data = malloc(list->data_size);
	DIE(newNode->data == NULL, "malloc");

	newNode->address = address;
	newNode->origin = -1;

	newNode->next = NULL;
	newNode->prev = NULL;

	if (position > list->size) {
		position = list->size;
	}

	if (position == 0) {
		newNode->next = list->head;
		if (list->head != NULL)
			list->head->prev = newNode;

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

node_t *remove_nth_position(doublyLinkedList_t *list, int position) {
	node_t *current = list->head;

	if (position >= list->size) {
		position = list->size - 1;
	}

	if (position == 0) {
		list->head = list->head->next;
		if (list->head != NULL)
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
	int position = 0;

	while (current != NULL) {
		if (current->address == address) {
			return remove_nth_position(list, position);
		}
		current = current->next;
		position++;
	}

	return NULL;
}

void addNewEmptyNode(doublyLinkedList_t *list, int position, long address) {
	node_t *newNode = (node_t *) malloc(sizeof(node_t));
	DIE(newNode == NULL, "malloc");

	newNode->address = address;
	newNode->origin = -1;

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

node_t *createEmptyNodeWithOrigin(int data_size, long address, long origin) {
	node_t *newNode = (node_t *) malloc(sizeof(node_t));
	DIE(newNode == NULL, "malloc");

	newNode->data = malloc(data_size);
	DIE(newNode->data == NULL, "malloc");

	newNode->address = address;
	newNode->origin = origin;

	newNode->next = NULL;
	newNode->prev = NULL;

	return newNode;
}

void sort_by_address(doublyLinkedList_t *list) {
	if (list == NULL || list->head == NULL) {
		return;
	}

	int swapped;
	node_t *current;
	node_t *last = NULL;

	do {
		swapped = 0;
		current = list->head;

		while (current->next != last) {
			if (current->address > current->next->address) {
				// Swap nodes
				long tempAddress = current->address;
				void *tempData = current->data;

				current->address = current->next->address;
				current->data = current->next->data;

				current->next->address = tempAddress;
				current->next->data = tempData;

				swapped = 1;
			}
			current = current->next;
		}
		last = current;
	}
	while (swapped);
}

void bubbleSortArrayOfListsBySize(array_of_lists_t *array_of_lists) {
	int swapped;
	doublyLinkedList_t **array = array_of_lists->lists;
	int size = array_of_lists->number;
	doublyLinkedList_t *temp;

	do {
		swapped = 0;

		for (int i = 0; i < size - 1; i++) {
			if (array[i]->data_size > array[i + 1]->data_size) {
				temp = array[i];
				array[i] = array[i + 1];
				array[i + 1] = temp;
				swapped = 1;
			}
		}
	}
	while (swapped);
}

doublyLinkedList_t *getListWithSize(array_of_lists_t *arrayOfLists, int size) {
	for (int i = 0; i < arrayOfLists->number; i++) {
		if (arrayOfLists->lists[i]->data_size == size) {
			return arrayOfLists->lists[i];
		}
	}

	return NULL;
}

void addNodeToNthPosition(doublyLinkedList_t *list, node_t *node, int position) {
	if (position > list->size) {
		position = list->size;
	}

	if (position == 0) {
		if (list->head != NULL) {
			list->head->prev = node;
		}

		node->next = list->head;
		list->head = node;
		node->prev = NULL;
	} else {
		node_t *current = list->head;

		for (int i = 0; i < position - 1; i++) {
			current = current->next;
		}

		node->next = current->next;
		node->prev = current;
		current->next = node;
		if (node->next != NULL) {
			node->next->prev = node;
		}
	}

	list->size++;

}

node_t *removeNAddress(array_of_lists_t *arrayOfLists, long address, int *nodeSize) {

	for (int i = 0; i < arrayOfLists->number; i++) {
		node_t *current = arrayOfLists->lists[i]->head;

		int position = 0;

		while (current != NULL) {
			if (current->address == address) {
				*nodeSize = arrayOfLists->lists[i]->data_size;
				return remove_nth_position(arrayOfLists->lists[i], position);
			}

			current = current->next;
			position++;
		}
	}

	return NULL;
}

node_t *removeNeighbourNode(node_t *node, array_of_lists_t *arrayOfLists, int *sizeOfBlock, int *sizeOfNeighbour) {
	node_t *current = NULL;

	for (int i = 0; i < arrayOfLists->number; i++) {
		current = arrayOfLists->lists[i]->head;
		sort_by_address(arrayOfLists->lists[i]);

		while (current) {
			if (current->origin != node->origin) {
				current = current->next;
				continue;
			}

			if (current->address + arrayOfLists->lists[i]->data_size == node->address) {
				*sizeOfNeighbour = arrayOfLists->lists[i]->data_size;
				return removeNodeByAddress(arrayOfLists->lists[i], current->address);
			}

			if (current->address == node->address + *sizeOfBlock) {
				*sizeOfNeighbour = arrayOfLists->lists[i]->data_size;
				return removeNodeByAddress(arrayOfLists->lists[i], current->address);
			}

			current = current->next;
		}
	}

	return NULL;
}

//int isNodeComplete(node_t *node, array_of_lists_t *arrayOfLists) {
//	node_t *current;
//
//	for (int i = 0; i < arrayOfLists->number; i++) {
//		current = arrayOfLists->lists[i]->head;
//
//		while (current != NULL) {
//			if (current->address + arrayOfLists->lists[i]->data_size == node->address && ) {
//				return 1;
//			}
//
//			current = current->next;
//		}
//	}
//
//	return -1;
//}

int getSizeOfBlockByAddress(array_of_lists_t *arrayOfLists, long address) {
	for (int i = 0; i < arrayOfLists->number; i++) {
		node_t *current = arrayOfLists->lists[i]->head;

		while (current != NULL) {
			if (current->address == address) {
				return arrayOfLists->lists[i]->data_size;
			}

			current = current->next;
		}
	}

	return 0;
}

node_t *getNodeByAddress(array_of_lists_t *arrayOfLists, long address) {
	for (int i = 0; i < arrayOfLists->number; i++) {
		node_t *current = arrayOfLists->lists[i]->head;

		while (current != NULL) {
			if (current->address == address) {
				return current;
			}

			current = current->next;
		}
	}

	return NULL;
}

int getSizeOfPartialBlockByAddress(array_of_lists_t *arrayOfLists, long address) {
	for (int i = 0; i < arrayOfLists->number; i++) {
		node_t *current = arrayOfLists->lists[i]->head;

		while (current != NULL) {
			if (current->address <= address && address < current->address + arrayOfLists->lists[i]->data_size) {
				return arrayOfLists->lists[i]->data_size - (address - current->address);
			}

			current = current->next;
		}
	}

	return 0;
}

char *getPartialBlockDataByAddress(array_of_lists_t *arrayOfLists, long address) {
	for (int i = 0; i < arrayOfLists->number; i++) {
		node_t *current = arrayOfLists->lists[i]->head;

		while (current != NULL) {
			if (current->address <= address && address < current->address + arrayOfLists->lists[i]->data_size) {
				return (char *)current->data + (address - current->address);
			}

			current = current->next;
		}
	}

	return NULL;
}