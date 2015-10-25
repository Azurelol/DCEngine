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
  Space::Space(std::string& name, GameSession& gamesession) : Entity(name), GameSessionRef(&gamesession) {
    if (TRACE_ON && TRACE_CONSTRUCTOR)
      trace << ObjName << "::Space - Constructor \n";
    type_ = EntityType::Space;
  }
  
  /**************************************************************************/
  /*!
  \brief  Destructor. Clears the entities vector.
  */
  /**************************************************************************/
  Space::~Space() {
    GameObjectContainer.clear();
  }

  /**************************************************************************/
  /*!
  \brief  Initializes the space.
  */
  /**************************************************************************/
  void Space::Initialize() {
    trace << "|" << ObjName << "::Initialize| \n";

    // Add Space-type components
    AddComponent(ComponentPtr(new SoundSpace(*this)));
    AddComponent(ComponentPtr(new TimeSpace(*this)));
    AddComponent(ComponentPtr(new PhysicsSpace(*this)));
    AddComponent(ComponentPtr(new CameraViewport(*this)));
    AddComponent(ComponentPtr(new GraphicsSpace(*this)));

    // Initialize Space-components
    for (auto component : ComponentsContainer) {
      component->Initialize();
    }        

    trace << "[" << ObjName << "::Initialize - Initializing all GameObjects...] \n";
    // Initialize all entities (in effect, initializing all attached components)
    for (auto gameObject : GameObjectContainer) {
      // TEMPORARY: Should space, gamesession be even set this way?
      //            Should they not be set on the constructor?
      //            The problem is the Level object is constructing them,
      //            at the moment.
      gameObject->SpaceRef = this;
      gameObject->GamesessionRef = GameSessionRef;
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
      trace << ObjName << "::Update \n";
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


    for (auto systems : SystemsContainer) {
      if (systems == system)
        throw std::exception("Attempted to add two copies of the same system to one space!");
    }

    SystemsContainer.push_back(system);
    
    if (TRACE_ON)
      trace << ObjName << "::AddSystem " << "- Added " << system->SysName << "\n";
  }

  GameSession& Space::getGameSession() {
    return *GameSessionRef;
  }

  /**************************************************************************/
  /*!
  \brief  Loads a level, container for entities, into the space. 
  */
  /**************************************************************************/
  void Space::LoadLevel(LevelPtr level) {
    if (TRACE_ON)
      trace << ObjName << "::LoadLevel - Loading " << level->Name() << " level.\n";

    // Set it as the current level
    CurrentLevel = level;

    // Load GameObjects into the space
    for (auto gameObject : CurrentLevel->GameObjects) {
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

    GameObjectContainer.push_back(std::shared_ptr<GameObject>(new GameObject));
    return GameObjectContainer.back();
  }

  /**************************************************************************/
  /*!
  \brief  Finds a GameObject in the current space and returns a pointer to it.
  \return A pointer to the GameObject that was added. NULL if the GameObject
          was not found.
  */
  /**************************************************************************/
  GameObject* Space::FindObjectByName(const std::string name) {
    // Search through the space's gameobjects
    for (auto gameObj : GameObjectContainer) {
      if (gameObj->Name() == name)
        return gameObj.get();
    }
    // No match was found.
    return NULL;
  }

  GameObjectVec* Space::AllObjects()
  {
    return &GameObjectContainer;
  }

  /**************************************************************************/
  /*!
  \brief  Adds an entity directly to the space.
  */
  /**************************************************************************/
  void Space::AddObject(GameObjectPtr gameObject) {
    GameObjectContainer.push_back(gameObject);
    ChildrenContainer.push_back(dynamic_cast<Entity*>(gameObject.get()));

    if (TRACE_GAMEOBJECT_ADD)
      trace << ObjName << "::AddEntity - Added " << gameObject->Name() << " to the space.\n";
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
      for (auto &it : GameObjectContainer) {
        
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
    GameObjectContainer.clear();
    SystemsContainer.clear();
  }



} // DCEngine