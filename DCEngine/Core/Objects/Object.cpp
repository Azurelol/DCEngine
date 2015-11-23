#include "Object.h"

namespace DCEngine {

  // Keeps track of all Objects that have been created.
  unsigned int Object::ObjectsCreated = 0;

  /**************************************************************************/
  /*!
  @brief  Object constructor.
  @param  name The name of the Object.
  @note   Every object in the engine derives from this Object, thus they all
          set their names through this constructor.
  */
  /**************************************************************************/
  Object::Object(std::string name) : ObjectName(name), ObjectID(ObjectsCreated++)
  {

  }
  /**************************************************************************/
  /*!
  @brief  Object default constructor.
  @note   Every object in the engine derives from this Object, thus they all
  set their names through this constructor.
  */
  /**************************************************************************/
  Object::Object() : ObjectName("Object"), ObjectID(ObjectsCreated++)
  {
  }

  /**************************************************************************/
  /*!
  @brief  Object destructor.
  */
  /**************************************************************************/
  Object::~Object()
  {
  }

  /**************************************************************************/
  /*!
  @brief Gets the name of the object.
  @return A reference to the string that contains this objet's name.
  */
  /**************************************************************************/
  const std::string & Object::Name() const
  {
    return ObjectName;
  }

  /**************************************************************************/
  /*!
  @brief Sets the name of the object.
  @param name The new name for the Object.
  */
  /**************************************************************************/
  void Object::setName(std::string name)
  {
    ObjectName = name;
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