/* memory.h - embedded implementation of libc's memory utilities
 *
 * Copyright (c) 2021 Cleanware
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef MEMORY_H
#define MEMORY_H

#ifndef HEA_SIZE
	#define HEAP_SIZE (1024 * 1024) /* 1 MB */
#endif

#ifndef NULL
	#define NULL (void *)0
#endif

struct block
{
    int size, free;
    struct block *next;
};

void *malloc(int size);
void *realloc(void *ptr, int size);
void free(void *ptr);
int memsize(void *ptr);
void memcpy(void *dest, void *src, int size);
int onheap(void *ptr);
int memuse(void);
int talloc(void);
void split(struct block *, int);
void merge(void);

unsigned char HEAP[HEAP_SIZE];
Block *FREE_BLOCKS = (void *)HEAP;
int TOTAL_ALLOCATIONS = 0;

void split(struct block *block, int size)
{
    struct block *new_block = (struct block *)(block + size + sizeof(struct block));

    new_block->size = block->size - size - sizeof(struct block);
    new_block->free = 1;
    new_block->next = block->next;

    block->size = size;
    block->free = 0;
    block->next = new_block;
}

void merge(void)
{
    if (FREE_BLOCKS->size == 0) return;

    struct block *current = FREE_BLOCKS;

    while(current && current->next)
    {
        if(current->free && current->next->free)
        {
            current->size += current->next->size + sizeof(struct block);
            current->next = current->next->next;

            continue;
        }

        current = current->next;
    }
}

void *malloc(int size)
{
    struct block *current = FREE_BLOCKS;

    if(FREE_BLOCKS->size == 0)
    {
        FREE_BLOCKS->size = HEAP_SIZE - sizeof(struct block);
        FREE_BLOCKS->free = 1;
        FREE_BLOCKS->next = NULL;
    }

    TOTAL_ALLOCATIONS += size;

    while(current != NULL)
    {
        if(current->free)
        {
            if(current->size == size)
            {
                current->free = 0;
                current++;

                return (void *)current;
            }
            else if(current->size > size + sizeof(struct block))
            {
                split(current, size);
                current++;

                return (void *)current;
            }
        }

        current = current->next;
    }

    return NULL;
}

void *realloc(void *ptr, int size)
{
    if(onheap(ptr))
    {
        void *new_ptr = malloc(size);

        if (new_ptr) {
            memcpy(new_ptr, ptr, size);

            free(ptr);

            return new_ptr;
        }
        else
            return NULL;
    }
    else
        return NULL;
}

void free(void *ptr)
{
    if(onheap(ptr))
    {
        struct block *current = ptr;
        current--;

        current->free = 1;

        merge();
    }
}

int memsize(void *ptr)
{
    if(onheap(ptr))
    {
        struct block *current = ptr;
        current--;

        return current->size;
    }else
        return -1;
}

void memcpy(void *dest, void *src, int size)
{
    char *b_dest = (char *)dest;
    char *b_src = (char *)src;

    int i;
    for(i = 0; i < size; i++)
        b_dest[i] = b_src[i];
}

int onheap(void *ptr)
{
    return ((void *)HEAP) <= ptr && ptr <= (((void *)HEAP) + HEAP_SIZE);
}

int memuse(void)
{
    if(FREE_BLOCKS->size == 0)
        return 0;

    struct block *current = FREE_BLOCKS;
    int result = 0;

    while(current != NULL)
    {
        if(!current->free)
            result += current->size;

        current = current->next;
    }

    return result;
}

int talloc(void)
{
    return TOTAL_ALLOCATIONS;
}

#endif