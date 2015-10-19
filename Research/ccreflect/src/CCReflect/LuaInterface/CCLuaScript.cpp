/*--------------------qMopey-Header--------------------
<Game Title>  : qMopey
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 1/22/2013
File Name     : CCLocationScript.cpp
Purpose       : Wrapper for a Lua coroutine.
-----------------------------------------------------*/
#include "Precompiled.h"
#include "CCLuaInterface.h"
#include "CCLuaScript.h"

#ifndef CC_REG_EXPLICIT

DEFINE_META( LuaScript )
{
  ADD_MEMBER( scriptName );
  ADD_MEMBER( lua_state );
  ADD_MEMBER( script_id );
}

#endif // CC_REG_EXPLICIT

unsigned LuaScript::script_id_gen( 0 );

LuaScript::LuaScript( )
{
}

LuaScript::LuaScript( lua_State *l ) :
  lua_state( l ),
  script_id( script_id_gen++ )
{
}

// Start the coroutine and pass obj as the first argument to Lua
void LuaScript::Load( RefVariant obj )
{
  lua_getglobal( lua_state, scriptName ); // Stack index: 1
  ErrorIf( !lua_isfunction( lua_state, 1 ), "Did not retrieve a function from scriptName." );

  // Push object to lua
  obj.ToLua( lua_state );

  // Start the coroutine
  lua_resume( lua_state, NULL, 1 );
  lua_settop( lua_state, 0 );
}

void LuaScript::Load( RefVariant obj, const char *nameOfScript )
{
  lua_getglobal( lua_state, nameOfScript ); // Stack index: 1
  ErrorIf( !lua_isfunction( lua_state, 1 ), S( "Did not retrieve scriptName: " ) + nameOfScript );

  // Push object to lua
  LuaInterface::StackDump( LuaInterface::Get( ).State( ) );
  obj.ToLua( lua_state );

  // Start the coroutine
  LuaInterface::StackDump( LuaInterface::Get( ).State( ) );
  lua_resume( lua_state, NULL, 1 );
  lua_settop( lua_state, 0 );
}

// Resumes a coroutine within Lua
void LuaScript::Resume( void )
{
  lua_resume( lua_state, NULL, 0 );
}

// Cleans up the coroutine in Lua by nil'ing out the table in Lua where the coroutine resides
// allowing Lua garbage collection to take hold of it
void LuaScript::ShutDown( void )
{
  LuaInterface::Get( ).RemoveScript( this );
}

// Sets the string holding the name of the function in Lua the script is running under
void LuaScript::SetScriptName( const char *luaFuncName )
{
  scriptName = luaFuncName;
}

unsigned LuaScript::GetID( void ) const
{
  return script_id;
}

lua_State *LuaScript::GetState( void )
{
  return lua_state;
}
