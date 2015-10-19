/*--------------------qMopey-Header--------------------
<Game Title>  : qMopey
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 9/15/2012
File Name     : MetaData.h
Purpose       : Declaration of the MetaData class.
                The MetaData class gathers and stores
                information about data types. This
                information is important for
                automating processes, like
                serializationand script binding.
-----------------------------------------------------*/

#pragma once

#include <iostream>
#include <hash_map>
#include <type_traits>
#include "CCLuaIncludes.h"
#include "RemoveQualifiers.h"

class Property;
class Variant;
class VariantBase;
class VariantFunction;
class RefVariant;
class MetaData;
class File;

struct VoidType
{
  // ...
};

template <typename T>
struct EmptyType
{
  // ...
};

template <bool, class T, T v>
struct setter
{
  static T get( void )
  {
    return v;
  };
};

template <class T, T v>
struct setter<false, T, v>
{
  static T get( void )
  {
    return NULL;
  };
};

typedef void (*ToLuaCB)( lua_State *, RefVariant );
typedef void (*FromLuaCB)( lua_State *, int, RefVariant& );
typedef void (*SerializeMeCB)( File&, RefVariant );
typedef RefVariant (*DeserializeMeCB)( File&, const MetaData * );

//
// Member
// Purpose: Stores information (name and offset of member) about a data member of a specific class. Multiple
//          Member objects can be stored in MetaData objects within a std::vector.
// Note   : Construction of members for a MetaData instance invocates the creation of
//          a singly-linked list of Member objects using the Next function within the
//          member class.
//
class Member
{
public:
  Member( const S& string, unsigned val, MetaData *m );
  ~Member( );

  const S &Name( void ) const; // Gettor for name
  unsigned Offset( void ) const; // Gettor for offset
  const MetaData *Meta( void ) const; // Gettor for data
  Property *GetProperty( void ) const;
  template <typename Gettor, typename Settor>
  void SetProperty( Gettor gettor, Settor settor );

  Member *& Next( void );
  Member *const& Next( void ) const;

private:
  S name;
  Property *prop;
  unsigned offset;
  const MetaData *meta;
  Member *next;

  friend class MetaData;
};

template <typename Gettor, typename Settor>
void Member::SetProperty( Gettor gettor, Settor settor )
{
#ifndef CC_REG_EXPLICIT
  prop = MetaManager::Allocator( ).Allocate<Property>( );
  new (prop) Property( gettor, settor );
#else
  prop = new Property( gettor, settor );
#endif // CC_REG_EXPLICIT
}

typedef Variant (*Conversion)( VariantBase * );

struct MetaConvert
{
  const MetaData *meta;
  Conversion convert;
};

template <typename T>
inline T *Cast( void *data )
{
  return reinterpret_cast<T *>(data);
}

template <typename T>
inline const T *Cast( const void *data )
{
  return reinterpret_cast<const T *>(data);
}

template <typename T>
inline void *New( void )
{
#ifndef CC_REG_EXPLICIT
  void *data = MetaManager::Allocator( ).Allocate( sizeof( T ) );
  new (data) T( );
#else
  void *data = new T( );
#endif // CC_REG_EXPLICIT
  return data;
}

template <typename T>
inline void *PODNew( void )
{
#ifndef CC_REG_EXPLICIT
  return MetaManager::Allocator( ).Allocate( sizeof( T ) );
#else
  return operator new( sizeof( T ) );
#endif // CC_REG_EXPLICIT
}

PERM_DSIABLE_WARNING( 4345 )
template <typename T>
inline void PlacementNew( void *data )
{
  new (data) T( );
}

template <typename T>
inline void PODPlacementNew( void *data )
{
}

template <typename T>
inline void PlacementDelete( void* data )
{
  Cast<T>(data)->~T( );
}

template <typename T>
inline void PODPlacementDelete( void* data )
{
}

template <typename T>
inline void Delete( void *data )
{
#ifndef CC_REG_EXPLICIT
  Cast<T>(data)->~T( );
  MetaManager::Allocator( ).Free( data, sizeof( T ) );
#else
  delete (T *)data;
#endif // CC_REG_EXPLICIT
}

