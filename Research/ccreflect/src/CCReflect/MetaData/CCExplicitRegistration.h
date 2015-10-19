/*--------------------qMopey-Header--------------------
<Game Title>  : qMopey
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 9/7/2013
File Name     : CCExplicitRegistration.h
Purpose       : 
-----------------------------------------------------*/

#pragma

template <typename T>
struct StripMemberness;

template <typename T, typename U>
struct StripMemberness<T U::*>
{
  typedef T type;
};
//
// META_DATA
// Purpose : This macro goes on the inside of a class within the public section. It declares
//           a single RegisterMetaData function, of which needs to be manually written in
//           a cpp file. This function must also be manually called.
//
#define META_DATA( type ) \
  static void RegisterMetaData( );

//
// META_TYPE
// Purpose: Retrieves the proper MetaData instance of an object by type.
//
#define META_TYPE( T ) \
  META_MANAGER->GetMeta<RemQualPtr<T>::type>( )
#define META_TYPE_PTR( T ) \
  META_MANAGER->GetMeta<RemQual<T>::type>( )

//
// META
// Purpose: Retrieves the proper MetaData instance of an object by an object's type.
//
#define META( OBJECT ) \
  META_MANAGER->GetMeta<RemQual<decltype( OBJECT )>::type>( )
#define META_PTR( OBJECT ) \
  META_MANAGER->GetMeta<RemQualPtr<decltype( OBJECT )>::type>( )

//
// META_STR
// Purpose : Finds a MetaData instance by string name
//
#define META_STR( str ) \
  META_MANAGER->GetMeta( str )

//
// DEFINE_META
// Purpose: Defines a MetaData object instance for a specific type of object, and returns
//          the type info MetaData object.
//
#define DEFINE_META( T ) \
  META_MANAGER->RegisterMeta<T>( #T, sizeof( T ), true )

//
// DEFINE_META
// Purpose: Defines a MetaData object instance for a specific type of object, and returns
//          the type info MetaData object. Speficially for POD data types.
//
#define DEFINE_META_POD( T ) \
  META_MANAGER->RegisterMeta<T>( #T, sizeof( T ), false )

//
// DEFINE_MEMBER
// Purpose: Creates and returns a Member * object. This pointer can be passed to the
//          AddMember function within a MetaData instance.
//
#define DEFINE_MEMBER( T, MEMBER ) \
  META_MANAGER->CreateMember( #MEMBER, (unsigned)(&((T *)0)->MEMBER), META_TYPE( StripMemberness<decltype(&T::MEMBER)>::type ) )

//
// REGISTER_FUNCTION
// Purpose: Creates a VariantFunction on the heap, and registers it within the
//          global MetaManager object. This binds the given function to Lua.
//          The NAME parameter is the name of the function, as a string, to be
//          seen from within Lua.
//
#define REGISTER_FUNCTION( NAME, FN ) \
  new VariantFunction( #NAME, &FN, 0 )

template <typename T>
struct GetMetaHelper
{
  static MetaData **GetPtr( )
  {
    static MetaData *m = NULL;
    return &m;
  }

  static void SetPtr( MetaData *meta )
  {
    MetaData **m = GetPtr( );
    if(*m)
      return;
    *m = meta;
  }

  static MetaData *Get( )
  {
    return *GetPtr( );
  }
};

class MetaManager
{
public:
  MetaManager( );
  ~MetaManager( );

  void Insert( VariantFunction *instance );
  void GetFunction( const char *name );
  const MetaData *GetMeta( const char *name );
  Member *CreateMember( const char *memberName, unsigned memberOffset, MetaData *meta );

  template <typename T>
  MetaData *GetMeta( )
  {
    return GetMetaHelper<T>::Get( );
  }

  template <typename T>
  MetaData *RegisterMeta( const char *name, unsigned size, bool POD );

private:
  HashTable<VariantFunction *, const char *> m_fnMap;
  HashTable<const MetaData *, const char *> m_metaMap;
  BlockAllocator m_allocator;
};

extern MetaManager *META_MANAGER;

template <typename T>
MetaData *MetaManager::RegisterMeta( const char *name, unsigned size, bool POD )
{
  MetaData *m = (MetaData *)m_allocator.Allocate( sizeof( MetaData ) );
  new (m) MetaData( );
  m->Init( S( name ), size, POD );

  if(POD)
  {
    typedef std::conditional<HasDefaultCtor<T>::value, T, EmptyType<T>>::type new_type;
    m->New = setter<HasDefaultCtor<T>::value, void *(*)( void ), &New<new_type>>::get( );
    m->PlacementNew = setter<HasDefaultCtor<T>::value, void (*)( void * ), &PlacementNew<new_type>>::get( );
    m->Copy = setter<HasCopyCtor<T>::value, void (*)( void *, const void * ), &Copy<new_type>>::get( );
    m->Delete = Delete<T>;
    m->PlacementCopy = setter<HasCopyCtor<T>::value, void (*)( void *, const void * ), &PlacementCopy<new_type>>::get( );
    m->PlacementDelete = PlacementDelete<T>;
    m->NewCopy = setter<HasCopyCtor<T>::value, void (*)( void **, const void * ), &NewCopy<new_type>>::get( );
  }
  else
  {
    m->New = PODNew<T>;
    m->PlacementNew = PODPlacementNew<T>;
    m->Copy = PODCopy<T>;
    m->Delete = PODDelete<T>;
    m->PlacementCopy = PODPlacementCopy<T>;
    m->PlacementDelete = PODPlacementDelete<T>;
    m->NewCopy = PODNewCopy<T>;
  }

  GetMetaHelper<T>::SetPtr( m );
  m_metaMap.Add( m, name );

  return m;
}
