/*-------------------qMopey-Header----------------------
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 2/16/2013
File Name     : File.h
Purpose       : 
------------------------------------------------------*/

#pragma once

#include <cstdio>
#include "../String/S.h"

class Path;

namespace FileAccess
{
  enum FA // FileAccess type
  {
    Read,  // Read only
    Write, // Clear file contents
    Append // Write to end of file
  };
};

class File
{
public:
    /*----------------
       Ctors/Dtors
    ----------------*/
  File( );
  File( const Path& path, FileAccess::FA access_type );
  File( FILE *file, const char *file_name );
  ~File( );

    /*----------------
        Modifiers
    ----------------*/
  void Open( const Path& path, FileAccess::FA access_type );
  void Open( const char *name, FileAccess::FA access_type );
  void Close( void ); // Close file pointer
  void clear( void ); // Close file pointer (if open) and clear file name
  void Write( const char *format, ... );
  void Read( const char *format, ... );
  S GetLine( void );        // Retrieve one line delimited by '\n'
  S GetLine( char delim );  // Retrieve one line delimited by delim
  S GetLine( const char *delims );  // Retrieve one line delimited by chars within delims

    /*----------------
        Operations
    ----------------*/
  // Copy all contents from file pointer onward
  void CopyTo( File& file );

  // Copy a number of bytes from file pointer onward
  void CopyTo( File& file, int32 bytes );

  // Copy all contents from file pointer onward
  void CopyFrom( File& file );
  
  // Copy a number of bytes from file pointer onward
  void CopyFrom( File& file, int32 bytes );

  // The following modify the file pointer with fseek
  void SeekFromBeginOfFile( int32 offset );
  void SeekFromEndOfFile( int32 offset );
  void SeekByOffset( int32 offset );

    /*----------------
       Data Access
    ----------------*/
  bool Validate( void ) const;
  const char *Name( void ) const;

private:
  FILE *fp; // Pointer to file
  S m_name;   // Name of file opened, including path
};

extern File STDOUT;
