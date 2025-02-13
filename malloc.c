#include "malloc.h"
#include <unistd.h>
#include <stddef.h>


/** Static free list to manage available memory **/
static block_t *free_list = NULL;

/**
* align_size - Aligns an unsigned value to the ceiling multiple of 8
* @size: The size to align
*
* Return: The aligned size
*/
size_t align_size(size_t size)
{
	size += 7;
	size /= 8;
	return size * 8;
}

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

	/** Align the requested size to the nearest multiple of 8 **/
	size = align_size(size);

	/** Calculate total allocation size, including the block header **/
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

	/** Always return the pointer to the allocated memory, not the block itself **/
	return ((void *)(new_block + 1));
}