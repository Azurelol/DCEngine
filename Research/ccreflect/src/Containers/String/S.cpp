/*-------------------qMopey-Header----------------------
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 2/14/2013
File Name     : S.cpp
Purpose       : 
------------------------------------------------------*/

#include "Precompiled.h"
#include <cstring> // strlen, memcpy

TODO( "Same string check like in the rvalue assignment." )

struct S_impl
{
  const char *string;
  uint32 size;
  uint32 len;
  uint32 hash;
  uint32 *ref_count;
  static BlockAllocator& Allocator( void )
  {
    static BlockAllocator instance;
    return instance;
  }
private:
  static BlockAllocator allocator;
};

/*------------------------------------------------------------------------------------------------

                                    Constructors / Destructors

------------------------------------------------------------------------------------------------*/
S::S( uint32 size, const char *s )
{
  uint32 len = std::strlen( s );
  impl = RCAST( S_impl, S_impl::Allocator( ).Allocate( sizeof( S_impl ) ) );
  impl->len = len;
  impl->size = std::max( len + 1, size );
  impl->string = RCAST( const char, S_impl::Allocator( ).Allocate( size ) );
  std::memcpy( (void *)impl->string, s, len + 1 );
  impl->ref_count = RCAST( uint32, S_impl::Allocator( ).Allocate( sizeof( uint32 ) ) );
  *impl->ref_count = 1;
  impl->hash = Hash( impl->string );
}

S::S( const char *s )
{
  uint32 len = std::strlen( s );
  impl = RCAST( S_impl, S_impl::Allocator( ).Allocate( sizeof( S_impl ) ) );
  impl->string = RCAST( const char, S_impl::Allocator( ).Allocate( len + 1 ) );
  impl->len = len;
  impl->size = len + 1;
  std::memcpy( (void *)(impl->string), s, impl->size );
  impl->ref_count = RCAST( uint32, S_impl::Allocator( ).Allocate( sizeof( uint32 ) ) );
  *impl->ref_count = 1;
  impl->hash = Hash( impl->string );
}

S::S( const S& rhs ) : impl( rhs.impl )
{
  ++*impl->ref_count;
}

S::S( S&& rhs ) : impl( rhs.impl )
{
  rhs.impl = NULL;
}

// Private constructor for + operator
S::S( S_impl *impl_ ) : impl( impl_ )
{
}

S::~S( )
{
  if(!impl)
    return;

  if(--*impl->ref_count)
    return;

  S_impl::Allocator( ).Free( (void *)impl->string, impl->size );
  S_impl::Allocator( ).Free( impl->ref_count, sizeof( uint32 ) );
  S_impl::Allocator( ).Free( impl, sizeof( S_impl ) );
}

/*------------------------------------------------------------------------------------------------

                                          Modifiers

------------------------------------------------------------------------------------------------*/
S& S::operator=( const S& rhs )
{
  if(impl == rhs.impl)
    return *this;

  if(!(--(*(impl->ref_count))))
  {
    S_impl::Allocator( ).Free( ( void *)impl->string, impl->size );
    S_impl::Allocator( ).Free( impl->ref_count, sizeof( uint32 ) );
    S_impl::Allocator( ).Free( impl, sizeof( S_impl ) );
  }

  impl = rhs.impl;
  ++(*(impl->ref_count));

  return *this;
}

S& S::operator=( const char *rhs )
{
  // Self assignment check
  if(impl->string == rhs)
    return *this;

  // Decrement our refcount, and free the implementation
  // if refcount hits zero
  if(!(--(*(impl->ref_count))))
  {
    S_impl::Allocator( ).Free( ( void *)impl->string, impl->size );
    S_impl::Allocator( ).Free( impl->ref_count, sizeof( uint32 ) );
    S_impl::Allocator( ).Free( impl, sizeof( S_impl ) );
  }

  // Create a new implementation based off of the char *rhs
  uint32 len = std::strlen( rhs );
  impl = RCAST( S_impl, S_impl::Allocator( ).Allocate( sizeof( S_impl ) ) );
  impl->string = RCAST( const char, S_impl::Allocator( ).Allocate( len + 1 ) );
  impl->len = len;
  impl->size = len + 1;
  std::memcpy( (void *)impl->string, rhs, impl->size );
  impl->ref_count = RCAST( uint32, S_impl::Allocator( ).Allocate( sizeof( uint32 ) ) );
  *impl->ref_count = 1;
  impl->hash = Hash( impl->string );

  return *this;
}

