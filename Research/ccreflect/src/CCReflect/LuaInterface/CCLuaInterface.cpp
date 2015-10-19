/*--------------------qMopey-Header--------------------
<Game Title>  : qMopey
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 9/15/2012
File Name     : CCLuaInterface.cpp
Purpose       : Implementation of the LuaInterface.
-----------------------------------------------------*/
#include "Precompiled.h"
#include "CCLuaInterface.h"
#include "CCVariantFunction.h"
#include "CCTextSerialize.h"

const char *ObjectMetaTable = "ObjectMetaTable";

typedef lua_State * lua_StatePtr;
typedef lua_State ** lua_StatePtrPtr;

#ifndef CC_REG_EXPLICIT

DEFINE_META_POD( lua_StatePtr )
{
}

DEFINE_META_POD( lua_StatePtrPtr )
{
}

#endif // CC_REG_EXPLICIT

LuaInterface::LuaInterface( )
{
  L = luaL_newstate( );
  luaL_openlibs( L );

  //Path p;
  //p.WorkingDirectory( );
  //LoadFilesInFolder( p );

  // Lua binding
  BindAllFnsToLua( );

  // Setup dt within lua
  lua_pushnumber( L, 0 );
  lua_setglobal( L, "dt" );
}

LuaInterface::~LuaInterface( )
{
  StackDump( );
  lua_close( L );
}

// Returns the stack size of a particular value on the lua stack
unsigned PeakStackSize( lua_State *lua, int index )
{
  switch(lua_type( lua, index ))
  {
  case LUA_TSTRING:   return META_TYPE( const char * )->Size( );
  case LUA_TBOOLEAN:  return META_TYPE( bool )->Size( );
  case LUA_TNUMBER:   return META_TYPE( float )->Size( );
  case LUA_TNIL:      return 0;
  default:            Error( "Cannot retrieve this type from Lua as of yet!" ); return 0;
  }
}

// Runs luaL_dofile on a specified lua text file
void LuaInterface::LoadFile( lua_State *lua, const char *path )
{
  int errorStatus = luaL_dofile( lua, path );

  if(errorStatus)
    Error( lua_tostring( lua, -1 ) );
}

// Runs luaL_dofile on all files in a specified folder
void LuaInterface::LoadFilesInFolder( Path& path )
{
  std::vector<S> fileNames( path.GetExt( ".lua" ) );

  for(unsigned i = 0; i < fileNames.size( ); ++i)
    LoadFile( L, (path.Get( ) + "\\"+ fileNames[i]).c_str( ) );
}

// Create a coroutine ready to act as an indedendent script
// The unsigned returned is the key to the coroutine index within lua and is
// used to control garbage collection on the coroutine itself
void LuaInterface::CreateScript( LuaScript &script )
{
  lua_getglobal( L, "AddCoroutine" );
  ErrorIf( !lua_isfunction( L, -1 ), "AddCoroutine function is not found! It is either not in Lua, or is a variable in Lua that isn't a function." );
  new (&script) LuaScript( lua_newthread( L ) );
  lua_pushinteger( L, script.script_id );
  lua_pcall( L, 2, 0, NULL );
}

// Nils out the table value of a Lua coroutine, letting it fall into the Lua garbage collector's hand
void LuaInterface::RemoveScript( LuaScript *script )
{
  lua_getglobal( L, "RemoveCoroutine" ); // 1
  ErrorIf( !lua_isfunction( L, 1 ), "AddCoroutine function is either not found, or is not a function!" );
  lua_pushnumber( L, script->script_id ); // 2
  lua_pcall( L, 1, 0, NULL );
}

