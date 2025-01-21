#include "malloc.h"
#include <unistd.h>
#include <stddef.h>
#include <sys/mman.h>

#define PAGE_SIZE 4096

/**
 * naive_malloc - Allocates memory in page-sized chunks with a header
 * @size: The number of bytes
 * Return: A pointer to the allocated memory or NULL if the allocation fails.
 */
void *naive_malloc(size_t size)
{
    /** Calculate the total size to be allocated: header size + requested size **/
    size_t total_size = size + sizeof(size_t);

    /** Round up to the nearest multiple of the page size **/
    total_size = (total_size + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);

    /** Use mmap to allocate memory in page-sized chunks **/
    void *ptr = mmap(NULL, total_size, PROT_READ | PROT_WRITE,
                     MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (ptr == MAP_FAILED) {
        return NULL;
    }

    /** Store the size in the header (first part of the allocated memory) **/
    *(size_t *)ptr = size;

    /** Return a pointer to the memory just after the header **/
    return (void *)((char *)ptr + sizeof(size_t));
}