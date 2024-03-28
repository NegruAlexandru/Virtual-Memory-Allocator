#include <stdio.h>
#include <stdlib.h>
#include "lists_structs.h"
#include "lists_functions.h"
#include <string.h>
#include <errno.h>
#include "datatype_functions.h"

dll_t *dll_create(int dataSize) {
	dll_t *list = (dll_t *) malloc(sizeof(dll_t));
	DIE(!list, "malloc");

	list->size = 0;
	list->data_size = dataSize;
	list->head = NULL;

	return list;
}

void dll_create_node(dll_t *list, int position, long address) {
	node_t *newNode = (node_t *) malloc(sizeof(node_t));
	DIE(!newNode, "malloc");

	newNode->data = malloc(list->data_size);
	DIE(!newNode->data, "malloc");

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

node_t *dll_remove_by_pos(dll_t *list, int position) {
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

node_t *dll_remove_by_addr(dll_t *list, long address) {
	node_t *current = list->head;
	int position = 0;

	while (current != NULL) {
		if (current->address == address) {
			return dll_remove_by_pos(list, position);
		}
		current = current->next;
		position++;
	}

	return NULL;
}

node_t *node_create_with_origin(int data_size, long address, long origin) {
	node_t *newNode = (node_t *) malloc(sizeof(node_t));
	DIE(!newNode, "malloc");

	newNode->data = malloc(data_size);
	DIE(!newNode->data, "malloc");

	newNode->address = address;
	newNode->origin = origin;

	newNode->next = NULL;
	newNode->prev = NULL;

	return newNode;
}

void dll_sort_by_address(dll_t *list) {
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

void aol_sort(aol_t *aol) {
	int swapped;
	dll_t **array = aol->lists;
	int size = aol->size;
	dll_t *temp;

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

dll_t *aol_get_list_by_size(aol_t *arrayOfLists, int size) {
	for (int i = 0; i < arrayOfLists->size; i++) {
		if (arrayOfLists->lists[i]->data_size == size) {
			return arrayOfLists->lists[i];
		}
	}

	return NULL;
}

void dll_add_by_pos(dll_t *list, node_t *node, int position) {
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

node_t *aol_remove_by_addr(aol_t *arrayOfLists, long address, int *node_size) {

	for (int i = 0; i < arrayOfLists->size; i++) {
		node_t *current = arrayOfLists->lists[i]->head;

		int position = 0;

		while (current != NULL) {
			if (current->address == address) {
				*node_size = arrayOfLists->lists[i]->data_size;
				return dll_remove_by_pos(arrayOfLists->lists[i], position);
			}

			current = current->next;
			position++;
		}
	}

	return NULL;
}

node_t *aol_remove_neighbour(node_t *node, aol_t *arrayOfLists, int *size_of_block, int *size_of_neighbour) {
	node_t *current = NULL;

	for (int i = 0; i < arrayOfLists->size; i++) {
		current = arrayOfLists->lists[i]->head;
		dll_sort_by_address(arrayOfLists->lists[i]);

		while (current) {
			if (current->origin != node->origin) {
				current = current->next;
				continue;
			}

			if (current->address + arrayOfLists->lists[i]->data_size == node->address) {
				*size_of_neighbour = arrayOfLists->lists[i]->data_size;
				return dll_remove_by_addr(arrayOfLists->lists[i], current->address);
			}

			if (current->address == node->address + *size_of_block) {
				*size_of_neighbour = arrayOfLists->lists[i]->data_size;
				return dll_remove_by_addr(arrayOfLists->lists[i], current->address);
			}

			current = current->next;
		}
	}

	return NULL;
}

int node_get_size_of_block(aol_t *arrayOfLists, long address) {
	for (int i = 0; i < arrayOfLists->size; i++) {
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

node_t *dll_get_node_by_addr(aol_t *arrayOfLists, long address) {
	for (int i = 0; i < arrayOfLists->size; i++) {
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

int node_get_size_of_partial_block(aol_t *arrayOfLists, long address) {
	for (int i = 0; i < arrayOfLists->size; i++) {
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
