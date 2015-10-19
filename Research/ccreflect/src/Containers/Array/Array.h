/*-------------------qMopey-Header----------------------
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Steven George
                Randy Gaul
                 - Added templated allocator support
                   and EmplaceBack functions/overloads
                   on 2/28/2013
Creation Date : 2/24/2013
File Name     : Array.h
Purpose       : A templatized growable array type.
------------------------------------------------------*/

#pragma once

// Internal usage -- allows to default allocator usage
extern STD_Allocator Array_StandardAllocator;

template<typename T, typename Allocator = STD_Allocator>
class Array
{
public:
    /*----------------
       Ctors/Dtors
    ----------------*/
  Array( void );
  Array( Allocator& allocator );
  Array( const uint32 capacity );
  Array( const uint32 capacity, Allocator& allocator );
  Array( const uint32 size, const T & val );
  Array( const uint32 size, const T & val, Allocator& allocator );
  Array( const T array[], const uint32 size );
  Array( const T array[], const uint32 size, Allocator& allocator );
  Array( const Array & rhs );
  Array( const Array & rhs, Allocator& allocator );
  ~Array( void );

    /*----------------
        Modifiers
    ----------------*/
  void PushBack( const T & val );
  template<typename Arg1>
  void EmplaceBack( Arg1&& arg1 );
  template<typename Arg1, typename Arg2>
  void EmplaceBack( Arg1&& arg1, Arg2&& arg2 );
  template<typename Arg1, typename Arg2, typename Arg3>
  void EmplaceBack( Arg1&& arg1, Arg2&& arg2, Arg3&& arg3 );
  inline void PopBack( void );
  void PopBack( uint32 i );

  void Assign( const uint32 size, const T & val );
  void Erase( const uint32 index );
  void Clear( void );

    /*----------------
       Data Access
    ----------------*/
  inline unsigned Size( void ) const { return m_size; }
  inline unsigned Capacity( void ) const { return m_capacity; }
  inline bool Empty( void ) const { return m_size == 0; }
  inline Allocator& GetAllocator( void ) { return m_allocator; }
  inline const Allocator& GetAllocator( void ) const { return m_allocator; }
  inline T* Data( void ) { return m_array; }
  inline const T* Data( void ) const { return m_array; }

    /*----------------
        Operators
    ----------------*/
  Array & operator=( const Array & rhs );
  
  inline T & operator[]( const uint32 index ) { return m_array[index]; }
  inline const T & operator[]( const uint32 index ) const { return m_array[index]; }

private:
  T * m_array;

  unsigned m_size;
  unsigned m_capacity;

  Allocator& m_allocator;

  void Grow(void);
};

/*------------------------------------------------------------------------------------------------

                                    Constructors / Destructors

------------------------------------------------------------------------------------------------*/
template<typename T, typename Allocator>
Array<T, Allocator>::Array( void )
  : m_array(0)
  , m_size(0)
  , m_capacity(0)
  , m_allocator( Array_StandardAllocator )
{
}

template<typename T, typename Allocator>
Array<T, Allocator>::Array( Allocator& allocator )
  : m_array(0)
  , m_size(0)
  , m_capacity(0)
  , m_allocator( allocator )
{
}

template<typename T, typename Allocator>
Array<T, Allocator>::Array( const uint32 capacity )
  : m_array(0)
  , m_size(0)
  , m_capacity(capacity)
  , m_allocator( Array_StandardAllocator )
{
  // Allocate an array and default construct each element
  m_array = (T *)m_allocator.Allocate( sizeof( T ) * capacity );
  for(uint32 i = 0; i < capacity; ++i)
    new (m_array + i) T( );
}