template <typename T>
inline void PODDelete( void *data )
{
#ifndef CC_REG_EXPLICIT
  MetaManager::Allocator( ).Free( data, sizeof( T ) );
#else
  delete (T *)data;
#endif // CC_REG_EXPLICIT
}

template <typename T>
inline void PlacementCopy( void *data, const void *src )
{
  new (data) T( *(Cast<T>( src )) );
}

template <typename T>
inline void PODPlacementCopy( void *data, const void *src )
{
  std::memcpy( data, src, sizeof( T ) );
}

template <typename T>
inline void Copy( void *dest, const void *src )
{
  *(Cast<T>( dest )) = *(Cast<T>( src ));
}

template <typename T>
inline void PODCopy( void *dest, const void *src )
{
  std::memcpy( dest, src, sizeof( T ) );
}

template <typename T>
inline void NewCopy( void **dest, const void *src )
{
  *dest = new T( *(Cast<T>( src )) );
}

template <typename T>
inline void PODNewCopy( void **dest, const void *src )
{
#ifndef CC_REG_EXPLICIT
  *dest = MetaManager::Allocator( ).Allocate( sizeof( T ) );
#else
  *dest = operator new( sizeof( T ) );
#endif // CC_REG_EXPLICIT
  std::memcpy( *dest, src, sizeof( T ) );
}

//
// MetaData
// Purpose: Object for holding various info about any C++ type for the MetaData reflection system.
//
class MetaData
{
public:
  friend class TextSerializer;

  MetaData( );
  ~MetaData( );

  void Init( S& string, unsigned val, bool pod );
  bool HasMembers( void ) const;
  void AddMember( const Member *member );
  bool HasFunctions( void ) const;
  void AddFunction( VariantFunction *function );
  Conversion GetConversion( const MetaData *meta ) const;
  void AddConversion( const MetaData *meta, Conversion conv );
  void SetToLua( ToLuaCB fn = NULL );
  void ToLua( lua_State *lua, RefVariant var ) const;
  void SetFromLua( FromLuaCB fn = NULL );
  void FromLua( lua_State *lua, int index, RefVariant& ref ) const;
  void PrintMembers( std::ostream& os ) const;
  void SetSerializeMeCB( SerializeMeCB fn = NULL );
  void Serialize( File& file, RefVariant ) const;
  void SetDeserializeMeCB( DeserializeMeCB fn = NULL );
  RefVariant Deserialize( File& file ) const;
  void SetEnumTable( const char **table );
  const char **GetEnumTable( void ) const;
  void SetEnumTableSize( unsigned size );
  unsigned GetEnumTableSize( void ) const;

  // Various gettors
  unsigned Size( void ) const;
  const char *Name( void ) const;
  const Member *Members( void ) const;
  const Member *GetMember( const char *memberName ) const;
  Property *GetProperty( const char *memberName ) const;
  bool HasProperty( const char *memberName ) const;
  const std::hash_map<const std::string, VariantFunction *> *Functions( void ) const;
  VariantFunction *GetFunction( std::string name ) const;
  bool isPOD( void ) const;

  void *(*New)( void );
  void (*Copy)( void *data, const void *src );
  void (*Delete)( void *data );
  void (*PlacementCopy)( void *data, const void *src );
  void (*PlacementNew)( void *data );
  void (*PlacementDelete)( void *data );
  void (*NewCopy)( void **dest, const void *src );

private:
  unsigned size;
  S name;
  ToLuaCB toLua;
  FromLuaCB fromLua;
  SerializeMeCB serialize;
  DeserializeMeCB deserialize;
  const char **ENUM_TABLE;
  unsigned ENUM_TABLE_SIZE;
  bool notPOD;

  Member *members;
  Member *lastMember;
  std::hash_map<const std::string, VariantFunction *> functions;
  std::hash_map<const MetaData *, Conversion> conversionFuncs;
};
