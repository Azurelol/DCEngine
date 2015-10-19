/*--------------------qMopey-Header--------------------
<Game Title>  : qMopey
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 9/18/2012
File Name     : CCTextSerialize.cpp
Purpose       : Serializes in text format any object
                that is registered with the meta
                data system.
-----------------------------------------------------*/
#include "Precompiled.h"
#include "CCTextSerialize.h"

#include <algorithm>

TextSerializer::TextSerializer( ) : level( 0 )
{
}

TextSerializer::~TextSerializer( )
{
}
  
// Serializes a single object recursively. Should not call this by hand, use the SERIALIZE
// macro instead (see top of file).
void TextSerializer::Serialize( File& file, RefVariant var )
{
  const MetaData *meta = var.GetMeta( );
  void *data = var.GetData( );

  file.Write( "%s\n", meta->Name( ) );
  Padding( file, level++ );
  file.Write( "{\n" );
  for(Member *member = const_cast<Member *>(meta->Members( )); member; member = member->Next( ))
  {
    Padding( file, level );
    const MetaData *memberMeta = member->Meta( );
    file.Write( "%s ", member->Name( ).c_str( ) );
    void *offsetData = PTR_ADD( var.GetData( ), member->Offset( ) );
    memberMeta->Serialize( file, RefVariant( memberMeta, offsetData ) );
  }
  Padding( file, --level );
  file.Write( "}\n" );
}
  
// Pads the output with a specific type of string; used by the Serialize function.
void TextSerializer::Padding( File& file, unsigned count )
{
  for(unsigned i = 0; i < count; ++i)
    file.Write( "  " );
}

int& TextSerializer::GetLevel( void )
{
  return level;
}

RefVariant TextSerializer::Deserialize( File& file, const MetaData *meta )
{
  if(!meta)
  {
    meta = DeserializePeekType( file );

    // Hit EOF
    if(!meta)
      return RefVariant( );
  }

  RefVariant var( meta, meta->New( ) );

  int startLevel = level;

  do
  {
    const Member *mem = DeserializePeekMember( file, meta->members, startLevel );
    if(mem)
    {
      RefVariant member;
      member = mem->Meta( )->Deserialize( file );
      member.GetMeta( )->Copy( PTR_ADD( var.GetData( ), mem->Offset( ) ), member.GetData( ) );
      member.Delete( );
    }
  } while(level > startLevel);

  return std::move( var );
}

const MetaData *TextSerializer::DeserializePeekType( File& file, int stopLevel )
{
  S type( 32, "" );

  for(;;)
  {
    type = file.GetLine( );

    if(type == "BREAK")
    {
      Error( "Breakpoint hit while deserializing!" );
      continue;
    }
    else if(type == "{")
    {
      ++level;
      continue;
    }
    else if(type == "}")
    {
      --level;
      if(level == stopLevel)
        return NULL;
      continue;
    }
    break;
  }

  if(file.Validate( ))
    return META_STR( type.c_str( ) );
  else
    return NULL;
}

const Member *TextSerializer::DeserializePeekMember( File& file, const Member *members, int startLevel )
{
  S memberName( 32, "" );

  for(;;)
  {
    memberName = file.GetLine( );

    if(memberName == "BREAK")
    {
      Error( "Breakpoint hit while deserializing!" );
      continue;
    }
    if(memberName == "{")
    {
      ++level;
      continue;
    }
    else if(memberName == "}")
    {
      --level;
      if(level == startLevel)
        return NULL;
      continue;
    }
    break;
  }

  for(Member *member = const_cast<Member *>(members); member; member = member->Next( ))
  {
    if(member->Name( ) == memberName)
      return member;
  }
  return NULL;
}
