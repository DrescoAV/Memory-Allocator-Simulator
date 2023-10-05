#include "vma.h" // Include the header file containing data structures and function declarations

// Function to create a new list
list_t *create_list()
{
    // Allocate memory for the list structure
    list_t *list = malloc(sizeof(list_t));
    list->head = NULL;
    list->tail = NULL;
    list->data_size = 0;
    list->size = 0;
    return list;
}

// Function to create a new node
node_t *create_node()
{
    // Allocate memory for the node structure
    node_t *node = malloc(sizeof(node_t));
    return node;
}

// Function to insert a node at the end of the list
void insert_node_at_end(node_t **head, node_t **tail, node_t *new_node)
{
    // If the list is empty, set new_node as both head and tail
    if (*head == NULL)
    {
        new_node->next = new_node->prev = NULL;
        *head = new_node;
        *tail = new_node;
        return;
    }

    // Insert new_node at the end of the list
    (*tail)->next = new_node;
    new_node->next = NULL;
    new_node->prev = (*tail);
    (*tail) = new_node;
}

// Function to insert a node at the beginning of the list
void insert_node_at_begging(node_t **head, node_t *new_node)
{
    // Insert new_node at the beginning of the list
    new_node->prev = NULL;
    new_node->next = (*head);
    (*head)->prev = new_node;
    (*head) = new_node;
}

// Function to allocate a new arena with the specified size
arena_t *alloc_arena(const uint64_t size)
{
    // Allocate memory for the arena structure
    arena_t *arena = malloc(sizeof(arena_t));
    arena->arena_size = size;          // Set arena size
    arena->alloc_list = create_list(); // Initialize allocation list using create_list() function
    return arena;                      // Return the newly created arena
}

// Function to delete a list and its nodes, considering block or miniblock types
void delete_list(node_t *head, int is_block_or_miniblock)
{
    node_t *temp;

    // Iterate through the list and delete nodes and their data based on the type (block or miniblock)
    while (head != NULL)
    {
        temp = head;
        head = head->next;

        // Check if the data in the node is block or miniblock and delete accordingly
        if (is_block_or_miniblock == 1)
            delete_node_data(temp, 1);
        else
            delete_node_data(temp, 0);

        // Free the memory allocated for the current node
        free(temp);
    }
}

// Function to delete data inside a node based on its type (block or miniblock)
void delete_node_data(node_t *node, int is_block_or_miniblock)
{
    block_t *block = NULL;
    miniblock_t *miniblock = NULL;
    list_t *list = NULL;

    // Check if the node contains a block or miniblock data based on the flag is_block_or_miniblock
    if (is_block_or_miniblock == 1)
    {
        block = node->data;           // Set the block pointer to the data inside the node
        list = block->miniblock_list; // Get the list of miniblocks from the block structure
        delete_list(list->head, 0);   // Delete miniblocks in the miniblock list
        free(block->miniblock_list);
        free(block);
    }
    else
    {
        miniblock = node->data; // Set the miniblock pointer to the data inside the node
        free(miniblock->rw_buffer);
        free(miniblock);
    }
}

// Function to delete a node from a list
void delete_node_from_list(list_t *list, node_t *node)
{
    // Update pointers in the adjacent nodes to bypass the node to be deleted
    if (node == list->head)
    {
        list->head = node->next; // Update the head of the list
    }
    if (node == list->tail)
    {
        list->tail = node->prev; // Update the tail of the list
    }
    if (node->prev != NULL)
    {
        node->prev->next = node->next; // Update next pointer of the previous node
    }
    if (node->next != NULL)
    {
        node->next->prev = node->prev; // Update prev pointer of the next node
    }
    free(node); // Free memory allocated for the node structure
}

// Function to deallocate memory occupied by an arena and its associated data structures
void dealloc_arena(arena_t *arena)
{
    delete_list(arena->alloc_list->head, 1); // Delete the allocation list along with block and miniblock data
    free(arena->alloc_list);
    free(arena);
}

