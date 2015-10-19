#include "Precompiled.h"
#include "Messenger.h"
#include "Listener.h"

struct TestObject
{
  TestObject( )
  {
    myString = "Called!\n";
  }

  void CallMe( const char *string )
  {
    printf( myString );
    printf( string );
  }

  Messenger m;
  const char *myString;
};


int main( void )
{
  TestObject o1;
  TestObject o2;

  RegisterDefaultMetaData( );
  MetaData *meta = DEFINE_META( TestObject );
  meta->AddMember( DEFINE_MEMBER( TestObject, myString ) );
  DEFINE_META_POD( TestObject * );

  o1.m.Subscribe( eTestMessage, &TestObject::CallMe, &o2 );
  o1.m.PostMSG( eTestMessage, "Argument passed from PostMSG.\n" );

  return 0;
}
