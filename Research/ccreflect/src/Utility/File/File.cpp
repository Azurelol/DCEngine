/*-------------------qMopey-Header----------------------
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 2/16/2013
File Name     : Path.cpp
Purpose       : 
------------------------------------------------------*/

#include "Precompiled.h"

TODO( "Custom i/o with Win32 API instead of C lib." )

static const uint32 BUF_LEN = 4096;

File STDOUT( stdout, "stdout" );

// Super hacky thing found at: http://mrpt.googlecode.com/svn-history/r2406/trunk/libs/base/src/system/os.cpp
// The original code from this link actually had some annoying bugs I had to fix myself.
int32 vfscanf( FILE *file, const char *format, va_list args )
{
  uint32 count = 0;
  const char *p = format;

  for(;;)
  {
    char c = *(p++);
    if (c == 0)
      break;

    // Count number of specifiers to pass to fscanf
    if (c == '%' && *p != '%')
      ++count;
  }

  if (count <= 0)
    return 0;

  int ret_val;

  // copy stack pointer
  _asm
  {
    mov esi, esp;
  }

  const uint32 param_size = sizeof( void * );

  // push variable parameters pointers on stack
  for (int i = ((int)count) - 1; i >= 0; --i)
  {
    STATIC_ASSERT( param_size == 4 ); // The 4 a few lines down must be
                                      // modified to match the size of param_size
    _asm
    {
      mov eax, dword ptr[i];
      mov ecx, dword ptr[args];
      mov edx, dword ptr[ecx + eax * 4]; // 4 as in the size of void *
      push edx;
    }
  }

  int32 stackAdvance = (int32)((2 + count) * param_size);
  
  DISABLE_WARNING( 4996 )
  _asm
  {
    // now push on the fixed params
    mov eax, dword ptr [format];
    push eax;
    mov eax, dword ptr [file];
    push eax;

    // call fscanf, and move the result into ret_val
    call dword ptr [fscanf];
    mov ret_val, eax;

    // restore stack pointer
    mov eax, dword ptr[stackAdvance];
    add esp, eax;
    //mov esp, esi;
  }
  END_DISABLE( )

  return ret_val;
}

/*------------------------------------------------------------------------------------------------

                                    Constructors / Destructors

------------------------------------------------------------------------------------------------*/
File::File( ) : fp( NULL ), m_name( "" )
{
}

File::File( const Path& path, FileAccess::FA access_type ) : m_name( path.Get( ) )
{
  Open( path, access_type );
}

File::File( FILE *file, const char *file_name ) : fp( file ), m_name( file_name )
{
}

File::~File( )
{
  clear( );
}

/*------------------------------------------------------------------------------------------------

                                            Modifiers

------------------------------------------------------------------------------------------------*/
DISABLE_WARNING( 4996 )
void File::Open( const Path& path, FileAccess::FA access_type )
{
  switch(access_type)
  {
  case FileAccess::Read:
    fp = fopen( path.c_str( ), "r" );
    break;
  case FileAccess::Write:
    fp = fopen( path.c_str( ), "w" );
    break;
  case FileAccess::Append:
    fp = fopen( path.c_str( ), "a" );
    break;
  }
}

void File::Open( const char *name, FileAccess::FA access_type )
{
  switch(access_type)
  {
  case FileAccess::Read:
    fp = fopen( name, "r" );
    break;
  case FileAccess::Write:
    fp = fopen( name, "w" );
    break;
  case FileAccess::Append:
    fp = fopen( name, "a" );
    break;
  }
  m_name = name;
}
END_DISABLE( )

// Close file pointer
void File::Close( void )
{
  if(fp)
    fclose( fp );
  m_name = "";
}

// Close file pointer (if open) and clear file name
void File::clear( void )
{
  Close( );
  m_name = "";
}

void File::Write( const char *format, ... )
{
  va_list args;
  va_start( args, format );
  vfprintf( fp, format, args );
  va_end(args);
}

void File::Read( const char *format, ... )
{
  va_list args;
  va_start( args, format );
  vfscanf( fp, format, args );
  va_end(args);
}

