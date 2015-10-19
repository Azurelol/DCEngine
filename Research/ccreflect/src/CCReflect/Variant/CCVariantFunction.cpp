/*--------------------qMopey-Header--------------------
<Game Title>  : qMopey
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : ???
File Name     : CCVariantFunction.cpp
Purpose       : 
-----------------------------------------------------*/

#include "Precompiled.h"
#include "CCVariantFunction.h"
#include <malloc.h>

/*------------------------------------------------------------------------------------------------

                                          Ctors/Dtors

------------------------------------------------------------------------------------------------*/
VariantFunction::VariantFunction( const char *name_, const char *signature, uint32 arg_count )
  : name( name_ )
  , call( CallHelper<const char *> )
{
#ifdef DEBUG
  ErrorIf( strlen( signature ) > sizeof( MultiFnPtr ) - 1, S( "Function name " ) + signature + " too long to bind!" );
#endif
  strcpy_s( reinterpret_cast<char *>(&fn), sizeof( MultiFnPtr ) - 1, signature );
  (uint32)sig.argCount = arg_count;
}

VariantFunction::VariantFunction( ) :
  name( NULL ),
  sig( ),
  fn( ),
  call( NULL )
{
}

FunctionSignature::FunctionSignature( ) :
  ret( META_TYPE( VoidType ) ),
  argArray( NULL ),
  context( NULL ),
  argCount( 0 )
{
}

FunctionSignature::FunctionSignature( void (*fn)( void ) ) :
  ret( META_TYPE( VoidType ) ),
  argArray( NULL ),
  context( NULL ),
  argCount( 0 )
{
}

FunctionSignature::FunctionSignature( const FunctionSignature& rhs )
  : ret( rhs.ret )
  , argArray( rhs.argArray )
  , context( rhs.context )
  , argCount( rhs.argCount )
{
}

const FunctionSignature& FunctionSignature::operator=( const FunctionSignature& rhs )
{
  ret = rhs.ret;
  argArray = rhs.argArray;
  context = rhs.context;
  (unsigned)argCount = rhs.argCount;

  return *this;
}

/*------------------------------------------------------------------------------------------------

                                          FunctionSignature

------------------------------------------------------------------------------------------------*/
  /*----------------
      Data Access
  ----------------*/
unsigned FunctionSignature::ArgCount( void ) const
{
  return argCount;
}

const MetaData *FunctionSignature::GetRet( void ) const
{
  return ret;
}

const MetaData *FunctionSignature::GetArg( unsigned i ) const
{
  return argArray[i];
}

bool FunctionSignature::IsMemberFn( void ) const
{
  return (context) ? true : false;
}

const MetaData *FunctionSignature::Context( void ) const
{
  return context;
}

/*------------------------------------------------------------------------------------------------

                              Static Function Calls without Return Value

------------------------------------------------------------------------------------------------*/
void Call( void (*fn)( void ), RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  assert( argCount == 0 );

  fn( );
}

/*------------------------------------------------------------------------------------------------

                                          VariantFunction

------------------------------------------------------------------------------------------------*/
  /*----------------
      Modifiers
  ----------------*/
void VariantFunction::BindFromLua( lua_State *L, unsigned index )
{
  binding.FromLua( L, index );
}

const RefVariant& VariantFunction::GetBinding( void ) const
{
  return binding;
}

RefVariant& VariantFunction::GetBinding( void )
{
  return binding;
}

void VariantFunction::ClearBinding( void )
{
  binding = RefVariant( binding.GetMeta( ), NULL );
}

  /*----------------
    Functor Calls
  ----------------*/
void VariantFunction::operator( )( RefVariant& ret, RefVariant *args, unsigned argCount ) const
{
  call( fn, const_cast<RefVariant &>(binding), ret, args, argCount );
}

void VariantFunction::operator( )( RefVariant& ret ) const
{
  call( fn, const_cast<RefVariant &>(binding), ret, NULL, 0 );
}

void VariantFunction::operator( )( void ) const
{
  call( fn, const_cast<RefVariant &>(binding), RefVariant( ), NULL, 0 );
}
  
// Lua function call binding
template <>
void CallHelper<const char *>( const MultiFnPtr& fn, RefVariant& binding, RefVariant& ret, RefVariant *args, unsigned argCount )
{
  CALL_LUA_FUNC( reinterpret_cast<const char *>(&fn), args, argCount );
}

  /*----------------
      Data Access
  ----------------*/
const char *VariantFunction::Name( void ) const
{
  return name;
}

const FunctionSignature& VariantFunction::GetSignature( void ) const
{
  return sig;
}

bool VariantFunction::BindingIsValid( void ) const
{
  return binding.IsValid( );
}
