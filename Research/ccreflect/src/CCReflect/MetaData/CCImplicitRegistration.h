/*--------------------qMopey-Header--------------------
<Game Title>  : qMopey
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 9/7/2013
File Name     : CCImplicitRegistration.h
Purpose       : 
-----------------------------------------------------*/

#pragma once

class Property;
class Variant;
class VariantBase;
class VariantFunction;
class RefVariant;
class MetaData;
class File;

#ifndef CC_REG_EXPLICIT

//
// DEFINE_META
// Purpose: Defines a MetaData object instance for a specific type of object. Also defines how to
//          register that instance's data members by defining the RegisterMetaData method, and
//          calling it on construction.
//          Example:
//            DEFINE_META( MyClass ) // Defines a MetaData instance of type MyClass
//            {
//              ADD_MEMBER( x ); // add member x of MyClass (MyClass::x) to the MetaData instance
//            }
// Notes  : You must declare the following within any class you wish to use this macro upon:
//          static void RegisterMetaData( void );
//            -This is required due to the ADD_MEMBER and REGISTER_FUNCTION macros requiring private
//             data access. Thus, the definition of the RegisterMetaData method must actually reside
//             within the object's definition. If access to the source code of the class you wish
//             to reflect isn't available, use the DEFINE_META_EXTERN macro -- no private data access
//             is available in such a scenario.
//
#define DEFINE_META( TYPE ) \
  MetaCreator<RemQualPtr<TYPE>::type> MetaInstance##TYPE( #TYPE, sizeof( TYPE ), true ); \
  void MetaCreator<RemQualPtr<TYPE>::type>::RegisterMetaData( void ) { TYPE::RegisterMetaData( ); } \
  void TYPE::RegisterMetaData( void )
