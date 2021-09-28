#ifndef POOL_ALLOCATOR_H
#define POOL_ALLOCATOR_H

#include <stddef.h>
#include <stdint.h>


typedef struct poolAllocNode PoolAllocNode;
typedef struct poolAllocNode{
    PoolAllocNode * restrict next; 
} PoolAllocNode;

typedef struct poolalloc PoolAlloc;
// Pool allocator
typedef struct poolalloc {

    // size of the memory in this allocator
    uint32_t totalSize;

    // size of each chunk
    uint32_t chunkSize;

    // num chunks allocated
    uint32_t allocated_chunks;
    
    // points to the first node in the free list
    PoolAllocNode * restrict head;

    // the pointer to the buffer held by this allocator
    uint8_t * restrict buffptr;
} PoolAlloc;



// function to create a pool allocator
extern PoolAlloc* alloc_createPoolAllocator(uint32_t size, uint16_t chunkAlignment, uint32_t chunkSize);

// function to allocate chunks
extern void * alloc_poolAllocAllocate(PoolAlloc * restrict pa);

// free all the allocated chunk in allocator
// doesn't deallocate the memory allocated by the createPoolAllocator function
extern void alloc_poolAllocFreeAll(PoolAlloc * restrict pa); 

// free the chunk given by ptr 
// ptr should be allocate by using poolAllocAllocate function
extern void alloc_poolAllocFree(PoolAlloc * restrict pa, void * restrict ptr);

// free pool allocator
extern void alloc_freePoolAllocator(PoolAlloc * restrict pAlloc);

#endif

