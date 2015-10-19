/*-----------------Alloctopus-Header--------------------
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 9/6/2013

File Name     : Messenger.cpp
Purpose       : 
------------------------------------------------------*/

#include "Precompiled.h"
#include "Messenger.h"
#include "Listener.h"

BlockAllocator Messenger::s_listenerAllocator;

void Messenger::Unsubscribe( Listener *listener )
{
  auto range = m_listeners.equal_range( listener->id );

  for(auto it = range.first; it != range.second; ++it)
  {
    if(range.first->second == listener)
    {
      m_listeners.erase( range.first );
      listener->~Listener( );
      s_listenerAllocator.Free( listener, sizeof( Listener ) );
      return;
    }
  }
}

void Messenger::PostMSG( const EMessage id )
{
  auto subscribers = m_listeners.equal_range( id );

  for(auto i = subscribers.first; i != subscribers.second; ++i)
    i->second->function( );
}

void Messenger::PostMSG( const EMessage id, const RefVariant &arg )
{
  auto subscribers = m_listeners.equal_range( id );

  for(auto i = subscribers.first; i != subscribers.second; ++i)
    i->second->function( arg );
}

void Messenger::PostMSG( const EMessage id, const RefVariant &arg0, const RefVariant &arg1 )
{
  auto subscribers = m_listeners.equal_range( id );

  for(auto i = subscribers.first; i != subscribers.second; ++i)
    i->second->function( arg0, arg1 );
}
