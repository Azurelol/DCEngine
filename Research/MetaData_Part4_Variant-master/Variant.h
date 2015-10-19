// This work is licensed under a Creative Commons Attribution 3.0 Unported License.
// http://creativecommons.org/licenses/by/3.0/deed.en_US

#pragma once

class Variant
{
public:
  template <typename T>
  Variant( const T& value );

  Variant( const MetaData *m, void *d );
  Variant( );

  template <typename T>
  T& GetValue( void );

  template <typename T>
  const T& GetValue( void ) const;

  const MetaData *GetMeta( void ) const;

  Variant& operator=( const Variant& rhs );
  template <typename TYPE>
  Variant& operator=( const TYPE& rhs );

private:
  const MetaData *meta;
  void *data;
};

template <typename T>
Variant::Variant( const T& value ) : meta( META_TYPE( T ) ), data( NULL )
{
  data = meta->NewCopy( &value );
}

template <typename T>
T& Variant::GetValue( void )
{
  return *reinterpret_cast<T *>(data);
}

template <typename T>
const T& Variant::GetValue( void ) const
{
  return *reinterpret_cast<T *>(data);
}

template <typename TYPE>
Variant& Variant::operator=( const TYPE& rhs )
{
  // We require a new copy if meta does not match!
  if(meta != META_TYPE( TYPE ))
  {
    assert( META_TYPE( TYPE ) ); // Cannot create instance of NULL meta!

    meta->Delete( data );
    meta = META_TYPE( TYPE );
    data = meta->NewCopy( &rhs );
  }
  else
  {
    meta->Copy( data, &rhs );
  }
  return *this;
}