template<typename T, typename Allocator>
Array<T, Allocator>::Array( const uint32 capacity, Allocator& allocator )
  : m_array(0)
  , m_size(0)
  , m_capacity(capacity)
  , m_allocator( allocator )
{
  // Allocate an array and default construct each element
  m_array = (T *)m_allocator.Allocate( sizeof( T ) * capacity );
  for(uint32 i = 0; i < capacity; ++i)
    new (m_array + i) T( );
}

template<typename T, typename Allocator>
Array<T, Allocator>::Array( const uint32 size, const T & val )
  : m_array(0)
  , m_size(size)
  , m_capacity(size)
  , m_allocator( Array_StandardAllocator )
{
  // Allocate an array and default construct each element
  m_array = (T *)m_allocator.Allocate( sizeof( T ) * size );
  for(uint32 i = 0; i < size; ++i)
    new (m_array + i) T( val );
}

template<typename T, typename Allocator>
Array<T, Allocator>::Array( const uint32 size, const T & val, Allocator& allocator )
  : m_array(0)
  , m_size(size)
  , m_capacity(size)
  , m_allocator( allocator )
{
  // Allocate an array and default construct each element
  m_array = (T *)m_allocator.Allocate( sizeof( T ) * size );
  for(uint32 i = 0; i < size; ++i)
    new (m_array + i) T( val );
}

template<typename T, typename Allocator>
Array<T, Allocator>::Array( const T array[], const uint32 size )
  : m_array(0)
  , m_size(size)
  , m_capacity(size)
  , m_allocator( Array_StandardAllocator )
{
  // Allocate an array
  m_array = (T *)m_allocator.Allocate( sizeof( T ) * size );

  // Copy the elements by copy constructor
  for(unsigned i(0); i < size; i++)
  {
    new (m_array + i) T( array[i] );
  }
}

template<typename T, typename Allocator>
Array<T, Allocator>::Array( const T array[], const uint32 size, Allocator& allocator )
  : m_array(0)
  , m_size(size)
  , m_capacity(size)
  , m_allocator( allocator )
{
  // Allocate an array
  m_array = (T *)m_allocator.Allocate( sizeof( T ) * size );

  // Copy the elements by copy constructor
  for(uint32 i(0); i < size; i++)
  {
    new (m_array + i) T( array[i] );
  }
}

template<typename T, typename Allocator>
Array<T, Allocator>::Array( const Array & rhs )
  : m_array(0)
  , m_size(0)
  , m_capacity(0)
  , m_allocator( rhs.m_allocator )
{
  // Use the assignment operator to copy the elements from rhs into this
  *this = rhs;
}

template<typename T, typename Allocator>
Array<T, Allocator>::Array( const Array & rhs, Allocator& allocator )
  : m_array(0)
  , m_size(0)
  , m_capacity(0)
  , m_allocator( allocator )
{
  // Use the assignment operator to copy the elements from rhs into this
  *this = rhs;
}

template<typename T, typename Allocator>
Array<T, Allocator>::~Array( void )
{
  Clear();
}

/*------------------------------------------------------------------------------------------------

                                            Modifiers

------------------------------------------------------------------------------------------------*/
template<typename T, typename Allocator>
void Array<T, Allocator>::PushBack( const T & val )
{
  // Check if we need to expand the underlying array size
  if(m_size >= m_capacity)
  {
    Grow();
  }

  m_array[m_size++] = val;
}

template<typename T, typename Allocator>
template<typename Arg1>
void Array<T, Allocator>::EmplaceBack( Arg1&& arg1 )
{
  // Check if we need to expand the underlying array size
  if(m_size >= m_capacity)
  {
    Grow();
  }

  new (m_array + m_size++) T( std::forward<Arg1>(arg1) );
}

template<typename T, typename Allocator>
template<typename Arg1, typename Arg2>
void Array<T, Allocator>::EmplaceBack( Arg1&& arg1, Arg2&& arg2 )
{
  // Check if we need to expand the underlying array size
  if(m_size >= m_capacity)
  {
    Grow();
  }

  new (m_array + m_size++) T( std::forward<Arg1>(arg1), std::forward<Arg2>(arg2) );
}

