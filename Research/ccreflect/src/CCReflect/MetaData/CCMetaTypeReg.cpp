/*--------------------qMopey-Header--------------------
<Game Title>  : qMopey
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 9/15/2012
File Name     : CCMetaTypeReg.cpp
Purpose       : Registers various types of data with
the MetaData class.
-----------------------------------------------------*/

#include "Precompiled.h"
#include "CCVariantFunction.h"
#include "CCLuaInterface.h"
#include "CCTextSerialize.h"
#include "CCTextSerializeTemplates.h"

void IntToLua( lua_State *L, RefVariant var )
{
  lua_pushinteger( L, var.GetValue<int>( ) );
}
void IntFromLua( lua_State *L, int index, RefVariant& ref )
{
  assert( lua_isnumber( L, index ) ); // Type mismatch! Passed wrong argument type in Lua call to C++ function?
  int temp = lua_tointeger( L, index );
  ref.GetMeta( )->PlacementCopy( ref.GetData( ), &temp );
}
Variant InttoFloat( VariantBase *var )
{
  return Variant( (float)var->GetValue<int>( ) );
}

void FloatToLua( lua_State *lua, RefVariant var )
{
  lua_pushnumber( lua, var.GetValue<float>( ) );
}
void FloatFromLua( lua_State *lua, int index, RefVariant& ref )
{
  lua_State *L = lua;
  ErrorIf( !lua_isnumber( lua, index ), "Type mismatch! Passed wrong argument type in Lua call to C++ function?" );
  float temp = float( lua_tonumber( lua, index ) );
  ref.GetMeta( )->PlacementCopy( ref.GetData( ), &temp );
}
Variant FloattoInt( VariantBase *var )
{
  return Variant( (int)var->GetValue<float>( ) );
}

void DoubleToLua( lua_State *lua, RefVariant var )
{
  lua_pushnumber( lua, var.GetValue<double>( ) );
}
void DoubleFromLua( lua_State *lua, int index, RefVariant& ref )
{
  lua_State *L = lua;
  ErrorIf( !lua_isnumber( lua, index ), "Type mismatch! Passed wrong argument type in Lua call to C++ function?" );
  double temp = double( lua_tonumber( lua, index ) );
  ref.GetMeta( )->PlacementCopy( ref.GetData( ), &temp );
}

void BoolToLua( lua_State *lua, RefVariant var )
{
  lua_pushboolean( lua, var.GetValue<bool>( ) );
}
void BoolFromLua( lua_State *lua, int index, RefVariant& ref )
{
  ErrorIf( !lua_isboolean( lua, index ), "Type mismatch! Passed wrong argument type in Lua call to C++ function?" );
  int temp = lua_toboolean( lua, index );
  bool temp2 = (temp) ? true : false;
  ref.GetMeta( )->PlacementCopy( ref.GetData( ), &temp2 );
}

void CharToLua( lua_State *lua, RefVariant var )
{
  lua_pushinteger( lua, var.GetValue<char>( ) );
}
void CharFromLua( lua_State *lua, int index, RefVariant& ref )
{
  ErrorIf( !lua_isnumber( lua, index ), "Type mismatch! Passed wrong argument type in Lua call to C++ function?" );
  char temp = char( (char)lua_tointeger( lua, index ) );
  ref.GetMeta( )->PlacementCopy( ref.GetData( ), &temp );
}

void UnsignedToLua( lua_State *lua, RefVariant var )
{
  lua_pushinteger( lua, var.GetValue<unsigned>( ) );
}
void UnsignedFromLua( lua_State *lua, int index, RefVariant& ref )
{
  ErrorIf( !lua_isnumber( lua, index ), "Type mismatch! Passed wrong argument type in Lua call to C++ function?" );
  unsigned temp = unsigned( lua_tointeger( lua, index ) );
  ref.GetMeta( )->PlacementCopy( ref.GetData( ), &temp );
}

void StringToLua( lua_State *lua, RefVariant var )
{
  lua_pushstring( lua, var.GetValue<std::string>( ).c_str( ) );
}
void StringFromLua( lua_State *lua, int index, RefVariant& ref )
{
  ErrorIf( !lua_isstring( lua, index ), "Type mismatch! Passed wrong argument type in Lua call to C++ function?" );
  std::string temp( lua_tostring( lua, index ) );
  ref.GetMeta( )->PlacementCopy( ref.GetData( ), &temp );
}
void SerializeString( File& file, RefVariant var )
{
  file.Write( "%s \"%s\"\n", var.GetMeta( )->Name( ), var.GetValue<std::string>( ).c_str( ) );
}
RefVariant DeserializeString( File& file, const MetaData *meta = NULL )
{
  meta = META_TYPE( std::string );
  RefVariant var( meta, meta->New( ) );
  ErrorIf( TextSerializer::Get( ).DeserializePeekType( file, TextSerializer::Get( ).GetLevel( ) ) != meta, "Type mismatch!" );
  file.Read( "%*c" ); // ignore one whitespace
  S temp = file.GetLine( );
  temp.Remove( "\"" );
  var.GetValue<std::string>( ) = temp.c_str( );
  return std::move( var );
}

