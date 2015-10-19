/*-------------------qMopey-Header----------------------
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 2/13/2013
File Name     : InHashBase.h
Purpose       : 
Notes         : Can currently hash:
                  S, uint32, int32, const char *

                Find an item:
                  in the table use the
                  [] operator. NULL return means not in
                  the table.

                Insert an item:
                  Use the Add function.

                Remove an item:
                  Use the Remove function.
------------------------------------------------------*/

#pragma once

#include "Link.h"

#define HASH_DECL( TYPE, KEY, NODE_NAME ) \
  InHash<TYPE, KEY, OFFSET_OF( TYPE, NODE_NAME )> 

#define __IH_GET_NODE( DATA ) \
  (*reinterpret_cast<InKey<T, K> *>((PTR_ADD( (DATA), link_offset ))))

template <typename T, typename K>
class InKey : public Link<T>
{
public:
    /*----------------
       Ctors/Dtors
    ----------------*/
  InKey( );
  // Currently supported key types: const char *
  //                                int32
  //                                uint32
  InKey( K key );

    /*----------------
        Modifiers
    ----------------*/
  void SetKey( const K& key );
  InKey& operator=( const  InKey& rhs );

    /*----------------
       Data Access
    ----------------*/
  uint32 GetHash( void ) const;
  const K& GetKey( void ) const; // User must implement within T!

private:
  uint32 h; // hash
  K k;      // key
};

template <typename T, typename K>
class InHashBase
{
public:
    /*----------------
       Ctors/Dtors
    ----------------*/
  InHashBase( uint32 offset, uint32 rows = 7 );
  ~InHashBase( );

    /*----------------
        Modifiers
    ----------------*/
  void Add( T& toInsert );
  void Add( T *toInsert );
  void Add( InKey<T, K>& toInsert );
  void Remove( T& toRemove );
  void Remove( T *toRemove );
  void Remove( InKey<T, K>& toRemove );
  void Remove( const K& key );
  void UnlinkAll( void );
  void DeleteAll( void ); // unlinks then calls delete on object
  
    /*----------------
       Data Access
    ----------------*/
  T *operator[]( uint32 k );
  const T *operator[]( uint32 k ) const;
  T *operator[]( int32 k );
  const T *operator[]( int32 k ) const;
  T *operator[]( const char *k );
  const T *operator[]( const char *k ) const;
  T *operator[]( const S& k );
  const T *operator[]( const S& k ) const;
  void PrintInterals( void ) const;

private:
  uint32 link_offset;
  uint32 rows;
  InKey<T, K> *buckets;
  //f32 load_factor;
  //f32 max_load_factor;

  // Not currently supported
  InHashBase& operator=( const InHashBase& rhs );
  InHashBase( const InHashBase& rhs );
};

// InHash definition
// Uses some simple template metaprogramming to capture offset and key as template parameters. This
// allows for a more natural use of the HASH_DECL macro.
template <typename T, typename K, uint32 offset>
class InHash : public InHashBase<T, K>
{
public:
    InHash( uint32 rows );
};

template <typename T, typename K, uint32 offset>
InHash<T, K, offset>::InHash( uint32 rows ) : InHashBase<T, K>( offset, rows )
{
}

/*------------------------------------------------------------------------------------------------

                                    Constructors / Destructors

------------------------------------------------------------------------------------------------*/
    /*----------------
          Inkey
    ----------------*/
// Currently supported key types: const char *
//                                int32
//                                uint32
template <typename T, typename K>
InKey<T, K>::InKey( K key ) : Link<T>( ), k( key ), h( Hash( key ) )
{
}

template <typename T, typename K>
InKey<T, K>::InKey( ) : Link<T>( )
{
}

    /*----------------
        InHashBase
    ----------------*/
template <typename T, typename K>
InHashBase<T, K>::InHashBase( uint32 offset, uint32 rows_ ) : link_offset( offset ), rows( rows_ )
{
  assert( rows ); // Cannot have table size of zero!
  buckets = reinterpret_cast<InKey<T, K> *>(new InKey<T, K>[rows]);
}

template <typename T, typename K>
InHashBase<T, K>::~InHashBase( )
{
  delete [] reinterpret_cast<InKey<T, K> *>(buckets);
}

/*------------------------------------------------------------------------------------------------

                                            Modifiers

------------------------------------------------------------------------------------------------*/
    /*----------------
          Inkey
    ----------------*/
template <typename T, typename K>
void InKey<T, K>::SetKey( const K& key )
{
  h = Hash( key );
  k = key;
}

template <typename T, typename K>
InKey<T, K>& InKey<T, K>::operator=( const InKey& rhs )
{
  reinterpret_cast<Link<T> *>(this)->operator=( rhs );
  k = rhs.k;
  h = rhs.h;
}

    /*----------------
        InHashBase
    ----------------*/
template <typename T, typename K>
void InHashBase<T, K>::Add( T& toInsert )
{
  InKey<T, K>& ik = __IH_GET_NODE( &toInsert );
  ik.InsertAfter( buckets[ik.GetHash( ) % rows] );
}

template <typename T, typename K>
void InHashBase<T, K>::Add( T *toInsert )
{
  InKey<T, K>& ik = __IH_GET_NODE( toInsert );
  ik.InsertAfter( buckets[ik.GetHash( ) % rows] );
}

template <typename T, typename K>
void InHashBase<T, K>::Add( InKey<T, K>& toInsert )
{
  toInsert.InsertAfter( buckets[toInsert.GetHash( )] );
}


template <typename T, typename K>
void InHashBase<T, K>::Remove( T& toRemove )
{
  __IH_GET_NODE( &toRemove ).Unlink( );
}

