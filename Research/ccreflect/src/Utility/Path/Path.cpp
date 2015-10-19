/*-------------------qMopey-Header----------------------
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 2/16/2013
File Name     : Path.cpp
Purpose       : 
------------------------------------------------------*/

#include "Precompiled.h"
#include <Windows.h>
#include <Shlwapi.h>
#pragma comment( lib, "Shlwapi" )

/*------------------------------------------------------------------------------------------------

                                    Constructors / Destructors

------------------------------------------------------------------------------------------------*/
Path::Path( ) : path( "" )
{
}

Path::Path( const char *path_ ) : path( path_ )
{
}

/*------------------------------------------------------------------------------------------------

                                            Modifiers

------------------------------------------------------------------------------------------------*/
// Originally created by Steven George for his Directory Watcher
void Path::WorkingDirectory( void )
{
  // Initialize a string to save the working directory in
  S workingDirectory( MAX_PATH, "" );

  // Fill the string
  GetCurrentDirectoryA( MAX_PATH, workingDirectory.c_str( ) );

  // Recompute len, size and hash of workingDirectory
  workingDirectory.Sync( );

  path = std::move( workingDirectory );
}

// Adds or removes a file extension from the path
void Path::AddOrChangeExt( const char *ext )
{
  BOOL successful = PathAddExtensionA( path.c_str( ), ext );
  ErrorIf( !successful, "Extension not added properly." );
}

// Reduce a path with ./ or ../
void Path::Simplify( void )
{
  char temp_buf[MAX_PATH];
  BOOL successful = PathCanonicalizeA( temp_buf, path.c_str( ) );
  path = temp_buf;
  ErrorIf( !successful, "Error during simplification." );
}

// Squeeze the path into a smaller space by inserting ... (for visual display only)
void Path::Compact( uint32 amount )
{
  BOOL successful = PathCompactPathA( NULL, path.c_str( ), amount );
  ErrorIf( !successful, "Error, the path could not be compacted to this size." );
}

// Lowers the case of all letters in the path
void Path::ToLower( void )
{
  PathMakePrettyA( &path[0] );
}

// Removes the extension, if there's an extension on the end of the path
void Path::RemoveExt( void )
{
  PathRemoveExtensionA( path.c_str( ) );
}

// Removes both filename and extension from the end of the path
bool Path::RemoveNameAndExt( void )
{
 return PathRemoveFileSpecA( path.c_str( ) ) ? true : false;
}

  // Change the directory, similar to the cd command, jumps process to the directory this path
  // object represents.
void Path::ChangeDirectory( void )
{
  BOOL successful = SetCurrentDirectoryA( path.c_str( ) );
  ErrorIf( !successful, "Something bad happened during the SetCurrentDirectory call." );
}

/*------------------------------------------------------------------------------------------------

                                            Operations

------------------------------------------------------------------------------------------------*/
// Constructs a folder of the specified name at the path
void Path::CreateFolder( void ) const
{
  BOOL f = CreateDirectoryA( path.c_str( ), NULL );
  ErrorIf( f == ERROR_ALREADY_EXISTS, "The directory already exists." );
  ErrorIf( f == ERROR_PATH_NOT_FOUND, "The path cannot be created. One or more intermediate directories are not found." );
  ErrorIf( !f, "The path could not be created. An error occurred." );
}

// Deletes a folder of the specified name at end of the path
void Path::DeleteFolder( void ) const
{
 BOOL successful = RemoveDirectoryA( path.c_str( ) );
 ErrorIf( !successful, "Could not delete directory for some reason." );
}

// Clears a number of levels of directories equal to num. Stops if entire path is cleared.
void Path::pop( uint32 num )
{
  for(uint32 i = 0; i < num; ++i)
  {
    int j = path.find_last_of( '\\' );
    if(j != -1)
      path.Set( j, path.len( ), 0 );
    else
    {
      path = "";
      return;
    }
  }
}

/*------------------------------------------------------------------------------------------------

                                           Data Access

------------------------------------------------------------------------------------------------*/
S& Path::Get( void )
{
  return path;
}

const S& Path::Get( void ) const
{
  return path;
}

char *Path::c_str( void )
{
  return path.c_str( );
}

const char *Path::c_str( void ) const
{
  return path.c_str( );
}

// Returns a vector of strings representing filenames of a specific directory.
// Notes  : The directory name is not appended to the filename.
std::vector<S> Path::GetExt( const char *ext ) const
{
  // Use a few functions from MSDN to cycle through the files of a folder
  WIN32_FIND_DATAA fileData = { 0 };
  HANDLE handle;
  S searchString( S( "/*" ) + ext );

  S entireSearchString = path + searchString;

  handle = FindFirstFileA( entireSearchString.c_str( ), &fileData );

  std::vector<S> files;
  do
  {
    // For each file found, load it
    files.push_back( S( fileData.cFileName ) );
  }
  while(FindNextFileA( handle, &fileData ) != 0);

  return files;
}

// Returns a common prefix between two paths. NULL string if nothing matches.
S Path::GetCommon( const Path& lhs, const Path& rhs )
{
  char buffer[MAX_PATH];

  buffer[0] = 0;
  PathCommonPrefixA( lhs.path.c_str( ), rhs.path.c_str( ), buffer );

  return S( buffer );
}

bool Path::Validate( void ) const
{
  return PathFileExistsA( path.c_str( ) ) ? true : false;
}

// Verifies that a path is a valid directory.
bool Path::ValidateDirectory( void ) const
{
  return PathIsDirectoryA( path.c_str( ) ) ? true : false;
}

// Determines whether a specified path is an empty directory
bool Path::DirectoryIsEmpty( void ) const
{
  return PathIsDirectoryEmptyA( path.c_str( ) ) ? true : false;
}

// Searches a path and determines if it is relative.
bool Path::IsRelative( void ) const
{
  return PathIsRelativeA( path.c_str( ) ) ? true : false;
}
