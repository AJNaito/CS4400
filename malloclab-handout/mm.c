/*
 * mm-naive.c - The least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by allocating a
 * new page as needed.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/* always use 16-byte alignment */
#define ALIGNMENT 16
#define WORD_SIZE 4
#define DOUBLE_WORD 8

#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define GET(bp) (*(size_t*)(bp))
#define PUT(bp, val) (*(size_t*)(bp)) = val
#define PACK(size, alloc) (size | alloc)

#define GET_ALLOC(bp) (GET(bp) & 0x1)
#define GET_SIZE(bp) (GET(bp) & 0x7)

#define GET_NEXT(bp) ((block*)bp)->next
#define GET_PREV(bp) ((block*)bp)->prev

#define NEXT_BLKP(bp) ((char*)bp + (GET_SIZE(HDRP(bp))))
#define PREV_BLKP(bp) ((char*)bp - (GET_SIZE(bp - OVERHEAD)))

#define HDRP(bp) ((char*)bp - sizeof(block_header))
#define FTRP(bp) (((char*)bp + GET_SIZE(HDRP(bp))) - OVERHEAD)

#define OVERHEAD (sizeof(block_header) + sizeof(block_footer))
#define MIN_FREE sizeof(block)

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))

/* rounds up to the nearest multiple of mem_pagesize() */
#define PAGE_ALIGN(size) (((size) + (mem_pagesize()-1)) & ~(mem_pagesize()-1))

typedef size_t block_header;
typedef size_t block_footer;

typedef struct {
  void* next;
  void* prev;
} block;

// helper methods
void* coalesce(void* bp);
void* extend(size_t size);
void* first_fit(size_t size);
void* find_page(void* bp, size_t size);
void place(void* bp, size_t size);
void remove_from_list(void* bp);
void add_to_list(void* bp);
int is_empty(void* page, size_t page_size);


void* first_free;
void* first_page;
void *current_avail = NULL;
int current_avail_size = 0;
int min_page_size = 4096;
int max_page_size = 2 << 32;

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
  mem_reset();

  current_avail_size = min_page_size;
  if ((current_avail = mem_map(current_avail_size)) == NULL) {
    return -1;
  }

  PUT(current_avail, 0); // store next page
  PUT(current_avail + DOUBLE_WORD, 0); // store previous page

  // prolog stuff
  PUT(current_avail + (2 * DOUBLE_WORD), current_avail_size); // store page size and padding
  PUT(current_avail + (3 * DOUBLE_WORD), PACK(ALIGNMENT, 1));
  PUT(current_avail + (4 * DOUBLE_WORD), ALIGNMENT);
  PUT(current_avail + (current_avail_size - DOUBLE_WORD), PACK(0, 1)); // terminator header

  current_avail += 4 * DOUBLE_WORD; // in prolog header
  current_avail_size -= 3 * ALIGNMENT; // 48 bytes used at this point

  // free_list
  void* free_block = NEXT_BLKP(current_avail);
  first_free = free_block;

  PUT(HDRP(free_block), PACK(current_avail_size, 0));
  PUT(FTRP(free_block), current_avail_size);
  
  return 0;
}

void* extend(size_t size) {
  current_avail_size = PAGE_ALIGN(size);

  if ((current_avail = mem_map(current_avail_size)) == NULL) {
    return NULL;
  }

  PUT(current_avail, (size_t)first_page); // store next page
  PUT(current_avail + DOUBLE_WORD, 0); // store previous page

  GET_PREV(first_page) = current_avail;

  first_page = current_avail;

  // prolog stuff
  PUT(current_avail + (2 * DOUBLE_WORD), current_avail_size); // store page size and padding
  PUT(current_avail + (3 * DOUBLE_WORD), PACK(ALIGNMENT, 1));
  PUT(current_avail + (4 * DOUBLE_WORD), ALIGNMENT);
  PUT(current_avail + (current_avail_size - DOUBLE_WORD), PACK(0, 1)); // terminator header

  current_avail += 4 * DOUBLE_WORD; // in prolog header
  current_avail_size -= 3 * ALIGNMENT; // 48 bytes used at this point

  void* free_block = NEXT_BLKP(current_avail);

  if (first_free != NULL) {
    GET_PREV(first_free) = free_block;
    GET_NEXT(free_block) = first_free;
  }

  first_free = free_block;

  return first_free;
}

/* 
 * mm_malloc - Allocate a block by using bytes from current_avail,
 *     grabbing a new page if necessary.
 */
void *mm_malloc(size_t size)
{
  size_t min_size = MAX(size, MIN_FREE);
  size_t a_size = ALIGN(min_size + OVERHEAD);

  void* p = first_fit(a_size);

  if (p != NULL) {
    place(p, a_size);
    return p;
  } else {
    min_page_size *= 2;

    int page_size = MAX(min_page_size, a_size);
    if ((p = extend(page_size)) == NULL) {
      return NULL;
    }

    place(p, a_size);
  }

  return p;

  // int newsize = ALIGN(size);
  // void *p;
  
  // if (current_avail_size < newsize) {
  //   current_avail_size = PAGE_ALIGN(newsize);
  //   current_avail = mem_map(current_avail_size);
  //   if (current_avail == NULL)
  //     return NULL;
  // }

  // p = current_avail;
  // current_avail += newsize;
  // current_avail_size -= newsize;
  
  // return p;
}