void CharPToLua( lua_State *lua, RefVariant var )
{
  lua_pushstring( lua, var.GetValue<std::string>( ).c_str( ) );
}
void CharPFromLua( lua_State *lua, int index, RefVariant& ref )
{
  ErrorIf( !lua_isstring( lua, index ), "Type mismatch! Passed wrong argument type in Lua call to C++ function?" );
  const char *temp = lua_tostring( lua, index );
  ref = temp;
}

#ifndef CC_REG_EXPLICIT

DEFINE_META_POD( int )
{
  ADD_CONVERSION( float, InttoFloat );
  SetToLua( IntToLua );
  SetFromLua( IntFromLua );
  SetSerializeMeCB( SerializePOD<int> );
  SetDeserializeMeCB( DeserializePOD<int> );
}

DEFINE_META_POD( float )
{
  ADD_CONVERSION( int, FloattoInt );
  SetToLua( FloatToLua );
  SetFromLua( FloatFromLua );
  SetSerializeMeCB( SerializePOD<float> );
  SetDeserializeMeCB( DeserializePOD<float> );
}

DEFINE_META_POD_EX( float *, floatPtr_meta )
{
  SetSerializeMeCB( SerializePtr<float *> );
  SetDeserializeMeCB( DeserializePtr<float *> );
}

DEFINE_META_POD( double )
{
  SetToLua( DoubleToLua );
  SetFromLua( DoubleFromLua );
  SetSerializeMeCB( SerializePOD<double> );
  SetDeserializeMeCB( DeserializePOD<double> );
}

DEFINE_META_POD( bool )
{
  SetToLua( BoolToLua );
  SetFromLua( BoolFromLua );
  SetSerializeMeCB( SerializePOD<bool> );
  SetDeserializeMeCB( DeserializePOD<bool> );
}

DEFINE_META_POD( char )
{
  SetToLua( CharToLua );
  SetFromLua( CharFromLua );
  SetSerializeMeCB( SerializePOD<char> );
  SetDeserializeMeCB( DeserializePOD<char> );
}

DEFINE_META_POD( unsigned )
{
  SetToLua( UnsignedToLua );
  SetFromLua( UnsignedFromLua );
  SetSerializeMeCB( SerializePOD<unsigned> );
  SetDeserializeMeCB( DeserializePOD<unsigned> );
}

DEFINE_META_EXTERN_EX( std::string, string_meta ) // Corrupts memory if set to POD
{
  SetToLua( StringToLua );
  SetFromLua( StringFromLua );
  SetSerializeMeCB( SerializeString );
  SetDeserializeMeCB( DeserializeString );
}

DEFINE_META_POD_EX( char *, charPtr_meta )
{
  SetToLua( CharPToLua );
  SetFromLua( CharPFromLua );
  SetSerializeMeCB( SerializePOD<char *> );

  // No default deserialization due to need to new the char *
  //SetDeserializeMeCB( DeserializePOD<char *> );
}

DEFINE_META_EXTERN_EX( std::wstring, wstring_meta )
{
}

DEFINE_META_POD_EX( int *, intPtr_meta )
{
  SetSerializeMeCB( SerializePtr<int *> );
  SetDeserializeMeCB( DeserializePtr<int *> );
}

DEFINE_META_POD_EX( bool *, boolPtr_meta )
{
  SetSerializeMeCB( SerializePtr<bool *> );
  SetDeserializeMeCB( DeserializePtr<bool *> );
}

DEFINE_META_POD_EX( void *, voidPtr_meta )
{
  SetSerializeMeCB( SerializePtr<void *> );
  SetDeserializeMeCB( DeserializePtr<void *> );
}

DEFINE_META_EXTERN_EX( std::vector<int>, intVector_meta )
{
  SetSerializeMeCB( SerializeVector<int> );
  SetDeserializeMeCB( DeserializeVector<int> );
}
DEFINE_META_EXTERN_EX( std::vector<float>, floatVector_meta )
{
  SetSerializeMeCB( SerializeVector<float> );
  SetDeserializeMeCB( DeserializeVector<float> );
}
DEFINE_META_EXTERN_EX( std::vector<double>, doubleVector_meta )
{
  SetSerializeMeCB( SerializeVector<double> );
  SetDeserializeMeCB( DeserializeVector<double> );
}

DEFINE_META_EXTERN_EX( std::vector<bool>, boolVector_meta )
{
  SetSerializeMeCB( SerializeVector<bool> );
  SetDeserializeMeCB( DeserializeVector<bool> );
}
DEFINE_META_EXTERN_EX( std::vector<char>, charVector_meta )
{
  SetSerializeMeCB( SerializeVector<char> );
  SetDeserializeMeCB( DeserializeVector<char> );
}
DEFINE_META_EXTERN_EX( std::vector<unsigned>, unsignedVector_meta )
{
  SetSerializeMeCB( SerializeVector<unsigned> );
  SetDeserializeMeCB( DeserializeVector<unsigned> );
}

