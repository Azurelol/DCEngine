#include "Space.h"

// Space-level components
#include "../../Components/GraphicsSpace.h"
#include "../../Components/PhysicsSpace.h"
#include "../../Components/TimeSpace.h"
#include "../../Components/SoundSpace.h"
#include "../../Components/CameraViewport.h"

// Binding needs to know what a GameSession is for the Space CTOR.
#include "GameSession.h"
#include "../../Engine/Engine.h"

namespace DCEngine {
    

  
  /**************************************************************************/
  /*!
  \brief  Constructor for the Space class.
  */
  /**************************************************************************/
  Space::Space(std::string name, GameSession& gamesession) : Entity(name), GameSessionRef(&gamesession) {
    if (TRACE_ON && TRACE_CONSTRUCTOR)
      DCTrace << ObjectName << "::Space - Constructor \n";
    type_ = EntityType::Space;
  }
  
  /**************************************************************************/
  /*!
  \brief  Destructor. Clears the entities vector.
  */
  /**************************************************************************/
  Space::~Space() {        
    
  }

  /**************************************************************************/
  /*!
  \brief  Initializes the space.
  */
  /**************************************************************************/
  void Space::Initialize() {
    DCTrace << "|" << ObjectName << "::Initialize| \n";

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

    DCTrace << "[" << ObjectName << "::Initialize - Initializing all GameObjects...] \n";
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
  \brief  Terminates the Space.
  */
  /**************************************************************************/
  void Space::Terminate()
  {
    DestroyAll();
  }

  /**************************************************************************/
  /*!
  \brief  Updates every system in the space.
  */
  /**************************************************************************/
  void Space::Update(float dt) {
    if (TRACE_ON && TRACE_UPDATE)
      DCTrace << ObjectName << "::Update \n";
  }

  GameSession& Space::getGameSession() {
    return *GameSessionRef;
  }

  /**************************************************************************/
  /*!
  @brief  Destroy all the objects in the space.
  @note   
  */
  /**************************************************************************/
  void Space::DestroyAll()
  {
    for (auto object : GameObjectContainer) {
      RemoveObject(*object);
    }

  }

  /**************************************************************************/
  /*!
  @brief  Loads a level, a container for entities, into the space. 
  */
  /**************************************************************************/
  void Space::LoadLevel(LevelPtr level) {
    if (TRACE_ON)
      DCTrace << ObjectName << "::LoadLevel - Loading " << level->Name() << " level.\n";
        
    // Clear the current objects from the space
    //DestroyAll();

    // Set it as the current level
    CurrentLevel = level;

    // Load GameObjects into the space
    for (auto gameObject : CurrentLevel->GameObjects) {
      AddObject(gameObject);
    }      


    // Initialize every GameObject
    for (auto gameObject : GameObjectContainer) {
      gameObject->Initialize();
    }
    // Read the JSON string
    
    // For every object 

    // For the level, load each object: Construct an empty object from the factory
    // For each object, load the components: 
    // Have a map that maps the name of the component on strings, and maps it to a function
    // that creates the derived component and returns a base component class pointer.
    // Have some switch statement that takes whatever info is needed to serialize, the name of the coponent
    // perhaps the entity too. It can create the component, fil it with data and attach it to the entity.

    // For each component, load the data. Read the values in from the serialized string.



  }

  /**************************************************************************/
  /*!
  \brief  Reloads the current level.
  */
  /**************************************************************************/
  void Space::ReloadLevel()
  {
    // First, destroy all current objects in the space
    DestroyAll();
    // Load the level again
    LoadLevel(CurrentLevel);
  }

  /**************************************************************************/
  /*!
  @brief  Creates a GameObject, adds it to the space.
  @return A pointer to the GameObject that was added.
  @note   This function requests the GameObject to be created through
          the object factory.
  */
  /**************************************************************************/
  GameObjectPtr Space::CreateObject() {
    // Calls the object factory to create the object from an archetype
    auto gameObject = Daisy->getSystem<Systems::Factory>()->CreateGameObject("Transform", *this, true);
    GameObjectContainer.push_back(gameObject);
    return gameObject;
  }

  /**************************************************************************/
  /*!
  @brief  Creates a GameObject from an Archetype and adds it to the space.
  @return A pointer to the GameObject that was added.
  \note   This function requests the entity to be created through
  the object factory.
  */
  /**************************************************************************/
  GameObjectPtr Space::CreateObject(std::shared_ptr<Archetype> archetype)
  {
    // Creates the GameObject from an Archetype

    return GameObjectPtr();
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
        return gameObj;
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
    //ChildrenContainer.push_back(dynamic_cast<Entity*>(gameObject.get()));

    if (TRACE_GAMEOBJECT_ADD)
      DCTrace << ObjectName << "::AddObject - Added " << gameObject->Name() << " to the space.\n";
  }

  /**************************************************************************/
  /*!
  @brief  Removes the GameObject from the Space.
  @param  A reference to the GameObject.
  */
  /**************************************************************************/
  void Space::RemoveObject(GameObject & gameObj)
  {
    // Remove the GameObject from the space's list of GameObjects
    for (auto gameObjPtr : GameObjectContainer) {
      if (gameObjPtr == &gameObj) {
        gameObjPtr = nullptr;
      }
    }

    // Mark the object for destruction on next frmae
    Daisy->getSystem<Systems::Factory>()->MarkGameObject(gameObj);
    if (TRACE_GAMEOBJECT_ADD)
      DCTrace << ObjectName << "::AddObject - Removed " << gameObj.Name() << " from the space.\n";

  }


  /**************************************************************************/
  /*!
  \brief  Removes all systems and entities from the space.
  */
  /**************************************************************************/
  void Space::Clear() {
    GameObjectContainer.clear();
  }



} // DCEngine