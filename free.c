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
    block_t *block = (block_t *)((char *)ptr - 1);

    /** Clear the block's next pointer and add it to the free list **/
    block->next = free_list;
    free_list = block;
}