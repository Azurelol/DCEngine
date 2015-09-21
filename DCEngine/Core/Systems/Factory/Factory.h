/*****************************************************************************/
/*!
\file   Factory.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   9/20/2015
\brief  The Factory system handles the creation of all game objects and components,
        from serialized data. 
*/
/******************************************************************************/
#pragma once
#include "../System.h"

// Libraries
#include <set>
#include "Serializer.h"
//#include <JSONCPP\json.h>
#include "../../Objects/Entities/GameObject.h"

namespace DCEngine {

  class Engine;

  namespace Systems {

    class Factory : public System {    
      friend class Engine;

    public:

      GameObjectPtr CreateGameObject(const std::string& fileName, bool init);
      void DestroyGameObject(GameObject& gameObj);
      void DestroyAllObjects(); //!< Destroys all objects

    private:

      unsigned LastGameObjectId; //!< Incrementally generate unique IDs
      std::vector<GameObject> gameObjVec;
      std::set<GameObject*> gameObjsToBeDeleted;

      Factory();
      void Initialize();
      void Update(float dt); //!< Delete all objects in the to-be-deleted list
      void Terminate();
      GameObjectPtr BuildAndSerialize(const std::string& fileName);
      

    };
  }

  using FactoryPtr = std::unique_ptr<Systems::Factory>;
}