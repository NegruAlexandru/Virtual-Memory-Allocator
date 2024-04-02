//Negru Alexandru 314CAb 2023-2024

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command_functions.h"
#include "lists_structs.h"
#include "allocator_functions.h"
#include "lists_functions.h"

// function that initializes the heap
void command_init(char *command, aol_t **aol_free_memory,
				  aol_t **aol_allocated_memory, int *free_mode)
{
	long address = 0;
	int nr_of_lists = 0, memory_size = 0, count;

	// parse the command
	count = sscanf(command, "%lx %d %d %d", &address, &nr_of_lists,
				   &memory_size, free_mode);
	DIE(count != 4, "Invalid command");

	// create the free memory data_info structure
	*aol_free_memory = aol_create_increasing_size(address,
												  nr_of_lists, memory_size);

	// create the allocated memory data_info structure, initialized with 0 lists
	*aol_allocated_memory = aol_create_increasing_size(address,
													   INIT_NR_OF_LISTS_ALLOC,
													   memory_size);
}

// function that calls the malloc function
void command_malloc(char *command, aol_t *aol_free_memory,
					aol_t *aol_allocated_memory, int *nr_mallocs,
					int *nr_fragmentations, long *fragment_group)
{
	int memory_size = 0, count;

	// parse the command
	count = sscanf(command, "%d", &memory_size);
	DIE(count != 1, "Invalid command");

	// call the malloc function
	int malloc_return = malloc_function(aol_free_memory, aol_allocated_memory,
									   memory_size, fragment_group);

	// check the return value for stats
	if (malloc_return == 0) {
		(*nr_mallocs)++;
	} else if (malloc_return == 1) {
		(*nr_mallocs)++;
		(*nr_fragmentations)++;
	}
}

// function that calls the free function
void command_free(char *command, aol_t *aol_free_memory,
				  aol_t *aol_allocated_memory, int *nr_frees, int free_mode)
{
	long address = 0, count;

	// parse the command
	count = sscanf(command, "%lx", &address);
	DIE(count != 1, "Invalid command");

	// check if the address is valid
	if (address != 0) {
		// call the free function
		int free_return = free_function(aol_allocated_memory, aol_free_memory,
									   address, free_mode);

		// check the return value for stats
		if (free_return == 0)
			(*nr_frees)++;
	}
}

// function that writes to the allocated memory
void command_write(char *command, aol_t *aol_free_memory,
				   aol_t *aol_allocated_memory, int nr_mallocs,
				   int nr_frees, int nr_fragmentations)
{
	long address = 0;
	int memory_size, count;

	// parse the command
	count = sscanf(command, "%lx", &address);
	DIE(count != 1, "Invalid command");

	char *token = strtok(command, "\"");
	token = strtok(NULL, "\"");
	memory_size = atoi(strtok(NULL, " "));

	// check the size of the token and the memory size for minimum
	if (strlen(token) < memory_size)
		memory_size = (int)strlen(token);

	// check if the memory is allocated
	if (is_space_to_write(aol_allocated_memory, address,
						  memory_size)) {
		// write the data_info
		write_to_allocated_memory(aol_allocated_memory,
								  token, address, memory_size);
	} else {
		// print the error message, the memory dump and exit
		printf("Segmentation fault (core dumped)\n");
		print_memory_dump(aol_free_memory, aol_allocated_memory,
						  nr_mallocs, nr_frees, nr_fragmentations);
		delete_aol(aol_free_memory);
		delete_aol(aol_allocated_memory);
		exit(0);
	}
}

// function that reads from the allocated memory
void command_read(char *command, aol_t *aol_free_memory,
				  aol_t *aol_allocated_memory, int nr_mallocs,
				  int nr_frees, int nr_fragmentations)
{
	long address = 0;
	int memory_size = 0, count;

	// parse the command
	count = sscanf(command, "%lx %d", &address, &memory_size);
	DIE(count != 2, "Invalid command");

	// check if the memory is allocated
	if (is_requested_memory_allocated(aol_allocated_memory, address,
									  memory_size)) {
		// read the data_info
		char *data = read_from_allocated_memory(aol_allocated_memory,
													  address, memory_size);

		// print the data_info
		for (int i = 0; i < memory_size; i++)
			printf("%c", data[i]);
		printf("\n");

		free(data);
	} else {
		// print the error message, the memory dump and exit
		printf("Segmentation fault (core dumped)\n");
		print_memory_dump(aol_free_memory, aol_allocated_memory, nr_mallocs,
						  nr_frees, nr_fragmentations);
		delete_aol(aol_free_memory);
		delete_aol(aol_allocated_memory);
		exit(0);
	}
}

// function that destroys the heap
void command_destroy(aol_t *aol_free_memory, aol_t *aol_allocated_memory)
{
	// free all the memory and exit
	delete_aol(aol_free_memory);
	delete_aol(aol_allocated_memory);
	exit(0);
}

// function that prints the memory dump
void command_dump(aol_t *aol_free_memory, aol_t *aol_allocated_memory,
				  int nr_mallocs, int nr_frees, int nr_fragmentations)
{
	// print the memory dump
	print_memory_dump(aol_free_memory, aol_allocated_memory,
					  nr_mallocs, nr_frees, nr_fragmentations);
}
