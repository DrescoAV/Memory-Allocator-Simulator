#pragma once
#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definition of a doubly-linked list node
typedef struct node_t
{
	void *data;
	struct node_t *next, *prev;
} node_t;

// Definition of a doubly-linked list
typedef struct
{
	size_t size;
	size_t data_size;
	node_t *tail, *head;
} list_t;

// Definition of a block in the arena
typedef struct
{
	uint64_t start_address;
	size_t size;
	void *miniblock_list;
} block_t;

// Definition of a miniblock within a block
typedef struct
{
	uint64_t start_address;
	size_t size;
	uint8_t perm;
	void *rw_buffer;
} miniblock_t;

// Definition of the arena
typedef struct
{
	uint64_t arena_size;
	list_t *alloc_list;
} arena_t;

// Function prototypes for creating, manipulating, and deallocating data structures
list_t *create_list();
node_t *create_node();
void insert_node_at_end(node_t **head, node_t **tail, node_t *new_node);
void insert_node_at_begging(node_t **head, node_t *new_node);
void delete_list(node_t *head, int is_block_or_miniblock);
void delete_node_data(node_t *node, int is_block_or_miniblock);
void delete_node_from_list(list_t *list, node_t *node);

// Function prototypes for arena management
arena_t *alloc_arena(const uint64_t size);
void dealloc_arena(arena_t *arena);

// Function prototypes for allocation, deallocation, and manipulation of blocks and miniblocks
int check_already_allocated(arena_t *arena, const uint64_t address,
							const uint64_t size);
node_t *check_have_left_neighbour(arena_t *arena, const uint64_t address);
node_t *check_have_right_neighbour(arena_t *arena, const uint64_t address,
								   const uint64_t size);
void alloc_block(arena_t *arena, const uint64_t address, const uint64_t size);
node_t *find_miniblock_using_address(arena_t *arena, const uint64_t address,
									 node_t **return_block);
void free_block(arena_t *arena, const uint64_t address);

// Function prototypes for reading and writing data
void read(arena_t *arena, uint64_t address, uint64_t size);
void write(arena_t *arena, const uint64_t address, const uint64_t size,
		   int8_t *dataa);

// Function prototypes for memory protection management
int8_t mprotect_aux(char *string);
void pmap(const arena_t *arena);
void mprotect(arena_t *arena, uint64_t address, int8_t *permission);