// Function to check if a specific address range is already allocated in the arena
int check_already_allocated(arena_t *arena, const uint64_t address, const uint64_t size)
{
    node_t *node_block = arena->alloc_list->head;
    block_t *in_block;
    uint64_t block_address;
    uint64_t block_size;

    // Iterate through the nodes in the allocation list to check for overlapping address ranges
    while (node_block != NULL)
    {
        in_block = node_block->data;             // Get the block structure from the node data
        block_address = in_block->start_address; // Get the start address of the block
        block_size = in_block->size;             // Get the size of the block

        // Check if the given address range overlaps with the current block's address range
        if (address < block_address && address + size > (block_address))
        {
            return 1; // Address range overlaps, return 1 indicating already allocated
        }
        if (address >= block_address && address < (block_address + block_size))
        {
            return 1; // Address range overlaps, return 1 indicating already allocated
        }

        node_block = node_block->next; // Move to the next node in the allocation list
    }

    return 0; // Address range does not overlap with any allocated block, return 0 indicating not already allocated
}

// Function to check if there is a left neighbor block of the given address
node_t *check_have_left_neighbour(arena_t *arena, const uint64_t address)
{
    node_t *node_block = arena->alloc_list->head; // Pointer to iterate through the allocation list
    block_t *in_block;
    uint64_t left_block_address = 0;

    // If the arena contains only one block, return NULL as there is no left neighbor
    if (arena->alloc_list->head == arena->alloc_list->tail)
        return NULL;

    // Iterate through the nodes in the allocation list to find the left neighbor block
    while (node_block != NULL)
    {
        in_block = node_block->data;
        left_block_address = in_block->start_address + in_block->size; // Calculate start address of the
                                                                       // potential left neighbor block

        // Check if the calculated left block address matches the given address
        if (left_block_address == address)
        {
            return node_block; // Return the node containing the left neighbor block
        }

        node_block = node_block->next; // Move to the next node in the allocation list
    }

    return NULL; // No left neighbor block found, return NULL
}

// Function to check if there is a right neighbor block of the given address and size
node_t *check_have_right_neighbour(arena_t *arena, const uint64_t address, const uint64_t size)
{
    node_t *node_block = arena->alloc_list->head;
    block_t *in_block;
    uint64_t right_block_address = 0;

    // If the arena contains only one block, return NULL as there is no right neighbor
    if (node_block == arena->alloc_list->tail)
        return NULL;

    // Iterate through the nodes in the allocation list to find the right neighbor block
    while (node_block != NULL)
    {
        in_block = node_block->data;                   // Get the block structure from the node data
        right_block_address = in_block->start_address; // Get the start address of the current block

        // Check if the calculated right block address matches the given address + size
        if (right_block_address == (address + size))
        {
            return node_block; // Return the node containing the right neighbor block
        }
        node_block = node_block->next;
    }

    return NULL; // No right neighbor block found, return NULL
}

