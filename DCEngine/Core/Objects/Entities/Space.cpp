#include "Space.h"

// Space-level components
#include "../../Components/GraphicsSpace.h"
#include "../../Components/PhysicsSpace.h"
#include "../../Components/TimeSpace.h"
#include "../../Components/SoundSpace.h"
#include "../../Components/CameraViewport.h"

namespace DCEngine {
    
  extern std::unique_ptr<Engine> Daisy;
  
  /**************************************************************************/
  /*!
  \brief  Constructor for the Space class.
  */
  /**************************************************************************/
  Space::Space(std::string& name, GameSession& gamesession) : Entity(name), gamesession_(&gamesession) {
    if (TRACE_ON && TRACE_CONSTRUCTOR)
      trace << _name << "::Space - Constructor \n";
    type_ = EntityType::Space;
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
    trace << "|" << _name << "::Initialize| \n";

    // Add Space-type components
    AddComponent(ComponentPtr(new SoundSpace(*this)));
    AddComponent(ComponentPtr(new TimeSpace(*this)));
    AddComponent(ComponentPtr(new GraphicsSpace(*this)));
    AddComponent(ComponentPtr(new CameraViewport(*this)));

    // Initialize Space-components
    for (auto component : components_) {
      component->Initialize();
    }        

    trace << "[" << _name << "::Initialize - Initializing all GameObjects...] \n";
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
  }

  /**************************************************************************/
  /*!
  \brief  Updates every system in the space.
  */
  /**************************************************************************/
  void Space::Update(float dt) {
    if (TRACE_ON && TRACE_UPDATE)
      trace << _name << "::Update \n";
  }

  void Space::Serialize(Json::Value & root) {
  }

  void Space::Deserialize(Json::Value & root) {
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

  GameSession& Space::getGameSession() {
    return *gamesession_;
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
    }      

  }

  /**************************************************************************/
  /*!
  \brief  Creates a GameObject, adds it to the space.
  \return A pointer to the entity that was added.
  \note   This function requests the entity to be created through
          the object factory.
  */
  /**************************************************************************/
  GameObjectPtr Space::CreateObject() {
    // Calls the object factory to create the object from an archetype

    gameobjects_.push_back(std::shared_ptr<GameObject>(new GameObject));
    return gameobjects_.back();
  }

  /**************************************************************************/
  /*!
  \brief  Finds a GameObject in the current space and returns a pointer to it.
  \return A pointer to the GameObject that was added. NULL if the GameObject
          was not found.
  */
  /**************************************************************************/
  GameObject* Space::FindObjectByName(const std::string & name) {
    // Search through the space's gameobjects
    for (auto gameObj : gameobjects_) {
      if (gameObj->Name() == name)
        return gameObj.get();
    }
    // No match was found.
    return NULL;
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