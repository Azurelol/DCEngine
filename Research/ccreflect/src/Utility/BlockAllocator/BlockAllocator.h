/*-------------------qMopey-Header----------------------
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 1/25/2013
File Name     : BlockAllocator.h
Purpose       : Allocator for smaller blocks of memory
                (< 640 byes) that persist longer than
                a single cycle.

                Internal fragmentation is traded for
                zero external fragmentation, and memory
                is aligned to either 4 byte boundaries. 

Note          : Does not ever free empty pages. In order
                to remove free pages more memory per
                page must be allocated in headers for
                each allocation as bookkeeping.

                If an allocation is greater than the
                max_allocation_size malloc is used
                instead.

Todo          : Use max page size of 1024 * 36, separate
                out blocks within these pages.
                4 byte alignment.

Usage         : BlockAllocator allocator;
                const int size = 32; // bytes

                // Simple allocation
                void *p = allocator.Allocate( size );

                // Provide size to free
                allocator.Free( p, size );
------------------------------------------------------*/
#pragma once

// This block should probably be in your precompiled somewhere
#include <cassert>
typedef float f32;
typedef double f64;
typedef signed char int8;
typedef signed short int16;
typedef signed int int32;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
// This block should probably be in your precompiled somewhere

const unsigned max_allocation_size = 640; // Largest allocation supported
const unsigned size_count = 16; // Amount of different sizes to allocate
const unsigned page_size = 256; // Number of blocks per page

// CRTP metaprogramming technique to wrap up allocator
// as a generic object. Useful for templated classes that require
// the use of an allocator (with a default allocator), so that
// run-time branching can be avoided. These two structs can probably
// just be ignored, as I only needed them for my SmartPtr class.

// base for allocation and deallocation
template <typename Derived>
struct Allocator
{
  void *Allocate( int32 size )
  {
    return static_cast<Derived *>(this)->Allocate( size );
  }
  void Free( void *data, int32 size )
  {
    return static_cast<Derived *>(this)->Free( data, size );
  }
};

// Standard allocator using malloc and free
struct STD_Allocator : public Allocator<STD_Allocator>
{
  void *Allocate( int32 size )
  {
    return operator new( size );
  }

  void Free( void *data, int32 size )
  {
    delete data;
  }
};

// Custom BlockAllocator
class BlockAllocator : public Allocator<BlockAllocator>
{
  struct block
  {
    block *next;
  };

  struct page
  {
    uint32 block_size;
    page *next;
    block *data;
  };

public:
  BlockAllocator( );
  ~BlockAllocator( );

  // Typecasting convenience allocate method
  template <typename T>
  T *Allocate( void );

  void *Allocate( int32 size );

  // Typecasting convenience free method
  template <typename T>
  void Free( T *data );
  template <typename T>
  void Free( T& data );

  void Free( void *data, int32 size );

  void clear( void );

private:
  static uint32 block_size_lookup[max_allocation_size + 1];
  static bool block_size_lookup_initialized;

  block *freeLists[size_count];
  page *pages;

  uint32 page_count;
};

// Typecasting convenience allocate method
template <typename T>
T *BlockAllocator::Allocate( void )
{
  return reinterpret_cast<T *>(Allocate( sizeof( T ) ));
}

// Typecasting convenience free method
template <typename T>
void BlockAllocator::Free( T *data )
{
  Free( data, sizeof( T ) );
}

template <typename T>
void BlockAllocator::Free( T& data )
{
  Free( &data, sizeof( T ) );
}