/*-------------------qMopey-Header----------------------
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 2/12/2013
File Name     : AssertEq.h
Purpose       : Useful assertions for unit testing.
------------------------------------------------------*/

#pragma once

#include <iostream>

// Cause ASSERT_EQ to break (or not) in multiple ways
#define __ASSERT_BREAK( )
//#define __ASSERT_BREAK( ) __debugbreak( )
//#define __ASSERT_BREAK( ) std::terminate( )

#ifdef _DEBUG
#define ASSERT_EQ( EXPECTED, ACTUAL )                                                               \
  do                                                                                                \
  {                                                                                                 \
  auto __ASSERT_EQ_ACTUAL = (ACTUAL);                                                               \
  auto __ASSERT_EQ_EXPECTED = (EXPECTED);                                                           \
  if (__ASSERT_EQ_ACTUAL != __ASSERT_EQ_EXPECTED)                                                   \
    {                                                                                               \
    std::cout << "EXPECTED: " #ACTUAL " == " #EXPECTED << std::endl;                                \
    std::cout << "     GOT: " << __ASSERT_EQ_ACTUAL << " != " << __ASSERT_EQ_EXPECTED << ": FAIL!"; \
    std::cout << std::endl << std::endl;                                                            \
    __ASSERT_BREAK( );                                                                              \
    }                                                                                               \
  } while(false)
#else
// Generate no assembly in release
#define ASSERT_EQ( EXPECTED, ACTUAL ) \
  (void)0
#endif // _DEBUG

#ifdef _DEBUG
#define ASSERT_NQ( EXPECTED, ACTUAL )                                                                 \
  do                                                                                                  \
  {                                                                                                   \
    auto __ASSERT_NQ_ACTUAL = (ACTUAL);                                                               \
    auto __ASSERT_NQ_EXPECTED = (EXPECTED);                                                           \
    if (__ASSERT_NQ_ACTUAL == __ASSERT_NQ_EXPECTED)                                                   \
    {                                                                                                 \
      std::cout << "EXPECTED: " #ACTUAL " == " #EXPECTED << std::endl;                                \
      std::cout << "     GOT: " << __ASSERT_NQ_ACTUAL << " == " << __ASSERT_NQ_EXPECTED << ": FAIL!"; \
      std::cout << std::endl << std::endl;                                                            \
      __ASSERT_BREAK( );                                                                              \
    }                                                                                                 \
  } while(false)
#else
// Generate no assembly in release
#define ASSERT_NQ( EXPECTED, ACTUAL ) \
  (void)0
#endif // _DEBUG

#define DEFINE_TEST( NUM, ... )                                               \
  void test##NUM( void )                                                      \
  {                                                                           \
    std::cout << "Begin test # " << #NUM << ": " << __VA_ARGS__ << std::endl; \
    PrintDotLine( );

inline void PrintDotLine( void )
{
  std::cout << "--------\n";
}
