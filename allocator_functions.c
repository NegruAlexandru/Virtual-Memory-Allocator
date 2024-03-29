//Negru Alexandru 314CAb 2023-2024

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "allocator_functions.h"
#include "lists_structs.h"
#include "lists_functions.h"

aol_t *aol_create_increasing_size(long address, int number, int memory_size) {
	aol_t *arr = (aol_t *)malloc(sizeof(aol_t));
	DIE(!arr, "malloc");

	arr->lists = (dll_t **)malloc(number * sizeof(dll_t *));
	DIE(!arr->lists, "malloc");

	arr->size = number;

	int data_size = 8;

	for (int i = 0; i < number; i++) {
		arr->lists[i] = dll_create(data_size);

		for (int j = 0; j < memory_size / data_size; j++) {
			dll_create_node(arr->lists[i], j, address + j * data_size + i * memory_size);
		}

		data_size *= 2;
	}

	return arr;
}

void print_memory_dump(aol_t *aol_free_memory, aol_t *aol_allocated_memory, int nr_of_mallocs, int nr_of_frees, int nr_of_fragmentations) {
	int total_allocated_memory = 0;
	int total_free_memory = 0;

	aol_sort(aol_allocated_memory);
	aol_sort(aol_free_memory);

	int free_blocks = 0;
	for (int i = 0; i < aol_free_memory->size; i++) {
		total_free_memory += aol_free_memory->lists[i]->size * aol_free_memory->lists[i]->data_size;
		free_blocks += aol_free_memory->lists[i]->size;
	}

	int allocated_blocks = 0;
	for (int i = 0; i < aol_allocated_memory->size; i++) {
		total_allocated_memory += aol_allocated_memory->lists[i]->size * aol_allocated_memory->lists[i]->data_size;
		allocated_blocks += aol_allocated_memory->lists[i]->size;
	}

	printf("+++++DUMP+++++\n");
	printf("Total memory: %d bytes\n", total_free_memory + total_allocated_memory);
	printf("Total allocated memory: %d bytes\n", total_allocated_memory);
	printf("Total free memory: %d bytes\n", total_free_memory);
	printf("Free blocks: %d\n", free_blocks);
	printf("Number of allocated blocks: %d\n", allocated_blocks);
	printf("Number of malloc calls: %d\n", nr_of_mallocs);
	printf("Number of fragmentations: %d\n", nr_of_fragmentations);
	printf("Number of free calls: %d\n", nr_of_frees);
	for (int i = 0; i < aol_free_memory->size; i++) {
		if (aol_free_memory->lists[i]->size == 0) {
			continue;
		}
		int data_size = aol_free_memory->lists[i]->data_size;
		int size = aol_free_memory->lists[i]->size;

		printf("Blocks with %d bytes - %d free block(s) :", data_size, size);

		node_t *current = aol_free_memory->lists[i]->head;

		for (int j = 0; j < size; j++) {
			printf(" 0x%lx", current->address);
			current = current->next;
		}
		printf("\n");
	}
	printf("Allocated blocks :");

	long address = -1;

	for (int i = 0; i < allocated_blocks; i++) {
		long min_address = 0xfffffff;

		for (int j = 0; j < aol_allocated_memory->size; j++) {
			if (aol_allocated_memory->lists[j]->size == 0) {
				continue;
			}

			node_t *current = aol_allocated_memory->lists[j]->head;

			for (int k = 0; k < aol_allocated_memory->lists[j]->size; k++) {
				if (current->address < min_address && current->address > address) {
					min_address = current->address;
				}
				current = current->next;
			}
		}

		address = min_address;
		printf(" (0x%lx - %d)", address, node_get_size_of_block(aol_allocated_memory, address));
	}

	printf("\n");
	printf("-----DUMP-----\n");
}

node_t *node_get_ideal_block(aol_t *aol, int size_needed, int *size_of_block) {
	for (int i = 0; i < aol->size; i++) {
		if (aol->lists[i]->data_size == size_needed) {
			if (aol->lists[i]->size == 0) {
				continue;
			}
			*size_of_block = aol->lists[i]->data_size;
			dll_sort_by_address(aol->lists[i]);
			node_t *block = dll_remove_by_pos(aol->lists[i], 0);
			return block;
		}
	}
	return NULL;
}

node_t *node_get_available_block(aol_t *aol, int size_needed, int *size_block) {
	int position = -1;

	for (int i = 0; i < aol->size; i++) {
		if (aol->lists[i]->data_size > size_needed) {
			if (aol->lists[i]->size == 0) {
				continue;
			}
			dll_sort_by_address(aol->lists[i]);

			position = i;

			break;
		}
	}

	if (position == -1) {
		return NULL;
	}

	*size_block = aol->lists[position]->data_size;
	dll_sort_by_address(aol->lists[position]);
	node_t *block = dll_remove_by_pos(aol->lists[position], 0);
	return block;
}

void move_block_to_aol(aol_t *aol, node_t *block, int size) {
	for (int i = 0; i < aol->size; i++) {
		if (aol->lists[i]->data_size == size) {
			dll_add_by_pos(aol->lists[i], block, aol->lists[i]->size);
			return;
		}
	}

	dll_t **tmp = (dll_t **)realloc(aol->lists, (aol->size + 1) * sizeof(dll_t *));

	DIE(!tmp, "realloc");

	aol->lists = tmp;

	aol->lists[aol->size] = dll_create(size);

	dll_add_by_pos(aol->lists[aol->size], block, 0);

	aol->size++;
}

