/*-------------------qMopey-Header----------------------
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 2/12/2013
File Name     : InListBase.h
Purpose       : Definition of intrusively linked list.

Note          : Place LIST_LINK in class/struct
                definition to use declare a node.

                Links automatically unlink themselves
                from the list so long as they are not
                currently linked within any list.

                UNTESTED means a particular method
                has not yet been tested -- use at your
                own risk.
------------------------------------------------------*/

#pragma once

#include "Link.h"

// Construct a list with a convenient macro.
// This macro just fills out some template parameters in a nice way.
//
// Usage:
//   LIST_DECL( HostType, node ) list1;
//                 ^       ^       ^
//          Type of data   |  Name of the linked
//          containing a   |  list to construct.
//          node.          |
//                  Name of the node
//                  within the HostType
//                  to use as links.
#define LIST_DECL( TYPE, NODE_NAME ) \
  InList<TYPE, OFFSET_OF( TYPE, NODE_NAME )>

template<typename T>
class InListBase
{
public:
  typedef bool (*Compare)( const T& lhs, const T& rhs );

    /*----------------
       Ctors/Dtors
    ----------------*/
  InListBase( uint32 offset );
  ~InListBase( );

    /*----------------
        Modifiers
    ----------------*/
  void push_front( T& toInsert ); // UNTESTED
  void push_back( T& toInsert );
  void push_front( T *toInsert ); // UNTESTED
  void push_back( T *toInsert ); // UNTESTED
  void push_front( Link<T>& toInsert ); // UNTESTED
  void push_back( Link<T>& toInsert ); // UNTESTED
  void insert( Link<T>& toInsert, Compare comparison ); // UNTESTED
  void erase( T& n ); // UNTESTED
  void erase( T *n ); // UNTESTED
  void erase( Link<T>& n ); // UNTESTED
  void erase( Link<T>& b, Link<T>& e );
  T& pop_front( void ); // UNTESTED
  T& pop_back( void ); // UNTESTED
  void clear( void );

    /*----------------
       Data Access
    ----------------*/
  Link<T> *begin( void );
  const Link<T> *begin( void ) const; // UNTESTED
  Link<T> *end( void );
  const Link<T> *end( void ) const; // UNTESTED
  Link<T> *rbegin( void ); // UNTESTED
  const Link<T> *rbegin( void ) const; // UNTESTED
  Link<T> *rend( void ); // UNTESTED
  const Link<T> *rend( void ) const; // UNTESTED

  bool empty( void ) const;

private:
  Link<T> head;
  Link<T> tail;
  uint32 link_offset;

  // Not currently supported
  //InListBase& operator=( const InListBase& rhs );
  //InListBase( const InListBase& rhs );
};

// InList definition
// Uses some simple template metaprogramming to capture offset as a template parameter. This
// allows for a more natural use of the LIST_DECL macro.
template <typename T, uint32 offset>
class InList : public InListBase<T>
{
public:
    InList( );
};

template <typename T, uint32 offset>
InList<T, offset>::InList( ) : InListBase<T>( offset )
{
}

/*------------------------------------------------------------------------------------------------

                                    Constructors / Destructors

------------------------------------------------------------------------------------------------*/
template <typename T>
InListBase<T>::InListBase( uint32 offset ) : head( &tail, &head ), tail( &tail, &head ), link_offset( offset )
{
  head.link_offset = offset;
  tail.link_offset = offset;
}

template <typename T>
InListBase<T>::~InListBase( )
{
  clear( );
}

/*------------------------------------------------------------------------------------------------

                                            Modifiers

------------------------------------------------------------------------------------------------*/
template <typename T>
void InListBase<T>::push_front( T& toInsert )
{
  Link<T> &l = __IL_GET_NODE( toInsert );
  assert( !l.IsLinked( ) ); // Cannot add a node attached to a list!
  
  l.next = head.next;
  l.prev = &head;

  head.next->prev = &l;
  head.next = &l;

  l.link_offset = link_offset;
}

template <typename T>
void InListBase<T>::push_back( T& toInsert )
{
  Link<T>& l = __IL_GET_NODE( toInsert );
  assert( !l.IsLinked( ) ); // Cannot add a node attached to a list!
   
  l.next = &tail;
  l.prev = tail.prev;
  
  tail.prev->next = &l;
  tail.prev = &l;

  l.link_offset = link_offset;
}