S& S::operator=( S&& rhs )
{
  // Self assignment check
  if(this == &rhs)
    return *this;

  // Same string check
  if(impl->hash == rhs.impl->hash)
  {
    if(impl == rhs.impl)
    {
      rhs.impl = NULL;
      return *this;
    }
  }

  // Decrement our refcount, and free the implementation
  // if refcount hits zero
  if(!(--(*(impl->ref_count))))
  {
    S_impl::Allocator( ).Free( ( void *)impl->string, impl->size );
    S_impl::Allocator( ).Free( impl->ref_count, sizeof( uint32 ) );
    S_impl::Allocator( ).Free( impl, sizeof( S_impl ) );
  }

  impl = rhs.impl;
  rhs.impl = NULL;

  return *this;
}

S& S::operator+=( const char *rhs )
{
  // Create a new implementation based off of the char *rhs
  uint32 new_stringlen = std::strlen( rhs );
  uint32 len = new_stringlen + impl->len;

  // Can we reuse our old impl?
  if(!--*impl->ref_count)
  {
    // Set our refcount to 1 since we're reusing our old impl
    *impl->ref_count = 1;

    // Do we have enough space to just copy in the new string?
    if(len + 1 < impl->size)
    {
      char *temp_string = (char *)S_impl::Allocator( ).Allocate( impl->size );
      std::memcpy( (void *)temp_string, impl->string, impl->len );
      std::memcpy( (void *)(temp_string + impl->len), rhs, new_stringlen + 1 );
      impl->string = temp_string;
      impl->len = len;
    }

    // We must reallocate to make room for the new string
    else
    {
      char *old_string = (char *)impl->string;
      uint32 new_size = len + 1;
      impl->string = (char *)S_impl::Allocator( ).Allocate( new_size );
      std::memcpy( (void *)impl->string, old_string, impl->len );
      std::memcpy( (void *)(impl->string + impl->len), rhs, new_stringlen + 1 );
      impl->len += new_stringlen;
      impl->size = new_size;
    }
  }
  
  // We need to make a new impl
  else
  {
    S_impl *old_impl = impl;
    impl = (S_impl *)S_impl::Allocator( ).Allocate( sizeof( S_impl ) );
    impl->size = old_impl->len + new_stringlen + 1;
    impl->string = (char *)S_impl::Allocator( ).Allocate( impl->size );
    *impl->ref_count = 1;
    impl->len = impl->size - 1;
    std::memcpy( (void *)impl->string, old_impl->string, old_impl->len );
    std::memcpy( (void *)(impl->string + old_impl->len), rhs, new_stringlen + 1 );
  }
  
  impl->hash = Hash( impl->string );
  return *this;
}

S& S::operator+=( S& rhs )
{
  // Create a new implementation based off of the char *rhs
  uint32 len = rhs.len( ) + impl->len;

  // Can we reuse our old impl?
  if(!(--(*(impl->ref_count))))
  {
    // Set our refcount to 1 since we're reusing our old impl
    *impl->ref_count = 1;

    // Do we have enough space to just copy in the new string?
    if(len + 1 < impl->size)
    {
      char *temp_string = (char *)S_impl::Allocator( ).Allocate( impl->size );
      std::memcpy( (void *)temp_string, impl->string, impl->len );
      std::memcpy( (void *)(temp_string + impl->len), (void *)rhs.c_str( ), rhs.len( ) + 1 );
      impl->string = temp_string;
      impl->len = len;
    }

    // We must reallocate to make room for the new string
    else
    {
      char *old_string = (char *)impl->string;
      impl->string = (char *)S_impl::Allocator( ).Allocate( len + 1 );
      std::memcpy( (void *)impl->string, old_string, impl->len );
      std::memcpy( (void *)(impl->string + impl->len), (void *)rhs.c_str( ), rhs.len( ) + 1 );
      impl->len += rhs.len( );
      impl->size = len + 1;
    }
  }
  
  // We need to make a new impl
  else
  {
    S_impl *old_impl = impl;
    impl = (S_impl *)S_impl::Allocator( ).Allocate( sizeof( S_impl ) );
    impl->size = len + 1;
    impl->string = (char *)S_impl::Allocator( ).Allocate( impl->size );
    *impl->ref_count = 1;
    impl->len = impl->size - 1;
    std::memcpy( (void *)impl->string, old_impl->string, old_impl->len );
    std::memcpy( (void *)(impl->string + old_impl->len), (void *)rhs.c_str( ), rhs.len( ) + 1 );
  }
  
  impl->hash = Hash( impl->string );
  return *this;
}