int malloc_function(aol_t *aol_free_memory, aol_t *aol_allocated_memory, int size_needed, long *fragment_group) {
	//modified by getBlock functions
	int size_of_block = 0;

	//searching for free block
	node_t *block = node_get_ideal_block(aol_free_memory, size_needed, &size_of_block);

	if (!block) {
		block = node_get_available_block(aol_free_memory, size_needed, &size_of_block);

		if (!block) {
			printf("Out of memory\n");
			return -1;
		}
	}

	//if block is exactly the size needed
	if (size_of_block == size_needed) {
		// block being moved to allocated memory
		move_block_to_aol(aol_allocated_memory, block, size_of_block);

		dll_sort_by_address(aol_get_list_by_size(aol_allocated_memory, size_of_block));
		aol_sort(aol_allocated_memory);

		return 0;
	}

	//if block is bigger than the size needed
	if (size_of_block > size_needed) {
		node_t *requested_block;
		node_t *remaining_block;

		if (block->origin >= 0) {
			requested_block = node_create_with_origin(size_needed, block->address, block->origin);
			remaining_block = node_create_with_origin(size_of_block - size_needed, block->address + size_needed,
													  block->origin);
		} else {
			requested_block = node_create_with_origin(size_needed, block->address, *fragment_group);
			remaining_block = node_create_with_origin(size_of_block - size_needed, block->address + size_needed,
													  *fragment_group);
			(*fragment_group) = (*fragment_group) + 1;
		}

		move_block_to_aol(aol_allocated_memory, requested_block, size_needed);
		dll_sort_by_address(aol_get_list_by_size(aol_allocated_memory, size_needed));
		aol_sort(aol_allocated_memory);

		move_block_to_aol(aol_free_memory, remaining_block, size_of_block - size_needed);
		dll_sort_by_address(aol_get_list_by_size(aol_free_memory, size_of_block - size_needed));
		aol_sort(aol_free_memory);

		free(block->data);
		free(block);

		return 1;
	}

	return -1;
}

int free_function(aol_t *aol_allocated_memory, aol_t *aol_free_memory, long address, int free_mode) {
	int block_size = 0;

	node_t *block = aol_remove_by_addr(aol_allocated_memory, address, &block_size);

	if (!block) {
		printf("Invalid free\n");
		return -1;
	}

	if (free_mode == 0) {
		move_block_to_aol(aol_free_memory, block, block_size);
		dll_sort_by_address(aol_get_list_by_size(aol_free_memory, block_size));

		return 0;

	} else if (free_mode == 1) {
		if (block->origin >= 0) {

			int neighbour_size = 0;

			node_t *neighbour = aol_remove_neighbour(block, aol_free_memory, &block_size, &neighbour_size);

			while (neighbour) {
				block_size += neighbour_size;

				if (block->address < neighbour->address) {
					void *tmp = realloc(block->data, block_size);

					DIE(!tmp, "realloc");

					block->data = tmp;

					free(neighbour->data);
					free(neighbour);
				} else {
					void *tmp = realloc(block->data, block_size);

					DIE(!tmp, "realloc");

					block->data = tmp;
					block->address = neighbour->address;

					free(neighbour->data);
					free(neighbour);
				}

				neighbour = aol_remove_neighbour(block, aol_free_memory, &block_size, &neighbour_size);
			}

			move_block_to_aol(aol_free_memory, block, block_size);
			dll_sort_by_address(aol_get_list_by_size(aol_free_memory, block_size));

			return 0;
		}

		move_block_to_aol(aol_free_memory, block, block_size);
		dll_sort_by_address(aol_get_list_by_size(aol_free_memory, block_size));

		return 0;
	}

	return -1;
}

void delete_aol(aol_t *aol) {
	for (int i = 0; i < aol->size; i++) {
		node_t *current = aol->lists[i]->head;

		while (current != NULL) {
			free(current->data);
			node_t *tmp = current;
			current = current->next;
			free(tmp);
		}

		free(aol->lists[i]);
	}

	free(aol->lists);
	free(aol);
}

int is_space_to_write(aol_t *aol, long address, int size_to_write) {
	while (size_to_write > 0) {
		int size = node_get_size_of_partial_block(aol, address);

		if (!size) {
			return 0;
		}

		if (size >= size_to_write) {
			return 1;
		}

		address += size;
		size_to_write -= size;
	}

	return 1;
}

void write_to_allocated_memory(aol_t *aol, char *data, long address, int size_to_write) {
	while (size_to_write > 0) {
		int size = node_get_size_of_partial_block(aol, address);

		node_t *block = dll_get_node_by_addr(aol, address);

		if (size >= size_to_write) {
			memcpy(block->data, data, size_to_write);
			return;
		}

		memcpy(block->data, data, size);

		data += size;
		address += size;
		size_to_write -= size;
	}
}

int is_requested_memory_allocated(aol_t *aol, long address, int size_to_read) {
	while (size_to_read > 0) {
		int size = node_get_size_of_partial_block(aol, address);
		if (!size) {
			return 0;
		}

		if (size >= size_to_read) {
			return 1;
		}

		address += size;
		size_to_read -= size;
	}

	return 1;
}

char *read_from_allocated_memory(aol_t *aol, long address, int size_to_read) {
	char *data = (char *)malloc(size_to_read * sizeof(char));
	DIE(!data, "malloc");

	char *data_start = data;

	while (size_to_read > 0) {
		int size = node_get_size_of_partial_block(aol, address);

		node_t *block = dll_get_node_by_addr(aol, address);

		if (size >= size_to_read) {
			memcpy(data, block->data, size_to_read);
			return data_start;
		}

		memcpy(data, block->data, size);
		data += size;
		address += size;
		size_to_read -= size;
	}

	return data_start;
}