template <typename T>
void InListBase<T>::push_front( T *toInsert )
{
  Link<T> &l = __IL_GET_NODE( *toInsert );
  assert( !l.IsLinked( ) ); // Cannot add a node attached to a list!
  
  l.next = head.next;
  l.prev = &head;

  head.next->prev = &l;
  head.next = &l;

  l.link_offset = link_offset;
}

template <typename T>
void InListBase<T>::push_back( T *toInsert )
{
  Link<T> &l = __IL_GET_NODE( *toInsert );
  assert( !l.IsLinked( ) ); // Cannot add a node attached to a list!
   
  l.next = &tail;
  l.prev = tail.prev;
  
  tail.prev->next = &l;
  tail.prev = &l;

  l.link_offset = link_offset;
}

template <typename T>
void InListBase<T>::push_front( Link<T>& toInsert )
{
  assert( !toInsert.IsLinked( ) ); // Cannot add a node attached to a list!
  
  toInsert.next = head.next;
  toInsert.prev = &head;

  head.next->prev = &toInsert;
  head.next = &toInsert;

  l.link_offset = link_offset;
}

template <typename T>
void InListBase<T>::push_back( Link<T>& toInsert )
{
  assert( !toInsert.IsLinked( ) ); // Cannot add a node attached to a list!
   
  toInsert.next = &tail;
  toInsert.prev = tail.prev;
  
  tail.prev->next = &toInsert;
  tail.prev = &toInsert;

  toInsert.link_offset = link_offset;
}

template <typename T>
void InListBase<T>::insert( Link<T>& toInsert, Compare comparison )
{
  Link<T> *i = begin( );

  if(i == end( ))
  {
    push_back( toInsert );
    return;
  }

  while(comparison( __IL_GET_DATA( &toInsert ), __IL_GET_DATA( i )))
  {
    i = i->next;
    if(i == end( ))
    {
      push_back( toInsert );
      return;
    }
  }

  toInsert.InsertBefore( i );
}


template <typename T>
void InListBase<T>::erase( T& n )
{
  assert( &__IL_GET_NODE( n ) != &head && &__IL_GET_NODE( n ) != &tail ); // Cannot erase head or tail!
  __IL_GET_NODE( n ).Unlink( );
}

template <typename T>
void InListBase<T>::erase( T *n )
{
  assert( &__IL_GET_NODE( *n ) != &head && &__IL_GET_NODE( *n ) != &tail ); // Cannot erase head or tail!
  __IL_GET_NODE( *n ).Unlink( );
}

template <typename T>
void InListBase<T>::erase( Link<T>& n )
{
  assert( &n != &head && &n != &tail ); // Cannot erase head or tail!
  n.Unlink( );
}

template <typename T>
void InListBase<T>::erase( Link<T>& b, Link<T>& e )
{
  Link<T> *i = &b;

  while(i != &e)
  {
    Link<T> *temp = i;
    i = i->next;
    temp->Unlink( );
  }
}

template <typename T>
T& InListBase<T>::pop_front( void )
{
  assert( !empty( ) ); // Cannot delete front node; list is empty!
  Link<T> *link = head.next;
  link->Unlink( );
  return __IL_GET_DATA( link );
}

template <typename T>
T& InListBase<T>::pop_back( void )
{
  assert( !empty( ) ); // Cannot delete front node; list is empty!
  Link<T> *link = tail.prev;
  link->Unlink( );
  return __IL_GET_DATA( link );
}

template <typename T>
void InListBase<T>::clear( void )
{
  erase( *begin( ), *end( ) );
}

/*------------------------------------------------------------------------------------------------

                                           Data Access

------------------------------------------------------------------------------------------------*/
template <typename T>
Link<T> *InListBase<T>::begin( void )
{
  return head.next;
}

template <typename T>
const Link<T> *InListBase<T>::begin( void ) const
{
  return head.next;
}

template <typename T>
Link<T> *InListBase<T>::end( void )
{
  return &tail;
}

template <typename T>
const Link<T> *InListBase<T>::end( void ) const
{
  return &tail;
}

template <typename T>
Link<T> *InListBase<T>::rbegin( void )
{
  return tail.prev;
}

template <typename T>
const Link<T> *InListBase<T>::rbegin( void ) const
{
  return tail.prev;
}

template <typename T>
Link<T> *InListBase<T>::rend( void )
{
  return &head;
}

template <typename T>
const Link<T> *InListBase<T>::rend( void ) const
{
  return &head;
}

template <typename T>
bool InListBase<T>::empty( void ) const
{
  return head.next == &tail && tail.prev == &head;
}
