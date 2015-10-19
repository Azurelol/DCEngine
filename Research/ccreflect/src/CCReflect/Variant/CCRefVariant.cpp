/*--------------------qMopey-Header--------------------
<Game Title>  : qMopey
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 9/15/2012
File Name     : CCRefVariant.cpp
Purpose       : Implementation for RefVariant. The
                RefVariant is just a Variant that
                only does shallow copies.
-----------------------------------------------------*/
#include "Precompiled.h"

// Constructors
RefVariant::RefVariant( )
{
}

RefVariant::RefVariant( Variant *rhs )
{
  data = rhs->GetData( );
  meta = rhs->GetMeta( );
}

RefVariant::RefVariant( Variant&& rhs ) : VariantBase( rhs.meta, rhs.data )
{
  rhs.data = NULL;
  rhs.meta = NULL;
}

RefVariant::RefVariant( const RefVariant& rhs )
{
  data = rhs.data;
  meta = rhs.meta;
}

RefVariant::RefVariant( RefVariant&& rhs ) : VariantBase( rhs.meta, rhs.data )
{
  rhs.data = NULL;
  rhs.meta = NULL;
}

RefVariant::RefVariant( const MetaData *meta, void *data )
  : VariantBase( meta, data )
{
}

RefVariant::RefVariant( const Variant& rhs )
{
  data = rhs.GetData( );
  meta = rhs.GetMeta( );
}

void RefVariant::Assign( const RefVariant& rhs )
{
  meta->Copy( data, rhs.data );
}

RefVariant& RefVariant::operator=( RefVariant&& rhs )
{
  data = rhs.data;
  rhs.data = NULL;
  meta = rhs.meta;
  rhs.meta = NULL;
  return *this;
}

void RefVariant::Assign( RefVariant&& rhs )
{
  data = rhs.data;
  rhs.data = NULL;
  meta = rhs.meta;
  rhs.meta = NULL;
}

bool RefVariant::operator==( const RefVariant& rhs ) const
{
  return data == rhs.data && meta == rhs.meta;
}

bool RefVariant::operator!=( const RefVariant& rhs ) const
{
  return !(*this == rhs);
}

RefVariant& RefVariant::operator=( const RefVariant& rhs )
{
  meta = rhs.meta;
  data = rhs.data;
  return *this;
}

void RefVariant::Delete( void )
{
  if(meta && data)
  {
    meta->Delete( data );
    meta = NULL;
  }
}

void RefVariant::Copy( const MetaData *m, void *d )
{
  meta = m;
  data = d;
}

void RefVariant::PlacementDelete( void )
{
  meta->PlacementDelete( data );
}

void RefVariant::PlacementNew( void )
{
  meta->PlacementNew( data );
}

void RefVariant::ToLua( lua_State *lua ) const
{
  meta->ToLua( lua, *this );
}

void RefVariant::FromLua( lua_State *lua, int index )
{
  meta->FromLua( lua, index, *this );
}

void RefVariant::Serialize( File& file ) const
{
  meta->Serialize( file, *this );
}

RefVariant RefVariant::Deserialize( File& file ) const
{
  return meta->Deserialize( file );
}

bool RefVariant::IsValid( void ) const
{
  return meta != META_TYPE( VoidType ) && data != NULL && meta != NULL;
}
