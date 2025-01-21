#include "malloc.h"
#include <unistd.h>

/**
 * _free - Frees a previously allocated memory block
 * @ptr: The pointer to the memory block to be freed
 *
 */
void _free(void *ptr)
{
    if (!ptr) {
        return;
    }

    /** Get the block header by subtracting the size of the header from the pointer **/
    block_t *block = (block_t *)((char *)ptr - sizeof(size_t));

    /** Clear the block's next pointer and add it to the free list **/
    block->next = free_list;
    free_list = block;

    /** If the block is large enough (page-sized or more), we can release it back to the system **/
    size_t block_size = block->size + sizeof(size_t);

    if (block_size >= PAGE_SIZE) {
        /** If the block is large enough (page-sized or more), we release it back to the system **/
        munmap((void *)block, block_size);
    }
}