#include "Object.h"

namespace DCEngine {

  // Keeps track of all Objects that have been created.
  unsigned int Object::ObjectsCreated = 0;

  Object::Object(std::string name) : ObjectName(name), ObjectID(ObjectsCreated++)
  {
  }

  Object::Object() : ObjectName("Object"), ObjectID(ObjectsCreated++)
  {
  }

  Object::~Object()
  {
  }

  const std::string & Object::Name() const
  {
    return ObjectName;
  }

#if(DCE_BINDING_OBJECT_CLASSES_INTERNALLY)
  ZilchDefineType(Object, "Object", DCEngineCore, builder, type) {

    // CTOR/DTOR
    ZilchBindConstructor(builder, type, Object, "name", std::string);
    ZilchBindConstructor(builder, type, Object, ZilchNoNames);
    ZilchBindDestructor(builder, type, Object);
    // Methods
    ZilchBindMethod(builder, type, &Object::Name, ZilchNoOverload, "Name", ZilchNoNames);
    // Fields
    ZilchBindField(builder, type, &Object::ObjectName, "ObjectName", Zilch::PropertyBinding::Get);
    ZilchBindField(builder, type, &Object::ObjectID, "ObjectID", Zilch::PropertyBinding::Get);
    ZilchBindField(builder, type, &Object::ObjectOwner, "ObjectOwner", Zilch::PropertyBinding::GetSet);
  }
#endif

}