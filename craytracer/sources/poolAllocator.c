#include "poolAllocator.h"

#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdalign.h>

#define ALIGNED_MALLOC(alignment, size) aligned_alloc((alignment), (size))
#define CUSTOM_MALLOC(size) malloc((size))

#ifndef NDEBUG
static bool isPowerOfTwo(uintptr_t align){
    return (align & (align - 1)) == 0;
}
#endif 

static uint32_t alignSizeForward(uint32_t size, uint16_t align){
    assert(isPowerOfTwo(align) && "align is not a power of two");
    
    uint16_t modulo = size & (align - 1);

    if(modulo != 0){
        size += align - modulo;
    }

    return size;
}

PoolAlloc* alloc_createPoolAllocator(uint32_t size, uint16_t chunkAlignment, uint32_t chunkSize){
    assert(isPowerOfTwo(chunkAlignment) && "align is not a power of two");
    
    uint32_t chunkCount = size / chunkSize;

    if(chunkAlignment < alignof(PoolAllocNode)){
        chunkAlignment = alignof(PoolAllocNode);
    }
    
    // align the size 
    size = alignSizeForward(size, chunkAlignment);
    chunkSize = alignSizeForward(chunkSize, chunkAlignment);
    
    uint32_t diff = chunkCount - size/chunkSize;
    if(diff > 0){
        size += diff * chunkSize;
    }

    assert(chunkSize >= sizeof(PoolAllocNode) && "chunk size less than header size");
    assert(size >= chunkSize && "total size is less than chunk size");

    PoolAlloc* pa = (PoolAlloc *) CUSTOM_MALLOC(sizeof(PoolAlloc));
    
    pa->totalSize = size;
    pa->chunkSize = chunkSize;
    pa->buffptr = ALIGNED_MALLOC(chunkAlignment, size);
    pa->head = NULL;
    
    // create the free list
    alloc_poolAllocFreeAll(pa);
    
    return pa;
}

void * alloc_poolAllocAllocate(PoolAlloc * restrict pa){
    PoolAllocNode * node = pa->head;
    if(node == NULL){

#ifndef NDEBUG
        printf("%u\n", pa->dbgS.allocatedChunks);
#endif
        assert(0 && "Pool allocator is empty");
        return NULL;
    }

    pa->head = pa->head->next;

#ifndef NDEBUG
    pa->dbgS.allocatedChunks += 1;
#endif
    // zero the memory and return it 
    return memset(node, 0, pa->chunkSize);
}

void alloc_poolAllocFree(PoolAlloc * restrict pa, void * restrict ptr){
    if(ptr == NULL){
        return;
    }
    
    void * start = pa->buffptr;
    void * end = &pa->buffptr[pa->totalSize];
    
    if(!(start <= ptr && ptr < end)){
        assert(0 && "Invalid memory");
        return;
    }

    PoolAllocNode * node = (PoolAllocNode *) ptr;
    node->next = pa->head;
    pa->head = node;

#ifndef NDEBUG
    pa->dbgS.allocatedChunks -= 1;
#endif
}

void alloc_poolAllocFreeAll(PoolAlloc * restrict pa){
    uint32_t chunkCount = pa->totalSize / pa->chunkSize;

    PoolAllocNode * node = NULL;
    for(uint32_t i = 0; i < chunkCount; i++){
        node = (PoolAllocNode *) &pa->buffptr[i * pa->chunkSize];
        node->next = pa->head;
        pa->head = node;
    }

#ifndef NDEBUG
    pa->dbgS.allocatedChunks = 0;
#endif
} 

extern void alloc_freePoolAllocator(PoolAlloc * restrict pAlloc){
    free(pAlloc->buffptr);
    free(pAlloc);
}
