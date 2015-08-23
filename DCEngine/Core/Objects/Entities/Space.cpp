#include "Space.h"

// TEST: Level loading
#include "..\..\..\Projects\Dollhouse\Dollhouse.h"

namespace DCEngine {

  extern std::unique_ptr<Engine> Daisy;
  
  /**************************************************************************/
  /*!
  \brief  Constructor for the Space class.
  */
  /**************************************************************************/
  Space::Space(std::string & name) : Entity(name) {
    if (TRACE_ON)
      trace << _name << "::Space - Constructor \n";

    // TEST
    //Initialize();
  }
  
  /**************************************************************************/
  /*!
  \brief  Destructor. Clears the entities vector.
  */
  /**************************************************************************/
  Space::~Space() {
    _entities.clear();
  }

  /**************************************************************************/
  /*!
  \brief  Initializes the space.
  */
  /**************************************************************************/
  void Space::Initialize() {
    trace << _name << "::Initialize \n";

    // TESTING: Level loading
    LoadLevel(LevelPtr(new DollHouse));
    
    // Initialize all entities (in effect, initializing all attached components)
    for (auto entity : _entities)
      entity->Initialize();

  }

  /**************************************************************************/
  /*!
  \brief  Updates every system in the space.
  */
  /**************************************************************************/
  void Space::Update(float dt) {
    if (TRACE_ON)
      trace << _name << "::Update \n";

    // !!! BAND-AID: Pass the update to every entity, who propagate it to their
    // components. In the desired implementation, components will be updated
    // through an update event... which requires an event system.
    for (auto entity : _entities)
      entity->Update(dt);

    for (auto system : _systems)
      system->Update(dt);

    //for (auto &system : _systems) {
    //  // Checks if the system has all the entities in this space which
    //  // meet the requirements
    //  PopulateEntities(system);

    //  // Update the system now that it has been filled with the correct entities.
    //  system->Update(dt);
    //}
  }

  /**************************************************************************/
  /*!
  \brief  Adds an engine's system to the space.
  */
  /**************************************************************************/
  void Space::AddSystem(SystemPtr system) {
    for (auto systems : _systems) {
      if (systems == system)
        throw std::exception("Attempted to add two copies of the same system to one space!");
    }

    _systems.push_back(system);
    
    if (TRACE_ON)
      trace << _name << "::AddSystem " << "- Added " << system->_name << "\n";
  }

  /**************************************************************************/
  /*!
  \brief  Loads a level, container for entities, into the space. 
  */
  /**************************************************************************/
  void Space::LoadLevel(LevelPtr level) {
    trace << _name << "::LoadLevel - Loading " << level->Name() << " level.\n";

    // Set it as the current level
    _currentLevel = level;

    // Load entities into the space
    for (auto entity : _currentLevel->Entities) {
      AddEntity(entity);
    }      

  }

  /**************************************************************************/
  /*!
  \brief  Creates an entity, adds it to the space.
  \return A pointer to the entity that was added.
  */
  /**************************************************************************/
  EntityPtr Space::CreateEntity() {
    _entities.push_back(std::shared_ptr<Entity>(new Entity));
    return _entities.back();
  }

  /**************************************************************************/
  /*!
  \brief  Adds an entity directly to the space.
  */
  /**************************************************************************/
  void Space::AddEntity(EntityPtr entity) {
    _entities.push_back(entity);

    if (TRACE_ON)
      trace << _name << "::AddEntity - Added " << entity->Name() << " to the space.\n";
  }

  /**************************************************************************/
  /*!
  \brief  Checks if the entity has all of a set of components by OR-ing
          together multiple MaskComponente values.
  \return True if the component has every specified component.
  */
  /**************************************************************************/
  void Space::PopulateEntities(SystemPtr sys) const {
    
    // Clear out old entities
    sys->ClearEntities();
    
    if (sys->Mask() != static_cast<int>(BitfieldComponent::NoObjects)) {
      
      // Add any entities living in this space that fit the system
      // to its cache.
      for (auto &it : _entities) {
        
        auto m = sys->Mask();
        if (it->CheckMask(m))
          sys->_entities.push_back(it);
      }
    }
  }

  /**************************************************************************/
  /*!
  \brief  Removes all systems and entities from the space.
  */
  /**************************************************************************/
  void Space::Clear() {
    _entities.clear();
    _systems.clear();
  }

} // DCEngine