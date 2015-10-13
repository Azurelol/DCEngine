#include "Precompiled.h"
#include "Object.h"

DEFINE_META( Object )
{
  ADD_MEMBER( ID );
  ADD_MEMBER( active );
  ADD_MEMBER( x );
}

Object::Object( int id ) : ID( 0 ), active( true )
{
}

Object::~Object( )
{
}