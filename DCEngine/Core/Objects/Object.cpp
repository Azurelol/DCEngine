/*****************************************************************************/
/*!
\file   Object.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/5/2015
\brief  The base object class, from which everything else is derived ??
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "Object.h"
#include "../../Core/Engine/Engine.h"

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
  Object::Object(std::string name) : ObjectName(name), ObjectOwner(nullptr), ObjectID(ObjectsCreated++)
  {

  }
  /**************************************************************************/
  /*!
  @brief  Object default constructor.
  @note   Every object in the engine derives from this Object, thus they all
  set their names through this constructor.
  */
  /**************************************************************************/
  Object::Object() : ObjectName("Object"), ObjectOwner(nullptr), ObjectID(ObjectsCreated++)
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
  @brief Serializes the base object.
  @todo  Serialize by type for Object is currently not working.
  */
  /**************************************************************************/
  void Object::Serialize(Zilch::JsonBuilder & builder)
  {
    auto interface = Daisy->getSystem<Systems::Reflection>()->Handler();
    // Name
    {
      builder.Key("Name");
      builder.Value(this->Name().c_str());
    }
    // Not working atm
    SerializeByType(builder, interface->GetState(), this, this->ZilchGetDerivedType()->BaseType->ZilchGetDerivedType()->BaseType );
  }

  /**************************************************************************/
  /*!
  @brief Deserializes the base object.
  */
  /**************************************************************************/
  void Object::Deserialize(Zilch::JsonValue * properties)
  {

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