// Binds all VariantFunctions to Lua -- Should only be called upon program start in: assumes
// no VariantFunctions have destructed
void LuaInterface::BindAllFnsToLua( void )
{
  // Use generated name as the metatable name of this type in Lua
  // Setup the __index method to point to self
  lua_newtable( L );
  lua_setglobal( L, ObjectMetaTable );
  lua_getglobal( L, ObjectMetaTable );
  lua_pushstring(L, "__index");
  lua_pushcfunction( L, FunctionLookup );
  lua_settable( L, 1 );
  lua_settop( L, 0 );

  for(const VariantFunction *func = VariantFunction::Head( ); func; func = func->Next( ))
  {
    lua_pushlightuserdata( L, const_cast<VariantFunction *>(func) );
    lua_pushcclosure( L, GenericFunc, 1 );
    lua_setglobal( L, func->Name( ) );
  }
}

// Find specific function within lua 
int FunctionLookup( lua_State *L )
{
  // Grab the lua function and push it onto the stack
  lua_getglobal( L, lua_tostring( L, 2 ) );

#ifdef _DEBUG
  ErrorIf( lua_isnil( L, 3 ), S( "The function " ) + lua_tostring( L, 2 ) + " could not be found within Lua!"
    " Are you sure that this function is bound to Lua with the REGISTER_FUNCTION macro?" );
#endif

  // Remove the string representation of the lua function
  lua_remove( L, 2 );

  // We're returning just one argument: the lua function to call next
  return 1;
}

// A generic function for Lua to call any C++ function
int GenericFunc( lua_State *L )
{
  // Grab upvalue for this particular function
  VariantFunction *func = reinterpret_cast<VariantFunction *>(lua_touserdata( L, lua_upvalueindex( 1 ) ));
  assert( func ); // Upvalue not found?

  int argCount = lua_gettop( L );

#ifdef _DEBUG
  if(func->GetSignature( ).IsMemberFn( ))
    ErrorIf( argCount != (func->GetSignature( ).ArgCount( ) + 1), "Argument count mismatch! Did you pass in the wrong amount of arguments to a Lua call to C++ function?" ); 
  else
    ErrorIf( argCount != func->GetSignature( ).ArgCount( ), "Argument count mismatch! Did you pass in the wrong amount of arguments to a Lua call to C++ function?" );
#endif

  // Allocate space for the return value on the stack
  RefVariant ret( func->GetSignature( ).GetRet( ), alloca( func->GetSignature( ).GetRet( )->Size( ) ) );
  // Run default constructor for return type
  ret.PlacementNew( );

  // Allocate space for the arguments and placement new defaults
  RefVariant *stackArgs = reinterpret_cast<RefVariant *>(alloca( sizeof( RefVariant ) * func->GetSignature( ).ArgCount( ) ));

  for(unsigned i = 0; i < func->GetSignature( ).ArgCount( ); ++i)
  {
    new (stackArgs + i) RefVariant( func->GetSignature( ).GetArg( i ), alloca( func->GetSignature( ).GetArg( i )->Size( ) ) );
    stackArgs[i].PlacementNew( );
  }

  // Load either the call context followed by arguments, or load all the arguments
  // depending on whether we are dealing with a static or member function
  if(func->GetSignature( ).IsMemberFn( ))
  {
    // Load call context from Lua
    func->BindFromLua( L, 1 );

    // Make sure we're using a proper context
    ErrorIf( !func->BindingIsValid( ), "Invalid binding! Meta does not exist for this type." );

    // Load all arguments from the lua stack
    for(unsigned i = 0; i < func->GetSignature( ).ArgCount( ); ++i)
    {
      // Skip the context arg
      stackArgs[i].FromLua( L, i + 2 );
    }
  }
  else
  {
    // Load all arguments from the lua stack
    for(unsigned i = 0; i < func->GetSignature( ).ArgCount( ); ++i)
    {
      stackArgs[i].FromLua( L, i + 1 );
    }
  }

  // Call function
  (*func)( ret, stackArgs, func->GetSignature( ).ArgCount( ) );

  // Run destructors on the arguments
  for(unsigned i = 0; i < func->GetSignature( ).ArgCount( ); ++i)
  {
    stackArgs[i].PlacementDelete( );
  }

  // Clear the binding, no effect if there is no binding
  func->ClearBinding( );

  // Push return value (if any) onto the Lua stack
  if(ret.IsValid( ))
  {
    ret.ToLua( L );
    // Destructor for return value
    ret.PlacementDelete( );
    return 1;
  }

  // Destructor for return value
  ret.PlacementDelete( );

  LuaInterface::Get( ).StackDump( LuaInterface::Get( ).State( ) );

  // Return number of arguments
  return 0;
}

