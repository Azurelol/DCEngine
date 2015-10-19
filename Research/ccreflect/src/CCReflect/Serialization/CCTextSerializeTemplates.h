#pragma once

template <typename TYPE>
void SerializeVector( File& file, RefVariant var )
{
  file.Write( "%s\n", var.GetMeta( )->Name( ) );
  int& level = TextSerializer::Get( ).GetLevel( );
  TextSerializer::Get( ).Padding( file, level++ );
  file.Write( "{\n" );
  std::vector<TYPE> &vec = var.GetValue<std::vector<TYPE>>( );
  for(unsigned i = 0; i < vec.size( ); ++i)
  {
    TextSerializer::Get( ).Padding( file, level );
    META( vec[i] )->Serialize( file, RefVariant( vec[i] ) );
  }
  TextSerializer::Get( ).Padding( file, --level );
  file.Write( "}\n" );
}

template <typename TYPE>
void SerializePOD( File& file, RefVariant var );

template <>
void SerializePOD<int>( File& file, RefVariant var );
template <>
void SerializePOD<unsigned>( File& file, RefVariant var );
template <>
void SerializePOD<float>( File& file, RefVariant var );
template <>
void SerializePOD<double>( File& file, RefVariant var );
template <>
void SerializePOD<bool>( File& file, RefVariant var );
template <>
void SerializePOD<char>( File& file, RefVariant var );
template <>
void SerializePOD<char *>( File& file, RefVariant var );

template <typename TYPE>
RefVariant DeserializePOD( File& file, const MetaData *meta = NULL );

template <>
RefVariant DeserializePOD<int>( File& file, const MetaData *meta );
template <>
RefVariant DeserializePOD<float>( File& file, const MetaData *meta );
template <>
RefVariant DeserializePOD<bool>( File& file, const MetaData *meta );
template <>
RefVariant DeserializePOD<char>( File& file, const MetaData *meta );

template <typename TYPE>
void SerializePtr( File& file, RefVariant var )
{
  file.Write( "%s ", var.GetMeta( )->Name( ) );
  file.Write( "%x\n", (uint32)var.GetValue<TYPE>( ) );
}

template <typename TYPE>
void SerializeEnum( File& file, RefVariant var )
{
  file.Write( "%s ", var.GetMeta( )->Name( ) );
  file.Write( "%s\n", (var.GetMeta( )->GetEnumTable( ))[var.GetValue<TYPE>( )] );
}

template <typename TYPE>
RefVariant DeserializePtr( File& file, const MetaData *meta = NULL )
{
  meta = META_TYPE( TYPE );
  RefVariant var( meta, meta->New( ) );
  uint32 temp;
  file.Read( "%x", &temp );
  var.GetValue<TYPE>( ) = (TYPE)temp;
  return std::move( var );
}

template <typename TYPE>
RefVariant DeserializeEnum( File& file, const MetaData *meta = NULL )
{
  if(!meta)
    meta = META_TYPE( TYPE );
  else if(meta != TextSerializer::Get( ).DeserializePeekType( is, TextSerializer::Get( ).GetLevel( ) ))
    Error( "Type mismatch! You must be reading in the wrong "
      "specified type as to what the member actually is!" );
  RefVariant var( meta, meta->New( ) );
  S temp = file.GetLine( );
  var.GetValue<TYPE>( ) = (TYPE)StrToEnum( temp.c_str( ), meta );
  return std::move( var );
}

template <typename TYPE>
RefVariant DeserializeVector( File& file, const MetaData *meta = NULL )
{
  meta = META_TYPE( std::vector<TYPE> );
  RefVariant var( meta, meta->New( ) );
  std::vector<TYPE> &vec = var.GetValue<std::vector<TYPE>>( );
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
    else if(META_TYPE( TYPE ) != type)
    {
      Error( "Mismatch of type! Someone has written a type to a file (probably by hand) that isn't the actual type of data in this vector." );
      continue;
    }
    RefVariant temp( META_TYPE( TYPE )->Deserialize( file ) );
    vec.push_back( temp.GetValue<TYPE>( ) );
    temp.Delete( );
    if(!level)
      break;
  }
  return std::move( var );
}

template <>
RefVariant DeserializeVector<std::string>( File& file, const MetaData *meta );

template <>
RefVariant DeserializeVector<bool>( File& file, const MetaData *meta );
