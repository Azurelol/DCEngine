/*-----------------Alloctopus-Header--------------------
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : Tue Apr 30 15:30:31 2013

File Name     : CCEnum.h
Purpose       : 
------------------------------------------------------*/

#pragma once

#include "CCEnumEditorConfig.h"
#include <unordered_map>

#undef E_NAME
#undef E_PARENT
#undef E_ENTRY
#undef E_END

// Construct actual enumerations
#define E_NAME( NAME ) \
  enum E##NAME {

#define E_PARENT( PARENT ) \

#define E_ENTRY( ENTRY ) \
  e##ENTRY,

#define E_END( NAME ) \
  };

#include "CCEnumData.h"

struct Enum : public CtorLister<Enum>
{
  Enum( const char *name, const Enum *parent );
  Enum( const char *name, const Enum *parent, const char **literals );
  Enum( const char *name, const Enum *parent, const Array<S>& literals );

  S m_name;
  const Enum *m_parent;
  Array<S> m_literals;

  InKey<Enum, S> node;

  static Enum *GetEnumByString( const S& name );
#ifdef COMPILE_RUNTIME_EDITOR
  bool m_active;
  void RemoveFromMap( void );
  void AddEntry( const char * );
#endif // COMPILE_RUNTIME_EDITOR
};

// Declare Enum objects
#undef  E_NAME
#define E_NAME( NAME ) \
  extern "C" Enum Enum##NAME; \
  __pragma( comment ( linker, "/INCLUDE:_"__STRINGIZE( Enum##NAME ) ) )

#undef  E_PARENT
#define E_PARENT( PARENT )

#undef  E_ENTRY
#define E_ENTRY( ENTRY )

#undef  E_END
#define E_END( NAME )

#include "CCEnumData.h"

// For representing a NULL in the parent member of an Enum
struct NullEnum
{
};

// Declare the NULL enum
extern NullEnum EnumNULL;
