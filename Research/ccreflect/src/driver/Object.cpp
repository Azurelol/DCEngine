#include "Precompiled.h"
#include "Object.h"

typedef Object *ObjectPtr;

#ifndef CC_REG_EXPLICIT

DEFINE_META( Object )
{
  ADD_MEMBER( scriptName );
}

DEFINE_META_POD( ObjectPtr )
{
}
  
REGISTER_FUNCTION( PrintName, Object::PrintName );
REGISTER_FUNCTION( ObjectPrintInt, Object::Print );
REGISTER_FUNCTION( PrintALotOfStuff, Object::PrintALotOfStuff );

#else

void Object::RegisterMetaData( )
{
  MetaData *meta = DEFINE_META( Object );
  meta->AddMember( DEFINE_MEMBER( Object, scriptName ) );

  DEFINE_META_POD( ObjectPtr );

  REGISTER_FUNCTION( PrintName, Object::PrintName );
  REGISTER_FUNCTION( ObjectPrintInt, Object::Print );
  REGISTER_FUNCTION( PrintALotOfStuff, Object::PrintALotOfStuff );
}

#endif // CC_REG_EXPLICIT

Object::Object( ) :
  scriptName( "DefaultObject" ),
  name( "DefaultName" )
{
  LuaInterface::Get( ).CreateScript( script );
  script.Load( *this, scriptName );
}

Object::Object( const std::string& n, const char *s ) :
  scriptName( s ),
  name( n )
{
  LuaInterface::Get( ).CreateScript( script );
  script.Load( this, scriptName );
}

Object::~Object( )
{
}

int Object::PrintName( void ) const
{
  std::cout << "  " << name << std::endl;
  return 17;
}

void Object::Print( int x ) const
{
  std::cout << "  " << "Object::Print( int x )" << " " << x << std::endl;
}

void Object::PrintALotOfStuff( int x, int y, int z ) const
{
  std::cout << "  " << "PrintALotOfStuff( int x, int y, int z )" << " " << x << " " << y << " " << z << std::endl;
}

void Object::Update( float dt )
{
  script.Resume( );
}

const std::string& Object::GetName( void ) const
{
  return name;
}
