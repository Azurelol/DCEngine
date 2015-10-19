#pragma once

#include "CCLuaIncludes.h"
#include "CCRefVariant.h"

class MetaData;

class LuaReference
{
public:
  LuaReference( const MetaData *meta, void *data );
  LuaReference( const RefVariant& ref );
  ~LuaReference( );

  RefVariant& GetObject( void ) const;

private:
  RefVariant object;
};

void GenericToLua( lua_State *L, RefVariant ref );
void GenericFromLua( lua_State *L, int index, RefVariant& ref );