// TODO: Return values from Lua
void LuaInterface::GenericCallLua( const char *luaFuncName, RefVariant *args, unsigned argCount )
{
  lua_pushcfunction( L , LuaErrorFunc ); // Stack index: 1

  lua_getglobal( L, luaFuncName ); // Stack index: 2

  for(unsigned i = 0; i < argCount; ++i)
  {
    args[i].ToLua( L );
  }

  int LuaErrorFuncIndex = -((int)(argCount + 2));
  lua_pcall( L, argCount, 1, LuaErrorFuncIndex );
  lua_remove( L, lua_gettop( L ) - 1 ); //Pop error function or error data
  //Variant var = PopStackValue( L, lua_gettop( L ) );
  lua_settop( L, 0 );
  //return var;
}

void LuaInterface::CallLuaFunc( const char *name, RefVariant *args, unsigned argCount )
{
  GenericCallLua( name, args, argCount );
}

int LuaErrorFunc( lua_State *L )
{			
  ErrorLog( "\nLUA ERROR: %s\n", lua_tostring( L, -1 ) );
  Error( S( "Lua error occurred! " ) + lua_tostring( L, -1 ) );
  lua_pop( L, 1 ); // pop the error string
  return 0;
}

// Dumps the contents of the Lua stack to the Visual Studio debug output
void LuaInterface::StackDump( void ) const
{
  int top = lua_gettop( L );

  ErrorLog( "\nLua Stack Dump:" );
  ErrorLog( "  Sizeof stack: %d", top );

  for(int i = 1; i <= top; ++i)
  {
    int type = lua_type(L,i);
    switch (type)
    {
    case LUA_TSTRING:   ErrorLog( "  %d: %s", i , lua_tostring(  L, i    ) ); break;
    case LUA_TBOOLEAN:  ErrorLog( "  %d: %d", i , lua_toboolean( L, i    ) ); break;
    case LUA_TNUMBER:   ErrorLog( "  %d: %g", i , lua_tonumber(  L, i    ) ); break;
    default:            ErrorLog( "  %d: %s", i , lua_typename(  L, type ) ); break;
    }
  }

  ErrorLog( "  -->End stack dump.\n" );
}

// Dumps the contents of the Lua stack to the Visual Studio debug output
void LuaInterface::StackDump( lua_State *L )
{
  int top = lua_gettop( L );

  ErrorLog( "\nLua Stack Dump:" );
  ErrorLog( "  Sizeof stack: %d", top );

  for(int i = 1; i <= top; ++i)
  {
    int type = lua_type(L,i);
    switch (type)
    {
    case LUA_TSTRING:   ErrorLog( "  %d: %s", i , lua_tostring(  L, i    ) ); break;
    case LUA_TBOOLEAN:  ErrorLog( "  %d: %d", i , lua_toboolean( L, i    ) ); break;
    case LUA_TNUMBER:   ErrorLog( "  %d: %g", i , lua_tonumber(  L, i    ) ); break;
    default:            ErrorLog( "  %d: %s", i , lua_typename(  L, type ) ); break;
    }
  }

  ErrorLog( "  -->End stack dump.\n" );
}

// Grabs the main global lua_State pointer
lua_State *LuaInterface::State( void )
{
  return L;
}

// Sets the lua global dt variable -- There is no concept of spaces within Lua
void LuaInterface::Update( float dt )
{
  CALL_LUA_FUNC( "LuaSetDT", dt );
}

// If you end up calling this function then you probably passed too many arguments to Lua?
void LuaInterface::CallLuaFunc( ... )
{
  Error( "Not enough overlords! Make more overlords.........." );
}

// Overload for no argument call to a function in Lua from C++
void LuaInterface::CallLuaFunc( const char *name )
{
  GenericCallLua( name, NULL, 0 );
}
