/*--------------------qMopey-Header--------------------
<Game Title>  : qMopey
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 9/15/2012
File Name     : CCLuaInterface.h
Purpose       : Declaration of the LuaInterface.
-----------------------------------------------------*/
#pragma once

#include "CCLuaIncludes.h"
#include "CCLuaReference.h"
#include "CCLuaScript.h"

typedef lua_CFunction LuaFunction;
struct GameObject;

#define CALL_LUA_FUNC( NAME, ... ) \
  LuaInterface::Get( ).CallLuaFunc( NAME, __VA_ARGS__ )

#define LUA_INTERFACE \
  LuaInterface::Get( )

extern const char *ObjectMetaTable;

class LuaInterface : public Singleton<LuaInterface>
{
public:
  LuaInterface( );
  ~LuaInterface( );

  // Dumps the contents of the Lua stack to the Visual Studio debug output
  void StackDump( void ) const;
  static void StackDump( lua_State *L );

  // Grabs the main global lua_State pointer
  lua_State *State( void );

  // Sets the lua global dt variable -- There is no concept of spaces within Lua
  void Update( float dt );

  // Returns the stack size of a particular value on the lua stack
  unsigned PeakStackSize( lua_State *lua, int index );

  // Runs luaL_dofile on a specified lua text file
  void LoadFile( lua_State *lua, const char *path );

  // Runs luaL_dofile on all files in a specified folder
  void LoadFilesInFolder( Path& path );

  // Create a coroutine ready to act as an indedendent script
  // The unsigned returned is the key to the coroutine index within lua and is
  // used to control garbage collection on the coroutine itself
  void CreateScript( LuaScript &script );

  // Nils out the table value of a Lua coroutine, letting it fall into the Lua garbage collector's hand
  void RemoveScript( LuaScript *script );

  // Binds all VariantFunctions to Lua -- Should only be called upon program start in: assumes
  // no VariantFunctions have destructed
  void BindAllFnsToLua( void );

  // Lots of overloads for calling a Lua function from C++
  void CallLuaFunc( const char *name );
  template <typename Arg0>
  void CallLuaFunc( const char *name, Arg0 arg0 );
  template <typename Arg0, typename Arg1>
  void CallLuaFunc( const char *name, Arg0 arg0, Arg1 arg1 );
  template <typename Arg0, typename Arg1, typename Arg2>
  void CallLuaFunc( const char *name, Arg0 arg0, Arg1 arg1, Arg2 arg2 );
  template <typename Arg0, typename Arg1, typename Arg2, typename Arg3>
  void CallLuaFunc( const char *name, Arg0 arg0, Arg1 arg1, Arg2 arg2, Arg3 arg3 );
  template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
  void CallLuaFunc( const char *name, Arg0 arg0, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4 );
  template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
  void CallLuaFunc( const char *name, Arg0 arg0, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5 );
  template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
  void CallLuaFunc( const char *name, Arg0 arg0, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6 );
  template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
  void CallLuaFunc( const char *name, Arg0 arg0, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7 );
  template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
  void CallLuaFunc( const char *name, Arg0 arg0, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8 );

  // Called from VariantFunction
  void CallLuaFunc( const char *name, RefVariant *args, unsigned argCount );

  void CallLuaFunc( ... ); // Bad call!

private:
  lua_State *L;

  // Not to be used directly
  void GenericCallLua( const char *luaFuncName, RefVariant *args, unsigned argCount );
};

int LuaErrorFunc( lua_State *L );
int GenericFunc( lua_State *L );
int FunctionLookup( lua_State *L );

template <typename Arg0>
void LuaInterface::CallLuaFunc( const char *name, Arg0 arg0 )
{
  RefVariant *stackArgs = reinterpret_cast<RefVariant *>(alloca( sizeof( RefVariant ) ));

  new (stackArgs) RefVariant( arg0 );

  GenericCallLua( name, stackArgs, 1 );
}

template <typename Arg0, typename Arg1>
void LuaInterface::CallLuaFunc( const char *name, Arg0 arg0, Arg1 arg1 )
{
  RefVariant *stackArgs = reinterpret_cast<RefVariant *>(alloca( sizeof( RefVariant ) * 2 ));

  new (stackArgs) RefVariant( arg0 );
  new (stackArgs + 1) RefVariant( arg1 );

  GenericCallLua( name, stackArgs, 2 );
}

