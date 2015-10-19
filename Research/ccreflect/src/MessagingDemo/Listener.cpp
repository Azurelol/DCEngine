/*-----------------Alloctopus-Header--------------------
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 9/6/2013

File Name     : Listener.cpp
Purpose       : 
------------------------------------------------------*/

#include "Precompiled.h"
#include "Listener.h"

bool Listener::operator==( const Listener &rhs ) const
{
  return id == rhs.id;
}
