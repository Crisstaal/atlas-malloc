#include "malloc.h"
#include <unistd.h>
#include <stddef.h>
#include <sys/mman.h>

#define PAGE_SIZE 4096
#define PAGE_COUNT 2

/** Pointer to the start of our "heap" (just a big char array) */
static char *mem_pages = NULL;
/** Position of the next chunk to be allocated in the `mem_pages` array (offset in bytes) */
static size_t mem_position = 0;
/** Memory remaining in our "heap"
 * (not really necessary if we `mmap` one big chunk of memory
 * to use without adding more `mmap` memory later)
 */
static size_t mem_available = PAGE_SIZE * PAGE_COUNT;

/**
 * struct mem_chunk - Structure of a memory chunk
 * @chunk_size: The number of bytes held in the chunk
 * @mem_ptr: The address right after the `chunk_size` header
 */
typedef struct mem_chunk
{
    size_t chunk_size;
    char *mem_ptr;
} mem_chunk_t;

/**
 * naive_malloc - Allocates memory in page-sized chunks with a header
 * @size: The number of bytes
 * Return: A pointer to the allocated memory or NULL if the allocation fails.
 */
void *naive_malloc(size_t size)
{
    mem_chunk_t *chunk_ptr = NULL;

    /** Check if memory pages allocated */
    if (mem_pages == NULL)
    {
	/** Memory hasn't been mapped yet, so let's map it */
        mem_pages = mmap(NULL, PAGE_SIZE * PAGE_COUNT, PROT_READ | PROT_WRITE,
                     MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
        if (mem_pages == MAP_FAILED)
            return (NULL);
    }


    chunk_ptr = (mem_chunk_t *)&mem_pages[mem_position]; /* interpret data at `mem_position` as a `mem_chunk_t` pointer */
    chunk_ptr->chunk_size = size; /* after aligning the size to a multiple of 8 */
    
    /* update book-keeping variables */
    mem_position += size + sizeof(size_t);
    mem_available -= size + sizeof(size_t);
    /* return pointer to the caller */
    return chunk_ptr->mem_ptr;
}