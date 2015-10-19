/*-------------------qMopey-Header----------------------
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 2/14/2013
File Name     : CtorLister.h
Purpose       : 
------------------------------------------------------*/

#pragma once

// Constructs a singly link list of any type of object. Objects are inserted into the
// list upon construction. Intended use with CRTP:

// Example:
// class Object : public CtorLister<Object>
// {
// };

// The above is all that is needed to auto-generate a list of objects

template <typename T>
class CtorLister
{
public:
  CtorLister( )
  {
    next = Head( );
    Head( ) = static_cast<const T *>(this);
  }

  const T *Next( void ) const
  {
    return next;
  }

  static const T *& Head( void )
  {
    static const T *p = 0;
    return p;
  }

private:
  const T *next;
};
