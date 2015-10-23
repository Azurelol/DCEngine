#include "Object.h"


  Object::Object(int id) : ID(0), active(true)
  {
  }

  Object::~Object()
  {
  }

  DerivedObject::DerivedObject(int id) : Object(id)
  {

  }

  DerivedObject::~DerivedObject()
  {

  }
