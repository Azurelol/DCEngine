/*---------/*-----------------Alloctopus-Header--------------------
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 9/6/2013

File Name     : Messenger.h
Purpose       : 
------------------------------------------------------*/

#pragma once

struct Listener;

class Messenger
{
public:
  template <typename Signature>
  Listener *Subscribe( const EMessage id, Signature signature );
  template <typename Signature, typename Self>
  Listener *Subscribe( const EMessage id, Signature signature, Self self );
  void Unsubscribe( Listener * listener );

  void PostMSG( const EMessage id );
  void PostMSG( const EMessage id, const RefVariant &arg );
  void PostMSG( const EMessage id, const RefVariant &arg0, const RefVariant &arg1 );

private:
  std::unordered_multimap<EMessage, Listener *> m_listeners;
  static BlockAllocator s_listenerAllocator;
};

template <typename Signature>
Listener *Messenger::Subscribe( const EMessage id, Signature signature )
{
  Listener *listener = RCAST( Listener, s_listenerAllocator.Allocate( sizeof( Listener ) ) );
  new (listener) Listener( id, signature );

  m_listeners.insert( std::pair<EMessage, Listener*>( id, listener ) );
  return listener;
}

template <typename Signature, typename Self>
Listener *Messenger::Subscribe( const EMessage id, Signature signature, Self self )
{
  Listener *listener = RCAST( Listener, s_listenerAllocator.Allocate( sizeof( Listener ) ) );
  new (listener) Listener( id, signature, self );

  m_listeners.insert( std::pair<EMessage, Listener*>( id, listener ) );
  return listener;
}
