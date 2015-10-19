/*--------------------qMopey-Header--------------------
<Game Title>  : qMopey
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 2/2/2013
File Name     : 
Purpose       : 
-----------------------------------------------------*/
#include "Precompiled.h"

#define print( str, ... ) \
  printf( str "\n", __VA_ARGS__ )

const Enum *currentEnumeration = NULL;

void PrintEnumEntries( const Enum *e )
{
  for(uint32 i = 0; i < e->m_literals.Size( ); ++i)
  {
    const S& name = e->m_literals[i];
    const Enum *entry = Enum::GetEnumByString( name );
    if(entry)
      print( "    %d %s ...", i, name.c_str( ) );
    else
      print( "    %d %s", i, name.c_str( ) );
  }
}

#ifdef COMPILE_RUNTIME_EDITOR
void AddNewEnum( void )
{
  print( "" );
  print( "Enter a name for this enumeration:" );
  print( "" );
  printf( ">>> " );
  std::string input;
  std::cin >> input;

  if(currentEnumeration)
    ((Enum *)currentEnumeration)->AddEntry( input.c_str( ) );

  // Create a new root enum
  else
  {
    Enum *e = (Enum *)malloc( sizeof( Enum ) );
    new (e) Enum( input.c_str( ), NULL );
  }
}

void RemoveEnum( Enum *e )
{
  if(e)
  {
    // Erase entry from parent
    if(e->m_parent)
    {
      const S& name = e->m_name;
      for(uint32 i = 0; i < e->m_literals.Size( ); ++i)
      {
        if(e->m_literals[i] == name)
        {
          e->m_literals.Erase( i );
        }
      }
    }

    e->m_active = false;
    e->RemoveFromMap( );
    currentEnumeration = e->m_parent;
  }
}
#endif // COMPILE_RUNTIME_EDITOR

void PrintAllTopLevelEnums( void )
{
  const Enum *e = Enum::Head( );
  uint32 i = 0;

  while(e)
  {
#ifdef COMPILE_RUNTIME_EDITOR
    if(!e->m_parent && e->m_active)
#else
    if(!e->m_parent)
#endif // COMPILE_RUNTIME_EDITOR
      print( "    %d %s ...", i++, e->m_name.c_str( ) );
    e = e->Next( );
  }

  print( "" );
}

// Sets current enumeration for viewing
void SetCurrentEnum( uint32 num )
{
  if(currentEnumeration)
  {
    for(uint32 i = 0; i < currentEnumeration->m_literals.Size( ); ++i)
    {
      if(i == num)
      {
        const S& name = currentEnumeration->m_literals[i];
        Enum *found = Enum::GetEnumByString( name );
        if(found)
          currentEnumeration = found;
        else
        {
          print( "\nThe enum %s does not have a sub-level.", name.c_str( ) );
          print( "Do you want to create one? y/n" );
          print( "" );
          print( ">>> " );
          std::string input;
          std::cin >> input;
          if(input[0] == 'y' || input[0] == 'Y')
          {
            const Enum *parent = currentEnumeration;
            currentEnumeration = (Enum *)malloc( sizeof( Enum ) );
            new ((Enum *)currentEnumeration) Enum( name.c_str( ), parent );
          }
        }
      }
    }
  }
  else
  {
    const Enum *e = Enum::Head( );
    uint32 i = 0;

    while(e)
    {
      if(!e->m_parent)
      {
        if(i == num)
        {
          currentEnumeration = Enum::GetEnumByString( e->m_name );
          break;
        }

        ++i;
      }
      e = e->Next( );
    }
  }
}

// Recursively save all enum branches starting from root
void SaveEnumToEnumDataHeader( File& file, const Enum *e )
{
#ifdef COMPILE_RUNTIME_EDITOR
  if(e->m_active)
#endif // COMPILE_RUNTIME_EDITOR
  {
    file.Write( "E_NAME( %s )\n", e->m_name.c_str( ) );
    file.Write( "  E_PARENT( %s )\n", e->m_parent ? e->m_parent->m_name.c_str( ) : "NULL" );

    for(uint32 i = 0; i < e->m_literals.Size( ); ++i)
      file.Write( "  E_ENTRY( %s )\n", e->m_literals[i].c_str( ) );
    file.Write( "  E_ENTRY( count_%s )\n", e->m_name.c_str( ) );

    file.Write( "E_END( %s )\n", e->m_name.c_str( ) );
    file.Write( "\n" );
  }

  // Recursively save all child enums
  for(uint32 i = 0; i < e->m_literals.Size( ); ++i)
  {
    Enum *child = Enum::GetEnumByString( e->m_literals[i] );
    if(child)
      SaveEnumToEnumDataHeader( file, child );
  }
}