DEFINE_META_EXTERN_EX( std::vector<std::string>, stringVector_meta )
{
  SetSerializeMeCB( SerializeVector<std::string> );
  SetDeserializeMeCB( DeserializeVector<std::string> );
}

#else

void RegisterDefaultMetaData( )
{
  MetaManager *m = new MetaManager( );

  MetaData *meta = DEFINE_META_POD( int );
  meta->AddConversion( META_TYPE( float ), InttoFloat );
  meta->SetToLua( IntToLua );
  meta->SetFromLua( IntFromLua );
  meta->SetSerializeMeCB( SerializePOD<int> );
  meta->SetDeserializeMeCB( DeserializePOD<int> );

  meta = DEFINE_META_POD( float );
  meta->AddConversion( META_TYPE( int ), FloattoInt );
  meta->SetToLua( FloatToLua );
  meta->SetFromLua( FloatFromLua );
  meta->SetSerializeMeCB( SerializePOD<float> );
  meta->SetDeserializeMeCB( DeserializePOD<float> );

  meta = DEFINE_META_POD( float * );
  meta->SetSerializeMeCB( SerializePtr<float *> );
  meta->SetDeserializeMeCB( DeserializePtr<float *> );

  meta = DEFINE_META_POD( double );
  meta->SetToLua( DoubleToLua );
  meta->SetFromLua( DoubleFromLua );
  meta->SetSerializeMeCB( SerializePOD<double> );
  meta->SetDeserializeMeCB( DeserializePOD<double> );

  meta = DEFINE_META_POD( bool );
  meta->SetToLua( BoolToLua );
  meta->SetFromLua( BoolFromLua );
  meta->SetSerializeMeCB( SerializePOD<bool> );
  meta->SetDeserializeMeCB( DeserializePOD<bool> );

  meta = DEFINE_META_POD( char );
  meta->SetToLua( CharToLua );
  meta->SetFromLua( CharFromLua );
  meta->SetSerializeMeCB( SerializePOD<char> );
  meta->SetDeserializeMeCB( DeserializePOD<char> );

  meta = DEFINE_META_POD( unsigned );
  meta->SetToLua( UnsignedToLua );
  meta->SetFromLua( UnsignedFromLua );
  meta->SetSerializeMeCB( SerializePOD<unsigned> );
  meta->SetDeserializeMeCB( DeserializePOD<unsigned> );

  meta = DEFINE_META( std::string );
  meta->SetToLua( StringToLua );
  meta->SetFromLua( StringFromLua );
  meta->SetSerializeMeCB( SerializeString );
  meta->SetDeserializeMeCB( DeserializeString );

  meta = DEFINE_META_POD( char * );
  meta->SetToLua( CharPToLua );
  meta->SetFromLua( CharPFromLua );
  meta->SetSerializeMeCB( SerializePOD<char *> );
  // No default deserialization due to need to new the char *
  //SetDeserializeMeCB( DeserializePOD<char *> );

  DEFINE_META( std::wstring );

  meta = DEFINE_META_POD( int * );
  meta->SetSerializeMeCB( SerializePtr<int *> );
  meta->SetDeserializeMeCB( DeserializePtr<int *> );

  meta = DEFINE_META_POD( bool * );
  meta->SetSerializeMeCB( SerializePtr<bool *> );
  meta->SetDeserializeMeCB( DeserializePtr<bool *> );

  meta = DEFINE_META_POD( void * );
  meta->SetSerializeMeCB( SerializePtr<void *> );
  meta->SetDeserializeMeCB( DeserializePtr<void *> );

  meta = DEFINE_META( std::vector<int> );
  meta->SetSerializeMeCB( SerializeVector<int> );
  meta->SetDeserializeMeCB( DeserializeVector<int> );

  meta = DEFINE_META( std::vector<float> );
  meta->SetSerializeMeCB( SerializeVector<float> );
  meta->SetDeserializeMeCB( DeserializeVector<float> );

  meta = DEFINE_META( std::vector<double> );
  meta->SetSerializeMeCB( SerializeVector<double> );
  meta->SetDeserializeMeCB( DeserializeVector<double> );

  meta = DEFINE_META( std::vector<bool> );
  meta->SetSerializeMeCB( SerializeVector<bool> );
  meta->SetDeserializeMeCB( DeserializeVector<bool> );

  meta = DEFINE_META( std::vector<char> );
  meta->SetSerializeMeCB( SerializeVector<char> );
  meta->SetDeserializeMeCB( DeserializeVector<char> );

  meta = DEFINE_META( std::vector<unsigned> );
  meta->SetSerializeMeCB( SerializeVector<unsigned> );
  meta->SetDeserializeMeCB( DeserializeVector<unsigned> );

  meta = DEFINE_META( std::vector<std::string> );
  meta->SetSerializeMeCB( SerializeVector<std::string> );
  meta->SetDeserializeMeCB( DeserializeVector<std::string> );

  meta = META_MANAGER->RegisterMeta<VoidType>( "VoidMeta", 0, false );
}

#endif // CC_REG_EXPLICIT
