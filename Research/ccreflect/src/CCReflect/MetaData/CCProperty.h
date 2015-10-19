/*--------------------qMopey-Header--------------------
<Game Title>  : qMopey
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 4/19/2013
File Name     : CCProperty.h
Purpose       : Wrapper for gettor/settor access of
                an object member.
-----------------------------------------------------*/
#pragma once

#include "CCVariantFunction.h"
#include "CCRefVariant.h"

#define PROPERTY( HOST, GETTOR, SETTOR ) \
  Property( &HOST::GETTOR, &HOST::SETTOR )

class Property
{
public:
  template <typename Gettor, typename Settor>
  Property( Gettor gettor, Settor settor );
  ~Property( );

  // Get or set the member this property wraps
  template <typename T>
  void Get( T& ret );
  template <typename T>
  void Set( T& val );

  // Bind an object to use this property with
  template <typename T>
  void Bind( T& self );
  template <typename T>
  void Bind( T *self );

private:
  VariantFunction m_get;
  VariantFunction m_set;
};

template <typename Gettor, typename Settor>
Property::Property( Gettor gettor, Settor settor )
  : m_get( "Gettor", gettor )
  , m_set( "Settor", settor )
{
}

template <typename T>
void Property::Get( T& ret )
{
  RefVariant ref( ret );
  return m_get( ref );
}

template <typename T>
void Property::Set( T& val )
{
  m_set( val );
}

template <typename T>
void Property::Bind( T& self )
{
  m_get.Bind( self );
  m_set.Bind( self );
}

template <typename T>
void Property::Bind( T *self )
{
  m_get.Bind( self );
  m_set.Bind( self );
}