bool MainMenu( void )
{
  print( "" );

  if(!currentEnumeration)
  {
    print( "Viewing: Root level enumerations" );
    print( "  #  - Select an enum to view" );
#ifdef COMPILE_RUNTIME_EDITOR
    print( "  a  - Add a new enum" );
    print( "  d  - Delete an enum" );
    print( "  s  - save" );
#endif // COMPILE_RUNTIME_EDITOR
    print( "  q  - exit" );
    print( "" );
    PrintAllTopLevelEnums( );
    printf( ">>> " );
  }
  else
  {
    print( "Viewing: %s", currentEnumeration->m_name.c_str( ) );
    print( "  #  - Select an enum to view" );
#ifdef COMPILE_RUNTIME_EDITOR
    print( "  a  - Add a new enum to this branch" );
    print( "  d  - Delete an entry to %s", currentEnumeration->m_name.c_str( ) );
#endif // COMPILE_RUNTIME_EDITOR
    const Enum *parent = currentEnumeration->m_parent;
    print( "  .. - Go back to %s", parent ? parent->m_name.c_str( ) : "Root level enumerations" );
#ifdef COMPILE_RUNTIME_EDITOR
    print( "  s  - save" );
#endif // COMPILE_RUNTIME_EDITOR
    print( "  q  - exit" );
    print( "" );
    PrintEnumEntries( currentEnumeration );
    print( "" );
    printf( ">>> " );
  }

  std::string input;
  std::cin >> input;
  char c = input[0];

  switch(c)
  {
#ifdef COMPILE_RUNTIME_EDITOR
  case 'd':
    {
      if(!currentEnumeration)
      {
        print( "" );
        print( "Enter the name of the enum to delete:" );
        print( "" );
        printf( ">>> " );
        std::string input;
        std::cin >> input;

        Enum *e = Enum::GetEnumByString( S( input.c_str( ) ) );
        if(e && !e->m_parent)
        {
          RemoveEnum( e );
        }
        else
          print( "\nERROR : Could not find the enumeration %s!", input.c_str( ) );
      }
      else
      {
        print( "" );
        print( "Enter the name of the entry to delete:" );
        print( "" );
        printf( ">>> " );
        std::string input;
        std::cin >> input;
        const S name( input.c_str( ) );
        for(uint32 i = 0; i < currentEnumeration->m_literals.Size( ); ++i)
        {
          if(currentEnumeration->m_literals[i] == name)
          {
            ((Enum *)currentEnumeration)->m_literals.Erase( i );
            Enum *entry = Enum::GetEnumByString( name );
            if(entry)
              RemoveEnum( entry );
          }
        }
      }
    }
    break;
  case 'a':
    AddNewEnum( );
    break;
#endif // COMPILE_RUNTIME_EDITOR
  case 'q':
    return 0;
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
    SetCurrentEnum( c - '0' );
    break;
  case '.':
    if(input == ".." && currentEnumeration)
      currentEnumeration = currentEnumeration->m_parent;
    break;
#ifdef COMPILE_RUNTIME_EDITOR
  case 's':
    {
      time_t t;
      time( &t );
      File EnumData;
      EnumData.Open( "../src/CCReflect/Enum/EnumData.h", FileAccess::Write );
      EnumData.Write( "/*-----------------Alloctopus-Header--------------------\n" );
      EnumData.Write( "Copyright (C) 2013 DigiPen Institute of Technology.\n" );
      EnumData.Write( "\n" );
      EnumData.Write( "Creator(s)    : Randy Gaul\n" );
      EnumData.Write( "Creation Date : %s\n", ctime( &t ) );
      EnumData.Write( "File Name     : EnumData.h\n" );
      EnumData.Write( "Purpose       : \n" );
      EnumData.Write( "Note          : Lower case e to access enum entries.\n" );
      EnumData.Write( "                Upper case E to access enum types.\n" );
      EnumData.Write( "------------------------------------------------------*/\n" );
      EnumData.Write( "\n" );
      EnumData.Write( "// WARNING : THIS FILE IS AUTO-GENERATED BY THE ENUM EDITOR\n" );
      EnumData.Write( "\n" );
      const Enum *e = Enum::Head( );
      while(e)
      {
        if(!e->m_parent)
          SaveEnumToEnumDataHeader( EnumData, e );
        e = e->Next( );
      }
    }
    break;
#endif // COMPILE_RUNTIME_EDITOR
  }

  return true;
}

int main( void )
{
  print( "Welcome to the Enumeration Editor!" );
  print( "" );

  while( MainMenu( ) )
    ;

  return 0;
}