// Function to allocate a block in the arena with the given address and size
void alloc_block(arena_t *arena, const uint64_t address, const uint64_t size)
{
    // Error checking for invalid allocation addresses and overlapping allocations
    if (address >= arena->arena_size)
    {
        printf("The allocated address is outside the size of the arena\n");
    }
    else if (address + size > arena->arena_size)
    {
        printf("The end address is past the size of the arena\n");
    }
    else if (check_already_allocated(arena, address, size))
    {
        printf("This zone was already allocated.\n");
    }
    else
    {
        node_t *node_block = create_node();

        // Update the size and data size of the large allocation list
        arena->alloc_list->size++;
        arena->alloc_list->data_size += size;

        // Insert the new node at the end of the large allocation list
        insert_node_at_end(&(arena->alloc_list->head), &(arena->alloc_list->tail), node_block);

        block_t *block = malloc(sizeof(block_t));
        node_block->data = block;
        block->start_address = address;
        block->size = size;

        // Create a new list for miniblocks inside the block structure
        list_t *mini_list = create_list();
        block->miniblock_list = mini_list;

        // Allocate memory for the first node in the miniblock list
        node_t *node_miniblock = create_node();

        // Insert the new node at the end of the miniblock list
        insert_node_at_end(&(mini_list->head), &(mini_list->tail), node_miniblock);

        // Allocate memory for the first miniblock associated with the node in the miniblock list
        miniblock_t *miniblock = malloc(sizeof(miniblock_t));

        // Connect the node in the miniblock list to the miniblock structure
        node_miniblock->data = miniblock;

        miniblock->start_address = address;
        miniblock->size = size;
        miniblock->perm = 6;

        // Allocate memory for the read-write buffer inside the miniblock
        miniblock->rw_buffer = malloc(size);

        // Update the size and data size of the miniblock list
        mini_list->size++;
        mini_list->data_size = mini_list->data_size + size;

        // Check and merge with the right neighbor block if available
        node_t *right_neighbour = check_have_right_neighbour(arena, address, size);
        block_t *in_block_right = NULL;
        list_t *mini_list_right = NULL;

        if (right_neighbour != NULL)
        {
            in_block_right = right_neighbour->data;
            mini_list_right = in_block_right->miniblock_list;

            // Update pointers to merge with the right neighbor block and its miniblock list
            mini_list_right->head->prev = mini_list->tail;
            mini_list->tail->next = mini_list_right->head;
            mini_list->tail = mini_list_right->tail;
            mini_list_right->tail = NULL;
            mini_list_right->head = NULL;

            // Update sizes and pointers after merging
            mini_list->data_size += mini_list_right->data_size;
            mini_list->size += mini_list_right->size;
            arena->alloc_list->size--;

            // Update pointers in the allocation list after merging with the right neighbor block
            right_neighbour->next->prev = right_neighbour->prev;
            if (right_neighbour->prev != NULL)
            {
                right_neighbour->prev->next = right_neighbour->next;
            }
            else
            {
                arena->alloc_list->head = right_neighbour->next;
            }
            right_neighbour->next = NULL;
            right_neighbour->prev = NULL;

            // Free memory occupied by the merged right neighbor miniblock list and block structure
            free(mini_list_right);
            free(in_block_right);
            free(right_neighbour);
        }

        // Check and merge with the left neighbor block if available
        node_t *left_neighbour = check_have_left_neighbour(arena, address);
        block_t *in_block_left = NULL;
        list_t *mini_list_left = NULL;

        if (left_neighbour != NULL)
        {
            in_block_left = left_neighbour->data;
            mini_list_left = in_block_left->miniblock_list;

            // Update pointers to merge with the left neighbor block and its miniblock list
            mini_list->head->prev = mini_list_left->tail;
            mini_list_left->tail->next = mini_list->head;
            mini_list_left->tail = mini_list->tail;
            mini_list->head = NULL;
            mini_list->tail = NULL;

            // Update sizes and pointers after merging
            mini_list_left->data_size += mini_list->data_size;
            mini_list_left->size += mini_list->size;
            arena->alloc_list->size--;

            // Update pointers in the allocation list after merging with the left neighbor block
            arena->alloc_list->tail = node_block->prev;
            node_block->prev->next = NULL;
            node_block->prev = NULL;

            // Update block structure and miniblock list pointers after merging
            block_t *temp = block;
            block = in_block_left;
            free(mini_list);
            mini_list = mini_list_left;
            free(temp);
            free(node_block);
        }

        block->size = mini_list->data_size; // Update the block size after merging
    }
}

