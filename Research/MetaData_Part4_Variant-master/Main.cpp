// This work is licensed under a Creative Commons Attribution 3.0 Unported License.
// http://creativecommons.org/licenses/by/3.0/deed.en_US

#include "Precompiled.h"
#include "Object.h"

DEFINE_META_POD( int );
DEFINE_META_POD( int * );
DEFINE_META_POD( char );
DEFINE_META_POD( char * );
DEFINE_META_POD( float );
DEFINE_META_POD( bool );
DEFINE_META_POD( double );
DEFINE_META_POD( std::string );

void main( void )
{
  META_TYPE( Object )->PrintMembers( std::cout );

  Variant v = 1;

  v = true;

  v = std::string( "std::string!\n" );

  std::cout << v.GetValue<std::string>( );

  int x = 5;

  v = &x;

  std::cout << *v.GetValue<int *>( ) << std::endl;

  const char *c = "Char *!\n";

  v = c;

  std::cout << v.GetValue<char *>( );

  // This is not supported! Cannot take address of a string literal the way we need to
  // v = "String literal!\n";

  getchar( );
}
