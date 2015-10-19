/*-----------------Alloctopus-Header--------------------
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 9/6/2013

File Name     : Listener.h
Purpose       : 
------------------------------------------------------*/

#pragma once

struct Listener
{
  template<typename Signature>
  Listener( const EMessage messageID, const Signature &signature );
  template<typename Signature, typename Self>
  Listener( const EMessage messageID, const Signature &signature, Self self );

  bool Listener::operator==( const Listener &rhs ) const;

  const EMessage id;
  VariantFunction function;
};

template<typename Signature>
Listener::Listener( const EMessage messageID, const Signature &signature )
  : id( messageID )
  , function( "Listener", signature )
{
}

template<typename Signature, typename Self>
Listener::Listener( const EMessage messageID, const Signature &signature, Self self )
  : id( messageID )
  , function( "Listener", signature )
{
  function.Bind( self );
}
