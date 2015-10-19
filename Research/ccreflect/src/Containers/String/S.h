/*-------------------qMopey-Header----------------------
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 2/14/2013
File Name     : S.h
Purpose       : Refcounted, fast allocated, high
                performance, cache hashed, general
                purpose std::string replacement.

                Rvalue constructor and assignment are
                present.
------------------------------------------------------*/

#pragma once

#include <iosfwd>

// TODO( "Push back optimization." )

struct S_impl;

// String class
class S
{
public:
    /*----------------
       Ctors/Dtors
    ----------------*/
  // Allocate space with whichever is larger: size or len + 1 of s
  explicit S( uint32 size, const char *s );
  explicit S( const char *s = "" );
  S( const S& rhs );
  S( S&& rhs );
  ~S( );

    /*----------------
        Modifiers
    ----------------*/
  S& operator=( const S& rhs );
  S& operator=( const char *rhs );
  S& operator=( S&& rhs );
  S& operator+=( const char *rhs );
  S& operator+=( S& rhs );

    /*----------------
        Operations
    ----------------*/
  S operator+( const S& rhs ) const;
  S operator+( const char *rhs ) const;
  friend S operator+( const char *lhs, const S& rhs );
  void Swap( S& rhs ); // Very fast swap compared to = operator
  void Set( int32 a, int32 b, char c ); // std::memset to c
  void Erase( int32 a, int32 b ); // std::memcpy b through NUL to a's index
  int32 find_first_of( char c ); // -1 if not found
  int32 find_last_of( char c ); // -1 if not found
  void Sync( void ); // Recomputes len, size and hash - use if manually modified the internal string
  void Grow( void );
  void Remove( const char *chars );

    /*----------------
       Data Access
    ----------------*/
  char& operator[]( int32 i );
  char operator[]( int32 i ) const;
  uint32 len( void ) const;
  uint32 size( void ) const;
  char *c_str( void );
  const char *c_str( void ) const;
  uint32 GetHash( void ) const;
  bool operator==( const S& rhs ) const; // hash comparison
  bool operator!=( const S& rhs ) const; // hash comparison
  bool operator==( const char *rhs ) const; // hash comparison
  bool operator!=( const char *rhs ) const; // hash comparison
  friend bool operator==( const char *lhs, const S& rhs ); // hash comparison
  friend bool operator!=( const char *lhs, const S& rhs ); // hash comparison
  friend std::ostream& operator<<( std::ostream& lhs, const S& rhs );

private:
  // pimpl idiom for 4 byte size
  S_impl *impl;

  // Private constructor for + operator
  S( S_impl *impl );

  friend class StringBuilder;
};
