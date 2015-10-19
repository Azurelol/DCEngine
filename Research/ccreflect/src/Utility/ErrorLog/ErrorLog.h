/*-------------------qMopey-Header----------------------
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 2/16/2013
File Name     : ErrorLog.h
Purpose       : 
------------------------------------------------------*/

#pragma once

// Define the debug break using the MS specific or general using assembly interrupt 3
#ifdef _MSC_VER
#define DEBUG_BREAK( ) \
  __debugbreak( )
#else
#define DEBUG_BREAK do { __asm int 3 } while(0)
#endif

class S;

void CreateDebugConsole( void );
void ErrorHandler( bool box, const char *exp, const char *file, int line, const char *msg, ... );
void ErrorHandler( bool box, const char *exp, const char *file, int line, const S& msg, ... );

// Output a note during compilation. Can double click on NOTE to jump to in-file.
#define NOTE( MESSAGE ) \
  __pragma( message (__FILE__ "(" __STRINGIZE(__LINE__) ") : NOTE [ " MESSAGE " ]") )

// Output a warning during compilation. Can double click on WARN to jump to in-file.
#define WARN( MESSAGE ) \
  __pragma( message (__FILE__ "(" __STRINGIZE(__LINE__) ") : WARNING [ " MESSAGE " ]") )

// Output a todo during compilation. Can double click on TODO to jump to in-file.
#define TODO( MESSAGE ) \
  __pragma( message (__FILE__ "(" __STRINGIZE(__LINE__) ") : TODO [ " MESSAGE " ]") )

// do while information
// http://cnicholson.net/2009/02/stupid-c-tricks-adventures-in-assert/
#ifdef _DEBUG

// Displays an error box with a message if the expression returns false
#define ErrorIf( EXP, ... )                                     \
  do                                                            \
  {                                                             \
    if(EXP)                                                     \
    {                                                           \
      ErrorHandler( 1, #EXP, __FILE__, __LINE__, __VA_ARGS__ ); \
      DEBUG_BREAK( );                                           \
    }                                                           \
  } while(0)

// Displays an error box with provided expression
#define Error( ... )                                            \
  do                                                            \
  {                                                             \
    ErrorHandler( 1, 0, __FILE__, __LINE__, __VA_ARGS__ );      \
    DEBUG_BREAK( );                                             \
  } while(0)

// Logs an error to debug output if expression is false
#define ErrorIfLog( EXP, ... )                                  \
  do                                                            \
  {                                                             \
    if(EXP)                                                     \
    {                                                           \
      ErrorHandler( 0, #EXP, __FILE__, __LINE__, __VA_ARGS__ ); \
    }                                                           \
  } while(0)

// Logs an error to debug output
#define ErrorLog( ... )                                         \
  do                                                            \
  {                                                             \
    ErrorHandler( 0, 0, __FILE__, __LINE__, __VA_ARGS__ );      \
  } while(0)

#else
#define ErrorIf( ... )    void(0)
#define Error( ... )      void(0)
#define ErrorIfLog( ... ) void(0)
#define ErrorLog( ... )   void(0)
#endif // _DEBUG
