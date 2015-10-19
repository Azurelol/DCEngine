/*--------------------qMopey-Header--------------------
<Game Title>  : qMopey
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 1/22/2013
File Name     : CCLocationScript.h
Purpose       : Wrapper for a Lua coroutine.
-----------------------------------------------------*/
#pragma once

#include "CCMetaData.h"
#include "CCLuaIncludes.h"

class LuaScript
{
  friend class LuaInterface;

public:
  // Does nothing for performance. Pass a pointer to the LUA_INTERFACE to a LuaScript,
  // and the LuaInterface will placement new a LuaScript with the private constructor.
  // See LuaInterface.cpp for more details.
  LuaScript( );

  // Start the coroutine and pass obj as the first argument to Lua
  void Load( RefVariant obj );
  void Load( RefVariant obj, const char *nameOfScript );

  // Resumes a coroutine within Lua
  void Resume( void );
  
  // Cleans up the coroutine in Lua by nil'ing out the table in Lua where the coroutine resides
  // allowing Lua garbage collection to take hold of it
  void ShutDown( void );

  // Sets the string holding the name of the function in Lua the script is running under
  void SetScriptName( const char *luaFuncName );

  unsigned GetID( void ) const;
  lua_State *GetState( void );

  META_DATA( LuaScript );

private:
  // Holds the name of the function in Lua to use as the coroutine routine
  const char *scriptName;

  // Holds a coroutine that is represented by this LuaScript object
  lua_State *lua_state;

  // integer id for holding coroutine within Lua to prevent garbage collection from
  // cleaning it up
  unsigned script_id;

  // Private constructor for LuaInterface. See LuaInterface.cpp for more details.
  LuaScript( lua_State *l );

  static unsigned script_id_gen;
};
