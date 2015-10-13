#pragma once

class S
{
public:
  S( const char *src );
  ~S( );

  bool IsEqual( const S& rhs ) const;
  void Concat( const S& rhs );
  friend std::ostream& operator<<( std::ostream &os, S &rhs );

  META_DATA( S );

private:
  const char *data;
  unsigned len;
};