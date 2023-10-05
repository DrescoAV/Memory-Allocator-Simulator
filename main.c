#include "vma.h" // Include the header file for the virtual memory allocator

int main(void)
{
	uint64_t arena_size = 0;
	arena_t *arena = NULL; // Declare a pointer to an arena structure
	char input[255];
	char input_buffer;
	uint64_t block_size = 0;
	uint64_t address = 0;
	uint64_t data_size = 0;
	int8_t *data = NULL;

	// Read arena size and allocate memory for the arena
	scanf("%s%ld%c", input, &arena_size, &input_buffer);
	arena = alloc_arena(arena_size);

	// Loop to process commands until DEALLOC_ARENA is encountered
	while (strcmp(input, "DEALLOC_ARENA") != 0)
	{
		scanf("%s", input); // Read the command

		if (strcmp(input, "ALLOC_BLOCK") == 0)
		{
			// Read address and block size, then allocate a block
			scanf("%ld%c%ld%c", &address, &input_buffer, &block_size, &input_buffer);
			alloc_block(arena, address, block_size);
		}
		else if (strcmp(input, "FREE_BLOCK") == 0)
		{
			// Read address and free a block
			scanf("%ld%c", &address, &input_buffer);
			free_block(arena, address);
		}
		else if (strcmp(input, "WRITE") == 0)
		{
			// Read address, data size, and data, then perform a write operation
			scanf("%ld%c%ld%c", &address, &input_buffer, &data_size, &input_buffer);
			data = malloc(data_size);
			fread(data, 1, data_size, stdin);
			write(arena, address, data_size, data);
		}
		else if (strcmp(input, "READ") == 0)
		{
			// Read address and data size, then perform a read operation
			scanf("%ld%c%ld%c", &address, &input_buffer, &data_size, &input_buffer);
			read(arena, address, data_size);
		}
		else if (strcmp(input, "PMAP") == 0)
		{
			// Perform a pmap operation
			pmap(arena);
		}
		else if (strncmp(input, "MPROTECT", 8) == 0 && strlen(input) == 8)
		{
			// Read address and permission string, then perform a memory protection operation
			uint64_t address;
			scanf("%ld", &address);
			char string[100], c;
			scanf("%c", &c);
			fgets(string, 100, stdin);
			int8_t data = mprotect_aux(string);
			mprotect(arena, address, &data);
		}
		else if (strcmp(input, "DEALLOC_ARENA") != 0)
		{
			// Invalid command
			printf("Invalid command. Please try again.\n");
		}
	}

	dealloc_arena(arena); // Deallocate the arena
	return 0;
}
