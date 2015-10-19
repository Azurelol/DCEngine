#include "Precompiled.h"
#include "CCTextSerializeTemplates.h"
#include "CCTextSerialize.h"

template <>
void SerializePOD<int>( File& file, RefVariant var )
{
  file.Write( "%s ", var.GetMeta( )->Name( ) );
  file.Write( "%d\n", var.GetValue<int>( ) );
}

template <>
void SerializePOD<unsigned>( File& file, RefVariant var )
{
  file.Write( "%s ", var.GetMeta( )->Name( ) );
  file.Write( "%u\n", var.GetValue<unsigned>( ) );
}

template <>
void SerializePOD<float>( File& file, RefVariant var )
{
  file.Write( "%s ", var.GetMeta( )->Name( ) );
  file.Write( "%f\n", var.GetValue<float>( ) );
}

template <>
void SerializePOD<double>( File& file, RefVariant var )
{
  file.Write( "%s ", var.GetMeta( )->Name( ) );
  file.Write( "%lf\n", var.GetValue<double>( ) );
}

template <>
void SerializePOD<bool>( File& file, RefVariant var )
{
  file.Write( "%s ", var.GetMeta( )->Name( ) );
  if(var.GetValue<bool>( ))
    file.Write( "true\n" );
  else
    file.Write( "false\n" );
}

template <>
void SerializePOD<char>( File& file, RefVariant var )
{
  file.Write( "%s ", var.GetMeta( )->Name( ) );
  file.Write( "%c\n", var.GetValue<char>( ) );
}

template <>
void SerializePOD<char *>( File& file, RefVariant var )
{
  file.Write( "%s ", var.GetMeta( )->Name( ) );
  file.Write( "\"%s\"\n", *(char **)var.GetData( ) );
}

template <>
RefVariant DeserializePOD<int>( File& file, const MetaData *meta )
{
  if(!meta)
    meta = META_TYPE( int );
  else if( meta != TextSerializer::Get( ).DeserializePeekType( file, TextSerializer::Get( ).GetLevel( )) )
    Error( "Type mismatch! You must be reading in the wrong "
      "specified type as to what the member actually is!" );
  RefVariant var( meta, meta->New( ) );
  file.Read( "%d", var.GetData( ) );
  return std::move( var );
}

template <>
RefVariant DeserializePOD<unsigned>( File& file, const MetaData *meta )
{
  if(!meta)
    meta = META_TYPE( unsigned );
  else if( meta != TextSerializer::Get( ).DeserializePeekType( file, TextSerializer::Get( ).GetLevel( )) )
    Error( "Type mismatch! You must be reading in the wrong "
      "specified type as to what the member actually is!" );
  RefVariant var( meta, meta->New( ) );
  file.Read( "%u", var.GetData( ) );
  return std::move( var );
}

template <>
RefVariant DeserializePOD<float>( File& file, const MetaData *meta )
{
  if(!meta)
    meta = META_TYPE( float );
  else if( meta != TextSerializer::Get( ).DeserializePeekType( file, TextSerializer::Get( ).GetLevel( )) )
    Error( "Type mismatch! You must be reading in the wrong "
      "specified type as to what the member actually is!" );
  RefVariant var( meta, meta->New( ) );
  file.Read( "%f", var.GetData( ) );
  return std::move( var );
}

template <>
RefVariant DeserializePOD<double>( File& file, const MetaData *meta )
{
  if(!meta)
    meta = META_TYPE( double );
  else if( meta != TextSerializer::Get( ).DeserializePeekType( file, TextSerializer::Get( ).GetLevel( )) )
    Error( "Type mismatch! You must be reading in the wrong "
      "specified type as to what the member actually is!" );
  RefVariant var( meta, meta->New( ) );
  file.Read( "%lf", var.GetData( ) );
  return std::move( var );
}

template <>
RefVariant DeserializePOD<bool>( File& file, const MetaData *meta )
{
  if(!meta)
    meta = META_TYPE( bool );
  else if( meta != TextSerializer::Get( ).DeserializePeekType( file, TextSerializer::Get( ).GetLevel( )) )
    Error( "Type mismatch! You must be reading in the wrong "
      "specified type as to what the member actually is!" );
  RefVariant var( meta, meta->New( ) );
  S temp = file.GetLine( );
  S true_s( "true" );
  if(true_s == temp)
    var.GetValue<bool>( ) = true;
  else
    var.GetValue<bool>( ) = false;
  return std::move( var );
}

template <>
RefVariant DeserializePOD<char>( File& file, const MetaData *meta )
{
  if(!meta)
    meta = META_TYPE( char );
  else if( meta != TextSerializer::Get( ).DeserializePeekType( file, TextSerializer::Get( ).GetLevel( )) )
    Error( "Type mismatch! You must be reading in the wrong "
      "specified type as to what the member actually is!" );
  RefVariant var( meta, meta->New( ) );
  file.Read( "%c", var.GetData( ) );
  return std::move( var );
}

template <>
RefVariant DeserializeVector<std::string>( File& file, const MetaData *meta )
{
  meta = META_TYPE( std::vector<std::string> );
  RefVariant var( meta, meta->New( ) );
  std::vector<std::string> &vec = var.GetValue<std::vector<std::string>>( );
  int& level = TextSerializer::Get( ).GetLevel( );
  int stopLevel = level;
  ErrorIf( TextSerializer::Get( ).DeserializePeekType( file, stopLevel ) != meta, "Mismatch of type within vector! This vector is not of this type." );
  for(;;)
  {
    const MetaData *type = TextSerializer::Get( ).DeserializePeekType( file, stopLevel );
    if(!type)
    {
      break;
    }
    else if(META_TYPE( std::string ) != type)
    {
      Error( "Mismatch of type in text file, to what type this vector holds!" );
      continue;
    }
    RefVariant temp = META_TYPE( std::string )->Deserialize( file );
    vec.push_back( temp.GetValue<std::string>( ) );
    if(!level)
      break;
  }
  return std::move( var );
}

template <>
RefVariant DeserializeVector<bool>( File& file, const MetaData *meta )
{
  meta = META_TYPE( std::vector<bool> );
  RefVariant var( meta, meta->New( ) );
  std::vector<bool> &vec = var.GetValue<std::vector<bool>>( );
  int& level = TextSerializer::Get( ).GetLevel( );
  int stopLevel = level;
  ErrorIf( TextSerializer::Get( ).DeserializePeekType( file, stopLevel ) != meta, "Mismatch of type within vector! This vector is not of this type." );
  for(;;)
  {
    const MetaData *type = TextSerializer::Get( ).DeserializePeekType( file, stopLevel );
    if(!type)
    {
      break;
    }
    else if(META_TYPE( bool ) != type)
    {
      Error( "Mismatch of type in text file, to what type this vector holds!" );
      continue;
    }
    RefVariant temp = META_TYPE( bool )->Deserialize( file );
    vec.push_back( temp.GetValue<bool>( ) );
    temp.Delete( );
    if(!level)
      break;
  }
  return std::move( var );
}
