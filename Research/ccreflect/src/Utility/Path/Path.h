/*-------------------qMopey-Header----------------------
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 2/16/2013
File Name     : Path.h
Purpose       : 
------------------------------------------------------*/

#pragma once

#include <vector>

class Path
{
public:
    /*----------------
       Ctors/Dtors
    ----------------*/
  Path( );
  Path( const char *path_ );
  
    /*----------------
        Modifiers
    ----------------*/
  void WorkingDirectory( void );

  // Adds or removes a file extension from the path
  void AddOrChangeExt( const char *ext );

  // Reduce a path with ./ or ../
  void Simplify( void );

  // Squeeze the path into a smaller space by inserting ... (for visual display only)
  void Compact( uint32 amount );
  
  // Lowers the case of all letters in the path
  void ToLower( void );

  // Removes the extension, if there's an extension on the end of the path
  void RemoveExt( void );

  // Removes both filename and extension from the end of the path
  bool RemoveNameAndExt( void );

  // Change the directory, similar to the cd command, jumps process to the directory this path
  // object represents.
  void ChangeDirectory( void );
 
    /*----------------
        Operations
    ----------------*/
  // Constructs a folder of the specified name at end of the path
  void CreateFolder( void ) const;
  
  // Deletes a folder of the specified name at end of the path
  void DeleteFolder( void ) const;

  // Clears a number of levels of directories equal to num. Stops if entire path is cleared.
  void pop( uint32 num );

    /*----------------
       Data Access
    ----------------*/
  S& Get( void );
  const S& Get( void ) const;
  char *c_str( void );
  const char *c_str( void ) const;

  // Returns a vector of strings representing filenames of a specific directory.
  // Notes  : The directory name is not appended to the filename.
  std::vector<S> GetExt( const char *ext ) const;
  
  // Returns a common prefix between two paths. NULL string if nothing matches.
  S GetCommon( const Path& lhs, const Path& rhs ); 

  // Determines whether a path to a file or folder is valid.
  bool Validate( void ) const;

  // Verifies that a path is a valid directory.
  bool ValidateDirectory( void ) const;

  // Determines whether a specified path is an empty directory
  bool DirectoryIsEmpty( void ) const;

  // Searches a path and determines if it is relative.
  bool IsRelative( void ) const;

private:
  S path;
};
