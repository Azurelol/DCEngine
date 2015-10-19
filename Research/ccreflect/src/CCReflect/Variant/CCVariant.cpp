/*--------------------qMopey-Header--------------------
<Game Title>  : qMopey
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 9/15/2012
File Name     : CCVariant.cpp
Purpose       : Implementation of the Variant class
-----------------------------------------------------*/

#include "Precompiled.h"

Variant::Variant( const Variant& rhs )
{
  meta = rhs.GetMeta( );
  if(rhs.GetData( ))
    meta->NewCopy( &data, rhs.data );
}

Variant::Variant( const RefVariant& rhs )
{
  meta = rhs.GetMeta( );
  if(rhs.GetData( ))
    meta->NewCopy( &data, rhs.data );
}

Variant::Variant( Variant&& rhs ) : VariantBase( rhs.meta, rhs.data )
{
  rhs.data = NULL;
  rhs.meta = NULL;
}

Variant::~Variant( )
{
  if(meta && data)
  {
    meta->Delete( data );
    meta = NULL;
  }
}

Variant& Variant::operator=( const Variant& rhs )
{
  if(this == &rhs)
    return *this;

  if(meta)
  {
    if(meta == rhs.meta)
    {
      // If MetaData is same, freely copy from data from rhs
      // As this means the data is the same type
      meta->Copy( data, rhs.data );
    }
    else
    {
      // Delete current data to make room for rhs's data
      // If the data in rhs isn't NULL, then make a new
      // copy of it.
      meta->Delete( data );
      meta = rhs.GetMeta( );
      if(meta)
        meta->NewCopy( &data, &rhs.data );
    }
  }

  return *this;
}

Variant& Variant::operator=( Variant&& rhs )
{
  data = rhs.data;
  rhs.data = NULL;
  meta = rhs.meta;
  rhs.meta = NULL;
  return *this;
}

bool Variant::operator==( const Variant& rhs ) const
{
  return this == &rhs;
}
  
bool Variant::operator!=( const Variant& rhs ) const
{
  return !(*this == rhs);
}

void Variant::ToLua( lua_State *lua ) const
{
  RefVariant var( *this );
  meta->ToLua( lua, var );
}

void Variant::Serialize( File& file ) const
{
  meta->Serialize( file, *this );
}

RefVariant Variant::Deserialize( File& file ) const
{
  return meta->Deserialize( file );
}
