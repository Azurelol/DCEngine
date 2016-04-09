/*****************************************************************************/
/*!
@file   Factory.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   9/20/2015
@brief  The Factory system handles the creation of all game objects and
components, from serialized data.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Factory.h"
#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {
    
    /**************************************************************************/
    /*!
    \brief  Default constructor for the Factory system.
    */
    /**************************************************************************/
    Factory::Factory() : System("FactorySystem", EnumeratedSystem::Factory) {
    }

    /**************************************************************************/
    /*!
    @brief  Initializes the Factory system.
    @todo   Find another way to add the rebound components.
    */
    /**************************************************************************/
    void Factory::Initialize() {
      if (TRACE_ON && TRACE_INITIALIZE)
        DCTrace << "Factory::Initialize \n";
      
      // Construct the component factories for each core component type
      ConstructComponentFactoryMap();
      // Construct the component factories for 'Rebound' components
      //ReboundComponentsAddToLibrary();
      ReboundComponentsAddToFactory();
    }

    /**************************************************************************/
    /*!
    \brief  Updates the factory system.
    */
    /**************************************************************************/
    void Factory::Update(float dt) {
      if (TRACE_UPDATE)
        DCTrace << "Factory::Update \n";
      SystemTimer profile(this->Name());
      // Destroy all objects on the to-be-deleted list
      DestroyGameObjects();      
      // Destroy all marked components
      DestroyComponents();
      // Destroy all marked spaces
      DestroySpaces();
      // Rebuild all specified entities
      RebuildEntities();
    }

    /**************************************************************************/
    /*!
    \brief  Terminates the Factory system.
    */
    /**************************************************************************/
    void Factory::Terminate() {
        DCTrace << "Factory::Terminate \n";
    }

    /*=====================*
    *  Object Construction *
    *=====================*/

    /**************************************************************************/
    /*!
    @brief  Creates a game object with default components.
    @param  name The name of the GameObject.
    @param  space A reference to the space where the object will be constructed on.
    @param  init Whether the GameObject should be initialized right away.
    @return A pointer to GameObject created on the space.
    @note   The factory owns a container of strong pointers of all active GameObjects.
            Everyone else just has raw pointers, which do not have ownership.
    */
    /**************************************************************************/
    GameObjectPtr Factory::CreateGameObject(std::string name, Space& space, bool init) {

      ActiveGameObjects.emplace_back(GameObjectStrongPtr(new GameObject(name, space, *space.getGameSession())));
      auto gameObjPtr = ActiveGameObjects.back().get();
      gameObjPtr->AddComponentByName(std::string("Transform"));
      // If the object needs to be initialized right away
      if (init)
        gameObjPtr->Initialize();
      return gameObjPtr;
    }

    /**************************************************************************/
    /*!
    @brief  Creates a game object from an Archetype.
    @param  archetype The archetype from which to construct this object from.
    @param  space A reference to the space where the object will be constructed on.
    @param  init Whether the GameObject should be initialized right away.
    @return A GameObject created on the space.
    @todo   Refactor this so it uses some JSON interface class rather than
            .. this.
    */
    /**************************************************************************/
    GameObjectPtr Factory::CreateGameObject(ArchetypePtr archetype, Space & space, bool init)
    {
      return BuildGameObjectFromArchetype(archetype, space, init);
    }

    /**************************************************************************/
    /*!
    @brief  Creates a Component by a given name.
    @param  name The name of the component.
    @param  entity A reference to the entity.
    @return Returns a component by strong pointer.
    */
    /**************************************************************************/
    ComponentStrongPtr Factory::CreateComponentByName(const std::string & name, Entity & entity)
    { 
      // Look up the component by name among all boundtypes      
      for (auto componentType : Component::AllComponents()) {
        auto componentTypeName = std::string(componentType->Name.c_str());
        if (componentTypeName == name) {
          //DCTrace << "Factory::CreateComponentByName - " << componentTypeName << " ? " << name << " \n";
          return std::move(CreateComponentByType(componentType, entity));
        }
      }

      DCTrace << "Factory::CreateComponentByName - Could not find the component: '" << name << "' \n";
      return nullptr;
    }

    /**************************************************************************/
    /*!
    @brief  Creates a Component by a given name through Zilch
    @param  name The name of the component.
    @param  entity A reference to the entity.
    @return Returns a component by strong pointer.
    */
    /**************************************************************************/
    ComponentHandle Factory::CreateComponentByNameFromZilch(const std::string & name, Entity & entity)
    {      
      auto state = Daisy->getSystem<Reflection>()->Handler()->GetState();
      Zilch::ExceptionReport report;

      // Get the component's BoundType
      auto boundType = Component::BoundType(name);
      // Allocate the component on the heap through Zilch
      Zilch::Handle componentHandle; 

      // If the component could not be found..
      //if (!boundType)
      //  return componentHandle;


      // C++ Components
      if (!Zilch::TypeBinding::IsA(boundType, ZilchComponent::ZilchGetStaticType())) {
        componentHandle = state->AllocateHeapObject(boundType, report, Zilch::HeapFlags::ReferenceCounted);
        Zilch::Call ctorCall(boundType->Constructors[0], state);
        ctorCall.SetHandle(Zilch::Call::This, componentHandle);
      // Call the component's constructor explicitly
        ctorCall.Set(0, entity);
        ctorCall.Invoke(report);
      }
      // Zilch Components
      else {  
        componentHandle = state->AllocateDefaultConstructedHeapObject(boundType, report, Zilch::HeapFlags::ReferenceCounted);
        if (!componentHandle.IsNull())
          Component::Dereference(componentHandle)->PostDefaultConstructor(name, entity);
      }
      // Return the handle to this component
      return componentHandle;
    }

    /**************************************************************************/
    /*!
    @brief  Creates a Component from a BoundType through the use of an instance
    of that component's factory.
    @param  boundType A pointer to the component's BoundType.
    @param  entity A reference to the entity.
    @return Returns a component by strong pointer.
    */
    /**************************************************************************/
    ComponentStrongPtr Factory::CreateComponentByType(Zilch::BoundType * boundType, Entity & entity)
    {
      if (ComponentFactories[boundType].get() == nullptr)
        throw DCException("Factory::CreateComponentByType - Tried to construct '" + std::string(boundType->Name.c_str()) + "' that's not bound yet!");

      return ComponentFactories[boundType].get()->ConstructComponent(entity);
    }

    /**************************************************************************/
    /*!
    @brief Marks the component for deletion on the next frame.
    @param component A reference to the component.
    */
    /**************************************************************************/
    void Factory::MarkComponent(ComponentHandle component)
    {
      //ComponentsToBeDeletedByHandle.insert(component);
    }

    /**************************************************************************/
    /*!
    @brief Marks the component for deletion on the next frame.
    @param component A reference to the component.
    */
    /**************************************************************************/
    void Factory::MarkComponent(Component & component)
    {
      DCTrace << "Factory::MarkComponent - " << component.getObjectName() << " has been marked for removal \n";
      ComponentsToBeDeleted.insert(ComponentPtr(&component));
    }

    /**************************************************************************/
    /*!
    @brief Destroys all marked components. This removes them from the entities
           they are attached to.    
    @todo  May have to do different logic if multiple components from the same
           object are asked to be removed on the same frame...
    */
    /**************************************************************************/
    void Factory::DestroyComponents()
    {
      for (auto& component : ComponentsToBeDeleted) {
        DCTrace << "Factory::DestroyComponents - Destroying: " << component->Name() << "\n";
        component->Owner()->RemoveComponentByName(component->Name());
      }
      ComponentsToBeDeleted.clear();
    }

    /**************************************************************************/
    /*!
    @brief Destroys all marked spaces.
    @todo  May not be working properly...
    */
    /**************************************************************************/
    void Factory::DestroySpaces()
    {
      for (auto& space : SpacesToBeDeleted) {
        space->GameSessionRef->RemoveSpace(space);        
      }
      SpacesToBeDeleted.clear();
    }
    
    /**************************************************************************/
    /*!
    @brief Adds a component factory to the Factory system.
    @param type A pointer to the component BoundType.
    @param factory A strong pointer to the component factory.
    */
    /**************************************************************************/
    void Factory::AddComponentFactory(Zilch::BoundType * type, std::unique_ptr<AbstractComponentFactory> factory)
    {
      if (DCE_TRACE_FACTORY_COMPONENT_MAP) {
        DCTrace << "Factory::AddComponentFactory - Added: '" << type->Name.c_str() << "' \n";
      }
      ComponentFactories.emplace(type, std::move(factory));
    }

    /*===================*
    *  Object Management *
    *===================*/    
    /**************************************************************************/
    /*!
    @brief  Marks the GameObject to be destroyed.
    @param  gameObj A reference to the GameObject.
    */
    /**************************************************************************/
    void Factory::MarkGameObject(GameObject& gameObj) {
      // If it isn't marked for deleteing, insert it
      if (GameObjectsToBeDeleted.count(&gameObj))
        return;
      GameObjectsToBeDeleted.insert(GameObjectPtr(&gameObj));
    }

    /**************************************************************************/
    /*!
    @brief Marks an entity to have its components rebuilt on the next frame.
    @param entity A pointer to the entity.
    */
    /**************************************************************************/
    void Factory::MarkForRebuild(EntityPtr entity)
    {
      // Create an temporary archetype out of the entity
      auto archetype = BuildArchetype(entity->Name(), entity);
      EntitiesToRebuild.insert(EntityBuildData(entity, archetype));
    }

    /**************************************************************************/
    /*!
    @brief Marks a space for destruction.
    @param entity A pointer to the space.
    */
    /**************************************************************************/
    void Factory::MarkSpace(Space& space)
    {
      SpacesToBeDeleted.insert(SpacePtr(&space));
    }

    /**************************************************************************/
    /*!
    @brief  Destroys all ActiveGameObjects in the GameObjectsToBeDeleted list.
    @note   
    */
    /**************************************************************************/
    void Factory::DestroyGameObjects()
    {      
      if (GameObjectsToBeDeleted.size() < 1)
        return;

      if (DCE_TRACE_GAMEOBJECT_DESTRUCTOR) {
        DCTrace << "Factory::DestroyGameObjects - Removing: \n";
        for (auto& gameObject : GameObjectsToBeDeleted) {
          DCTrace << " - " << gameObject->Name() << "\n";
        }
      }      

      ActiveGameObjects.erase(
        std::remove_if( // Selectively remove elements in the second vector...
          ActiveGameObjects.begin(),
          ActiveGameObjects.end(),
          [&](std::unique_ptr<GameObject> const& p)
      {   // This predicate checks whether the element is contained
          // in the second vector of pointers to be removed...
        return std::find(
          GameObjectsToBeDeleted.cbegin(),
          GameObjectsToBeDeleted.cend(),
          p.get()
          ) != GameObjectsToBeDeleted.end();
      }),
        ActiveGameObjects.end()
        );

      GameObjectsToBeDeleted.clear();

      // This is set back to default, from 'Space::LoadLevel'
      SoundInstance::StopOnDestroyed = false;
    }

    /**************************************************************************/
    /*!
    @brief Rebuilds all specified entities.
    */
    /**************************************************************************/
    void Factory::RebuildEntities()
    {
      if (EntitiesToRebuild.empty())
        return;

      // Rebuild all entities
      for (auto& rebuildData : EntitiesToRebuild) {
        Rebuild(rebuildData);
      }
      EntitiesToRebuild.clear();
    }

    /**************************************************************************/
    /*!
    @brief Parses JSON data from the Archetype so that it can be used for
           construction.
    @param archetype A pointer to the archetype.
    @return The data, as a pointer.
    */
    /**************************************************************************/
    Zilch::JsonMember * Factory::ArchetypeData(ArchetypePtr archetype)
    {
      // 1. Get the Archetype data
      Zilch::CompilationErrors errors;
      Zilch::JsonReader reader;
      const Zilch::String what;
      Zilch::JsonValue* archetypeData = reader.ReadIntoTreeFromString(errors,
        archetype->Get().c_str(), what, nullptr);
      auto data = *archetypeData->OrderedMembers.data();
      return data;
    }

  }
}