template <typename Arg0, typename Arg1, typename Arg2>
void LuaInterface::CallLuaFunc( const char *name, Arg0 arg0, Arg1 arg1, Arg2 arg2 )
{
  RefVariant *stackArgs = reinterpret_cast<RefVariant *>(alloca( sizeof( RefVariant ) * 3 ));

  new (stackArgs) RefVariant( arg0 );
  new (stackArgs + 1) RefVariant( arg1 );
  new (stackArgs + 2) RefVariant( arg2 );

  GenericCallLua( name, stackArgs, 3 );
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3>
void LuaInterface::CallLuaFunc( const char *name, Arg0 arg0, Arg1 arg1, Arg2 arg2, Arg3 arg3 )
{
  RefVariant *stackArgs = reinterpret_cast<RefVariant *>(alloca( sizeof( RefVariant ) * 4 ));

  new (stackArgs) RefVariant( arg0 );
  new (stackArgs + 1) RefVariant( arg1 );
  new (stackArgs + 2) RefVariant( arg2 );
  new (stackArgs + 3) RefVariant( arg3 );

  GenericCallLua( name, stackArgs, 4 );
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
void LuaInterface::CallLuaFunc( const char *name, Arg0 arg0, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4 )
{
  RefVariant *stackArgs = reinterpret_cast<RefVariant *>(alloca( sizeof( RefVariant ) * 5 ));

  new (stackArgs) RefVariant( arg0 );
  new (stackArgs + 1) RefVariant( arg1 );
  new (stackArgs + 2) RefVariant( arg2 );
  new (stackArgs + 3) RefVariant( arg3 );
  new (stackArgs + 4) RefVariant( arg4 );

  GenericCallLua( name, stackArgs, 5 );
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
void LuaInterface::CallLuaFunc( const char *name, Arg0 arg0, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5 )
{
  RefVariant *stackArgs = reinterpret_cast<RefVariant *>(alloca( sizeof( RefVariant ) * 6 ));

  new (stackArgs) RefVariant( arg0 );
  new (stackArgs + 1) RefVariant( arg1 );
  new (stackArgs + 2) RefVariant( arg2 );
  new (stackArgs + 3) RefVariant( arg3 );
  new (stackArgs + 4) RefVariant( arg4 );
  new (stackArgs + 5) RefVariant( arg5 );

  GenericCallLua( name, stackArgs, 6 );
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
void LuaInterface::CallLuaFunc( const char *name, Arg0 arg0, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6 )
{
  RefVariant *stackArgs = reinterpret_cast<RefVariant *>(alloca( sizeof( RefVariant ) * 7 ));

  new (stackArgs) RefVariant( arg0 );
  new (stackArgs + 1) RefVariant( arg1 );
  new (stackArgs + 2) RefVariant( arg2 );
  new (stackArgs + 3) RefVariant( arg3 );
  new (stackArgs + 4) RefVariant( arg4 );
  new (stackArgs + 5) RefVariant( arg5 );
  new (stackArgs + 6) RefVariant( arg6 );

  GenericCallLua( name, stackArgs, 7 );
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
void LuaInterface::CallLuaFunc( const char *name, Arg0 arg0, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7 )
{
  RefVariant *stackArgs = reinterpret_cast<RefVariant *>(alloca( sizeof( RefVariant ) * 8 ));

  new (stackArgs) RefVariant( arg0 );
  new (stackArgs + 1) RefVariant( arg1 );
  new (stackArgs + 2) RefVariant( arg2 );
  new (stackArgs + 3) RefVariant( arg3 );
  new (stackArgs + 4) RefVariant( arg4 );
  new (stackArgs + 5) RefVariant( arg5 );
  new (stackArgs + 6) RefVariant( arg6 );
  new (stackArgs + 7) RefVariant( arg7 );

  GenericCallLua( name, stackArgs, 8 );
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
void LuaInterface::CallLuaFunc( const char *name, Arg0 arg0, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8 )
{
  RefVariant *stackArgs = reinterpret_cast<RefVariant *>(alloca( sizeof( RefVariant ) * 9 ));

  new (stackArgs) RefVariant( arg0 );
  new (stackArgs + 1) RefVariant( arg1 );
  new (stackArgs + 2) RefVariant( arg2 );
  new (stackArgs + 3) RefVariant( arg3 );
  new (stackArgs + 4) RefVariant( arg4 );
  new (stackArgs + 5) RefVariant( arg5 );
  new (stackArgs + 6) RefVariant( arg6 );
  new (stackArgs + 7) RefVariant( arg7 );
  new (stackArgs + 8) RefVariant( arg8 );

  GenericCallLua( name, stackArgs, 9 );
}