// Function to find a miniblock using its address, return the miniblock and its parent block
node_t *find_miniblock_using_address(arena_t *arena, const uint64_t address, node_t **return_block)
{
    block_t *block = NULL;
    miniblock_t *miniblock = NULL;
    node_t *node_list = arena->alloc_list->head;
    node_t *node_mini_list = NULL;
    list_t *mini_list = NULL;
    (*return_block) = NULL;

    // Iterate through the allocation list to find the miniblock with the given address
    while (node_list != NULL)
    {
        block = node_list->data;
        mini_list = block->miniblock_list;
        node_mini_list = mini_list->head;

        // Iterate through the miniblock list to find the miniblock with the given address
        while (node_mini_list != NULL)
        {
            miniblock = node_mini_list->data;
            if (miniblock->start_address == address)
            {
                *return_block = node_list;
                return node_mini_list; // Return the node containing the found miniblock
            }
            node_mini_list = node_mini_list->next; // Move to the next node in the miniblock list
        }
        node_list = node_list->next; // Move to the next node in the allocation list
    }

    return NULL; // Miniblock with the given address not found, return NULL
}

void free_block(arena_t *arena, const uint64_t address)
{
    node_t *node = NULL;
    node_t *mini_node = find_miniblock_using_address(arena, address, &node);
    block_t *block = NULL;
    miniblock_t *miniblock = NULL;
    list_t *mini_list = NULL;
    node_t *new_node = NULL;
    block_t *new_block = NULL;
    list_t *new_mini_list = NULL;
    uint64_t data_lost = 0;
    uint64_t size_lost = 0;
    node_t *aux = NULL;

    // Check if the provided address is valid
    if (mini_node == NULL)
    {
        printf("Invalid address for free.\n");
    }
    else
    {
        block = node->data;
        mini_list = block->miniblock_list;

        // If the miniblock list contains only one miniblock, delete the entire block from the allocation list
        if (mini_list->size == 1)
        {
            arena->alloc_list->size--;
            arena->alloc_list->data_size -= block->size;
            delete_node_data(node, 1);
            delete_node_from_list(arena->alloc_list, node);
        }
        else
        {
            if (mini_node == mini_list->head || mini_node == mini_list->tail)
            {
                miniblock = mini_node->data;
                mini_list->size--;
                mini_list->data_size = mini_list->data_size - miniblock->size;
                block->size = mini_list->data_size;
                arena->alloc_list->data_size -= miniblock->size;
                if (mini_node == mini_list->head)
                {
                    miniblock = mini_node->next->data;
                    block->start_address = miniblock->start_address;
                    mini_list->head = mini_node->next;
                }
                else
                {
                    mini_list->tail = mini_node->prev;
                }
                delete_node_data(mini_node, 0);
                delete_node_from_list(mini_list, mini_node);
            }

            else // If the miniblock to be freed is in the middle of the miniblock list
            {
                arena->alloc_list->size++;

                new_node = create_node();

                // Insert the new node into the allocation list
                // Update tail pointer if the new node is inserted at the end
                if (node == arena->alloc_list->tail)
                {
                    arena->alloc_list->tail = new_node;
                }
                new_node->next = node->next;
                new_node->prev = node;
                node->next = new_node;

                new_block = malloc(sizeof(block_t));
                new_node->data = new_block;

                if (mini_node->next != NULL)
                    miniblock = mini_node->next->data;

                new_block->start_address = miniblock->start_address;
                new_mini_list = malloc(sizeof(list_t));
                new_block->miniblock_list = new_mini_list;
                new_mini_list->head = mini_node->next; 
                new_mini_list->tail = mini_list->tail; 
                mini_list->tail = mini_node->prev;
                if (mini_node->next != NULL)
                    mini_node->next->prev = NULL;
                if (mini_node->prev != NULL)
                    mini_node->prev->next = NULL; 

                aux = new_mini_list->head;

                while (aux != NULL)
                {
                    miniblock = aux->data;
                    data_lost += miniblock->size;
                    size_lost++;
                    aux = aux->next;
                }
                new_mini_list->data_size = data_lost;
                new_mini_list->size = size_lost;
                miniblock = mini_node->data;
                mini_list->data_size = mini_list->data_size - miniblock->size - new_mini_list->data_size;
                mini_list->size = mini_list->size - size_lost - 1;
                new_block->size = new_mini_list->data_size;
                block->size -= new_mini_list->data_size + miniblock->size;
                arena->alloc_list->data_size -= miniblock->size;

                // Free memory occupied by the merged miniblock and node
                free(miniblock->rw_buffer);
                free(miniblock);
                free(mini_node);
            }
        }
    }
}

