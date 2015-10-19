/*-------------------qMopey-Header----------------------
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 2/14/2013
File Name     : Hash.h
Purpose       : 
------------------------------------------------------*/

#pragma once

// Honestly cannot remember where I got this!
uint32 Hash( uint32 a );
uint32 Hash( int32 a );
uint32 Hash( const S& string );
/* http://www.azillionmonkeys.com/qed/hash.html */
uint32 Hash( const char *key );

struct EnumHasher
{
  static const size_t bucket_size = 10;
  static const size_t min_buckets = (1 << 10);
  size_t operator()( const char *key );
  bool operator()( const char *lhs, const char *rhs );
};
