#include "Precompiled.h"

DEFINE_META( S )
{
  ADD_MEMBER( data );
  ADD_MEMBER( len );
}

S::S( const char *src ) : data( src )
{
}

S::~S( )
{
}

bool S::IsEqual( const S& rhs ) const
{
  if(strcmp( data, rhs.data ) == 0)
    return true;
  return false;
}

// Untested!
void S::Concat( const S& rhs )
{
  char *temp = new char[rhs.len + len];
  strcpy( temp, data );
  strcpy( temp + len, rhs.data );
}

std::ostream& operator<<( std::ostream &os, S &rhs )
{
  os << rhs.data;
  return os;
}