// Function to check if the given address is allocated within the arena
node_t *check_allocated(arena_t *arena, uint64_t address)
{
    node_t *node = arena->alloc_list->head;
    block_t *block = NULL;

    // Iterate through the blocks in the allocation list to check if the address is allocated
    while (node != NULL)
    {
        block = node->data;
        if (block->start_address <= address && address < (block->start_address + block->size))
        {
            return node; // Return the node containing the allocated block
        }
        node = node->next; // Move to the next block in the allocation list
    }
    return NULL; // Address not allocated, return NULL
}

// Function to read data from the allocated arena using the given address and size
void read(arena_t *arena, uint64_t address, uint64_t size)
{
    node_t *node = NULL;
    block_t *block = NULL;
    list_t *mini_list = NULL;
    node_t *mini_node = NULL;
    miniblock_t *miniblock = NULL;
    uint64_t data_wrote_total = 0;
    uint64_t data_in_miniblock = 0;
    node = check_allocated(arena, address);
    node_t *current = NULL;
    int8_t perm_read = 1; // Variable to check read permissions

    // If the address is allocated
    if (node != NULL)
    {
        block = node->data;
        mini_list = block->miniblock_list;
        mini_node = mini_list->head;

        // Iterate through the miniblocks to check read permissions
        current = mini_list->head;
        while (current != NULL)
        {
            miniblock = current->data;
            if ((int)(miniblock->perm) < 4)
            {
                perm_read = 0; // Invalid permissions for read, set the flag to 0
            }
            current = current->next;
        }

        // If read permissions are valid
        if (perm_read)
        {
            miniblock = mini_node->data;

            // Find the miniblock containing the given address
            while (!(miniblock->start_address <= address && address < (miniblock->start_address + miniblock->size)))
            {
                mini_node = mini_node->next;
                miniblock = mini_node->data;
            }
            data_in_miniblock = miniblock->start_address + miniblock->size - address;

            // Warning if size is bigger than the block size
            if (size > (block->size + block->start_address - address))
            {
                printf("Warning: size was bigger than the block size. Reading %ld characters.\n",
                       block->size + block->start_address - address);
            }

            // Read data from the miniblocks
            while (size > data_wrote_total)
            {
                if (data_wrote_total == 0)
                {
                    if (size > data_in_miniblock)
                    {
                        fwrite(miniblock->rw_buffer + (miniblock->size - data_in_miniblock), 1, miniblock->size, stdout);
                    }
                    else
                    {
                        fwrite(miniblock->rw_buffer + (miniblock->size - data_in_miniblock), 1, size - data_wrote_total, stdout);
                    }
                }
                else if (size - data_wrote_total <= miniblock->size)
                {
                    fwrite(miniblock->rw_buffer, 1, size - data_wrote_total, stdout);
                }
                else
                {
                    fwrite(miniblock->rw_buffer, 1, miniblock->size, stdout);
                }
                data_wrote_total += data_in_miniblock;
                mini_node = mini_node->next;

                // Break if the end of the miniblock list is reached
                if (mini_node == NULL)
                {
                    break;
                }
                miniblock = mini_node->data;
                data_in_miniblock = miniblock->size;
            }
        }
        else
        {
            printf("Invalid permissions for read.\n"); // Invalid permissions for read
            return;
        }
        printf("\n");
    }
    else
    {
        printf("Invalid address for read.\n"); // Invalid address for read
    }
}