#define DEFINE_META_EX( TYPE, INSTANCE_NAME ) \
  MetaCreator<RemQualPtr<TYPE>::type> INSTANCE_NAME( #TYPE, sizeof( TYPE ), true ); \
  void MetaCreator<RemQualPtr<TYPE>::type>::RegisterMetaData( void ) { TYPE::RegisterMetaData( ); } \
  void TYPE::RegisterMetaData( void )

// Forces linking (use this to link across static lib)
#define DECLARE_META( TYPE ) \
  extern "C" MetaCreator<RemQualPtr<TYPE>::type> MetaInstance##TYPE; \
  __pragma( comment ( linker, "/INCLUDE:_"__STRINGIZE( MetaInstance##TYPE ) ) )
#define DECLARE_META_EX( TYPE, INSTANCE_NAME ) \
  extern "C" MetaCreator<RemQualPtr<TYPE>::type> INSTANCE_NAME; \
  __pragma( comment ( linker, "/INCLUDE:_"__STRINGIZE( INSTANCE_NAME ) ) )

//
// META_DATA
// Purpose : This macro goes on the inside of a class within the public section. It declares
//           a few member functions for use by the MetaData system to retrieve information about
//           the class.
#define META_DATA( TYPE ) \
  typedef TYPE thisType; \
  static void RegisterMetaData( void )

// DEFINE_META_POD
// Works exactly like the DEFINE_META function, except optimized for Plain Old Datatypes.
#define DEFINE_META_POD( TYPE ) \
  MetaCreator<RemQualPtr<TYPE>::type> MetaInstance##TYPE( #TYPE, sizeof( TYPE ), false ); \
  void MetaCreator<RemQualPtr<TYPE>::type>::RegisterMetaData( void )
#define DEFINE_META_POD_EX( TYPE, INSTANCE_NAME ) \
  MetaCreator<RemQualPtr<TYPE>::type> INSTANCE_NAME( #TYPE, sizeof( TYPE ), false ); \
  void MetaCreator<RemQualPtr<TYPE>::type>::RegisterMetaData( void )

// DECLARE_META_ENUM
// Declares an enumeration MetaData instance, forces the compiler to link in a reference to the symbol
// associated with a global variable. The leading underscore is required as the compiler likes to add
// a leading underscore to all the MetaData instances.
#define DECLARE_META_ENUM( TYPE ) \
  extern "C" MetaCreator<RemQualPtr<E::TYPE>::type> MetaInstance##TYPE; \
  __pragma( comment ( linker, "/INCLUDE:_" __STRINGIZE( MetaInstance##TYPE ) ) )

// DEFINE_META_EXTERN
// External definition, no private data access. This means no direct access to private
// data members is supported. Useful for reflecting third party definitions that you don't have
// source access to.
#define DEFINE_META_EXTERN( TYPE ) \
  MetaCreator<RemQualPtr<TYPE>::type> MetaInstance##TYPE( #TYPE, sizeof( TYPE ), true ); \
  void MetaCreator<RemQualPtr<TYPE>::type>::RegisterMetaData( void )
#define DEFINE_META_EXTERN_EX( TYPE, INSTANCE_NAME ) \
  MetaCreator<RemQualPtr<TYPE>::type> INSTANCE_NAME( #TYPE, sizeof( TYPE ), true ); \
  void MetaCreator<RemQualPtr<TYPE>::type>::RegisterMetaData( void )

//
// ADD_MEMBER
// Purpose: For use within the definition of RegisterMetaData from the DEFINE_META macro.
//          This macro calls the AddMember method of the MetaCreator of a specific type.
//          Example:
//            DEFINE_META( MyClass ) // Defines a MetaData instance of type MyClass
//            {
//              ADD_MEMBER( x ); // add member x of MyClass (MyClass::x) to the MetaData instance
//            }
//
#define ADD_MEMBER( MEMBER ) \
  MetaCreator<thisType>::AddMember( #MEMBER, (uint32)(&((thisType *)0)->MEMBER), META( ((thisType *)0)->MEMBER ) )

#define ADD_PROPERTY( MEMBER, GETTOR, SETTOR ) \
  MetaCreator<thisType>::AddProperty( #MEMBER, &thisType::GETTOR, &thisType::SETTOR )

//
// REGISTER_FUNCTION
// Purpose : Adds a single function to a class MetaData instance.
// Param     NAME : Name of the function to be represented by in Lua
//           FN   : The function you want to bind. Be sure to use
//                  :: operator to reach functions inside a class
// Note           : The zero parameter is used just to call a specific
//                  constructor on the VariantFunction for creating
//                  and registering a VariantFunction at compile-time.
#define REGISTER_FUNCTION( NAME, FN ) \
  VariantFunction NAME##VariantFunction( #NAME, &FN, 0 )

// Only use these with the DEFINE_META macro
#define SET_TO_LUA( FN ) \
  MetaCreator<thisType>::SetToLua( FN )
#define SET_FROM_LUA( FN ) \
  MetaCreator<thisType>::SetFromLua( FN )
#define SET_SERIALIZE( FN ) \
  MetaCreator<thisType>::SetSerializeMeCB( FN )
#define SET_DESERIALIZE( FN ) \
  MetaCreator<thisType>::SetDeserializeMeCB( FN )

//
// META_TYPE
// Purpose: Retrieves the proper MetaData instance of an object by type.
//
#define META_TYPE( TYPE ) (MetaCreator<RemQualPtr<TYPE>::type>::Get( ))
#define META_TYPE_PTR( TYPE ) (MetaCreator<RemQual<TYPE>::type>::Get( ))

//
// META
// Purpose: Retrieves the proper MetaData instance of an object by an object's type.
//
#define META( OBJECT ) (MetaCreator<RemQualPtr<decltype( OBJECT )>::type>::Get( ))
#define META_PTR( OBJECT ) (MetaCreator<RemQual<decltype( OBJECT )>::type>::Get( ))

//
// META_STR
// Purpose : Finds a MetaData instance by string name
//
#define META_STR( STRING ) (MetaManager::Get( STRING ))

//
// GET_META_FN
// Purpose : Grabs a function (method) from a MetaData class instance.
//
#define GET_META_FN( OBJECT, NAME ) \
  META( OBJECT )->GetFunction( NAME )

#define SET_ENUM_TABLE( table ) \
  SetEnumTable( E::table, sizeof( E::table ) / sizeof( E::table[0] ) )

#define ADD_CONVERSION( _TO_META_, CONVERSION ) \
  AddConversion( META_TYPE( _TO_META_ ), CONVERSION )

//
// MetaManager
// Purpose: Just a collection of some functions for management of all the
//          various MetaData objects.
//
class MetaManager
{
public:
  typedef HashTable<const MetaData *, const char *> MetaMap;
  typedef HashTable<VariantFunction *, const char *> FnMap;

  // Insert a MetaData into the map of objects
  static void RegisterMeta( const MetaData *instance );

  // Retrieve a MetaData instance by string name from the map of MetaData objects
  static const MetaData *Get( const char *name );

  static void RegisterFn( VariantFunction *instance );

  // Retrieve a MetaData instance by string name from the map of MetaData objects
  static VariantFunction *GetFn( const S& name );

  static FnMap& GetFnMap( void )
  {
    static FnMap functionMap;
    return functionMap;
  }

  // Safe and easy singleton for map of MetaData objects
  static MetaMap& GetMap( void )
  {
    // Define static map here, so no need for explicit definition
    static MetaMap map;
    return map;
  }

  static BlockAllocator& Allocator( void )
  {
    static BlockAllocator instance;
    return instance;
  }
};

//
// MetaCreator
// Purpose: Used to create a single MetaData object of a specific type.
// Notes  : You might be wondering why an entire template is needed to construct a MetaData object,
//          since the MetaData class has its own constructor. This templated class is needed so that
//          we can have templated creation of each MetaData instance. However, the MetaData class
//          cannot be templated, otherwise we wouldn't be able to hold all of the MetaData objects in
//          a single array easily, as a templated object can vary in size depending on the type.
//
//          Only a single MetaCreator should ever be made for each type of object. The DEFINE_META macro
//          creates an instance of the MetaCreator for a specific type of object. See top of file for more info
//          on the DEFINE_META macro.
//
template <typename MetaType>
class MetaCreator
{
public:
  MetaCreator( const char *name, unsigned size, bool POD ) : typeName( name )
  {
    Init( typeName, size, POD );
  }

  static MetaData *Get( void )
  {
    static MetaData instance;
    return &instance;
  }

  static void Init( S& name, unsigned size, bool notPOD )
  {
    MetaData *m = Get( );
    m->Init( name, size, notPOD );

    if(notPOD)
    {
      typedef std::conditional<HasDefaultCtor<MetaType>::value, MetaType, EmptyType<MetaType>>::type new_type;
      m->New = setter<HasDefaultCtor<MetaType>::value, void *(*)( void ), &New<new_type>>::get( );
      m->PlacementNew = setter<HasDefaultCtor<MetaType>::value, void (*)( void * ), &PlacementNew<new_type>>::get( );
      m->Copy = setter<HasCopyCtor<MetaType>::value, void (*)( void *, const void * ), &Copy<new_type>>::get( );
      m->Delete = Delete<MetaType>;
      m->PlacementCopy = setter<HasCopyCtor<MetaType>::value, void (*)( void *, const void * ), &PlacementCopy<new_type>>::get( );
      m->PlacementDelete = PlacementDelete<MetaType>;
      m->NewCopy = setter<HasCopyCtor<MetaType>::value, void (*)( void **, const void * ), &NewCopy<new_type>>::get( );
    }
    else
    {
      m->New = PODNew<MetaType>;
      m->PlacementNew = PODPlacementNew<MetaType>;
      m->Copy = PODCopy<MetaType>;
      m->Delete = PODDelete<MetaType>;
      m->PlacementCopy = PODPlacementCopy<MetaType>;
      m->PlacementDelete = PODPlacementDelete<MetaType>;
      m->NewCopy = PODNewCopy<MetaType>;
    }

    RegisterMetaData( );
  }

  static void AddMember( const char *memberName, unsigned memberOffset, MetaData *meta )
  {
    Member *mem = (Member *)MetaManager::Allocator( ).Allocate( sizeof( Member ) );
    new (mem) Member( S( memberName ), memberOffset, meta );
    Get( )->AddMember( mem );
  }

  template <typename Gettor, typename Settor>
  static void AddProperty( const char *memberName, Gettor gettor, Settor settor )
  {
    ((Member *)Get( )->GetMember( memberName ))->SetProperty( gettor, settor );
  }

  static void AddFunction( VariantFunction *function )
  {
    MetaManager::RegisterFn( function );
    Get( )->AddFunction( function );
  }

  static void AddConversion( const MetaData *meta, Conversion conv )
  {
    Get( )->AddConversion( meta, conv );
  }

  static void SetToLua( ToLuaCB fn )
  {
    Get( )->SetToLua( fn );
  }

  static void SetFromLua( FromLuaCB fn )
  {
    Get( )->SetFromLua( fn );
  }

  static void SetSerializeMeCB( SerializeMeCB fn )
  {
    Get( )->SetSerializeMeCB( fn );
  }

  static void SetDeserializeMeCB( DeserializeMeCB fn )
  {
    Get( )->SetDeserializeMeCB( fn );
  }

  static void SetEnumTable( const char **table, unsigned size )
  {
    Get( )->SetEnumTable( table );
    Get( )->SetEnumTableSize( size );
  }

  static void RegisterMetaData( void );

private:
  S typeName;
  typedef MetaType thisType;
};

#endif // CC_REG_EXPLICIT
