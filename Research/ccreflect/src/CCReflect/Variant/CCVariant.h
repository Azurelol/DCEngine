/*--------------------qMopey-Header--------------------
<Game Title>  : qMopey
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 9/15/2012
File Name     : CCVariant.cpp
Purpose       : Declaration of the Variant class
-----------------------------------------------------*/

#pragma once

#include "CCVariantBase.h"

//
// Variant
// A variant is a data type that can hold any type of data that is defined within the MetaData system. It works
// by storing MetaData about the type it holds, and holds that type within a void *.
//

class Variant : public VariantBase
{
  friend class RefVariant;
public:
  Variant( ) {}
  template <typename TYPE> Variant( const TYPE& value );
  Variant( const Variant& rhs );
  Variant( const RefVariant& rhs );
  Variant( Variant&& rhs );
  ~Variant( );

  Variant& operator=( const Variant& rhs );
  Variant& operator=( Variant&& rhs );
  bool operator==( const Variant& rhs ) const;
  bool operator!=( const Variant& rhs ) const;
  template <typename TYPE> Variant& operator=( const TYPE& rhs );
  void ToLua( lua_State *lua ) const;
  void Serialize( File& file ) const;
  RefVariant Deserialize( File& file ) const;

  META_DATA( Variant );
};

template <typename TYPE>
Variant::Variant( const TYPE& value ) : VariantBase( META_TYPE( TYPE ), NULL )
{
#ifdef _DEBUG
  ErrorIf( meta->Name( ) == "", "This type isn't registered in MetaData, you cannot create a Variant from it! Please register this type with "
    "MetaData in order to create Variants or RefVariants of this type." );
#endif // _DEBUG
  meta->NewCopy( &data, &value );
}
  
template <typename TYPE>
Variant& Variant::operator=( const TYPE& rhs )
{
  // Copy contents of rhs, but check to see if MetaData of TYPE
  // matches first, and act accordingly
  if(meta != META_TYPE( TYPE ))
  {
    meta->Delete( data );
    meta = META_TYPE( TYPE );
    meta->NewCopy( &data, &rhs );
  }
  else
  {
    meta->Copy( data, &rhs );
  }
  return *this;
}
