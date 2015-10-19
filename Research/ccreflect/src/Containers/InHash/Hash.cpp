/*-------------------qMopey-Header----------------------
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 2/14/2013
File Name     : InHashBase.h
Purpose       : 
------------------------------------------------------*/

#include "Precompiled.h"

/* http://www.azillionmonkeys.com/qed/hash.html */
#undef get16bits
#if (defined(__GNUC__) && defined(__i386__)) || defined(__WATCOMC__) \
  || defined(_MSC_VER) || defined (__BORLANDC__) || defined (__TURBOC__)
#define get16bits(d) (*((const uint16 *) (d)))
#endif

#if !defined (get16bits)
#define get16bits(d) ((((uint32)(((const uint8 *)(d))[1])) << 8)\
                       +(uint32)(((const uint8 *)(d))[0]) )
#endif

// Honestly cannot remember where I got this!
uint32 Hash( uint32 a )
{
   a = (a+0x7ed55d16) + (a<<12);
   a = (a^0xc761c23c) ^ (a>>19);
   a = (a+0x165667b1) + (a<<5);
   a = (a+0xd3a2646c) ^ (a<<9);
   a = (a+0xfd7046c5) + (a<<3);
   a = (a^0xb55a4f09) ^ (a>>16);
   return a;
}

uint32 Hash( int32 a )
{
   a = (int32)((uint32)a+0x7ed55d16) + ((uint32)a<<12);
   a = (int32)((uint32)a^0xc761c23c) ^ ((uint32)a>>19);
   a = (int32)((uint32)a+0x165667b1) + ((uint32)a<<5);
   a = (int32)((uint32)a+0xd3a2646c) ^ ((uint32)a<<9);
   a = (int32)((uint32)a+0xfd7046c5) + ((uint32)a<<3);
   a = (int32)((uint32)a^0xb55a4f09) ^ ((uint32)a>>16);
   return a;
}

uint32 Hash( const S& string )
{
  return string.GetHash( );
}

/* http://www.azillionmonkeys.com/qed/hash.html */
uint32 Hash( const char *key )
{
  int len = std::strlen( key );
  uint32 hash = len;
  uint32 tmp;
  int rem;

    if (len <= 0 || key == NULL) return 0;

    rem = len & 3;
    len >>= 2;

    /* Main loop */
    for (;len > 0; len--) {
        hash  += get16bits (key);
        tmp    = (get16bits (key+2) << 11) ^ hash;
        hash   = (hash << 16) ^ tmp;
        key   += 2*sizeof (uint16);
        hash  += hash >> 11;
    }

    /* Handle end cases */
    switch (rem) {
        case 3: hash += get16bits (key);
                hash ^= hash << 16;
                hash ^= key[sizeof (uint16)] << 18;
                hash += hash >> 11;
                break;
        case 2: hash += get16bits (key);
                hash ^= hash << 11;
                hash += hash >> 17;
                break;
        case 1: hash += *key;
                hash ^= hash << 10;
                hash += hash >> 1;
    }

    /* Force "avalanching" of final 127 bits */
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;

    return hash;
}

size_t EnumHasher::operator()( const char *key )
{
  return (size_t)Hash( key );
}

bool EnumHasher::operator()( const char *lhs, const char *rhs )
{
  return strcmp( lhs, rhs ) == 0;
}
