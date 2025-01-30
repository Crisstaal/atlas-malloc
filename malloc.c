#include "malloc.h"
#include <unistd.h>
#include <stddef.h>

#define PAGE_SIZE 4096

/** Block header structure for managing memory blocks **/
typedef struct block {
    size_t size;
    struct block *next;
} block_t;

/** Static free list to manage available memory **/
static block_t *free_list = NULL;

/**
 * _malloc - Allocates memory in page-sized chunks with a header
 * @size: The number of bytes
 *
 * Return: A pointer to the allocated memory or NULL if the allocation fails.
 */
void *_malloc(size_t size)
{
    /** If size is 0, return NULL **/
    if (size == 0) {
        return NULL;
    }

    /** Calculate the total size to be allocated: header size + requested size **/
    size_t total_size = size + sizeof(block_t);

    /** Try to find a suitable free block in the free list **/
    block_t *prev = NULL, *curr = free_list;
    while (curr && curr->size < total_size) {
        prev = curr;
        curr = curr->next;
    }

    if (curr) {
        /** Found a suitable free block, remove it from the free list **/
        if (prev) {
            prev->next = curr->next;
        } else {
            free_list = curr->next;

        return (void *)(curr + 1);
        }
    }
        /** No suitable free block, expand the heap **/
    block_t *new_block = sbrk(total_size);
    if (new_block == (void *)-1){
        return NULL;

    new_block->size = total_size;
    return (void *)(new_block + 1);
}
}