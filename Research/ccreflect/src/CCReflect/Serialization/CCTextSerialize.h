/*--------------------qMopey-Header--------------------
<Game Title>  : qMopey
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 9/18/2012
File Name     : CCTextSerialize.h
Purpose       : Serializes in text format any object
                that is registered with the meta
                data system.
-----------------------------------------------------*/
#pragma once

#define SERIALIZER \
  TextSerializer::GetSerializer( )

  //
  // SERIALIZE
  // Purpose : Serializes a single object to file. Object must be registered within the meta
  //           data system. If no serializer has been initialized yet an error will show.
  //

#define SERIALIZE( OBJ ) \
  META( OBJ )->Serialize( OBJ )

#define DESERIALIZE( FS, TYPE ) \
    &TextSerializer::Get( ).Deserialize( FS, META_TYPE( GameObject ) ).GetValue<TYPE>( )

// Serializes objects to text format
class TextSerializer : public Singleton<TextSerializer>
{
public:
  friend class MetaData;
  // Serializes a single object recursively. Should not call this by hand, use the SERIALIZE
  // macro instead (see top of file).
  void Serialize( File& file, RefVariant var );

  RefVariant Deserialize( File& file, const MetaData *meta = NULL );

  // Pads the output with a specific type of string; used by the Serialize function.
  void Padding( File& file, unsigned count );
  const MetaData *DeserializePeekType( File& file, int stopLevel = 0 );
  int& GetLevel( void );
    
private:
  int level;
    
  friend class Singleton<TextSerializer>;
  TextSerializer( );
  ~TextSerializer( );
  
  const Member *DeserializePeekMember( File& file, const Member *members, int startLevel );
};
