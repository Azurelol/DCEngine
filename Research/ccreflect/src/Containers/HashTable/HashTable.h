/*-----------------Alloctopus-Header--------------------
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : Sat May 04 10:48:36 2013

File Name     : HashTable.h
Purpose       : 
------------------------------------------------------*/

#pragma once

#include "SlotAllocator.h"

template <typename T, typename K>
class HashTable
{
public:
    /*----------------
       Ctors/Dtors
    ----------------*/
  HashTable( uint32 rows = 7, uint32 buckets = 14 );
  ~HashTable( );

    /*----------------
        Modifiers
    ----------------*/
  void Add( T val, const char *key );
  void Add( T val, const S& key );
  void Add( T val, uint32 key );
  void Add( T val, int32 key );
  void Remove( const char *key );
  void Remove( const S& key );
  void Remove( uint32 key );
  void Remove( int32 key );
  void UnlinkAll( void ); // UNTESTED

    /*----------------
       Data Access
    ----------------*/
  T *operator[]( const char *key );
  const T *operator[]( const char *key ) const;
  T *operator[]( const S& key );
  const T *operator[]( const S& key ) const;
  T *operator[]( uint32 key );
  const T *operator[]( uint32 key ) const;
  T *operator[]( int32 key );
  const T *operator[]( int32 key ) const;

private:
  struct Bucket
  {
      /*----------------
          Modifiers
      ----------------*/
    void Unlink( SlotAllocator<Bucket>& array, uint32 myIndex );
    void InsertBefore( SlotAllocator<Bucket>& array, uint32 myIndex, uint32 index );
    void InsertAfter( SlotAllocator<Bucket>& array, uint32 myIndex, uint32 index );

    // Sync next->prev and prev->next
    void Sync( SlotAllocator<Bucket>& array, uint32 myIndex );

    T m_val;
    K m_key;
    uint32 m_next;
    uint32 m_prev;
  };

  typedef SlotAllocator<Bucket> b_array;
  b_array m_array;
  uint32 m_rows;
};

/*------------------------------------------------------------------------------------------------

                                    Constructors / Destructors

------------------------------------------------------------------------------------------------*/
template <typename T, typename K>
HashTable<T, K>::HashTable( uint32 rows, uint32 buckets )
  : m_array( rows + buckets )
  , m_rows( rows )
{
  // Allocate the first rows
  for(uint32 i = 0; i < rows; ++i)
  {
    Bucket *bucket = m_array[m_array.Allocate( )];
    bucket->m_next = i;
    bucket->m_prev = i;
  }
}

template <typename T, typename K>
HashTable<T, K>::~HashTable( )
{
}

/*------------------------------------------------------------------------------------------------

                                            Modifiers

------------------------------------------------------------------------------------------------*/
template <typename T, typename K>
void HashTable<T, K>::Add( T val, const char *key )
{
  // Create new bucket from SlotAllocator
  uint32 index = m_array.Allocate( );
  Bucket *newBucket = m_array[index];
  newBucket->m_val = val;
  newBucket->m_key = key;

  // Insert into row specified by the hash
  newBucket->InsertAfter( m_array, index, Hash( key ) % m_rows );
}

template <typename T, typename K>
void HashTable<T, K>::Add( T val, const S& key )
{
  // Create new bucket from SlotAllocator
  uint32 index = m_array.Allocate( );
  Bucket *newBucket = m_array[index];
  newBucket->m_val = val;
  newBucket->m_key = key;

  // Insert into row specified by the hash
  newBucket->InsertAfter( m_array, index, key.GetHash( ) % m_rows );
}

template <typename T, typename K>
void HashTable<T, K>::Add( T val, uint32 key )
{
  // Create new bucket from SlotAllocator
  uint32 index = m_array.Allocate( );
  Bucket *newBucket = m_array[index];
  newBucket->m_val = val;
  newBucket->m_key = key;

  // Insert into row specified by the hash
  newBucket->InsertAfter( m_array, index, Hash( key ) % m_rows );
}

template <typename T, typename K>
void HashTable<T, K>::Add( T val, int32 key )
{
  // Create new bucket from SlotAllocator
  uint32 index = m_array.Allocate( );
  Bucket *newBucket = m_array[index];
  newBucket->m_val = val;
  newBucket->m_key = key;

  // Insert into row specified by the hash
  newBucket->InsertAfter( m_array, index, Hash( key ) % m_rows );
}

template <typename T, typename K>
void HashTable<T, K>::Remove( const char *key )
{
  const uint32 h = Hash( key ) % m_rows;

  for(auto i = m_array[m_array[h]->m_next]; i != m_array[h]; i = m_array[i->m_next])
  {
    if(strcmp( i->m_key, key ) == 0)
    {
      uint32 index = m_array.GetIndex( i );
      i->Unlink( m_array, index );
      m_array.Free( index );
      if(index != m_array.Size( ))
        m_array[index]->Sync( m_array, index );
      break;
    }
  }
}

template <typename T, typename K>
void HashTable<T, K>::Remove( const S& key )
{
  const uint32 h = key.GetHash( ) % m_rows;

  for(auto i = m_array[m_array[h]->m_next]; i != m_array[h]; i = m_array[i->m_next])
  {
    if(i->m_key == key)
    {
      uint32 index = m_array.GetIndex( i );
      i->Unlink( m_array, index );
      m_array.Free( index );
      if(index != m_array.Size( ))
        m_array[index]->Sync( m_array, index );
      break;
    }
  }
}

