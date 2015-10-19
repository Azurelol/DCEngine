/*-----------------Alloctopus-Header--------------------
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : Fri May 03 21:10:05 2013

File Name     : SlotAllocator.h
Purpose       : Simple array based container to
                construct and destruct POD-like structs.
                Ideal for holding game objects and
                components. Works with pure
                handle translation.

Features      : O( 1 ) - Allocation
                O( 1 ) - Deletion
                O( 1 ) - Lookup
                Guaranteed contiguous memory for cache
                efficient traversal.

Limitations   : - All structs must be moveable from one
                  location to another by copy ctor
                  in an efficient manner.
                - Only supports a contiguous block of
                  memory of a single type of object.
------------------------------------------------------*/

#pragma once

template <typename T>
class SlotAllocator
{
public:
    /*----------------
       Ctors/Dtors
    ----------------*/
  SlotAllocator( );

  // Reserve constructor
  SlotAllocator( uint32 numElements );

    /*----------------
        Modifiers
    ----------------*/
  // Placement new an object and return index
  uint32 Allocate( void );
  template <typename Arg1>
  uint32 Allocate( const Arg1& arg1 );
  template <typename Arg1, typename Arg2>
  uint32 Allocate( const Arg1& arg1, const Arg2& arg2 );
  template <typename Arg1, typename Arg2, typename Arg3>
  uint32 Allocate( const Arg1& arg1, const Arg2& arg2, const Arg3& arg3 );

  // Placement delete + move rightmost element into deleted slot
  void Free( uint32 index );

  // Swap memory of two indices
  void Swap( uint32 a, uint32 b );

  // Shrink internal array to exactly number of elements
  void Shrink( void );

  // Reduce capacity to numElements clamped above size
  void ShrinkTo( uint32 numElements );

  // Reduce capacity by numElements, clamp capacity above size
  void ShrinkBy( uint32 numElements );

    /*----------------
       Data Access
    ----------------*/
  T *operator[]( uint32 index );
  const T *operator[]( uint32 index ) const;
  uint32 Size( void ) const; // Number of allocated elements
  uint32 Capacity( void ) const; // Number of elements total
  uint32 GetIndex( T *data ) const;

private:
  T *m_array;

  uint32 m_size;
  uint32 m_capacity;

  // Grow internal array
  void Grow( void );
  void GrowInteral( void );

  // Move data from one index to another
  void Move( uint32 a, uint32 b );
};

/*------------------------------------------------------------------------------------------------

                                    Constructors / Destructors

------------------------------------------------------------------------------------------------*/
template <typename T>
SlotAllocator<T>::SlotAllocator( )
  : m_array( NULL )
  , m_size( 0 )
  , m_capacity( 0 )
{
}

template <typename T>
SlotAllocator<T>::SlotAllocator( uint32 numElements )
  : m_array( (T *)malloc( sizeof( T ) * numElements ) )
  , m_size( 0 )
  , m_capacity( numElements )
{
}

/*------------------------------------------------------------------------------------------------

                                            Modifiers

------------------------------------------------------------------------------------------------*/
template <typename T>
uint32 SlotAllocator<T>::Allocate( void )
{
  if(m_size == m_capacity)
    Grow( );

  T *val = m_array + m_size;

  PERM_DSIABLE_WARNING( 4345 )
  // Placement new an object and return it's index
  new (val) T( );
  return m_size++;
}

template <typename T>
template <typename Arg1>
uint32 SlotAllocator<T>::Allocate( const Arg1& arg1 )
{
  if(m_size == m_capacity)
    Grow( );

  T *val = m_array + m_size;

  // Placement new an object and return it's index
  new (val) T( std::forward<Arg1>(arg1) );
  return m_size++;
}

template <typename T>
template <typename Arg1, typename Arg2>
uint32 SlotAllocator<T>::Allocate( const Arg1& arg1, const Arg2& arg2 )
{
  if(m_size == m_capacity)
    Grow( );

  T *val = m_array + m_size;

  // Placement new an object and return it's index
  new (val) T( std::forward<Arg1>(arg1), std::forward<Arg2>(arg2) );
  return m_size++;
}

template <typename T>
template <typename Arg1, typename Arg2, typename Arg3>
uint32 SlotAllocator<T>::Allocate( const Arg1& arg1, const Arg2& arg2, const Arg3& arg3 )
{
  if(m_size == m_capacity)
    Grow( );

  T *val = m_array + m_size;

  // Placement new an object and return it's index
  new (val) T( std::forward<Arg1>(arg1), std::forward<Arg2>(arg2), std::forward<Arg3>(arg3) );
  return m_size++;
}

template <typename T>
void SlotAllocator<T>::Free( uint32 index )
{
  // Destruct data at provided index
  T *val = (T *)m_array + index;
  val->~T( );

  // Place rightmost element in the now empty slot
  Move( index, m_size - 1 );
  --m_size;
}

template <typename T>
void SlotAllocator<T>::Grow( void )
{
  // Expand the capacity by 2 if it isn't zero
  // The + 10 prevents rapid expansion at smaller capacity sizes.
  m_capacity = m_capacity ? (m_capacity * 2 + 10) : 4;

  GrowInteral( );
}

template <typename T>
void SlotAllocator<T>::GrowInteral( void )
{
  // Allocate a new array
  T *array = (T *)malloc( sizeof( T ) * m_capacity );

  // Copy the pre-existing elements
  for(uint32 i = 0; i < m_size; ++i)
    new (array + i) T( m_array[i] );

  // Free our old data
  free( m_array );

  // set the pointer to the new array
  m_array = array;
}

template <typename T>
void SlotAllocator<T>::Swap( uint32 a, uint32 b )
{
  assert( a < m_size );
  assert( b < m_size );

  T c( std::move( m_array[a] ) );
  a = std::move( m_array[b] );
  b = std::move( m_array[c] );
}

template <typename T>
void SlotAllocator<T>::Move( uint32 a, uint32 b )
{
  m_array[a] = std::move( m_array[b] );
}

template <typename T>
void SlotAllocator<T>::Shrink( void )
{
  // Reduce capacity to size
  m_capacity = m_size;

  GrowInteral( );
}

template <typename T>
void SlotAllocator<T>::ShrinkTo( uint32 numElements )
{
  // Reduce capacity to numElements clamped above m_size
  if(numElements > m_size)
    m_capacity = numElements;
  else
    m_capacity = m_size;

  GrowInteral( );
}

template <typename T>
void SlotAllocator<T>::ShrinkBy( uint32 numElements )
{
  // Reduce capacity by numElements, clamp capacity above size
  const uint32 dif = m_capacity - numElements;
  if(dif > m_size)
    m_capacity = dif;
  else
    m_capacity = m_size;

  GrowInteral( );
}

/*------------------------------------------------------------------------------------------------

                                           Data Access

------------------------------------------------------------------------------------------------*/
template <typename T>
T *SlotAllocator<T>::operator[]( uint32 index )
{
  assert( index < m_size );
  return (T *)m_array + index;
}

template <typename T>
const T *SlotAllocator<T>::operator[]( uint32 index ) const
{
  assert( index < m_size );
  return (T *)m_array + index;
}

template <typename T>
uint32 SlotAllocator<T>::Size( void ) const
{
  return m_size;
}

template <typename T>
uint32 SlotAllocator<T>::Capacity( void ) const
{
  return m_capacity;
}

template <typename T>
uint32 SlotAllocator<T>::GetIndex( T *data ) const
{
  return data - m_array;
}
