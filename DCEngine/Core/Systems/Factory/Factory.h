/*****************************************************************************/
/*!
\file   Factory.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   9/20/2015
\brief  The Factory system handles the creation of all game objects and 
        components, from serialized data. 
*/
/******************************************************************************/
#pragma once
#include "../System.h"

// Libraries
#include <set>
// Headers
#include "../../Systems/Serialization/Serialization.h"
#include "../../Objects/ObjectsInclude.h"
#include "../../Objects/Entities/EntitiesInclude.h"
#include "../../ComponentsInclude.h"

namespace DCEngine {

  // Forward declarations
  class Engine;
  
  namespace Systems {

    class Factory : public System {    
      friend class Engine;

    public:                  
      /* GameObjects*/
      GameObjectPtr CreateGameObject(std::string& name, Space& space, bool init); //!< Creates a default gameObj
      GameObjectPtr CreateGameObject(const std::string& gameObjName, const Space& space, bool init);
      
      /* Components */
      ComponentPtr CreateComponent(const std::string& compName, bool init);
      template <typename ComponentClass> ComponentPtr CreateComponent(Entity& owner, bool init);

      /* Resources*/
      ResourcePtr CreateResource(const std::string& resourceName, bool init);

      void DestroyGameObject(GameObject& gameObj);
      void DestroyAllObjects(); //!< Destroys all objects

      void LoadResources(); //!< Load project resources into different maps
      void LoadLevel(std::string& levelName);

      // !< When components are constructed they register themselves to the map.
      template <typename ComponentClass> void RegisterComponent(std::string& componentName); 


    private:

      unsigned LastGameObjectId; //!< Incrementally generate unique IDs
      std::map<std::string, std::type_index> ComponentClassMap; 
      GameObjectVec gameObjVec; //!< Container of active GameObjects
      ComponentVec ComponentContainer; //!< Container of active Components
      std::set<GameObject*> gameObjsToBeDeleted; 

      /* Functions */
      Factory();
      void Initialize();
      void Update(float dt); //!< Delete all objects in the to-be-deleted list
      void Terminate();
      virtual void Serialize(Json::Value& root);
      virtual void Deserialize(Json::Value& root);

      GameObjectPtr BuildAndSerialize(const std::string& fileName);
      void DeserializeLevel(const std::string& levelName);  //!< Loads a level, from a level map

      


    }; 

    /**************************************************************************/
    /*!
                                      TEMPLATES
    */
    /**************************************************************************/
    /**************************************************************************/
    /*!
    @brief  Creates a component and returns it.
    */
    /**************************************************************************/
    template<typename ComponentClass>
    inline ComponentPtr Factory::CreateComponent(Entity& owner, bool init)
    {      
      // Create a component and set its owner
      ComponentPtr newComp = ComponentPtr(new ComponentClass(owner));
      // Add the component to the factory's list of active components
      ComponentContainer.push_back(newComp);
      // Return a reference to the component
      return newComp;
    }
  }

  using FactoryPtr = std::unique_ptr<Systems::Factory>;
}