#include "Space.h"

// TEST: Level loading
#include "..\..\..\Projects\Dollhouse\Dollhouse.h"

// Space Components
#include "../Components/SoundSpace.h"

namespace DCEngine {
    
  extern std::unique_ptr<Engine> Daisy;
  
  /**************************************************************************/
  /*!
  \brief  Constructor for the Space class.
  */
  /**************************************************************************/
  Space::Space(std::string& name) : Entity(name) {
    if (TRACE_ON && TRACE_CONSTRUCTOR)
      trace << _name << "::Space - Constructor \n";
    type_ = EntityType::Space;



    // TEST
    //Initialize();
  }
  
  /**************************************************************************/
  /*!
  \brief  Destructor. Clears the entities vector.
  */
  /**************************************************************************/
  Space::~Space() {
    gameobjects_.clear();
  }

  /**************************************************************************/
  /*!
  \brief  Initializes the space.
  */
  /**************************************************************************/
  void Space::Initialize() {
    trace << _name << "::Initialize \n";

    // Add Space-type components
    AddComponent(ComponentPtr(new SoundSpace(*this)));

    // Initialize Space-components
    for (auto component : _components) {
      component->Initialize();
    }    


    // TESTING: Level loading
    LevelPtr dollhouse = LevelPtr(new DollHouse(*this, *gamesession_));
    LoadLevel(dollhouse);
    //LoadLevel(LevelPtr(new DollHouse(*this, *gamesession_)));
    
    // Initialize all entities (in effect, initializing all attached components)
    for (auto gameObject : gameobjects_) {
      // TEMPORARY: Should space, gamesession be even set this way?
      //            Should they not be set on the constructor?
      //            The problem is the Level object is constructing them,
      //            at the moment.
      gameObject->space_ = this;
      gameObject->gamesession_ = gamesession_;
      gameObject->Initialize();
    }

    // TEMPORARY: THIS IS MY JAM
    
      
  }

  /**************************************************************************/
  /*!
  \brief  Updates every system in the space.
  */
  /**************************************************************************/
  void Space::Update(float dt) {
    if (TRACE_ON && TRACE_UPDATE)
      trace << _name << "::Update \n";

   


    // !!! BAND-AID: Pass the update to every entity, who propagate it to their
    // components. In the desired implementation, components will be updated
    // through an update event... which requires an event system.
    for (auto entity : gameobjects_)
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

  GameSession * Space::GetGameSession() {
    return gamesession_;
  }

  /**************************************************************************/
  /*!
  \brief  Loads a level, container for entities, into the space. 
  */
  /**************************************************************************/
  void Space::LoadLevel(LevelPtr level) {
    if (TRACE_ON)
      trace << _name << "::LoadLevel - Loading " << level->Name() << " level.\n";

    // Set it as the current level
    _currentLevel = level;

    // Load GameObjects into the space
    for (auto gameObject : _currentLevel->GameObjects) {
      AddObject(gameObject);
      // TEMPORARY: Set the component's space and gamesession
      for (auto component : gameObject->_components) {
        //component->_space = this;
        //component->_gameSession = _gameSession;
      }
    }      

  }

  /**************************************************************************/
  /*!
  \brief  Creates an entity, adds it to the space.
  \return A pointer to the entity that was added.
  */
  /**************************************************************************/
  GameObjectPtr Space::CreateObject() {
    gameobjects_.push_back(std::shared_ptr<GameObject>(new GameObject));
    return gameobjects_.back();
  }

  /**************************************************************************/
  /*!
  \brief  Adds an entity directly to the space.
  */
  /**************************************************************************/
  void Space::AddObject(GameObjectPtr gameObject) {
    gameobjects_.push_back(gameObject);

    if (TRACE_ON)
      trace << _name << "::AddEntity - Added " << gameObject->Name() << " to the space.\n";
  }

  /**************************************************************************/
  /*!
  \brief  Checks if the entity has all of a set of components by OR-ing
          together multiple MaskComponente values.
  \return True if the component has every specified component.
  */
  /**************************************************************************/
  void Space::PopulateObjects(SystemPtr sys) const {
    
    // Clear out old entities
    sys->ClearEntities();
    
    if (sys->Mask() != static_cast<int>(BitfieldComponent::NoObjects)) {
      
      // Add any entities living in this space that fit the system
      // to its cache.
      for (auto &it : gameobjects_) {
        
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
    gameobjects_.clear();
    _systems.clear();
  }



} // DCEngine