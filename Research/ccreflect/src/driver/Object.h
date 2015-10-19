#pragma once

class Object;

class Object
{
public:
  Object( );
  Object( const std::string& n, const char *s );
  ~Object( );

  void Update( float dt );
  const std::string& GetName( void ) const;

  int PrintName( void ) const;
  void Print( int x ) const;
  void PrintALotOfStuff( int x, int y, int z ) const;

  META_DATA( Object );

private:
  LuaScript script;
  const char *scriptName;
  std::string name;
};
