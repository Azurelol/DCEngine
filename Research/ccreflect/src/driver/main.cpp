#include "Precompiled.h"
#include <cstdio>
#include <Windows.h>
#include "Object.h"

struct Inner
{
  Inner( )
    : x( 5 )
  {
  }

  // This is required to use the DEFINE_META macro
  META_DATA( Inner );

  int x;
};

#ifndef CC_REG_EXPLICIT

// Used to define metadata for a type you do have source code acecss to
DEFINE_META( Inner )
{
  ADD_MEMBER( x );
}

#endif // CC_REG_EXPLICIT

struct Outer
{
  Outer( )
    : inner( )
    , name( "TheName" )
  {
  }

  void SetName( const char *_name ) { name = _name; }
  std::string& GetName( void ) { return name; }

  Inner inner;
  std::string name;
};

#ifndef CC_REG_EXPLICIT

// Used to define metadata for a type that you do not have source code access to
// (we have source code access to Outer, but I'm just demonstrating it)
DEFINE_META_EXTERN( Outer )
{
  ADD_MEMBER( inner );
  ADD_MEMBER( name );
  ADD_PROPERTY( name, GetName, SetName );
}

#endif // CC_REG_EXPLICIT

struct NewType
{
};

// Test serialization and property
void test0( void )
{
  const MetaData *m = META_TYPE( NewType );

  // Open a file for writing to
  File f;
  f.Open( "object.txt", FileAccess::Write );

  // Construct an object and serialize it, then close file
  Outer o;
  META( o )->Serialize( f, RefVariant( o ) );
  f.Close( );

  // Open file again to deserialize
  f.Open( "object.txt", FileAccess::Read );
  RefVariant ref = META_TYPE( Outer )->Deserialize( f );

  // Get object out of deserialized RefVariant and close file
  o = ref.GetValue<Outer>( );
  f.Close( );

  // Get a property, bind to an object and set the property
  Property *p = META_TYPE( Outer )->GetProperty( "name" );
  p->Bind( o );
  p->Set( "NewName!" );
}

// Coroutines, timers, lua stuff, etc.
int main( void )
{
  // Create explicit metadata and register information within it
  // CALL ONLY ONCE
  RegisterDefaultMetaData( );

  // Inner registration
  MetaData *meta = DEFINE_META( Inner );
  meta->AddMember( DEFINE_MEMBER( Inner, x ) );

  // Outer registration
  meta = DEFINE_META( Outer );
  meta->AddMember( DEFINE_MEMBER( Outer, inner ) );
  Member *member = DEFINE_MEMBER( Outer, name );
  meta->AddMember( member );
  member->SetProperty( &Outer::GetName, &Outer::SetName );
  DEFINE_META_POD( Outer * );

  // Register metadata for the Object type
  Object::RegisterMetaData( );

  // Test serialization and property
  test0( );

  // Load a lua file to hold coroutine scripts and script routines
  luaL_dofile( LuaInterface::Get( ).State( ), "../src/driver/test.lua" );

  Clock c;
  Timer timer( c );
  const float dt = .01f;
  float accumulator = 0.0f;

  // Set dt within lua
  LuaInterface::Get( ).Update( dt );

  // Create a test object with name ObjectInMain and have it register
  // for an update function in Lua called TestUpdateRoutine
  Object obj( "ObjectInMain", "TestUpdateRoutine" );
  
  // Serializing an object
  std::cout << "\nSerializing an object:\n";
  RefVariant( obj ).Serialize( STDOUT );

  std::cout << "\nEntering main loop...\n\n";
  while(true)
  {
    accumulator += timer.Elapsed( );
    timer.Start( );

    // Simulate engine delay
    Sleep( 10 );

    while(accumulator > dt)
    {
      // Update our object. The Object's update routine in written in Lua and is implemented
      // with a Lua coroutine.
      obj.Update( dt );
      accumulator -= dt;
    }
  }

  return 0;
}
