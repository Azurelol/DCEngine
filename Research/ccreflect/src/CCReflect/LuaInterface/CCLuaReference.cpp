#include "Precompiled.h"
#include "CCLuaReference.h"

LuaReference::LuaReference( const MetaData *meta, void *data ) : object( meta, data )
{
}

LuaReference::LuaReference( const RefVariant& ref ) : object( ref )
{
}

LuaReference::~LuaReference( )
{
}

RefVariant& LuaReference::GetObject( void ) const
{
  return const_cast<RefVariant&>(object);
}

void GenericToLua( lua_State *L, RefVariant ref )
{
  LuaReference *luaRef = reinterpret_cast<LuaReference *>(lua_newuserdata( L, sizeof( LuaReference ) ));
  new (luaRef) RefVariant( ref );
  lua_getglobal( L, ObjectMetaTable );
  lua_setmetatable( L, -2 );
}

void GenericFromLua( lua_State *L, int index, RefVariant& ref )
{
  ErrorIf( !lua_isuserdata( L, index ), "Did not pass a self parameter? Use Obj:Func() syntax instead of Obj.Func()" );

  // Copy our referenced object into the ref argument
  ref = reinterpret_cast<LuaReference *>(lua_touserdata( L , index ))->GetObject( );
}