/*------------------------------------------------------------------------------------------------

                                            Operations

------------------------------------------------------------------------------------------------*/
S S::operator+( const S& rhs ) const
{
  S_impl *new_impl;
  new_impl = (S_impl *)S_impl::Allocator( ).Allocate( sizeof( S_impl ) );
  new_impl->ref_count = (uint32 *)S_impl::Allocator( ).Allocate( sizeof( uint32 ) );
  uint32 newstring_len = std::strlen( rhs.c_str( ) );
  new_impl->len = newstring_len + impl->len;
  new_impl->size = new_impl->len + 1;
  new_impl->string = RCAST( char, S_impl::Allocator( ).Allocate( new_impl->size ) );
  *new_impl->ref_count = 1;
  
  std::memcpy( (void *)new_impl->string, impl->string, impl->len );
  std::memcpy( (void *)(new_impl->string + impl->len), rhs.c_str( ), newstring_len + 1 );
  new_impl->hash = Hash( new_impl->string );

  return S( new_impl );
}

S S::operator+( const char *rhs ) const
{
  S_impl *new_impl;
  new_impl = (S_impl *)S_impl::Allocator( ).Allocate( sizeof( S_impl ) );
  new_impl->ref_count = (uint32 *)S_impl::Allocator( ).Allocate( sizeof( uint32 ) );
  uint32 newstring_len = std::strlen( rhs );
  new_impl->len = newstring_len + impl->len;
  new_impl->size = new_impl->len + 1;
  new_impl->string = RCAST( char, S_impl::Allocator( ).Allocate( new_impl->size ) );
  *new_impl->ref_count = 1;
  
  std::memcpy( (void *)new_impl->string, impl->string, impl->len );
  std::memcpy( (void *)(new_impl->string + impl->len), rhs, newstring_len + 1 );
  new_impl->hash = Hash( new_impl->string );

  return S( new_impl );
}

S operator+( const char *lhs, const S& rhs )
{
  S_impl *new_impl;
  new_impl = (S_impl *)S_impl::Allocator( ).Allocate( sizeof( S_impl ) );
  new_impl->ref_count = (uint32 *)S_impl::Allocator( ).Allocate( sizeof( uint32 ) );
  uint32 newstring_len = std::strlen( rhs.impl->string );
  new_impl->len = newstring_len + rhs.impl->len;
  new_impl->size = new_impl->len + 1;
  new_impl->string = RCAST( char, S_impl::Allocator( ).Allocate( new_impl->size ) );
  *new_impl->ref_count = 1;
  
  std::memcpy( (void *)new_impl->string, rhs.impl->string, rhs.impl->len );
  std::memcpy( (void *)(new_impl->string + rhs.impl->len), rhs.impl->string, newstring_len + 1 );
  new_impl->hash = Hash( new_impl->string );

  return S( new_impl );
}

// Very fast swap compared to = operator
void S::Swap( S& rhs )
{
  S_impl *temp = impl;
  impl = rhs.impl;
  rhs.impl = temp;
}

// std::memset to 0
void S::Set( int32 a, int32 b, char c )
{
  assert( a >= 0 && a < static_cast<int32>(impl->size) );
  assert( b >= 0 && b < static_cast<int32>(impl->size) );
  assert( a <= b );
  std::memset( const_cast<char *>(impl->string) + a, c, b - a );
  Sync( );
}