template<typename T, typename Allocator>
template<typename Arg1, typename Arg2, typename Arg3>
void Array<T, Allocator>::EmplaceBack( Arg1&& arg1, Arg2&& arg2, Arg3&& arg3 )
{
  // Check if we need to expand the underlying array size
  if(m_size >= m_capacity)
  {
    Grow();
  }

  new (m_array + m_size++) T( std::forward<Arg1>(arg1), std::forward<Arg2>(arg2), std::forward<Arg3>(arg3) );
}

template<typename T, typename Allocator>
void Array<T, Allocator>::Assign( unsigned size, const T & val )
{
  Clear();

  for(unsigned i(0); i < size; i++)
  {
    PushBack(val);
  }
}

template<typename T, typename Allocator>
void Array<T, Allocator>::Erase( const uint32 index )
{
  // Shift all elements to the left
  for(unsigned i(index); i < m_size - 1; ++i)
  {
    m_array[i] = m_array[i + 1];
  }

  --m_size;
}

template<typename T, typename Allocator>
void Array<T, Allocator>::Clear( void )
{
  m_allocator.Free( m_array, m_capacity * sizeof( T ) );
  m_array = NULL;
  m_array = 0;
  m_size = 0;
  m_capacity = 0;
}

template <typename T, typename Allocator>
Array<T, Allocator> & Array<T, Allocator>::operator=( const Array & rhs )
{
  // Check if the current Array is equal to the one being copied
  if(this == &rhs)
  {
    return *this;
  }

  // See if there is enough room in the Array
  if(m_capacity < rhs.m_size)
  {
    Clear();
    m_capacity = rhs.m_size;
    m_array = (T *)m_allocator.Allocate( sizeof( T ) * m_capacity );
  }

  m_size = rhs.m_size;

  // Fill the contents of the Array with the other by default constructor
  for(unsigned i(0); i < m_size; ++i)
  {
    new (m_array + i) T( rhs.m_array[i] );
  }

  // Note -- intentionally not stealing other allocator

  return *this;
}

template<typename T, typename Allocator>
void Array<T, Allocator>::Grow( void )
{
  // Expand the capacity by 2 if it isn't zero
  // The + 10 prevents rapid expansion at smaller capacity sizes.
  m_capacity = m_capacity ? (m_capacity * 2 + 10) : 4;

  // Allocate a new array
  T * array = reinterpret_cast<T*>(m_allocator.Allocate( sizeof( T ) * m_capacity ));

  // Copy the pre-existing elements
  for(uint32 i(0); i < m_size; ++i)
    new (array + i) T( m_array[i] );

  // Free our old data
  m_allocator.Free( m_array, sizeof( T ) * m_size );

  // set the pointer to the new array
  m_array = array;
}

template<typename T, typename Allocator>
void Array<T, Allocator>::PopBack( void )
{
  if(m_size)
  {
    (m_array + --m_size)->~T( );
  }
}

template<typename T, typename Allocator>
void Array<T, Allocator>::PopBack( uint32 i )
{
  while(i)
  {
    assert( m_size ); // i is a larger number of elements than what exists
    (m_array + --m_size)->~T( );
    --i;
  }
}

// Simple wrapper around a static array
template <typename T, unsigned numElements>
class StackArray
{
public:
  StackArray( ) {}
  ~StackArray( ) {}

  inline const T *Data( void ) const { return data; }
  inline T *Data( void ) { return data; }
  inline void clear( void ) { memset( data, 0, sizeBytes ); }

  T& operator[]( unsigned i )
  {
    return data[i];
  }

  const T& operator[]( unsigned i ) const
  {
    return data[i];
  }

  // Compile time calculations
  enum
  {
    size = numElements,
    sizeBytes = numElements * sizeof( T ),
  };

private:
  T data[numElements];
};
