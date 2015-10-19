/*-------------------qMopey-Header----------------------
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 2/14/2013
File Name     : Singleton.h
Purpose       : 
------------------------------------------------------*/

#pragma once

// Converts a type into a single with CRTP
// Intended use with CRTP:

// Example:
// class Object : public Singleton<Object>
// {
// };

template <typename T>
class Singleton
{
public:
  static T& Get( void );

protected:
  Singleton( );
  Singleton( const Singleton& );
  Singleton& operator=( const Singleton& );
  ~Singleton( );
};

template <typename T>
T& Singleton<T>::Get( void )
{
  static T instance;
  return instance;
}

template <typename T>
Singleton<T>::Singleton( )
{
}

template <typename T>
Singleton<T>::~Singleton( )
{
}