void S::Erase( int32 a, int32 b )
{
  assert( a >= 0 && a < static_cast<int32>(impl->size) );
  assert( b >= 0 && b < static_cast<int32>(impl->size) );
  assert( a <= b );

  std::memcpy( const_cast<char *>(impl->string) + a,
               const_cast<char *>(impl->string) + b, impl->len - b + 1 );
  Sync( );
}

// -1 if not found
int32 S::find_first_of( char c )
{
  for(int32 i = 0; i < static_cast<int32>(impl->len); ++i)
  {
    if(impl->string[i] == c)
    {
      return i;
    }
  }

  return -1;
}

// -1 if not found
int32 S::find_last_of( char c )
{
  for(int32 i = impl->len - 1; i >= 0; --i)
  {
    if(impl->string[i] == c)
    {
      return i;
    }
  }

  return -1;
}

// Recomputes len, size and hash - use if manually modified the internal string
void S::Sync( void )
{
  impl->len = strlen( impl->string );
  impl->hash = Hash( impl->string );
}

void S::Grow( void )
{
  uint32 new_size = impl->size - 1 ? ((impl->size - 1) * 2) + 1 : 2;
  char *temp = (char *)S_impl::Allocator( ).Allocate( new_size );
  std::strcpy( temp, impl->string );
  S_impl::Allocator( ).Free( (void *)impl->string, impl->size );
  impl->string = temp;
  impl->size = new_size;
}

TODO( "Optimize to jump more per match found, reduce O( ) complexity." )
void S::Remove( const char *chars )
{
  for(; *chars; ++chars)
  {
    for(const char *t = impl->string; *t;)
    {
      // Found matching character
      if(*t == *chars)
      {
        // Left shift all elements passed the matching char, including the
        // NULL termination
        for(uint32 i = t - impl->string; i < impl->len; ++i)
        {
          const_cast<char *>(impl->string)[i] = impl->string[i + 1];
        }
        --impl->len;
      }
      else
        ++t;
    }
  }

  Sync( );
}

/*------------------------------------------------------------------------------------------------

                                           Data Access

------------------------------------------------------------------------------------------------*/
char& S::operator[]( int32 i )
{
  assert( i >= 0 && i < static_cast<int32>(impl->len) );
  return const_cast<char *>(impl->string)[i];
}

char S::operator[]( int32 i ) const
{
  assert( i >= 0 && i < static_cast<int32>(impl->len) );
  return impl->string[i];
}

uint32 S::len( void ) const
{
  return impl->len;
}

uint32 S::size( void ) const
{
  return impl->size;
}

char *S::c_str( void )
{
  return const_cast<char *>(impl->string);
}

const char *S::c_str( void ) const
{
  return impl->string;
}

uint32 S::GetHash( void ) const
{
  return impl->hash;
}

NOTE( "Uncomment to enable strlen check on top of "
      "hash comparisons." )
bool S::operator==( const S& rhs ) const
{
  return impl->hash == rhs.impl->hash;
  //return impl->hash == rhs.impl->hash && std::strcmp( impl->string, rhs.impl->string ) == 0;
}

bool S::operator!=( const S& rhs ) const
{
  return impl->hash != rhs.impl->hash;
  //return impl->hash != rhs.impl->hash && std::strcmp( impl->string, rhs.impl->string ) != 0;
}

bool S::operator==( const char *rhs ) const
{
  return impl->hash == Hash( rhs );
  //return impl->hash == rhs.impl->hash && std::strcmp( impl->string, rhs.impl->string ) == 0;
}

bool S::operator!=( const char *rhs ) const
{
  return impl->hash != Hash( rhs );
  //return impl->hash == rhs.impl->hash && std::strcmp( impl->string, rhs.impl->string ) == 0;
}

bool operator==( const char *lhs, const S& rhs )
{
  return Hash( lhs ) == rhs.impl->hash;
  //return impl->hash == rhs.impl->hash && std::strcmp( impl->string, rhs.impl->string ) == 0;
}

bool operator!=( const char *lhs, const S& rhs )
{
  return Hash( lhs ) != rhs.impl->hash;
  //return impl->hash != rhs.impl->hash && std::strcmp( impl->string, rhs.impl->string ) != 0;
}

std::ostream& operator<<( std::ostream& lhs, const S& rhs )
{
  lhs << rhs.impl->string;
  return lhs;
}