S File::GetLine( void )
{
  S temp( 8, "" );

  uint32 i = 0;
  char c;

  for(;;)
  {
    c = static_cast<char>(fgetc( fp ));
    if(feof( fp ))
      break;
    else if(c == '\n')
      break;
    *(temp.c_str( ) + i) = c;
    ++i;
    if(i == temp.size( ) - 1)
    {
      *(temp.c_str( ) + i) = 0; // temporary NULL for strcpy within grow
      temp.Grow( );
    }
  }

  *(temp.c_str( ) + i) = 0;
  temp.Sync( );
  return std::move( temp );
}

S File::GetLine( char delim )
{
  S temp( 8, "" );

  uint32 i = 0;
  char c;

  for(;;)
  {
    c = static_cast<char>(fgetc( fp ));
    if(feof( fp ))
      break;
    else if(c == delim)
      break;
    *(temp.c_str( ) + i) = c;
    ++i;
    if(i == temp.size( ) - 1)
    {
      *(temp.c_str( ) + i) = 0; // temporary NULL for strcpy within grow
      temp.Grow( );
    }
  }

  *(temp.c_str( ) + i) = 0;
  temp.Sync( );
  return std::move( temp );
}

S File::GetLine( const char *delims )
{
  S temp( 8, "" );

  uint32 i = 0;
  char c;

  for(;;)
  {
    c = static_cast<char>(fgetc( fp ));
    if(feof( fp ))
      break;
    for(const char *temp_delim = delims; temp_delim; ++temp_delim)
      if(c == *temp_delim)
        break;
    *(temp.c_str( ) + i) = c;
    ++i;
    if(i == temp.size( ) - 1)
    {
      *(temp.c_str( ) + i) = 0; // temporary NULL for strcpy within grow
      temp.Grow( );
    }
  }

  *(temp.c_str( ) + i) = 0;
  temp.Sync( );
  return std::move( temp );
}

/*------------------------------------------------------------------------------------------------

                                            Operations

------------------------------------------------------------------------------------------------*/
// Copy all contents from file pointer onward
void File::CopyTo( File& file )
{
  char buffer[BUF_LEN];

  for(;;)
  {
    uint32 bytes_read = fread( buffer, 1, BUF_LEN, fp );
         
    if(!bytes_read)
      break;

    fwrite( buffer, 1, bytes_read, file.fp );
  }
}

// Copy a number of bytes from file pointer onward
void File::CopyTo( File& file, int32 bytes )
{
  char buffer[BUF_LEN];

  for(;;)
  {
    uint32 bytes_read = fread( buffer, 1, BUF_LEN, fp );
         
    if(!bytes_read)
      break;

    bytes -= bytes_read;

    // Ensure we only read the amount of bytes necessary
    if(bytes < 0)
    {
      bytes_read += bytes;
      fwrite( buffer, 1, bytes_read, file.fp );
      return;
    }

    fwrite( buffer, 1, bytes_read, file.fp );
  }
}

void File::CopyFrom( File& file )
{
  char buffer[BUF_LEN];

  for(;;)
  {
    uint32 bytes_read = fread( buffer, 1, BUF_LEN, file.fp );
         
    if(!bytes_read)
      break;

    fwrite( buffer, 1, bytes_read, fp );
  }
}

void File::CopyFrom( File& file, int32 bytes )
{
  char buffer[BUF_LEN];

  for(;;)
  {
    uint32 bytes_read = fread( buffer, 1, BUF_LEN, file.fp );
         
    if(!bytes_read)
      break;

    bytes -= bytes_read;

    // Ensure we only read the amount of bytes necessary
    if(bytes < 0)
    {
      bytes_read += bytes;
      fwrite( buffer, 1, bytes_read, fp );
      return;
    }

    fwrite( buffer, 1, bytes_read, fp );
  }
}

void File::SeekFromBeginOfFile( int32 offset )
{
  fseek( fp, offset, SEEK_SET );
}

void File::SeekFromEndOfFile( int32 offset )
{
  fseek( fp, offset, SEEK_END );
}

void File::SeekByOffset( int32 offset )
{
  fseek( fp, offset, SEEK_CUR );
}


/*------------------------------------------------------------------------------------------------

                                           Data Access

------------------------------------------------------------------------------------------------*/
bool File::Validate( void ) const
{
  return fp && m_name != "" && !feof( fp ) ? true : false;
}

const char *File::Name( void ) const
{
  return m_name.c_str( );
}
