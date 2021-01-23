/* memory.h - simple implementation of custom memory functions
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

#ifndef HEAP_SIZE
	#define HEAP_SIZE (1024 * 1024) /* 1 MB */
#endif

#ifndef NULL
	#define NULL (void *)0
#endif

/*---------------------------------------------------------------------------*/
/*                              Data Structures                              */
/*---------------------------------------------------------------------------*/

struct block
{
    int size, free;
    struct block *next;
};

/*---------------------------------------------------------------------------------*/
/*                              Function Declarations                              */
/*---------------------------------------------------------------------------------*/

static void split(struct block *, int);
static void merge(void);
static void *malloc(int size);
static void *realloc(void *ptr, int size);
static void free(void *ptr);
static int memsize(void *ptr);
static void *memcpy(void *dest, void *src, int size);
static void *memmove(void *dest, void *src, int size);
static int onheap(void *ptr);
static int memuse(void);

/*-------------------------------------------------------------------*/
/*                              Globals                              */
/*-------------------------------------------------------------------*/

static unsigned char HEAP[HEAP_SIZE];
static struct block *FREE_BLOCKS = (struct block *)HEAP;

/*------------------------------------------------------------------------------------*/
/*                              Function Implementations                              */
/*------------------------------------------------------------------------------------*/

static void split(struct block *block, int size)
{
    struct block *new_block = (struct block *)(block + size + sizeof(struct block));

    new_block->size = block->size - size - sizeof(struct block);
    new_block->free = 1;
    new_block->next = block->next;

    block->size = size;
    block->free = 0;
    block->next = new_block;
}

static void merge(void)
{
    if(FREE_BLOCKS->size == 0) return;

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

static void *malloc(int size)
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

static void *realloc(void *ptr, int size)
{
    if(onheap(ptr))
    {
        void *new_ptr = malloc(size);

        if(new_ptr)
        {
            memcpy(new_ptr, ptr, size);

            free(ptr);

            return new_ptr;
        }
        else return NULL;
    }
    else return NULL;
}

static void free(void *ptr)
{
    if(onheap(ptr))
    {
        struct block *current = ptr;
        current--;

        current->free = 1;

        merge();
    }
}

static int memsize(void *ptr)
{
    if(onheap(ptr))
    {
        struct block *current = ptr;
        current--;

        return current->size;
    }
    else return -1;
}

static void *memcpy(void *dest, void *src, int size)
{
    char *b_dest = (char *)dest;
    char *b_src = (char *)src;

    int i;
    for(i = 0; i < size; i++) b_dest[i] = b_src[i];

    return dest;
}

static void *memmove(void *dest, void *src, int size)
{
    char *b_dest = (char *)dest;
    char *b_src = (char *)src;

    char *temp = (char *)malloc(sizeof(char) * size);

    if(temp == NULL) return NULL;

    int i;

    for(i = 0; i < size; i++) temp[i] = b_src[i];
    for(i = 0; i < size; i++) b_dest[i] = temp[i];

    return dest;
}

static int onheap(void *ptr)
{
    return ((void *)HEAP) <= ptr && ptr <= (((void *)HEAP) + HEAP_SIZE);
}

static int memuse(void)
{
    if(FREE_BLOCKS->size == 0) return 0;

    struct block *current = FREE_BLOCKS;
    int result = 0;

    while(current != NULL)
    {
        if(!current->free) result += current->size;
        current = current->next;
    }

    return result;
}

#endif
