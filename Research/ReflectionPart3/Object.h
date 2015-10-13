#pragma once

class Object
{
public:
  Object( int id );
  ~Object( );

  META_DATA( Object );

private:
  int ID;
  bool active;
  double x;
};