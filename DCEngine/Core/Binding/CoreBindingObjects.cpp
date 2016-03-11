/******************************************************************************/
/*!
@file   CoreBindingObjects.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@coauthor Gabriel Neumann
@par    email: g.neumann\@digipen.edu
@date   11/7/2015
@brief  Binds the Object class and its inherited classes used in the engine
        to the Daisy Chain Engine core library 'DCEngineCore'
@note   We include all the headers of the classes we are defining to Zilch
        here since we have decided to defne them all in a single, unified file
        rather than individually in each of their definition files.
        Perhaps I do not need to bind CTOR/DTORs for my classes if I don't plan
        on constructing them through Zilch.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Precompiled.h"
#include "CoreBindingObjects.h"
#include "CoreBinding.h"
#include "CoreBindingTypes.h"
#include "../Objects/Entities/Keyboard.h"
#include "../Engine/Engine.h"

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
    DCE_BINDING_INTERNAL_COMPONENT_SET_HANDLE_TYPE;
    // Constructor / Destructor
    ZilchBindConstructor(builder, type, Object, "name", std::string);
    ZilchBindConstructor(builder, type, Object, ZilchNoNames);
    ZilchBindDestructor(builder, type, Object);
    // Methods
    //ZilchBindMethod(builder, type, &Object::Name, ZilchNoOverload, "Name", ZilchNoNames);
    // Fields
    ZilchBindField(builder, type, &Object::ObjectName, "ObjectName", Zilch::PropertyBinding::Get);
    ZilchBindField(builder, type, &Object::ObjectID, "ObjectID", Zilch::PropertyBinding::Get);
    // Properties
    ZilchBindProperty(builder, type, &Object::getObjectName, &Object::setObjectName, "ObjectName");    

  }
  /*!************************************************************************\
  @brief  Engine, Daisy, Definition
  \**************************************************************************/
  ZilchDefineType(Engine, "Daisy", DCEngineCore, builder, type) {
    //ZilchBindMethod(builder, type, &(Daisy->OnLogicUpdateConnect), ZilchNoOverload, "OnLogicUpdateConnect", ZilchNoNames);
         //ZilchBindMethod(builder, type, &(Daisy->Connect<::DCEngine::Event>), ZilchNoOverload, "Connect", ZilchNoNames);

    // Methods
    ZilchBindMethod(builder, type, &Engine::getKeyboard, ZilchNoOverload, "getKeyboard", ZilchNoNames);
    ZilchBindMethod(builder, type, &Engine::getMouse, ZilchNoOverload, "getMouse", ZilchNoNames);
    ZilchBindMethod(builder, type, &Engine::getGameSession, ZilchNoOverload, "getGameSession", ZilchNoNames);
  }


  /*!************************************************************************\
  @brief  Resource Definition 
  \**************************************************************************/
  ZilchDefineType(Resource, "Resource", DCEngineCore, builder, type) {
    DCE_BINDING_SET_HANDLE_TYPE_POINTER;
    // Constructor / Destructor
    ZilchBindConstructor(builder, type, Resource, "type, name, resourcePath", std::string, std::string, std::string);
    ZilchBindDestructor(builder, type, Resource);
    // Properties
    DCE_BINDING_DEFINE_PROPERTY(Resource, ResourcePath);

  }

  /*!************************************************************************\
  @brief  Keyboard Definition
  \**************************************************************************/
  ZilchDefineType(Keyboard, "Keyboard", DCEngineCore, builder, type) {
    // Constructor / Destructor
    ZilchBindConstructor(builder, type, Keyboard, ZilchNoNames);
    ZilchBindDestructor(builder, type, Keyboard);
    // Methods
    //ZilchBindMethod(builder, type, &Keyboard::KeyIsDown, ZilchNoOverload, "KeyIsDown", Keys);
    //ZilchBindMethod(builder, type, &Keyboard::KeyIsUp, ZilchNoOverload, "KeyIsUp", Keys);
  }

  /*!************************************************************************\
  @brief  Mouse Definition
  \**************************************************************************/
  ZilchDefineType(Mouse, "Mouse", DCEngineCore, builder, type) {
   /* // Constructor / Destructor
    ZilchBindConstructor(builder, type, Mouse, ZilchNoNames);
    ZilchBindDestructor(builder, type, Mouse);
    // Fields
    ZilchBindMethod(builder, type, &Mouse::MouseDown, ZilchNoOverload, "MouseDown", ZilchNoNames);
    ZilchBindMethod(builder, type, &Mouse::MouseUp, ZilchNoOverload, "MouseUp", ZilchNoNames);*/
  }

  /*!************************************************************************\
  @brief  GameSession Definition
  \**************************************************************************/
  ZilchDefineType(GameSession, "GameSession", DCEngineCore, builder, type) {
    DCE_BINDING_SET_HANDLE_TYPE_POINTER;
    // Constructor / Destructor
    ZilchBindConstructor(builder, type, GameSession, "name", std::string);
    ZilchBindDestructor(builder, type, GameSession);      // Methods
    // Methods
    ZilchBindMethod(builder, type, &GameSession::CreateSpace, ZilchNoOverload, "CreateSpace", "name, initialize");
    ZilchBindMethod(builder, type, &GameSession::GetSpace, ZilchNoOverload, "FindSpaceByName", "name");
    ZilchBindMethod(builder, type, &GameSession::getDefaultSpace, ZilchNoOverload, "GetDefaultSpace", ZilchNoNames);
    ZilchBindMethod(builder, type, &GameSession::TestGameSession, ZilchNoOverload, "TestGameSession", ZilchNoNames);
  }



  /*!************************************************************************\
  @brief  Space Definition
  \**************************************************************************/
  ZilchDefineType(Space, "Space", DCEngineCore, builder, type) {
    DCE_BINDING_SET_HANDLE_TYPE_POINTER;
    // Constructor / Destructor
    ZilchBindConstructor(builder, type, Space, "name, gamesession", std::string, GameSession&);
    ZilchBindDestructor(builder, type, Space);
    // Fields

    // Properties
    // Methods
    ZilchBindMethod(builder, type, &Space::Initialize, ZilchNoOverload, "Initialize", ZilchNoNames);
    ZilchBindMethod(builder, type, &Space::Terminate, ZilchNoOverload, "Terminate", ZilchNoNames);
    ZilchBindMethod(builder, type, &Space::Destroy, ZilchNoOverload, "Destroy", ZilchNoNames);
    ZilchBindMethod(builder, type, &Space::LoadLevel, (void (Space::*)(std::string& level)), "LoadLevel", ZilchNoNames);
    ZilchBindMethod(builder, type, &Space::ReloadLevel, ZilchNoOverload, "ReloadLevel", ZilchNoNames);
    //ZilchBindMethod(builder, type, &Space::Update, ZilchNoOverload, "Update", ZilchNoNames);
    //ZilchBindMethod(builder, type, &Space::CreateObject, GameObject* , "CreateObject", "archetype");
    ZilchBindMethod(builder, type, &Space::CreateObject, (GameObject* (Space::*)()), "CreateObject", ZilchNoNames);
    ZilchBindMethod(builder, type, &Space::CreateObject, (GameObject* (Space::*)(std::string)), "CreateObject", ZilchNoNames);
    ZilchBindMethod(builder, type, &Space::FindObjectByName, (GameObject* (Space::*)(const std::string)), "FindObjectByName", ZilchNoNames);
    ZilchBindMethod(builder, type, &Space::DestroyAll, ZilchNoOverload, "DestroyAll", ZilchNoNames);
    ZilchBindMethod(builder, type, &Space::getGameSession, ZilchNoOverload, "getGameSession", ZilchNoNames);
    ZilchBindMethod(builder, type, &Space::TestSpace, ZilchNoOverload, "TestSpace", ZilchNoNames);
    //DCE_BINDING_DEFINE_METHOD_NO_ARGS(Space, TestScript);
  }



  /*!************************************************************************\
  @brief  Component Definition
  \**************************************************************************/
  ZilchDefineType(Component, "Component", DCEngineCore, builder, type) {
    // This sets Zilch's Handle manager setting
    DCE_BINDING_INTERNAL_COMPONENT_SET_HANDLE_TYPE;
    // Constructor / Destructor
    //ZilchBindConstructor(builder, type, GameObject, "name, space, gamesession", std::string, Space&, GameSession&);
    //ZilchBindConstructor(builder, type, Component, "name, owner", std::string, Entity&);
    ZilchBindMethod(builder, type, &Component::getSpace, ZilchNoOverload, "Space", ZilchNoNames);
    ZilchBindMethod(builder, type, &Component::getGameSession, ZilchNoOverload, "GameSession", ZilchNoNames);
    ZilchBindMethod(builder, type, &Component::Owner, ZilchNoOverload, "Owner", ZilchNoNames);
    ZilchBindMethod(builder, type, &Component::Initialize, ZilchNoOverload, "Initialize", ZilchNoNames);
    ZilchBindDestructor(builder, type, Component);
    // Fields
  }


  #endif

}