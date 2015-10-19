/*--------------------qMopey-Header--------------------
<Game Title>  : qMopey
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 9/15/2012
File Name     : CCRefVariant.cpp
Purpose       : Declaration for RefVariant. The
                RefVariant is just a Variant that
                only does shallow copies.
-----------------------------------------------------*/

#pragma once

#include "CCVariantBase.h"

//
// RefVariant
// A RefVariant is the same as the Variant, except does shallow copies.
//

class MetaData;
class Variant;

class RefVariant : public VariantBase
{
  friend class Variant;
public:
  RefVariant( );
  RefVariant( const MetaData *meta, void *data );
  RefVariant( Variant *rhs );
  RefVariant( const Variant& rhs );
  RefVariant( Variant&& rhs );
  RefVariant( const RefVariant& rhs );
  RefVariant( RefVariant&& rhs );
  template <typename TYPE> RefVariant( const TYPE *rhs );
  template <typename TYPE> RefVariant( const TYPE& rhs );
  template <typename TYPE> RefVariant( TYPE *rhs );
  bool operator==( const RefVariant& rhs ) const;
  bool operator!=( const RefVariant& rhs ) const;
  template <typename TYPE> RefVariant& operator=( TYPE *rhs );
  RefVariant& operator=( const RefVariant& rhs );
  void Assign( const RefVariant& rhs );
  RefVariant& operator=( RefVariant&& rhs );
  void Assign( RefVariant&& rhs );
  template <typename TYPE> void Assign( const TYPE& rhs );
  void Copy( const MetaData *m, void *d );
  void Delete( void ); // Not really safe, probably shouldn't be used except as: ref.GetMeta( )->Delete( ref-GetData( ) )
  void Serialize( File& file ) const;
  RefVariant Deserialize( File& file ) const;
  void PlacementDelete( void );
  void PlacementNew( void );
  void ToLua( lua_State *lua ) const;
  void FromLua( lua_State *lua, int index );
  bool IsValid( void ) const;
};

template <typename TYPE>
RefVariant::RefVariant( const TYPE& rhs )
  : VariantBase( META_TYPE( TYPE & ), const_cast<TYPE *>(&rhs) )
{
}

template <typename TYPE>
RefVariant::RefVariant( const TYPE *rhs )
  : VariantBase( META_TYPE( TYPE * ), const_cast<TYPE *>(rhs) )
{
}

template <typename TYPE>
RefVariant::RefVariant( TYPE *rhs )
  : VariantBase( META_TYPE( TYPE * ), const_cast<TYPE *>(rhs) )
{
}

template <typename TYPE>
void RefVariant::Assign( const TYPE& rhs )
{
  RefVariant ref( &rhs );
  Assign( ref );
}

template <typename TYPE>
RefVariant& RefVariant::operator=( TYPE *rhs )
{
  meta = META_TYPE( TYPE * );
#ifdef _DEBUG
  ErrorIf( meta == NULL, "This type isn't registered in MetaData, you cannot create a RefVariant from it! Please register this type with "
    "MetaData in order to create Variants or RefVariants of this type." );
#endif // _DEBUG
  data = (void *)rhs;
  return *this;
}
