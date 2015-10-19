/*-------------------qMopey-Header----------------------
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 2/16/2013
File Name     : ErrorLog.cpp
Purpose       : 
------------------------------------------------------*/

#include "Precompiled.h"
#include <io.h>
#include <fcntl.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

void CreateDebugConsole( void )
{
  AllocConsole( );
  HANDLE handle_out = GetStdHandle( STD_OUTPUT_HANDLE );
  int hCrt = _open_osfhandle( (long) handle_out, _O_TEXT );
  FILE* hf_out = _fdopen( hCrt, "w" );
  setvbuf( hf_out, NULL, _IONBF, 1 );
  *stdout = *hf_out;
}

void ErrorHandler( bool box, const char *exp, const char *file, int line, const S& msg, ... )
{
  const int BufferSize = 2048;
  char formatted_msg[BufferSize];

  //Print out the file and line in visual studio format so the error can be
  //double clicked in the output window file(line) : error
  int offset = sprintf_s( formatted_msg, "%s(%d) : ", file, line );	
  if(msg != NULL)
  {	
    va_list args;
    va_start(args, msg);
    vsnprintf_s( formatted_msg + offset, BufferSize - offset, _TRUNCATE , msg.c_str( ), args );
    va_end(args);
  }
  else
    strcpy_s( formatted_msg + offset, BufferSize - offset, "No Error Message" );

  // Output to visual studio output so can double click on error
  OutputDebugStringA( formatted_msg );
  OutputDebugStringA( "\n" );

  //Display a message box to the user
  if(box)
    MessageBoxA( NULL, formatted_msg, "Error!", 0 );
}

void ErrorHandler( bool box, const char *exp, const char *file, int line, const char *msg, ... )
{
  const int BufferSize = 2048;
  char formatted_msg[BufferSize];

  //Print out the file and line in visual studio format so the error can be
  //double clicked in the output window file(line) : error
  int offset = sprintf_s( formatted_msg, "%s(%d) : ", file, line );	
  if(msg != NULL)
  {	
    va_list args;
    va_start(args, msg);
    vsnprintf_s( formatted_msg + offset, BufferSize - offset, _TRUNCATE , msg, args );
    va_end(args);
  }
  else
    strcpy_s( formatted_msg + offset, BufferSize - offset, "No Error Message" );

  // Output to visual studio output so can double click on error
  OutputDebugStringA( formatted_msg );
  OutputDebugStringA( "\n" );

  //Display a message box to the user
  if(box)
    MessageBoxA( NULL, formatted_msg, "Error!", 0 );
}
