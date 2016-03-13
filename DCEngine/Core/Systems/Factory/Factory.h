/*****************************************************************************/
/*!
@file   Factory.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   9/20/2015
@brief  The Factory system handles the creation of all game objects and 
        components, from serialized data. 
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
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
#include "../../Resources/Archetype.h"
#include "ComponentFactory.h"

#define DCE_FACTORY_CONSTRUCT_COMPONENTS 0

namespace DCEngine {

  // Forward declarations
  class Engine;
  
  namespace Systems {

    class Factory : public System {    
      friend class Engine;

    public:             

      // GameObjects
      GameObjectPtr CreateGameObject(std::string name, Space& space, bool init); 
      GameObjectPtr CreateGameObject(ArchetypePtr archetype, Space& space, bool init);  
      GameObjectPtr BuildGameObject(SerializedMember* data, Space& space);
      EntityPtr BuildEntity(EntityPtr entity, SerializedMember* objectData);      
      // Archetypes
      using EntityBuildData = std::pair<EntityPtr, ArchetypePtr>;
      ArchetypePtr BuildArchetype(std::string, EntityPtr);
      void BuildFromArchetype(EntityPtr entity, ArchetypePtr archetype);
      GameObjectPtr BuildGameObjectFromArchetype(ArchetypePtr archetype, Space& space, bool init);
      void RebuildFromArchetype(EntityPtr entity);
      void Rebuild(EntityBuildData data);
      // Spaces
      void MarkSpace(Space&);
      void DestroySpaces();
      bool BuildFromLevel(LevelPtr level, Space&);
      GameObjectPtr BuildGameObjectAndChildren(Zilch::JsonMember* gameObject, Space& space, GameObjectPtr parent, bool nextFrame = false);
      LevelPtr BuildLevel(std::string name, Space&);
      // Components
      ComponentStrongPtr CreateComponentByName(const std::string& name, Entity& entity);
      ComponentHandle CreateComponentByNameFromZilch(const std::string& name, Entity& entity);
      ComponentStrongPtr CreateComponentByType(Zilch::BoundType* boundType, Entity& entity);
      template <typename ComponentClass> ComponentPtr CreateComponent(Entity& owner, bool init);
      void MarkComponent(ComponentHandle component);
      void MarkComponent(Component& component);
      void DestroyComponents();
      // Resources
      void MarkGameObject(GameObject& gameObj);
      void MarkForRebuild(EntityPtr entity);
      void DestroyGameObjects();         
      void AddComponentFactory(Zilch::BoundType*, std::unique_ptr<AbstractComponentFactory>);

    private:

      unsigned LastGameObjectId; //!< Incrementally generate unique IDs      
      GameObjectStrongVec ActiveGameObjects; //!< Container of active GameObjects
      ComponentVec ActiveComponents; //!< Container of active Components
      std::map<std::string, std::type_index> ComponentClassMap;
      
      std::set<EntityBuildData> EntitiesToRebuild;
      std::set<GameObjectPtr> GameObjectsToBeDeleted; 
      std::set<ComponentPtr> ComponentsToBeDeleted;
      std::set<SpacePtr> SpacesToBeDeleted;
      ComponentMap ComponentFactories;

      /* Functions */
      Factory();
      void Initialize();
      void Update(float dt); //!< Delete all objects in the to-be-deleted list
      void Terminate();      
      void ConstructComponentFactoryMap();
      Zilch::JsonMember* ArchetypeData(ArchetypePtr archetype);
      void RebuildEntities();
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
      ActiveComponents.push_back(newComp);
      // Return a reference to the component
      return newComp;
    }
  }

  using FactoryPtr = std::unique_ptr<Systems::Factory>;
}