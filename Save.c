/** malloc use of mmap contents
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
 *
void *_malloc(size_t size)
{
    /** If size is 0, return NULL **
    if (size == 0) {
        return NULL;
    }

    /** Calculate the total size to be allocated: header size + requested size **
    size_t total_size = size + sizeof(size_t);

    /** Round up to the nearest multiple of the page size for alignment **
    total_size = (total_size + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);

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
        }

        /** Return the memory just after the header **/
        return (void *)((char *)curr + sizeof(size_t));
    }

    /** No suitable free block, allocate a new one using mmap **/
    void *ptr = mmap(NULL, total_size, PROT_READ | PROT_WRITE,
                     MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (ptr == MAP_FAILED) {
        return NULL;
    }

    /** Store the size in the header (first part of the allocated memory) **/
    *(size_t *)ptr = size;

    /** Return a pointer to the memory just after the header **/
    return (void *)((char *)ptr + sizeof(size_t));
}**//
