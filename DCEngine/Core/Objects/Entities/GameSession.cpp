/******************************************************************************/
/*!
\file   GameSession.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/18/2015
\brief  The container object for the spaces in the game, at the highest hierarchy.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/

#include "GameSession.h"

#include "../../Debug/Debug.h"
#include "../../Engine/Engine.h"

namespace DCEngine {

  /*!************************************************************************\
  @brief  GameSession Definition
  \**************************************************************************/
  ZilchDefineType(GameSession, "GameSession", DCEngineCore, builder, type) {
    DCE_BINDING_SET_HANDLE_TYPE_POINTER;
    // Constructor / Destructor
    ZilchBindConstructor(builder, type, GameSession, "name", std::string);
    ZilchBindDestructor(builder, type, GameSession);
    ZilchBindMethod(builder, type, &GameSession::CreateSpace, ZilchNoOverload, "CreateSpace", "name, initialize");
    ZilchBindMethod(builder, type, &GameSession::GetSpace, ZilchNoOverload, "FindSpaceByName", "name");
    ZilchBindMethod(builder, type, &GameSession::getDefaultSpace, ZilchNoOverload, "GetDefaultSpace", ZilchNoNames);
    ZilchBindMethod(builder, type, &GameSession::Quit, ZilchNoOverload, "Quit", ZilchNoNames);
  }

  /**************************************************************************/
  /*!
  \brief GameSession constructor.
  */
  /**************************************************************************/
  GameSession::GameSession(std::string name) : Entity(name) {
    // Sets the default archetype
    setArchetype("GameSession");
    if (TRACE_ON && TRACE_CONSTRUCTOR) DCTrace << ObjectName << "::GameSession - Constructor\n";
    type_ = EntityType::GameSession;
  }

  /**************************************************************************/
  /*!
  \brief GameSession destructor.
  */
  /**************************************************************************/
  GameSession::~GameSession() {
  }

  /**************************************************************************/
  /*!
  \brief Returns the current GameSession.
  */
  /**************************************************************************/
  GameSession*  GameSession::Get()
  {
    return Daisy->getGameSession();
  }

  /**************************************************************************/
  /*!
  @brief Serializes a GameSession.
  @param builder A reference to the JSON builder.
  @note  This will serialize the GameSession's properties, then its components.
  */
  /**************************************************************************/
  void GameSession::Serialize(Zilch::JsonBuilder & builder)
  {
    // Grab a reference to the Zilch Interface
    auto interface = Daisy->getSystem<Systems::Reflection>()->Handler();
    builder.Key("GameSession");
    builder.Begin(Zilch::JsonType::Object);
    {
      // Serialize GameSession properties
      SerializeByType(builder, interface->GetState(), ZilchTypeId(GameSession), this);
      // Serialize the underlying Entity object, which includes its components.
      Entity::Serialize(builder);
    }
    builder.End();
  }

  /**************************************************************************/
  /*!
  @brief Deserializes the GameSession.
  @param properties A pointer to the object containing the properties.
  @note  This will deserialize the GameSession's properties, then its components.
  */
  /**************************************************************************/
  void GameSession::Deserialize(Zilch::JsonValue * properties)
  {
    // Grab a reference to the Zilch Interface
    auto interface = Daisy->getSystem<Systems::Reflection>()->Handler();
    // Deserialize the underlying Entity
    Entity::Deserialize(properties);
    // Deserialize the GameSession properties
    DeserializeByType(properties, interface->GetState(), ZilchTypeId(GameSession), this);
  }

  /**************************************************************************/
  /*!
  \brief Initializes the GameSession.
  */
  /**************************************************************************/
  void GameSession::Initialize() {
    DCTrace << "[" << ObjectName << "::Initialize - Initializing all spaces... ] \n";

    // Initialize all spaces
    for (auto& space : ActiveSpaces) {
      space.second->Initialize();
    }

    // Construct GameSession specific components
    AddComponentByName("DefaultGameSetup", true);
    
  }

  /**************************************************************************/
  /*!
  \brief Subscribes to Game-specific events.
  */
  /**************************************************************************/
  void GameSession::Subscribe()
  {
   // Daisy->Connect<Events::GameFocusIn>(*this, &GameSession::)
  }


  /**************************************************************************/
  /*!
  \brief  Creates a space with the given name.
  \param  The name of the space.
  \return A pointer to the space.
  */
  /**************************************************************************/
  SpacePtr GameSession::CreateSpace(std::string name, bool initialize) {
    DCTrace << ObjectName << "::CreateSpace - " << name << " has been constructed. \n";

    // Check if the space already exists.    
    //if (ActiveSpaces.count(name))
    //  return ActiveSpaces.at(name);

    auto space = ActiveSpaces.find(name);
    if (space != ActiveSpaces.end())
      return (*space).second.get();

    // If the space doesn't exist, create it
    else
      ActiveSpaces.emplace(name, Space::StrongPtr(new DCEngine::Space(name, *this)));

    /* http://en.cppreference.com/w/cpp/container/unordered_map/emplace
    Inserts a new element into the container by constructing it in-place
    with the given args if there is no element with the key in the container.
    Careful use of emplace allows the new element to be constructed while
    avoiding unnecessary copy or move operations. The constructor of the new
    element (i.e. std::pair<const Key, T>) is called with exactly the same arguments
    as supplied to emplace, forwarded via std::forward<Args>(args)....

    If rehashing occurs due to the insertion, all iterators are invalidated.
    Otherwise iterators are not affected. References are not invalidated.
    Rehashing occurs only if the new number of elements is equal to or greater
    than max_load_factor()*bucket_count().
    */

    // Set the space's gamesession
    GetSpace(name)->GameSessionRef = this;
    

    // Initialize the space
    if (initialize)
      GetSpace(name)->Initialize();


    return GetSpace(name);
  }

  /**************************************************************************/
  /*!
  \brief  Returns a pointer to the space.
  \param  The name of the space.
  \return A pointer to the space.
  */
  /**************************************************************************/
  SpacePtr GameSession::GetSpace(std::string name) {
    // Searches for a space with the specified name
    auto& it = ActiveSpaces.find(name);

    // Check if the space was found
    if (it != ActiveSpaces.end())
      return it->second.get();

    /*
    Dereferencing a std::map iterator gives you a std::pair,
    you can then access its elements with first and second. For example, (*it).first
    will give you the key and (*it).second will give you the value.
    These are equivalent to it->first and it->second.
    */

    throw std::range_error("The specified space does not exist.");
  }

  /**************************************************************************/
  /*!
  @brief  Returns a container of all the active spaces.
  @return A pointer to the container of all active spaces.
  */
  /**************************************************************************/
  Space::Container& GameSession::AllSpaces()
  {
    return ActiveSpaces;
  }

  /**************************************************************************/
  /*!
  @brief  Gets the default Space.
  @return A pointer to the default Space.
  */
  /**************************************************************************/
  SpacePtr GameSession::getDefaultSpace()
  {
    return DefaultSpace;
  }

  /**************************************************************************/
  /*!
  \brief Quits the Game.
  */
  /**************************************************************************/
  void GameSession::Quit()
  {
    DispatchGameEvents::GameRequestQuit();
  }


  /**************************************************************************/
  /*!
  \brief Updates the space, updating all its systems.
  */
  /**************************************************************************/
  void GameSession::UpdateSpace(SpacePtr space, float dt) {
    space->Update(dt);
  }

  /**************************************************************************/
  /*!
  \brief Removes a space.
  */
  /**************************************************************************/
  void GameSession::RemoveSpace(SpacePtr space)
  {
    auto it = ActiveSpaces.find(space->getObjectName());
    if (it != ActiveSpaces.end())
      ActiveSpaces.erase(it);
  }

}