// Function to write data into the allocated arena using the given address, size, and data
void write(arena_t *arena, const uint64_t address, const uint64_t size, int8_t *data)
{
    node_t *node = NULL;
    block_t *block = NULL;
    list_t *mini_list = NULL;
    node_t *mini_node = NULL;
    miniblock_t *miniblock = NULL;
    uint64_t data_wrote_total = 0;
    uint64_t data_in_miniblock = 0;
    node = check_allocated(arena, address); // Check if the address is allocated in the arena
    node_t *current = NULL;
    int8_t perm_write = 1; // Variable to check write permissions

    // If the address is allocated
    if (node != NULL)
    {
        block = node->data;
        mini_list = block->miniblock_list;
        mini_node = mini_list->head;

        // Iterate through the miniblocks to check write permissions
        current = mini_list->head;
        while (current != NULL)
        {
            miniblock = current->data;
            // Check if write permissions are valid
            if ((int)(miniblock->perm) != 2 && (int)(miniblock->perm) != 6 &&
                (int)(miniblock->perm) != 3 && (int)(miniblock->perm) != 7)
            {
                perm_write = 0;
            }
            current = current->next;
        }

        // If write permissions are valid
        if (perm_write)
        {
            miniblock = mini_node->data;

            // Find the miniblock containing the given address
            while (!(miniblock->start_address <= address &&
                     address < (miniblock->start_address + miniblock->size)))
            {
                mini_node = mini_node->next;
                miniblock = mini_node->data;
            }
            data_in_miniblock = miniblock->start_address + miniblock->size - address;

            // Write data into the miniblocks
            if (size <= data_in_miniblock)
            {
                memcpy(miniblock->rw_buffer, data, size);
            }
            else
            {
                while (size > data_wrote_total)
                {
                    if (data_wrote_total == 0)
                    {
                        memcpy(miniblock->rw_buffer +
                                   (miniblock->size - data_in_miniblock),
                               data, data_in_miniblock);
                    }
                    else if (size - data_wrote_total <= miniblock->size)
                    {
                        memcpy(miniblock->rw_buffer, data + data_wrote_total,
                               size - data_wrote_total);
                    }
                    else
                    {
                        memcpy(miniblock->rw_buffer, data + data_wrote_total,
                               miniblock->size);
                    }
                    data_wrote_total += data_in_miniblock;
                    mini_node = mini_node->next;
                    if (mini_node == NULL)
                    {
                        break;
                    }
                    miniblock = mini_node->data;
                    data_in_miniblock = miniblock->size;
                }
                if (size > data_wrote_total)
                {
                    printf(
                        "Warning: size was bigger than the block size. Writing %ld "
                        "characters.\n",
                        data_wrote_total);
                }
            }
        }
        else
        {
            printf("Invalid permissions for write.\n"); // Invalid permissions for write
        }
    }
    else
    {
        printf("Invalid address for write.\n"); // Invalid address for write
    }
    free(data); // Free the allocated data buffer
}

// Function to display the permissions of a miniblock (Read, Write, Execute)
void show_perm(miniblock_t *miniblock)
{
    if ((miniblock->perm >> 2) & 1)
        printf("R"); // Read permission
    else
        printf("-");

    if ((miniblock->perm >> 1) & 1)
        printf("W"); // Write permission
    else
        printf("-");

    if (miniblock->perm & 1)
        printf("X"); // Execute permission
    else
        printf("-");
}

