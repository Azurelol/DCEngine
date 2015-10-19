/*-------------------qMopey-Header----------------------
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 2/13/2013
File Name     : Link.h
Purpose       : An intrusive link. To be placed within
                an object to allow the object to be
                placed within a linked list as a node.
------------------------------------------------------*/

#pragma once
// Interal macros

#define __IL_GET_DATA( LINK ) \
  (*reinterpret_cast<T *>(PTR_SUB( (LINK), link_offset )))

#define __IL_GET_DATA_PTR( LINK ) \
  (reinterpret_cast<T *>(PTR_SUB( (LINK), link_offset )))

#define __IL_GET_NODE( DATA ) \
  (*reinterpret_cast<Link<T> *>(PTR_ADD( &DATA, link_offset )))

template <typename T>
class InListBase;

template <typename T>
class Link
{
public:
    /*----------------
       Ctors/Dtors
    ----------------*/
  Link( );
  ~Link( );

    /*----------------
        Modifiers
    ----------------*/
  void Unlink( void );
  void InsertBefore( Link& position ); // UNTESTEDC
  void InsertBefore( Link *position ); // UNTESTEDC
  void InsertAfter( Link& position );
  void InsertAfter( Link *position );
  Link& operator=( const Link& rhs );
  void Clear( void );

    /*----------------
       Data Access
    ----------------*/
  void Validate( void ) const;
  bool IsLinked( void ) const;
  T& GetData( void ); // UNTESTED
  const T& GetData( void ) const; // UNTESTED
  T *GetPtr( void ); // UNTESTED
  const T *GetPtr( void ) const; // UNTESTED

  Link *next;
  Link *prev;

protected:
  uint32 link_offset;
  friend class InListBase<T>;
  Link( Link *n, Link *p );
};

/*------------------------------------------------------------------------------------------------

                                    Constructors / Destructors

------------------------------------------------------------------------------------------------*/
DISABLE_WARNING( 4355 )
template <typename T>
Link<T>::Link( ) : next( this ), prev( this ), link_offset( 0 )
{
}
END_DISABLE( )

template <typename T>
Link<T>::~Link( )
{
#ifdef _DEBUG
  Validate( );
#endif // _DEBUG

  // Unlink self upon destruction
  if(IsLinked( ))
    Unlink( );
}

template <typename T>
Link<T>::Link( Link *n, Link *p ) : next( n ), prev( p ), link_offset( n->link_offset )
{
}

/*------------------------------------------------------------------------------------------------

                                            Modifiers

------------------------------------------------------------------------------------------------*/
template <typename T>
void Link<T>::Unlink( void )
{
  prev->next = next;
  next->prev = prev;
  next = this;
  prev = this;
  link_offset = 0;
}

template <typename T>
void Link<T>::InsertBefore( Link& position )
{
  assert( !IsLinked( ) ); // Cannot add a node attached to a list!

  next = &position;
  prev = position.prev;

  position.prev->next = this;
  position.prev = this;

  link_offset = position.link_offset;
}

template <typename T>
void Link<T>::InsertBefore( Link *position )
{
  assert( !IsLinked( ) ); // Cannot add a node attached to a list!

  next = position;
  prev = position->prev;

  position->prev->next = this;
  position->prev = this;

  link_offset = position->link_offset;
}

template <typename T>
void Link<T>::InsertAfter( Link& position )
{
  assert( !IsLinked( ) ); // Cannot add a node attached to a list!

  prev = &position;
  next = position.next;

  position.next->prev = this;
  position.next = this;

  link_offset = position.link_offset;
}

template <typename T>
void Link<T>::InsertAfter( Link *position )
{
  assert( !IsLinked( ) ); // Cannot add a node attached to a list!

  prev = position;
  next = position->next;

  position->next->prev = this;
  position->next = this;

  link_offset = position->link_offset;
}

template <typename T>
Link<T>& Link<T>::operator=( const Link& rhs )
{
  if(rhs.next == &rhs)
    next = this;
  else
    next = rhs.next;

  if(rhs.prev == &rhs)
    prev = this;
  else
    prev = rhs.prev;

  return *this;
}

template <typename T>
void Link<T>::Clear( void )
{
  next = this;
  prev = this;
}

/*------------------------------------------------------------------------------------------------

                                           Data Access

------------------------------------------------------------------------------------------------*/
template <typename T>
void Link<T>::Validate( void ) const
{
  // Make sure our nearby pointer connections form a valid list
  if(next && next != this)
    assert( next->prev == this );
  if(prev && prev != this)
    assert( prev->next == this );
}

template <typename T>
bool Link<T>::IsLinked( void ) const
{
  return next != this && prev != this;
}

template <typename T>
T& Link<T>::GetData( void )
{
  return __IL_GET_DATA( this );
}

template <typename T>
const T& Link<T>::GetData( void ) const
{
  return __IL_GET_DATA( (Link<T> *)this );
}

template <typename T>
T *Link<T>::GetPtr( void )
{
  return __IL_GET_DATA_PTR( (Link<T> *)this );
}

template <typename T>
const T *Link<T>::GetPtr( void ) const
{
  return __IL_GET_DATA_PTR( (Link<T> *)this );
}
