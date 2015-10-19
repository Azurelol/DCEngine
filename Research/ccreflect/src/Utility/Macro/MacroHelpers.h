/*-------------------qMopey-Header----------------------
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 2/12/2013
File Name     : Macrohelpers.h
Purpose       : 
------------------------------------------------------*/

#pragma once

// Just a more readable version of typecasting
#define RCAST( TYPE, VAL ) \
  reinterpret_cast<TYPE *>(VAL)

// Add a pointer by an integral offset (bytes)
#define PTR_ADD( PTR, OFFSET ) \
  (reinterpret_cast<char *>(PTR) + (OFFSET))

// Subtract a pointer by an integral offset (bytes)
#define PTR_SUB( PTR, OFFSET ) \
  (reinterpret_cast<char *>(PTR) - (OFFSET))

// Find the offset of a member within a class or struct
#define OFFSET_OF( TYPE, MEMBER ) \
  ((uint32)(&reinterpret_cast<TypeToPtr<RemPtr<TYPE>::type>::type>(NULL)->MEMBER))

// Get the size of a member within a class or struct
#define SIZEOF_MEMBER( TYPE, MEMBER ) \
  sizeof( reinterpret_cast<TypeToPtr<RemPtr<TYPE>::type>::type>(NULL)->MEMBER )

// Interal usage
#define __STRINGIZE_2( _ ) #_
#define __STRINGIZE( _ ) __STRINGIZE_2( _ )

// Don't forget to use END_DISABLE
#define DISABLE_WARNING( WARN_ID ) \
  __pragma( warning( push ) ) \
  __pragma( warning( disable : WARN_ID ) )

// Disables a warning without the push call
#define PERM_DSIABLE_WARNING( WARN_ID ) \
  __pragma( warning( disable : WARN_ID ) )


#define END_DISABLE( ) \
  __pragma( warning( pop ) )

#define PASTE( _, __ )  _##__
#define NAME_GENERATOR_INTERNAL( _ ) PASTE( GENERATED_NAME, _ )
#define NAME_GENERATOR( ) NAME_GENERATOR_INTERNAL( __COUNTER__ )

// Convenient macro for getting the size in elements of a stack array
#define numElemofArray( ARRAY ) \
  sizeof( ARRAY ) / sizeof( ARRAY[0] )
