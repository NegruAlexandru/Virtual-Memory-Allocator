//Negru Alexandru 314CAb 2023-2024

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lists_structs.h"
#include "allocator_functions.h"

#define MAX_INPUT_SIZE 600
#define INITIAL_NUMBER_OF_LISTS_ALLOCATED 0

int main() {

	aol_t *aol_free_memory = NULL, *aol_allocated_memory = NULL;

	int nr_mallocs = 0, nr_frees = 0, nr_fragmentations = 0, memory_size = 0, nr_of_lists = 0, free_mode = 0;

	long fragment_group = 0, address = 0;

	while (1) {
		char choice[MAX_INPUT_SIZE];
		char command[MAX_INPUT_SIZE];
		scanf("%s", choice);
		fgets(command, MAX_INPUT_SIZE, stdin);

		if (strcmp(choice, "INIT_HEAP") == 0) {
			sscanf(command, "%lx %d %d %d", &address, &nr_of_lists, &memory_size, &free_mode);

			aol_free_memory = aol_create_increasing_size(
					address, nr_of_lists, memory_size);

			aol_allocated_memory = aol_create_increasing_size(
					address, INITIAL_NUMBER_OF_LISTS_ALLOCATED, memory_size);

		} else if (strcmp(choice, "MALLOC") == 0) {
			sscanf(command, "%d", &memory_size);

			int mallocReturn = malloc_function(aol_free_memory, aol_allocated_memory,
											   memory_size, &fragment_group);

			if (mallocReturn == 0) {
				nr_mallocs++;
			} else if (mallocReturn == 1) {
				nr_mallocs++;
				nr_fragmentations++;
			}

		} else if (strcmp(choice, "FREE") == 0) {
			sscanf(command, "%lx", &address);

			if (address != 0) {
				int freeReturn = free_function(aol_allocated_memory, aol_free_memory, address,
											   free_mode);

				if (freeReturn == 0) {
					nr_frees++;
				}
			}

		} else if (strcmp(choice, "WRITE") == 0) {
			char *data = (char *) malloc(MAX_INPUT_SIZE * sizeof(char));

			sscanf(command, "%lx \"%s\" %d", &address, data, &memory_size);
			data[strlen(data) - 1] = '\0';

			if (is_space_to_write(aol_allocated_memory, address, memory_size)) {
				write_to_allocated_memory(aol_allocated_memory, data, address, memory_size);
				free(data);
			} else {
				printf("Segmentation fault (core dumped)\n");
				print_memory_dump(aol_free_memory, aol_allocated_memory, nr_mallocs, nr_frees,
								  nr_fragmentations);\
                delete_aol(aol_free_memory);
				delete_aol(aol_allocated_memory);
				free(data);
				break;
			}

		} else if (strcmp(choice, "READ") == 0) {
			char *data = (char *) malloc(MAX_INPUT_SIZE * sizeof(char));

			sscanf(command, "%lx \"%s %d", &address, data, &memory_size);
			data[strlen(data) - 1] = '\0';

			if (is_requested_memory_allocated(aol_allocated_memory, address, memory_size)) {
				char *data = read_from_allocated_memory(aol_allocated_memory, address, memory_size);

				for (int i = 0; i < memory_size; i++) {
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
		}  else {
			printf("Invalid command\n");
		}
	}

	return 0;
}
