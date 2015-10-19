#pragma once

template <typename T>
struct HasDefaultCtor
{
  template <typename U>
  static int32 SFINAE( decltype( U( ) ) * );
  template <typename U>
  static int8 SFINAE( ... );
 
  static const bool value = sizeof( SFINAE<T>( NULL ) ) == sizeof( int32 );
};

template <typename T>
struct HasCopyCtor
{
  static T MakeT( void );

  template <typename U>
  static int32 SFINAE( decltype( U( MakeT( ) ) ) * );
  template <typename U>
  static int8 SFINAE( ... );
 
  static const bool value = sizeof( SFINAE<T>( NULL ) ) == sizeof( int32 );
};

template <typename T>
struct HasCtor1Args
{
  struct Any
  {
    template <typename U>
    operator typename std::enable_if<!std::is_same<T, U>::value>::type( ) const;
  };

  template <typename U>
  static int32 SFINAE( decltype( U( Any( ) ) ) * );

  template <typename U>
  static int8 SFINAE( ... );

  static const bool value = sizeof( SFINAE<T>( NULL ) ) == sizeof( int32 );
};

template <typename T>
struct HasCtor2Args
{
  struct Any { template <typename U> operator U( void ); };

  template <typename U>
  static int32 SFINAE( decltype( U( Any( ), Any( ) ) ) * );

  template <typename U>
  static int8 SFINAE( ... );

  static const bool value = sizeof( SFINAE<T>( NULL ) ) == sizeof( int32 );
};

template <typename T>
struct HasCtor3Args
{
  struct Any { template <typename U> operator U( void ); };

  template <typename U>
  static int32 SFINAE( decltype( U( Any( ), Any( ), Any( ) ) ) * );

  template <typename U>
  static int8 SFINAE( ... );

  static const bool value = sizeof( SFINAE<T>( NULL ) ) == sizeof( int32 );
};

template <typename T>
struct HasCtor4Args
{
  struct Any { template <typename U> operator U( void ); };

  template <typename U>
  static int32 SFINAE( decltype( U( Any( ), Any( ), Any( ), Any( ) ) ) * );

  template <typename U>
  static int8 SFINAE( ... );

  static const bool value = sizeof( SFINAE<T>( NULL ) ) == sizeof( int32 );
};