// Function to print the memory map (block addresses, miniblock addresses, permissions)
void pmap(const arena_t *arena)
{
    printf("Total memory: 0x%lX bytes\n", arena->arena_size);
    printf("Free memory: 0x%lX bytes\n",
           arena->arena_size - arena->alloc_list->data_size);
    printf("Number of allocated blocks: %ld\n", arena->alloc_list->size);
    node_t *node = arena->alloc_list->head;
    uint64_t miniblocks_no = 0;
    block_t *block = NULL;
    list_t *list = NULL;

    // Iterate through the allocated blocks to count the total number of allocated miniblocks
    while (node != NULL)
    {
        block = node->data;
        list = block->miniblock_list;
        miniblocks_no += list->size;
        node = node->next;
    }
    printf("Number of allocated miniblocks: %ld\n", miniblocks_no); // Print number of allocated miniblocks

    size_t i, j, k;
    node = arena->alloc_list->head;
    node_t *mini_node = NULL;
    miniblock_t *miniblock = NULL;
    node_t *curr_node = NULL;
    int64_t min_address = 92233720307;
    int64_t min_last = -1;

    // Iterate through the allocated blocks and their miniblocks to print the memory map
    for (i = 1; i <= arena->alloc_list->size; i++)
    {
        curr_node = arena->alloc_list->head;
        min_address = 92233720307;
        for (k = 1; k <= arena->alloc_list->size; k++)
        {
            block = curr_node->data;
            if ((int64_t)block->start_address < min_address && (int64_t)block->start_address > min_last)
            {
                node = curr_node;
                min_address = block->start_address;
            }
            curr_node = curr_node->next;
        }
        block = node->data;
        printf("\nBlock %ld begin\n", i);
        printf("Zone: 0x%lX - 0x%lX\n", block->start_address,
               block->start_address + block->size);

        list = block->miniblock_list;
        mini_node = list->head;
        for (j = 1; j <= list->size; j++)
        {
            miniblock = mini_node->data;
            printf("Miniblock %ld:\t\t0x%lX\t\t-\t\t0x%lX\t\t| ", j,
                   miniblock->start_address,
                   miniblock->start_address + miniblock->size);
            show_perm(miniblock); // Print permissions of the miniblock
            printf("\n");
            mini_node = mini_node->next;
        }

        printf("Block %ld end", i);
        printf("\n");
        node = node->next;
        min_last = min_address;
    }
}

// Function to parse the input string and convert permission string to integer representation
int8_t mprotect_aux(char *string)
{
    int8_t result = 0;
    char *p = strtok(string, "\n |"); // Tokenize the input string using space and newline characters

    // Iterate through the tokens and convert permission strings to integer representation
    while (p)
    {
        // Check each token and update the result based on the permission string
        if (strcmp(p, "PROT_NONE") == 0)
            result += 0;
        if (strcmp(p, "PROT_READ") == 0)
            result += 4;
        if (strcmp(p, "PROT_WRITE") == 0)
            result += 2;
        if (strcmp(p, "PROT_EXEC") == 0)
            result += 1;

        p = strtok(NULL, "\n |"); // Move to the next token
    }
    return result; // Return the integer representation of the permissions
}

// Function to set memory protection permissions for a specific address in the arena
void mprotect(arena_t *arena, uint64_t address, int8_t *permission)
{
    uint64_t valid_address = 0;
    miniblock_t *minichosen_block = NULL;
    miniblock_t *miniblock = NULL;
    list_t *mini_list = NULL;
    node_t *mini_node = NULL;
    block_t *block = NULL;

    // Iterate through allocated blocks and their miniblocks to find the specified address
    node_t *counter = arena->alloc_list->head;
    for (uint64_t i = 0; i < arena->alloc_list->size; i++)
    {
        block = counter->data;
        mini_list = block->miniblock_list;
        mini_node = mini_list->head;

        // Iterate through miniblocks in the current block
        for (uint64_t j = 0; j < mini_list->size; j++)
        {
            miniblock = mini_node->data; // Get the current miniblock

            // Check if the start address of the miniblock matches the given address
            if (miniblock->start_address == address)
            {
                valid_address = 1;
                minichosen_block = miniblock;
            }
            mini_node = mini_node->next; // Move to the next miniblock
        }
        counter = counter->next; // Move to the next block
    }

    // If the given address is not found in the allocated memory
    if (!valid_address)
    {
        printf("Invalid address for mprotect.\n"); // Print error message
        return;
    }

    minichosen_block->perm = *permission; // Set the permissions of the chosen miniblock to the given value
}
