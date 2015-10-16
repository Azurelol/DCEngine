#include "Precompiled.h"
#include "Object.h"

DEFINE_META_POD( int );
DEFINE_META_POD( int * );
DEFINE_META_POD( char );
DEFINE_META_POD( char * );
DEFINE_META_POD( float );
DEFINE_META_POD( std::string );

#define PRINT_MEMBERS( TYPE ) \
  PrintMembers<TYPE>( #TYPE )

template <typename TYPE>
void PrintMembers( const char *type )
{
  std::cout << std::endl << "MetaData for " << type << " wrapper:" << std::endl;
  std::cout << META_TYPE( TYPE )->Name( ) << std::endl;
  std::cout << META_TYPE( TYPE )->Size( ) << std::endl;

  std::cout << "{" << std::endl;
  for(auto i = META_TYPE( TYPE )->members.begin( ); i != META_TYPE( TYPE )->members.end( ); ++i)
  {
    std::cout << "  " << (*i)->Name( ) << std::endl;
    std::cout << "  " << (*i)->Offset( ) << std::endl;
  }
  std::cout << "}" << std::endl;
}

void main( void )
{
  std::cout << META_TYPE( int )->Name( ) << std::endl;
  std::cout << META_TYPE( int )->Size( ) << std::endl;
  std::cout << std::endl;
  std::cout << META_TYPE( float )->Name( ) << std::endl;
  std::cout << META_TYPE( float )->Size( ) << std::endl;
  std::cout << std::endl;
  std::cout << META_TYPE( char * )->Name( ) << std::endl;
  std::cout << META_TYPE( char * )->Size( ) << std::endl;
  std::cout << std::endl;
  std::cout << META_TYPE( std::string )->Name( ) << std::endl;
  std::cout << META_TYPE( std::string )->Size( ) << std::endl;
  std::cout << std::endl;

  PRINT_MEMBERS( Object );

  S string( "String within the S wrapper!" );
  std::cout << std::endl << string << std::endl;

  PRINT_MEMBERS( S );

  const MetaData *meta = META( "asdf" );
  if(meta != META_TYPE( const char * ))
    std::cout << "ERROR!";

  // Ensure string literal meta is supported
  if(META_TYPE( const char * ) != META( "Testing with a string literal!" ))
    std::cout << "ERROR2!";

  getchar( );
}