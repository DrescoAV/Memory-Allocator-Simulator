# Virtually Memory Allocator Simulator

## Author: Andrei-Valerian Andreescu

## Table of Contents

- [Virtually Memory Allocator Simulator](#virtually-memory-Allocator-simulator)
  - [Author: Andrei-Valerian Andreescu](#author-andrei-valerian-andreescu)
  - [Table of Contents](#table-of-contents)
  - [Project Overview](#project-overview)
    - [Memory Allocation and Deallocation](#memory-allocation-and-deallocation)
    - [Data Reading and Writing](#data-reading-and-writing)
    - [Memory Protection](#memory-protection)
    - [Visualization](#visualization)
  - [How to Use](#how-to-use)
  - [Error Handling](#error-handling)
  - [Limitations](#limitations)
  - [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
    - [Installation](#installation)
  - [Example Explained](#example-explained)
    - [Initialize the Allocator](#initialize-the-allocator)
    - [Allocate an Arena](#allocate-an-arena)
    - [Allocate Blocks](#allocate-blocks)
    - [View Memory Map](#view-memory-map)
    - [Allocate Another Block](#allocate-another-block)
    - [View Memory Map Again](#view-memory-map-again)
    - [Write Data To Memory](#write-data-to-memory)
    - [Read Data from Memory](#read-data-from-memory)
    - [Free a Block](#free-a-block)
    - [View Memory Map Once More](#view-memory-map-once-more)
    - [Change Permissions](#change-permissions)
    - [Try Writing to Protected Memory](#try-writing-to-protected-memory)
    - [Clean Up](#clean-up)
  - [Contributing](#contributing)
  - [License](#license)

## Project Overview

The Virtual Memory Allocator Simulator is a software project that simulates the fundamental concepts of memory management within operating systems. It provides a controlled environment to explore and implement key memory allocation and manipulation operations. This project aims to help users gain a deep understanding of memory management principles and the inner workings of an allocator.

### Memory Allocation and Deallocation

One of the core functionalities of the Virtual Memory Allocator is memory allocation. Users can allocate memory blocks within a simulated memory arena. These memory blocks can be of varying sizes and are organized using a doubly-linked list data structure. The allocator ensures that memory blocks do not overlap, and it manages the allocation of new blocks as needed.

Memory deallocation is another critical feature. Users can free memory blocks or smaller sub-blocks called "miniblocks." The allocator optimizes memory usage by merging adjacent free memory areas, ensuring efficient use of memory resources.

![Howitworks](https://github.com/DrescoAV/Memory-Allocator-Simulator/blob/main/How_it_works.png)

### Data Reading and Writing

The project provides capabilities for reading and writing data within allocated memory blocks. Users can read a specified amount of data from a memory address and write data to a given address within the allocated memory. The allocator handles data access and manipulation, making it a valuable tool for understanding data management in memory.

### Memory Protection

As a bonus feature, the Virtual Memory Allocator allows users to change the permissions of memory areas. This feature provides fine-grained control over memory access. Users can specify different permissions, such as read, write, and execute, for specific memory regions. The allocator checks these permissions during read and write operations, enhancing the security and control of memory resources.

### Visualization

To aid in understanding the state of allocated memory, the project includes a visualization feature. Users can use the PMAP command to generate a comprehensive map of memory blocks and miniblocks, along with their permissions and sizes. This visualization helps users track memory allocations and understand how memory is organized.

## How to Use

Getting started with the Virtual Memory Allocator is straightforward:

1. **Allocate an Arena:** Begin by allocating an arena using the "**ALLOC_ARENA**" command and specifying the desired size of the memory arena.

2. **Memory Operations:** Perform memory allocation, deallocation, data reading, and writing operations using the provided commands, such as "**ALLOC_BLOCK**", "**FREE_BLOCK**", "**READ**", and "**WRITE**".

3. **Memory Protection:** Optionally, explore memory protection by changing the permissions of memory areas using the "**MPROTECT**" command. You have the following options : "**PROT_NONE**", "**PROT_READ**", "**PROT_WRITE**", "**PROT_EXEC**".

4. **Visualization:** Use the "**PMAP**" command to visualize the current state of memory blocks and miniblocks, gaining insights into memory management.

5. **Cleanup:** When you're done experimenting, free all resources by deallocating the arena with the "**DEALLOC_ARENA**" command.

## Error Handling

The Virtual Memory Allocator includes robust error handling to provide informative error messages for various scenarios. It ensures that invalid commands or operations are handled gracefully, helping users understand and debug their interactions with the allocator.

## Limitations

1. Simplified Memory Model: The project offers a simplified and idealized model of memory management. In real-world operating systems, memory management is much more complex, involving considerations like paging, swapping, and multiple memory hierarchies. The allocator's focus on basic memory allocation and deallocation may not cover all aspects of real-world scenarios.

2. Limited Security Considerations: While the project includes a memory protection feature, it does not cover advanced security mechanisms.

3. Simplified Permissions: The memory protection feature provides a basic level of memory access control with read, write, and execute permissions. However, it does not include more advanced memory protection mechanisms found in real operating systems, such as non-executable memory regions.

4. No Real File System Integration: In real operating systems, processes often rely on file systems for persistent storage. This project does not incorporate file system integration, focusing solely on memory allocation and manipulation.

5. Limited Practical Use: While the Virtual Memory Allocator is an excellent educational tool, its practical use is limited. It lacks the extensive memory management features and optimizations found in production-ready allocators like those in modern operating systems.

6. Memory Fragmentation: The allocator's simplistic memory management strategy may lead to memory fragmentation over time. In practice, memory allocators need to implement strategies to minimize fragmentation, such as buddy allocation or memory compaction. These aspects are not covered in this project.

7. Resource Overhead: The project may consume significant memory resources for maintaining its data structures, especially when managing a large number of memory blocks and miniblocks. In a real operating system, memory management components aim to be memory-efficient.

8. Not Suitable for Production: This project is intended for educational purposes and experimentation. It is not suitable for use in production environments and should not be used as a replacement for real memory management solutions in software development.

These limitations are essential to keep in mind when using the Virtual Memory Allocator project. While it provides a valuable learning experience for memory management concepts, it does not replace the complexity and robustness of real-world memory management systems.

## Getting Started

Follow these steps to get started with Quadtree Image Compression.

## Prerequisites

Before you begin, make sure you have the following prerequisites:

- A C compiler (e.g., GCC)
- A Linux distribution (optional for make command).

### Installation

1. Clone this repository to your local machine:

   ```shell
   git clone https://github.com/DrescoAV/Memory-Allocator-Simulator
   ```

2. Compile the program by typing "make" into your terminal (or compile manually using gcc).

## Example Explained

To understand better how to use the program, here is an example explained:

### Initialize the Allocator

Start the allocator by running the executable:

```bash
make
./vma
```

You should see an empty prompt awaiting your commands.

### Allocate an Arena

Allocate an arena to work with a virtual memory space. Let's allocate an arena with a size of 65536 bytes:

```bash
ALLOC_ARENA 65536
```

This command creates a virtual memory arena of 65536 bytes.

### Allocate Blocks

Now, let's allocate some memory blocks within the arena:

```bash
ALLOC_BLOCK 4096 10
ALLOC_BLOCK 12288 10
ALLOC_BLOCK 12308 10
```

The first command allocates a block starting at address 4096 with a size of 10 bytes.
The second command allocates a block starting at address 12288 with a size of 10 bytes.
The third command allocates a block starting at address 12308 with a size of 10 bytes.

### View Memory Map

You can view the memory map to see the allocated blocks and miniblocks within them:

```bash
PMAP
```

The output will display information about the allocated blocks and their miniblocks. You'll see the addresses, sizes, and permissions associated with each block and miniblock.

### Allocate Another Block

Let's allocate one more block, which should merge two existing blocks:

```bash
ALLOC_BLOCK 12298 10
```

This command allocates a block starting at address 12298 with a size of 10 bytes. Since it overlaps with the existing blocks, it will merge them into one.

### View Memory Map Again

After allocating the new block, view the memory map again:

```bash
PMAP
```

This time, you should see that the two blocks have merged into one larger block.

### Write Data To Memory

Now, let's write some data to memory. Write the string "Hello, OS!" starting at address 4096:

```bash
WRITE 4096 10 Hello, OS!
```

This command writes the given string to memory starting at address 4096.

### Read Data from Memory

Read the data back from memory:

```bash
READ 4096 8
```

This command reads 8 bytes of data starting at address 4096. It should display the string "Hello, O".

### Free a Block

Let's free one of the blocks to see how it affects the memory map:

```bash
FREE_BLOCK 12298
```

This command frees the block starting at address 12298. Since it was part of a larger block, the memory map will be updated accordingly.

### View Memory Map Once More

View the memory map to see the changes:

```bash
PMAP
```

You'll notice that the freed block has been split, and the memory map reflects the changes.

### Change Permissions

You can change permissions for a miniblock. Let's change the permissions of the miniblock starting at address 12308 to "PROT_NONE" (no access):

```bash
MPROTECT 12308 PROT_NONE
```

This command changes the permissions of the miniblock to deny any access.

### Try Writing to Protected Memory

Now, attempt to write to the protected memory:

```bash
WRITE 12308 5 Denied
```

Since the miniblock has "PROT_NONE" permissions, this command should result in an error message.

### Clean Up

Finally, when you're done experimenting, you can clean up and exit the allocator:

```bash
DEALLOC_ARENA
```

This command deallocates the arena and frees all associated resources.

## Contributing

Contributions to the Quadtree Compression project are welcome! If you'd like to contribute, please follow these guidelines:

1. Fork the repository.
2. Create a new branch for your feature or bug fix.
3. Make your changes and ensure they pass any existing tests.
4. Create a pull request with a clear description of your changes.

We appreciate your contributions to make this project better.

## License

[![MIT license](https://img.shields.io/badge/License-MIT-blue.svg)](https://github.com/DrescoAV/Memory-Allocator-Simulator/blob/main/LICENSE)

This project is licensed under the MIT License. See the [LICENSE](https://github.com/DrescoAV/Memory-Allocator-Simulator/blob/main/LICENSE) file for details.