template <typename T, typename K>
void HashTable<T, K>::Remove( uint32 key )
{
  const uint32 h = Hash( key ) % m_rows;

  for(auto i = m_array[m_array[h]->m_next]; i != m_array[h]; i = m_array[i->m_next])
  {
    if(i->m_key == key)
    {
      uint32 index = m_array.GetIndex( i );
      i->Unlink( m_array, index );
      m_array.Free( index );
      if(index != m_array.Size( ))
        m_array[index]->Sync( m_array, index );
      break;
    }
  }
}

template <typename T, typename K>
void HashTable<T, K>::Remove( int32 key )
{
  const uint32 h = Hash( key ) % m_rows;

  for(auto i = m_array[m_array[h]->m_next]; i != m_array[h]; i = m_array[i->m_next])
  {
    if(i->m_key == key)
    {
      uint32 index = m_array.GetIndex( i );
      i->Unlink( m_array, index );
      m_array.Free( index );
      if(index != m_array.Size( ))
        m_array[index]->Sync( m_array, index );
      break;
    }
  }
}

template <typename T, typename K>
void HashTable<T, K>::Bucket::Unlink( b_array& array, uint32 myIndex )
{
  array[m_prev]->m_next = m_next;
  array[m_next]->m_prev = m_prev;
  m_next = myIndex;
  m_prev = myIndex;
}

template <typename T, typename K>
void HashTable<T, K>::Bucket::InsertBefore( b_array& array, uint32 myIndex, uint32 index )
{
  Bucket *position = array[index];
  m_next = index;
  m_prev = position->m_prev;

  array[position->m_prev]->m_next = myIndex;
  position->m_prev = myIndex;
}

template <typename T, typename K>
void HashTable<T, K>::Bucket::InsertAfter( b_array& array, uint32 myIndex, uint32 index )
{
  Bucket *position = array[index];
  m_prev = index;
  m_next = position->m_next;
  
  array[position->m_next]->m_prev = myIndex;
  position->m_next = myIndex;
}

// Sync next->prev and prev->next
template <typename T, typename K>
void HashTable<T, K>::Bucket::Sync( b_array& array, uint32 myIndex )
{
  array[m_next]->m_prev = myIndex;
  array[m_prev]->m_next = myIndex;
}

/*------------------------------------------------------------------------------------------------

                                           Data Access

------------------------------------------------------------------------------------------------*/
template <typename T, typename K>
T *HashTable<T, K>::operator[]( const char *key )
{
  const uint32 h = Hash( key ) % m_rows;

  for(auto i = m_array[m_array[h]->m_next]; i != m_array[h]; i = m_array[i->m_next])
  {
    if(strcmp( i->m_key, key ) == 0)
      return &i->m_val;
  }

  // Not found in table!
  return NULL;
}

template <typename T, typename K>
const T *HashTable<T, K>::operator[]( const char *key ) const
{
  const uint32 h = Hash( key ) % m_rows;

  for(auto i = m_array[m_array[h]->m_next]; i != m_array[h]; i = m_array[i->m_next])
  {
    if(strcmp( i->m_key, key ) == 0)
      return &i->m_val;
  }

  // Not found in table!
  return NULL;
}

template <typename T, typename K>
T *HashTable<T, K>::operator[]( const S& key )
{
  const uint32 h = Hash( key ) % m_rows;

  for(auto i = m_array[m_array[h]->m_next]; i != m_array[h]; i = m_array[i->m_next])
  {
    if(i->m_key == key)
      return &i->m_val;
  }

  // Not found in table!
  return NULL;
}

template <typename T, typename K>
const T *HashTable<T, K>::operator[]( const S& key ) const
{
  const uint32 h = Hash( key ) % m_rows;

  for(auto i = m_array[m_array[h]->m_next]; i != m_array[h]; i = m_array[i->m_next])
  {
    if(i->m_key == key)
      return &i->m_val;
  }

  // Not found in table!
  return NULL;
}

template <typename T, typename K>
T *HashTable<T, K>::operator[]( uint32 key )
{
  const uint32 h = Hash( key ) % m_rows;

  for(auto i = m_array[m_array[h]->m_next]; i != m_array[h]; i = m_array[i->m_next])
  {
    if(i->m_key == key)
      return &i->m_val;
  }

  // Not found in table!
  return NULL;
}

template <typename T, typename K>
const T *HashTable<T, K>::operator[]( uint32 key ) const
{
  const uint32 h = Hash( key ) % m_rows;

  for(auto i = m_array[m_array[h]->m_next]; i != m_array[h]; i = m_array[i->m_next])
  {
    if(i->m_key == key)
      return &i->m_val;
  }

  // Not found in table!
  return NULL;
}

template <typename T, typename K>
T *HashTable<T, K>::operator[]( int32 key )
{
  const uint32 h = Hash( key ) % m_rows;

  for(auto i = m_array[m_array[h]->m_next]; i != m_array[h]; i = m_array[i->m_next])
  {
    if(i->m_key == key)
      return &i->m_val;
  }

  // Not found in table!
  return NULL;
}

template <typename T, typename K>
const T *HashTable<T, K>::operator[]( int32 key ) const
{
  const uint32 h = Hash( key ) % m_rows;

  for(auto i = m_array[m_array[h]->m_next]; i != m_array[h]; i = m_array[i->m_next])
  {
    if(i->m_key == key)
      return &i->m_val;
  }

  // Not found in table!
  return NULL;
}
