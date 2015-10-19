/*--------------------qMopey-Header--------------------
<Game Title>  : qMopey
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 9/15/2012
File Name     : CCMetaData.cpp
Purpose       : Definition of the MetaData class.
                The MetaData class gathers and stores
                information about data types. This
                information is important for
                automating processes, like
                serializationand script binding.
-----------------------------------------------------*/

#include "Precompiled.h"
#include <string>
#include <algorithm>
#include "CCVariantFunction.h"
#include "CCMetaData.h"
#include "CCTextSerialize.h"
#include "CCLuaReference.h"

#ifndef CC_REG_EXPLICIT

MetaCreator<VoidType> VoidTypeMetaCreator( "VoidMeta", 0, false );
void MetaCreator<VoidType>::RegisterMetaData( void )
{
}

#endif // CC_REG_EXPLICIT

Member::Member( const S& string, unsigned val, MetaData *m )
  : name( string )
  , offset( val )
  , meta( m )
  , next( NULL )
  , prop( NULL )
{
}

Member::~Member( )
{
}

const S &Member::Name( void ) const
{
  return name;
}

unsigned Member::Offset( void ) const
{
  return offset;
}

const MetaData *Member::Meta( void ) const
{
  return meta;
}

Property *Member::GetProperty( void ) const
{
  return prop;
}

Member *& Member::Next( void )
{
  return next;
}

Member *const& Member::Next( void ) const
{
  return next;
}

MetaData::MetaData( )
  : name( "" )
  , members( NULL )
{
}

S& CleanString( S& string )
{
  char chars[] = " ";

  for (unsigned int i = 0; i < strlen( chars ); ++i)
  {
    string.Remove( chars );
  }

  return string;
}

void MetaData::Init( S& string, unsigned val, bool pod )
{
  name = CleanString( string );
  size = val;
  toLua = NULL;
  fromLua = NULL;
  serialize = NULL;
  deserialize = NULL;
  notPOD = pod;
#ifndef CC_REG_EXPLICIT
  MetaManager::RegisterMeta( this );
#endif CC_REG_EXPLICIT
}

MetaData::~MetaData( )
{
  // Do not free these due to out-of-order destruction
  //if(members)
  //{
  //  for(Member *m = members; m;)
  //  {
  //    Member *temp = m->Next( );
  //    MetaManager::Allocator( ).Free( m, sizeof( Member ) );
  //    m = temp;
  //  }
  //}
}

bool MetaData::HasMembers( void ) const
{
  return (members) ? true : false;
}

void MetaData::AddMember( const Member *member )
{
  if(!members)
    members = const_cast<Member *>(member);
  else
    lastMember->Next( ) = const_cast<Member *>(member);

  lastMember = const_cast<Member *>(member);
}

bool MetaData::HasFunctions( void ) const
{
  return !functions.empty( );
}

void MetaData::AddFunction( VariantFunction *function )
{
  functions[function->Name( )] = function;
}

Conversion MetaData::GetConversion( const MetaData *meta ) const
{
  if(conversionFuncs.find( meta ) != conversionFuncs.end( ))
    return conversionFuncs.find( meta )->second;
  else
    return NULL;
}

void MetaData::AddConversion( const MetaData *meta, Conversion conv )
{
  conversionFuncs[meta] = conv;
}

void MetaData::SetToLua( ToLuaCB fn )
{
  toLua = fn;
}

void MetaData::ToLua( lua_State *lua, RefVariant var ) const
{
  if(toLua)
    toLua( lua, var );
  else
    GenericToLua( lua, var );
}

void MetaData::SetFromLua( FromLuaCB fn )
{
  fromLua = fn;
}

void MetaData::FromLua( lua_State *lua, int index, RefVariant& ref ) const
{
  if(fromLua)
    fromLua( lua, index, ref );
  else
    GenericFromLua( lua, index, ref );
}

void MetaData::SetSerializeMeCB( SerializeMeCB fn )
{
  serialize = fn;
}

void MetaData::PrintMembers( std::ostream& os ) const
{
  const Member *mem = members;

  os << "Members for Meta: " << name << std::endl;

  while(mem)
  {
    os << "  " << mem->Meta( )->Name( ) << " " << mem->Name( ) << std::endl;
    mem = mem->Next( );
  }
}

void MetaData::Serialize( File& file, RefVariant var ) const
{
  if(serialize)
    serialize( file, var );
  else
    TextSerializer::Get( ).Serialize( file, var );
}

void MetaData::SetDeserializeMeCB( DeserializeMeCB fn )
{
  deserialize = fn;
}

RefVariant MetaData::Deserialize( File& file ) const
{
  if(deserialize)
    return deserialize( file, this );
  else
  {
    return TextSerializer::Get( ).Deserialize( file );
  }
}

void MetaData::SetEnumTable( const char **table )
{
  ENUM_TABLE = table;
}

const char **MetaData::GetEnumTable( void ) const
{
  return ENUM_TABLE;
}

void MetaData::SetEnumTableSize( unsigned size )
{
  ENUM_TABLE_SIZE = size;
}

unsigned MetaData::GetEnumTableSize( void ) const
{
  return ENUM_TABLE_SIZE;
}

VariantFunction *MetaData::GetFunction( const std::string name ) const
{
  if(functions.find( name ) != functions.end( ))
  {
    return functions.find( name )->second;
  }
  return NULL;
}

bool MetaData::isPOD( void ) const
{
  if(notPOD)
    return false;
  return true;
}

#ifndef CC_REG_EXPLICIT

// Insert a MetaData into the map of objects
void MetaManager::RegisterFn( VariantFunction *instance )
{
  FnMap &map = GetFnMap( );
  map.Add( instance, instance->Name( ) );
}

// Retrieve a MetaData instance by string name from the map of MetaData objects
VariantFunction *MetaManager::GetFn( const S& name )
{
  auto ptr = GetFnMap( )[name.c_str( )];
  return ptr ? *ptr : NULL;
}

#endif // CC_REG_EXPLICIT

unsigned MetaData::Size( void ) const
{
  return size;
}

const char *MetaData::Name( void ) const
{
  return name.c_str( );
}

const Member *MetaData::Members( void ) const
{
  return members;
}

const Member *MetaData::GetMember( const char *memberName ) const
{
  for(auto m = members; m; m = m->next)
  {
    if(std::strcmp( memberName, m->name.c_str( ) ) == 0)
      return m;
  }

  return NULL;
}

Property *MetaData::GetProperty( const char *memberName ) const
{
  for(auto m = members; m; m = m->next)
  {
    TODO( "Hash table for this, reduce O( n ) to O( 1 )" )
    if(std::strcmp( memberName, m->name.c_str( ) ) == 0)
      return m->GetProperty( );
  }

  return NULL;
}

bool MetaData::HasProperty( const char *memberName ) const
{
  for(auto m = members; m; m = m->next)
  {
    if(std::strcmp( memberName, m->name.c_str( ) ) == 0)
      return m->prop ? true : false;
  }

  return false;
}

const std::hash_map<const std::string, VariantFunction *> *MetaData::Functions( void ) const
{
  return &functions;
}

#ifndef CC_REG_EXPLICIT

void MetaManager::RegisterMeta( const MetaData *instance )
{
  MetaMap& map = GetMap( );
  map.Add( instance, instance->Name( ) );
}

const MetaData *MetaManager::Get( const char *name )
{
  auto ptr = GetMap( )[name];
  return ptr ? *ptr : NULL;
}

#endif // CC_REG_EXPLICIT
