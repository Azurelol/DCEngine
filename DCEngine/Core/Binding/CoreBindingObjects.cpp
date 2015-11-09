/******************************************************************************/
/*!
@file   CoreBindingObjects.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/7/2015
@brief  Binds the Object class and its inherited classes used in the engine
        to the Daisy Chain Engine core library 'DCEngineCore'
@note   We include all the headers of the classes we are defining to Zilch
        here since we have decided to defne them all in a single, unified file
        rather than individually in each of their definition files.
*/
/******************************************************************************/
#include "CoreBindingObjects.h"
#include "CoreBinding.h"
#include "CoreBindingTypes.h"

// Object
#include "../Objects/Object.h"
// Object ->
#include "../Objects/Entity.h"
#include "../Objects/Resource.h"
#include "../Objects/Component.h"
// Object -> Entity ->
#include "../Objects/Entities/EntitiesInclude.h"
// Object -> Resource ->
#include "../Resources/ResourcesInclude.h"

namespace DCEngine {

  /*!************************************************************************\
  @brief Provides the definiton of this class to the Zilch library.
  \**************************************************************************/
  #if(!DCE_BINDING_OBJECT_CLASSES_INTERNALLY)

  /*!************************************************************************\
  @brief  Object Definition
  \**************************************************************************/
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

  /*!************************************************************************\
  @brief  Entity Definition
  \**************************************************************************/
  ZilchDefineType(Entity, "Entity", DCEngineCore, builder, type) {
    ZilchBindConstructor(builder, type, Entity, "name", std::string);
    ZilchBindDestructor(builder, type, Entity);
  }

  /*!************************************************************************\
  @brief  Resource Definition
  \**************************************************************************/
  ZilchDefineType(Resource, "Resource", DCEngineCore, builder, type) {

    // CTOR/DTOR
    ZilchBindConstructor(builder, type, Resource, "name", std::string);
    ZilchBindDestructor(builder, type, Resource);
    // Fields
  }

  /*!************************************************************************\
  @brief  GameSession Definition
  \**************************************************************************/
  ZilchDefineType(GameSession, "GameSession", DCEngineCore, builder, type) {
    ZilchBindConstructor(builder, type, GameSession, "name", std::string);
    ZilchBindDestructor(builder, type, GameSession);
  }
  
  /*!************************************************************************\
  @brief  Space Definition
  \**************************************************************************/
  ZilchDefineType(Space, "Space", DCEngineCore, builder, type) {
    // Constructor / Destructors
    ZilchBindConstructor(builder, type, Space, "name, gamesession", std::string, GameSession&);
    ZilchBindDestructor(builder, type, Space);
    // Fields

    // Properties
  }

  /*!************************************************************************\
  @brief  GameObject Definition
  \**************************************************************************/
  ZilchDefineType(GameObject, "GameObject", DCEngineCore, builder, type) {

    // CTOR/DTOR
    ZilchBindConstructor(builder, type, GameObject, "name, space, gamesession", std::string, Space&, GameSession&);
    ZilchBindConstructor(builder, type, GameObject, ZilchNoNames);
    ZilchBindDestructor(builder, type, GameObject);
    // Fields
  }

  /*!************************************************************************\
  @brief  Component Definition
  \**************************************************************************/
  ZilchDefineType(Component, "Component", DCEngineCore, builder, type) {

    // CTOR/DTOR
    ZilchBindConstructor(builder, type, GameObject, "name, space, gamesession", std::string, Space&, GameSession&);
    ZilchBindDestructor(builder, type, Component);
    // Fields
  }

  #endif

}