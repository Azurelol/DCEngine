/*-------------------qMopey-Header----------------------
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 1/25/2013
File Name     : BlockAllocator.cpp
Purpose       : Allocator for smaller blocks of memory
                (< 640 byes) that persist longer than
                a single cycle.
------------------------------------------------------*/
#include "Precompiled.h"

const uint32 block_sizes[] = {
  4,    // 0
  8,    // 1
  16,   // 2
  32,   // 3
  64,   // 4
  96,   // 5
  128,  // 6
  160,  // 7
  192,  // 8
  224,  // 9
  256,  // 10
  320,  // 11
  384,  // 12
  448,  // 13
  512,  // 14
  640   // 15
};

// Ensure the size_count matches number of elements in block_sizes
STATIC_ASSERT( numElemofArray( block_sizes ) == size_count );

uint32 BlockAllocator::block_size_lookup[max_allocation_size + 1];
bool BlockAllocator::block_size_lookup_initialized = false;

BlockAllocator::BlockAllocator( ) : pages( 0 ), page_count( 0 )
{
  std::memset( freeLists, 0, numElemofArray( freeLists ) * sizeof( page * ) );

  // Initialize the lookup table for sizes
  if(!block_size_lookup_initialized)
  {
    for(uint32 i = 0, j = 0; i < numElemofArray( block_size_lookup ); ++i)
    {
      if(i > block_sizes[j])
        ++j;

      block_size_lookup[i] = j;
    }
  }
}

BlockAllocator::~BlockAllocator( )
{
  // :)
  clear( );
}

void *BlockAllocator::Allocate( int32 size )
{
  // Do not handle size zero allocations
  if(size == 0)
    return NULL;

  // Do not handle negative sizes
  assert( 0 < size );

  if(size > max_allocation_size)
    return malloc( size );

  uint32 index = block_size_lookup[size];

  if(freeLists[index])
  {
    block *data = freeLists[index];
    freeLists[index] = data->next;
    return data;
  }
  else
  {
    // Allocate new page
    page *new_page = (page *)malloc( block_sizes[index] * page_size + sizeof( page ) );
    ++page_count;

    // Hook page into singly linked list of pages and init the block_size
    new_page->block_size = block_sizes[index];
    new_page->next = pages;
    new_page->data = (block *)((char *)new_page + sizeof( page ));
    pages = new_page;

    // Link all nodes within the new page together
    for(uint32 i = 0; i < page_size - 1; ++i)
    {
      block *node = (block *)((char *)new_page->data + new_page->block_size * i);
      block *next = (block *)((char *)new_page->data + new_page->block_size * (i + 1));
      node->next = next;
    }

    // Last node NULL
    block *last = (block *)((char *)new_page->data + new_page->block_size * (page_size - 1));
    last->next = NULL;

    // Push second block on the page onto freeList
    freeLists[index] = new_page->data->next;

    // Return first block on the page
    return new_page->data;
  }
}

void BlockAllocator::Free( void *data, int32 size )
{
  // Do not handle size zero allocations
  if(size == 0)
    return;

  // Do not handle negative sizes
  assert( 0 < size );

  if(size > max_allocation_size)
    return free( data );

  uint32 index = block_size_lookup[size];

#ifdef _DEBUG
  bool found = false;
  for(page *p = pages; p; p = p->next)
    if(data >= p->data && data < reinterpret_cast<char *>(p->data) + p->block_size * page_size)
    {
      found = true;
      assert( p->block_size >= (unsigned)size ); // size does not match block_size of the page it lies on
      break;
    }

  assert( found ); // Address provided to free did not lie within any pages

  // Uncomment these lines to detect double free. This is very slow due to the lack of
  // allocation headers.
  //for(block *p = freeLists[index]; p; p = p->next)
  //  assert( data != p ); // Double free detected
#endif // _DEBUG

  reinterpret_cast<block *>(data)->next = freeLists[index];
  freeLists[index] = reinterpret_cast<block *>(data);
}

void BlockAllocator::clear( void )
{
  for(uint32 i = 0; i < page_count; ++i)
  {
    page *next = pages->next;
    free( pages );
    pages = next;
  }

  std::memset( freeLists, 0, sizeof( block * ) * size_count );
  page_count = 0;
}
