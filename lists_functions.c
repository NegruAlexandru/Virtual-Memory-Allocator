//Negru Alexandru 314CAb 2023-2024

#include <stdio.h>
#include <stdlib.h>
#include "lists_structs.h"
#include "lists_functions.h"

// create a new doubly linked list
dll_t *dll_create(int data_size)
{
	dll_t *list = malloc(sizeof(dll_t));
	DIE(!list, "malloc");

	list->size = 0;
	list->data_size = data_size;
	list->head = NULL;

	return list;
}

// create a new node and add it to the list at the specified position
void dll_create_node(dll_t *list, int position, long address)
{
	node_t *new_node = malloc(sizeof(node_t));
	DIE(!new_node, "malloc");

	new_node->data = malloc(list->data_size);
	DIE(!new_node->data, "malloc");

	new_node->address = address;
	new_node->origin = -1;

	new_node->next = NULL;
	new_node->prev = NULL;

	if (position > list->size)
		position = list->size;

	if (position == 0) {
		new_node->next = list->head;
		if (list->head)
			list->head->prev = new_node;

		list->head = new_node;
	} else {
		node_t *current = list->head;

		for (int i = 0; i < position - 1; i++)
			current = current->next;

		new_node->next = current->next;
		new_node->prev = current;
		current->next = new_node;
		if (new_node->next)
			new_node->next->prev = new_node;
	}

	list->size++;
}

// remove a node from the list by position
node_t *dll_remove_by_pos(dll_t *list, int position)
{
	node_t *current = list->head;

	if (position >= list->size)
		position = list->size - 1;

	if (position == 0) {
		list->head = list->head->next;

		if (list->head)
			list->head->prev = NULL;

		list->size--;
	} else {
		for (int i = 0; i < position - 1; i++)
			current = current->next;

		node_t *to_remove = current->next;
		current->next = to_remove->next;

		if (to_remove->next)
			to_remove->next->prev = current;

		list->size--;

		current = to_remove;
	}
	return current;
}

// remove a node from the list by address
node_t *dll_remove_by_addr(dll_t *list, long address)
{
	node_t *current = list->head;
	int position = 0;

	while (current) {
		if (current->address == address)
			return dll_remove_by_pos(list, position);
		current = current->next;
		position++;
	}

	return NULL;
}

// create a new node with the specified address and origin
node_t *node_create_with_origin(int data_size, long address, long origin)
{
	node_t *new_node = malloc(sizeof(node_t));
	DIE(!new_node, "malloc");

	new_node->data = malloc(data_size);
	DIE(!new_node->data, "malloc");

	new_node->address = address;
	new_node->origin = origin;

	new_node->next = NULL;
	new_node->prev = NULL;

	return new_node;
}

// sort the list by address
void dll_sort_by_address(dll_t *list)
{
	if (!list || !list->head)
		return;

	int swapped;
	node_t *current;
	node_t *last = NULL;

	do {
		swapped = 0;
		current = list->head;

		while (current->next != last) {
			if (current->address > current->next->address) {
				// Swap nodes
				long temp_address = current->address;
				void *temp_data = current->data;
				long temp_origin = current->origin;

				current->address = current->next->address;
				current->data = current->next->data;
				current->origin = current->next->origin;

				current->next->address = temp_address;
				current->next->data = temp_data;
				current->next->origin = temp_origin;

				swapped = 1;
			}
			current = current->next;
		}
		last = current;
	} while (swapped);
}

// sort the array of lists by the size of the data
void aol_sort(aol_t *aol)
{
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
	} while (swapped);
}

// get the list from the array of lists with the specified size
dll_t *aol_get_list_by_size(aol_t *aol, int size)
{
	for (int i = 0; i < aol->size; i++) {
		if (aol->lists[i]->data_size == size)
			return aol->lists[i];
	}

	return NULL;
}

// add a node to the list at the specified position
void dll_add_by_pos(dll_t *list, node_t *node, int position)
{
	if (position > list->size)
		position = list->size;

	if (position == 0) {
		if (list->head)
			list->head->prev = node;

		node->next = list->head;
		list->head = node;
		node->prev = NULL;
	} else {
		node_t *current = list->head;

		for (int i = 0; i < position - 1; i++)
			current = current->next;

		node->next = current->next;
		node->prev = current;
		current->next = node;
		if (node->next)
			node->next->prev = node;
	}

	list->size++;
}

// remove a node from the array of lists by address
node_t *aol_remove_by_addr(aol_t *aol, long address,
						   int *node_size)
{
	for (int i = 0; i < aol->size; i++) {
		node_t *current = aol->lists[i]->head;

		int position = 0;

		while (current) {
			if (current->address == address) {
				*node_size = aol->lists[i]->data_size;
				return dll_remove_by_pos(aol->lists[i], position);
			}

			current = current->next;
			position++;
		}
	}

	return NULL;
}

// remove a neighbour of a node from the array of lists
node_t *aol_remove_neighbour(node_t *node, aol_t *aol, int *size_of_block,
							 int *size_of_neighbour)
{
	node_t *current = NULL;

	for (int i = 0; i < aol->size; i++) {
		current = aol->lists[i]->head;
		if (aol->lists[i]->size == 0)
			continue;
		dll_sort_by_address(aol->lists[i]);

		while (current) {
			if (current->origin != node->origin) {
				current = current->next;
				continue;
			}

			// return the neighbour before the node
			if (current->address + aol->lists[i]->data_size == node->address) {
				*size_of_neighbour = aol->lists[i]->data_size;
				return dll_remove_by_addr(aol->lists[i], current->address);
			}

			// return the neighbour after the node
			if (current->address == node->address + *size_of_block) {
				*size_of_neighbour = aol->lists[i]->data_size;
				return dll_remove_by_addr(aol->lists[i], current->address);
			}

			current = current->next;
		}
	}

	return NULL;
}

// get the size of the block with the specified address
int node_get_size_of_block(aol_t *aol, long address)
{
	for (int i = 0; i < aol->size; i++) {
		node_t *current = aol->lists[i]->head;

		while (current) {
			if (current->address == address)
				return aol->lists[i]->data_size;

			current = current->next;
		}
	}

	return 0;
}

// get the node with the specified address
node_t *dll_get_node_by_addr(aol_t *aol, long address)
{
	for (int i = 0; i < aol->size; i++) {
		node_t *current = aol->lists[i]->head;

		while (current) {
			if (current->address == address)
				return current;

			current = current->next;
		}
	}

	return NULL;
}

// get the size of the partial block with the specified address*-*
int node_get_size_of_partial_block(aol_t *aol, long address)
{
	for (int i = 0; i < aol->size; i++) {
		node_t *current = aol->lists[i]->head;

		while (current) {
			if (current->address <= address &&
				address < current->address +
						  aol->lists[i]->data_size) {
				return aol->lists[i]->data_size -
					   (address - current->address);
			}

			current = current->next;
		}
	}

	return 0;
}
