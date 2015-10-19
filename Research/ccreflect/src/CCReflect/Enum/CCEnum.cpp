/*-----------------Alloctopus-Header--------------------
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : Tue Apr 30 15:30:35 2013

File Name     : CCEnum.cpp
Purpose       : 
------------------------------------------------------*/

#include "Precompiled.h"
#include "CCEnum.h"

#undef E_NAME
#undef E_PARENT
#undef E_ENTRY
#undef E_END

static HASH_DECL( Enum, S, node ) enumMap( 20 );

Enum::Enum( const char *name, const Enum *parent )
  : m_name( name )
  , m_parent( parent )
#ifdef COMPILE_RUNTIME_EDITOR
  , m_active( true )
#endif // COMPILE_RUNTIME_EDITOR
{
  node.SetKey( S( name ) );
  enumMap.Add( this );
}

Enum::Enum( const char *name, const Enum *parent, const char **literals )
  : m_name( name )
  , m_parent( parent )
#ifdef COMPILE_RUNTIME_EDITOR
  , m_active( true )
#endif // COMPILE_RUNTIME_EDITOR
{
  for(auto i = literals; *i; ++i)
    m_literals.EmplaceBack( *i );

  // Erase the count_Name enum
  m_literals.Erase( m_literals.Size( ) - 1 );
  node.SetKey( S( name ) );
  enumMap.Add( this );
}

Enum::Enum( const char *name, const Enum *parent, const Array<S>& literals )
  : m_name( name )
  , m_parent( parent )
  , m_literals( literals )
#ifdef COMPILE_RUNTIME_EDITOR
  , m_active( true )
#endif // COMPILE_RUNTIME_EDITOR
{
  node.SetKey( S( name ) );
  enumMap.Add( this );
}

Enum *Enum::GetEnumByString( const S& name )
{
  return enumMap[name];
}

#ifdef COMPILE_RUNTIME_EDITOR
void Enum::RemoveFromMap( void )
{
  for(uint32 i = 0; i < m_literals.Size( ); ++i)
  {
    Enum *entry = Enum::GetEnumByString( m_literals[i] );
    if(entry)
      entry->RemoveFromMap( );
  }
  enumMap.Remove( this );
}

void Enum::AddEntry( const char *entry )
{
  m_literals.EmplaceBack( entry );
}
#endif // COMPILE_RUNTIME_EDITOR

// For representing a NULL in the parent member of an Enum
NullEnum EnumNULL;

// Create string literal arrays
#undef  E_NAME
#define E_NAME( NAME ) \
  const char *k_##NAME##literalArray[] = {

#undef  E_PARENT
#define E_PARENT( PARENT )

#undef  E_ENTRY
#define E_ENTRY( ENTRY ) \
  #ENTRY,

#undef  E_END
#define E_END( NAME ) \
  NULL \
  };

// To set &EnumNULL to actual NULL
template <typename T>
struct SetParentAddress
{
  static const bool value = true;
};

template <>
struct SetParentAddress<NullEnum>
{
  static const bool value = false;
};

#include "CCEnumData.h"

// Create Enum instances
#undef  E_NAME
#define E_NAME( NAME ) \
  Enum Enum##NAME( \
  #NAME,

#undef  E_PARENT
#define E_PARENT( PARENT ) \
  (Enum *)((SetParentAddress<decltype( Enum##PARENT )>::value) ? &Enum##PARENT : NULL),

#undef  E_ENTRY
#define E_ENTRY( ENTRY )

#undef  E_END
#define E_END( NAME ) \
  k_##NAME##literalArray );

#include "CCEnumData.h"
