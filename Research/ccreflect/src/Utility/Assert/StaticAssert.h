/*--------------------qMopey-Header--------------------
<Game Title>  : qMopey
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 2/2/2013
File Name     : StaticAssert.h
Purpose       : Compile time assertion. Can be placed
                outside of function scope within file
                scope. Error message produced can
                look a little weird, but should be
                obvious enough once you realize the
                error came from a compile time assert.
-----------------------------------------------------*/

#pragma once

template <bool>
struct StaticAssert;

template <>
struct StaticAssert<true>
{
  static const int value = true;
};

#define STATIC_ASSERT_PASTE_TOKENS2( a, b ) \
  a##b
#define STATIC_ASSERT_PASTE_TOKENS( a, b ) \
  STATIC_ASSERT_PASTE_TOKENS2( a, b )

#define STATIC_ASSERT( EXPRESSION ) \
  enum { STATIC_ASSERT_PASTE_TOKENS( StaticAssertFailure, __LINE__) = StaticAssert<(EXPRESSION)>::value }
