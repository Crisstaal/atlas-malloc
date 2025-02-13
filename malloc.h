#ifndef MALLOC_H
#define MALLOC_H

#include <stddef.h>

#define PAGE_SIZE 4096
/**
 * struct block - Block header structure for managing memory blocks
 * @size: Size of the allocated memory block
 * @next: Pointer to the next block in the free list
 */
typedef struct block {
    size_t size;
    struct block *next;
} block_t;

extern block_t *free_list;
void *naive_malloc(size_t size);
void *_malloc(size_t size);
void _free(void *ptr);

#endif