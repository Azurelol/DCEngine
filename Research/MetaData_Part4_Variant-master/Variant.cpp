// This work is licensed under a Creative Commons Attribution 3.0 Unported License.
// http://creativecommons.org/licenses/by/3.0/deed.en_US

#include "Precompiled.h"
#include "Variant.h"

Variant::Variant( const MetaData *m, void *d ) : meta( m ), data( d )
{
}

Variant::Variant( ) : meta( NULL ), data( NULL )
{
}

const MetaData *Variant::GetMeta( void ) const
{
  return meta;
}

Variant& Variant::operator=( const Variant& rhs )
{
  if(this == &rhs)
    return *this;

  if(meta)
  {
    if(meta == rhs.meta)
      meta->Copy( data, rhs.data );
    else
    {
      assert( rhs.meta ); // Cannot make an instance of NULL meta!

      meta->Delete( data );
      meta = rhs.GetMeta( );

      // We require a new copy if meta does not match!
      if(meta)
        data = meta->NewCopy( &rhs.data );
    }
  }

  return *this;
}
