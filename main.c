#include <stdio.h>
#include <stdlib.h>
#include "lists_structs.h"
#include "lists_functions.h"
#include "allocator_functions.h"
#include "datatype_functions.h"
#include <string.h>


#define MAX_INPUT_SIZE 600
#define MAX_ADDRESS_SIZE 10
#define INITIAL_NUMBER_OF_LISTS_ALLOCATED 0
int main() {

	aol_t *aol_free_memory = NULL, *aol_allocated_memory = NULL;

	int nr_mallocs = 0, nr_frees = 0, nr_fragmentations = 0;

	long fragment_group = 0;

	int free_mode = 0;

	while (1) {
		char choice[MAX_INPUT_SIZE];
		scanf("%s", choice);

		if (strcmp(choice, "INIT_HEAP") == 0) {
			char *address = (char *) malloc(MAX_ADDRESS_SIZE * sizeof(char));
			DIE(!address, "malloc");
			int nr_of_lists = 0, memory_size = 0;

			scanf("%s %d %d %d", address, &nr_of_lists, &memory_size, &free_mode);

			long addressLong = base16to10(remove_hexa_prefix(address));

			aol_free_memory = aol_create_increasing_size(
					addressLong, nr_of_lists, memory_size);

			aol_allocated_memory = aol_create_increasing_size(
					addressLong, INITIAL_NUMBER_OF_LISTS_ALLOCATED, memory_size);

		} else if (strcmp(choice, "MALLOC") == 0) {
			int mallocReturn = malloc_function(aol_free_memory, aol_allocated_memory,
											   &fragment_group);

			if (mallocReturn == 0) {
				nr_mallocs++;
			} else if (mallocReturn == 1) {
				nr_mallocs++;
				nr_fragmentations++;
			}

		} else if (strcmp(choice, "FREE") == 0) {
			char *address = (char *) malloc(MAX_ADDRESS_SIZE * sizeof(char));
			DIE(!address, "malloc");
			scanf("%s", address);

			long addressLong = base16to10(remove_hexa_prefix(address));

			if (addressLong != 0) {
				int freeReturn = free_function(aol_allocated_memory, aol_free_memory, addressLong,
											   free_mode);

				if (freeReturn == 0) {
					nr_frees++;
				}
			}
		} else if (strcmp(choice, "WRITE") == 0) {
			char *address = (char *) malloc(MAX_ADDRESS_SIZE * sizeof(char));
			DIE(!address, "malloc");
			char *data = (char *) malloc(MAX_INPUT_SIZE * sizeof(char));
			DIE(!data, "malloc");
			int sizeToWrite = 0;

			scanf("%s", address);
			fgets(data, MAX_INPUT_SIZE, stdin);

			char *token = strtok(data, "\"");
			token = strtok(NULL, "\"");

			sizeToWrite = atoi(strtok(NULL, " "));

			if (sizeToWrite > (int)strlen(token)) {
				sizeToWrite = strlen(token);
			}

			long addressLong = base16to10(remove_hexa_prefix(address));

			if (is_space_to_write(aol_allocated_memory, addressLong, sizeToWrite)) {
				write_to_allocated_memory(aol_allocated_memory, token, addressLong, sizeToWrite);
			} else {
				printf("Segmentation fault (core dumped)\n");
				print_memory_dump(aol_free_memory, aol_allocated_memory, nr_mallocs, nr_frees,
								  nr_fragmentations);\
                delete_aol(aol_free_memory);
				delete_aol(aol_allocated_memory);
				free(data);
				break;
			}

			free(data);

		} else if (strcmp(choice, "READ") == 0) {
			char *address = (char *) malloc(MAX_ADDRESS_SIZE * sizeof(char));
			DIE(!address, "malloc");
			int sizeToRead = 0;

			scanf("%s %d", address, &sizeToRead);

			long addressLong = base16to10(remove_hexa_prefix(address));

			if (is_requested_memory_allocated(aol_allocated_memory, addressLong, sizeToRead)) {
				char *data = read_from_allocated_memory(aol_allocated_memory, addressLong, sizeToRead);

				for (int i = 0; i < sizeToRead; i++) {
					printf("%c", data[i]);
				}
				printf("\n");

				free(data);
			} else {
				printf("Segmentation fault (core dumped)\n");
				print_memory_dump(aol_free_memory, aol_allocated_memory, nr_mallocs, nr_frees,
								  nr_fragmentations);
				delete_aol(aol_free_memory);
				delete_aol(aol_allocated_memory);
				break;
			}

		} else if (strcmp(choice, "DESTROY_HEAP") == 0) {
			delete_aol(aol_free_memory);
			delete_aol(aol_allocated_memory);
			break;

		} else if (strcmp(choice, "DUMP_MEMORY") == 0) {
			print_memory_dump(aol_free_memory, aol_allocated_memory, nr_mallocs, nr_frees,
							  nr_fragmentations);
		} else if (strcmp(choice, "info") == 0) {
			char *address = (char *) malloc(MAX_ADDRESS_SIZE * sizeof(char));
			DIE(!address, "malloc");
			scanf("%s", address);

			long addressLong = base16to10(remove_hexa_prefix(address));

			node_t *block = dll_get_node_by_addr(aol_free_memory, addressLong);

			printf("Block address: %lx, origin: %ld\n", block->address, block->origin);
		} else if (strcmp(choice, "origin") == 0) {
			int i = 0;
			scanf("%d", &i);

			for (int j = 0; j < aol_free_memory->size; j++) {
				dll_t *list = aol_free_memory->lists[j];
				node_t *current = list->head;

				while (current != NULL) {
					if (current->origin == i) {
						printf("Block address: %lx, origin: %ld\n", current->address, current->origin);
					}

					current = current->next;
				}
			}

		} else {
			printf("Invalid command\n");
		}
	}

	return 0;
}
