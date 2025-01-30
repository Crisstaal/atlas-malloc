#include "malloc.h"
#include <unistd.h>
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

/** Static free list to manage available memory **/
static block_t *free_list = NULL;

/**
 * _malloc - Allocates memory using sbrk
 * @size: The number of bytes
 *
 * Return: A pointer to the allocated memory or NULL if the allocation fails.
 */
void *_malloc(size_t size)
{
    block_t *prev = NULL, *curr = free_list;
    block_t *new_block;
    size_t total_size;

    if (size == 0)
        return (NULL);

    /** Calculate total allocation size **/
    total_size = size + sizeof(block_t);

    /** Search for a suitable free block **/
    while (curr && curr->size < total_size)
    {
        prev = curr;
        curr = curr->next;
    }

    if (curr)
    {
        /** Remove the block from the free list **/
        if (prev)
            prev->next = curr->next;
        else
            free_list = curr->next;

        return ((void *)(curr + 1));
    }

    /** No suitable free block, expand the heap **/
    new_block = sbrk(total_size);
    if (new_block == (void *)-1)
        return (NULL);

    new_block->size = total_size;
    return ((void *)(new_block + 1));
}