void* first_fit(size_t size) {
  void* bp;

  for (bp = first_free; bp != NULL; bp = GET_NEXT(bp)) {
    if (GET_SIZE(HDRP(bp)) >= size) {
      return bp;
    }
  }

  return NULL;
}

void place(void* bp, size_t size) {
  size_t free_size = GET_SIZE(HDRP(bp));

  if ((free_size - size) >= ALIGN(OVERHEAD + MIN_FREE)) {
    remove_from_list(bp);

    PUT(HDRP(bp), PACK(size, 1));
    PUT(FTRP(bp), size);

    bp = NEXT_BLKP(bp);
    PUT(HDRP(bp), PACK(free_size - size, 0));
    PUT(FTRP(bp), free_size - size);

    // add back to list
    GET_PREV(first_free) = bp;
    GET_NEXT(bp) = first_free;
    first_free = bp;
  } else {
    remove_from_list(bp);

    PUT(HDRP(bp), PACK(free_size, 1));
    PUT(FTRP(bp), free_size);
  }
}

void add_to_list(void* bp) {
  if (first_free == NULL) {
    first_free = bp;
    return;
  }

  GET_PREV(first_free) = bp;
  GET_NEXT(bp) = first_free;

  first_free = bp;
}

void remove_from_list(void* bp) {
  if (bp == first_free) {
    first_free = GET_NEXT(bp);

    if (first_free != NULL) {
      GET_PREV(first_free) = NULL;
    }
  } else {
    void* next = GET_NEXT(bp);
    void* prev = GET_NEXT(bp);

    GET_NEXT(prev) = next;

    if (next != NULL) {
      GET_PREV(next) = prev;
    }
  }

  GET_NEXT(bp) = NULL;
  GET_PREV(bp) = NULL; // clean block
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
  size_t freeSize = GET_SIZE(HDRP(ptr));

  PUT(HDRP(ptr), PACK(freeSize, 0));
  PUT(FTRP(ptr), freeSize);
  void* page = find_page(ptr);
  // printf("freeing page: %p limit: %p iterations: %d \n", page, page + GET((char*)page + (3 * DOUBLE_WORD)), freeing);
  
    // printf("before coalesce %p \n", ptr);
  void* free_ptr = coalesce(ptr);
    // printf("after \n");

  if (page_empty(page)) {
    int page_size = GET(page + ALIGNMENT);

    if (page == first_page) {
      first_page = GET_NEXT(page);
      // first_page = (void*)GET_ADDR(page);

      GET_PREV(first_page) = 0;
      // PUT_ADDR(first_page + DOUBLE_WORD, 0);
    } else {
      void* prev_page = GET_PREV(page);
      void* next_page = GET_NEXT(page);
      // void* prev_page = (void*)GET_ADDR(page + DOUBLE_WORD);
      // void* next_page = (void*)GET_ADDR(page);

      GET_NEXT(prev_page) = next_page;

      if (next_page != NULL) {
        GET_PREV(next_page) = prev_page;
      }

      // PUT_ADDR(prev_page, (size_t)next_page);
      // PUT_ADDR(next_page + DOUBLE_WORD, (size_t)prev_page);
    }
    // remove reference to unmapped pointer
    // printf("%p \n", free_ptr);
    // printf("%ld \n", mem_heapsize());
    remove_from_list(free_ptr);

    mem_unmap(page, page_size);
  }
}

int page_empty(void* page) {
  return (!GET_ALLOC(page + 48) && !(GET_SIZE(HDRP(NEXT_BLKP(page + 48))))); // next block after first_bp should be terminating
}

void* coalesce(void* bp) {
  void* next = NEXT_BLKP(bp);
  void* prev = PREV_BLKP(bp);

  int next_alloc = GET_ALLOC(HDRP(next));
  int prev_alloc = GET_ALLOC(HDRP(prev));
  size_t size = GET_SIZE(HDRP(bp));

  if (next_alloc && prev_alloc) {
    add_to_list(bp);
    return bp;
  } else if (!next_alloc && prev_alloc) {
    remove_from_list(next);

    size += GET_SIZE(HDRP(next));

    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), size);

    // add to free_list
    add_to_list(bp);
  } else if (next_alloc && !prev_alloc) {
    size += GET_SIZE(HDRP(prev));

    PUT(HDRP(prev), PACK(size, 0));
    PUT(FTRP(prev), size);
  } else {
    remove_from_list(next);
    
    size += GET_SIZE(HDRP(prev)) + GET_SIZE(HDRP(next));

    PUT(HDRP(prev), PACK(size, 0));
    PUT(FTRP(prev), size);
  }
}
