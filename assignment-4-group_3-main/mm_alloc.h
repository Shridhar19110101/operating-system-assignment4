/*
 * mm_alloc.h
 *
 * Exports a clone of the interface documented in "man 3 malloc".
 */

#pragma once

#ifndef _malloc_H_
#define _malloc_H_

#define BLOCK_SIZE 40
#include <stdlib.h>

void* mm_malloc(size_t size);
void* mm_realloc(void* ptr, size_t size);
void mm_free(void* ptr);

typedef struct meta_block *block_ptr;
extern block_ptr HeadPtr;

/* block struct */
typedef struct meta_block {
    size_t size;
    struct meta_block *next; 
    struct meta_block *prev; 
    int free; 
    void *block; 
    
 }meta_block;

/* Split block according to size, b must exist */
void split_block (block_ptr b, size_t s);

/* Try fusing block with neighbors */
block_ptr merge(block_ptr b);

/* Get the block from addr */
block_ptr get_block (void *p);

/* Add a new block at the of heap,
 * return NULL if things go wrong
 */
block_ptr extend_heap (block_ptr last , size_t s);

/* Copy info and contents of block from one location to another.
 * returns ptr to new block if successful or NULL otherwise
 */
void* mem_copy(block_ptr oldB, block_ptr newB);
#endif
