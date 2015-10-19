/*--------------------qMopey-Header--------------------
<Game Title>  : qMopey
Copyright (C) 2013 DigiPen Institute of Technology.

Creator(s)    : Randy Gaul
Creation Date : 9/15/2012
File Name     : CCVariantBase.h
Purpose       : Common base for VariantBase and
                Variant declaration.
-----------------------------------------------------*/

#pragma once

//
// VariantBase
// This class acts a common base for both the RefVariant and Variant.
// It simply provides a common point for both to inherit from -this is important so that
// RefVariant can reference another Variant. RefVariant acts like a normal Variant, except
// it does only shallow copies.
//

class MetaData;

class VariantBase
{
public:
  void *GetData( void ) const;
  const MetaData *GetMeta( void ) const;
  bool IsValid( void ) const;
  template <typename TYPE> TYPE& GetValue( void );
  template <typename TYPE> const TYPE& GetValue( void ) const;
  template <typename TYPE> TYPE ConvertValue( void );
  template <typename TYPE> const TYPE ConvertValue( void ) const;
  void Serialize( std::ostream& os ) const;

protected:
  VariantBase( ) : meta( NULL ), data( NULL ) {}
  VariantBase( const MetaData* meta, void* data ) : meta( meta ), data( data ) {}
  const MetaData *meta;
  void *data;
};

// These CastHelper structures are required to store two different types of Cast operators.
// Conventional overloading of the Cast operators won't work as the only difference between
// the two are template argument specializations and return values. The point of this is to
// allow RefVariants the ability to store pointer types within their void * directly, instead
// of storing a TYPE ** with their void pointer.
template <typename T>
struct CastHelper
{
  static T& Cast( void *& data )
  {
    return *reinterpret_cast<T *&>(data);
  }
};
 
template <typename T>
struct CastHelper<T *>
{
  static T *& Cast( void *& data )
  {
    return (T *&)data;
  }
};

template <typename TYPE>
TYPE& VariantBase::GetValue( void )
{
#ifdef _DEBUG
  ErrorIf( META_TYPE( TYPE ) != meta, "Type mismatch! You cannot convert a data type to another type with this function."
                                      " You can try using the ConvertValue function instead." );
#endif // _DEBUG
  return CastHelper<TYPE>::Cast( data );
}

template <typename TYPE>
const TYPE& VariantBase::GetValue( void ) const
{
  return CastHelper<TYPE>::Cast( data );
}

template <typename TYPE>
TYPE VariantBase::ConvertValue( void )
{
  Conversion conv = meta->GetConversion( META_TYPE( TYPE ) );
  if(conv)
  {
    return conv( this ).ConvertValue<TYPE>( );
  }
  else
    ErrorIf( META_TYPE( TYPE ) != meta, "Type mismatch! You cannot convert this type to the requested type! Please "
                                        "create a conversion function, otherwise you may have a bug or typo." );
  return CastHelper<TYPE>::Cast( data );
}

template <typename TYPE>
const TYPE VariantBase::ConvertValue( void ) const
{
  Conversion conv = meta->ConvertValue( META_TYPE( TYPE ) );
  if(conv)
  {
    return conv( this ).ConvertValue<TYPE>( );
  }
  else
    ErrorIf( META_TYPE( TYPE ) != meta, "Type mismatch! You cannot convert this type to the requested type! Please "
                                        "create a conversion function, otherwise you may have a bug or typo." );
  return CastHelper<TYPE>::Cast( data );
}