template <typename T, typename K>
void InHashBase<T, K>::Remove( T *toRemove )
{
  __IH_GET_NODE( toRemove ).Unlink( );
}

template <typename T, typename K>
void InHashBase<T, K>::Remove( InKey<T, K>& toRemove )
{
  toRemove.Unlink( );
}

template <typename T, typename K>
void InHashBase<T, K>::Remove( const K& key )
{
  const uint32 h = Hash( key ) % rows;
  for(auto i = buckets[h].next; i != buckets + h; i = i->next)
  {
    if(reinterpret_cast<InKey<T, K> *>(i)->GetKey( ) == key)
    {
      i->Unlink( );
      break;
    }
  }
}

template <typename T, typename K>
void InHashBase<T, K>::UnlinkAll( void )
{
  for(uint32 i = 0; i < rows; ++i)
    for(auto j = buckets[i].next; j != buckets + i;)
    {
      auto temp = j;
      temp = temp->next;
      j->Unlink( );
      j = temp;
    }
}

// unlinks then calls delete on object
template <typename T, typename K>
void InHashBase<T, K>::DeleteAll( void )
{
  for(uint32 i = 0; i < rows; ++i)
    for(auto j = buckets[i].next; j != buckets + i;)
    {
      auto temp = j;
      temp = temp->next;
      j->Unlink( );
      delete &__IL_GET_DATA( j );
      j = temp;
  }
}

/*------------------------------------------------------------------------------------------------

                                           Data Access

------------------------------------------------------------------------------------------------*/
    /*----------------
          Inkey
    ----------------*/
template <typename T, typename K>
uint32 InKey<T, K>::GetHash( void ) const
{
  return h;
}

template <typename T, typename K>
const K& InKey<T, K>::GetKey( void ) const
{
  return k;
}

    /*----------------
        InHashBase
    ----------------*/
template <typename T, typename K>
T *InHashBase<T, K>::operator[]( const char *k )
{
  const uint32 h = Hash( k ) % rows;
  for(auto i = buckets[h].next; i != buckets + h; i = i->next)
  {
    if(strcmp( reinterpret_cast<InKey<T, K> *>(i)->GetKey( ), k ) == 0)
      return &__IL_GET_DATA( i );
  }

  // Not found in table!
  return NULL;
}

template <typename T, typename K>
T *InHashBase<T, K>::operator[]( uint32 k )
{
  const uint32 h = Hash( k ) % rows;
  for(auto i = buckets[h].next; i != buckets + h; i = i->next)
  {
    if(reinterpret_cast<InKey<T, K> *>(i)->GetKey( ) == k)
      return &__IL_GET_DATA( i );
  }

  // Not found in table!
  return NULL;
}

template <typename T, typename K>
T *InHashBase<T, K>::operator[]( int32 k )
{
  const uint32 h = Hash( (uint32)k ) % rows;
  for(auto i = buckets[h].next; i != buckets + h; i = i->next)
  {
    if(reinterpret_cast<InKey<T, K> *>(i)->GetKey( ) == k)
      return &__IL_GET_DATA( i );
  }

  // Not found in table!
  return NULL;
}

template <typename T, typename K>
T *InHashBase<T, K>::operator[]( const S& k )
{
  const uint32 h = k.GetHash( ) % rows;
  for(auto i = buckets[h].next; i != buckets + h; i = i->next)
  {
    if(reinterpret_cast<InKey<T, K> *>(i)->GetKey( ) == k)
      return &__IL_GET_DATA( i );
  }

  // Not found in table!
  return NULL;
}

template <typename T, typename K>
const T *InHashBase<T, K>::operator[]( const char *k ) const
{
  const uint32 h = Hash( k ) % rows;
  for(auto i = buckets[h].next; i != buckets + h; i = i->next)
  {
    if(strcmp( reinterpret_cast<InKey<T, K> *>(i)->GetKey( ), k ) == 0)
      return &__IL_GET_DATA( i );
  }

  // Not found in table!
  return NULL;
}

template <typename T, typename K>
const T *InHashBase<T, K>::operator[]( uint32 k ) const
{
  const uint32 h = Hash( k ) % rows;
  for(auto i = buckets[h].next; i != buckets + h; i = i->next)
  {
    if(reinterpret_cast<InKey<T, K> *>(i)->GetKey( ) == k)
      return &__IL_GET_DATA( i );
  }

  // Not found in table!
  return NULL;
}

template <typename T, typename K>
const T *InHashBase<T, K>::operator[]( int32 k ) const
{
  const uint32 h = Hash( (uint32)k ) % rows;
  for(auto i = buckets[h].next; i != buckets + h; i = i->next)
  {
    if(reinterpret_cast<InKey<T, K> *>(i)->GetKey( ) == k)
      return &__IL_GET_DATA( i );
  }

  // Not found in table!
  return NULL;
}

template <typename T, typename K>
const T *InHashBase<T, K>::operator[]( const S& k ) const
{
  const uint32 h = k.GetHash( ) % rows;
  for(auto i = buckets[h].next; i != buckets + h; i = i->next)
  {
    if(reinterpret_cast<InKey<T, K> *>(i)->GetKey( ) == k)
      return &__IL_GET_DATA( i );
  }

  // Not found in table!
  return NULL;
}

template <typename T, typename K>
void InHashBase<T, K>::PrintInterals( void ) const
{
  for(uint32 i = 0; i < rows; ++i)
  {
    for(auto j = buckets[i].next; j != buckets + i; j = j->next)
    {
      printf( "key %u - ", reinterpret_cast<InKey<T, K> *>(j)->GetKey( ) );
      printf( "hash %u\n", reinterpret_cast<InKey<T, K> *>(j)->GetHash( ) );
    }
  }
}
