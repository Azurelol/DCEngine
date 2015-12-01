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

  GameSession::GameSession(std::string name) : Entity(name) {
    if (TRACE_ON && TRACE_CONSTRUCTOR)
      DCTrace << ObjectName << "::GameSession - Constructor\n";
    type_ = EntityType::GameSession;
  }

  GameSession::~GameSession() {
  }

  /**************************************************************************/
  /*!
  \brief Initializes the GameSession.
  1. Systems are added to a container in the GameSession.
  2. Creates the default space.
  3. Specify which systems should be updated.
  4. All systems in the container are initialized.
  */
  /**************************************************************************/
  void GameSession::Initialize() {
    DCTrace << "[" << ObjectName << "::Initialize - Initializing all spaces... ] \n";

    // Initialize all spaces
    for (auto space : ActiveSpaces)
      space.second->Initialize();
  }

  /**************************************************************************/
  /*!
  \brief The GameSession's every space, and its systems are updated.
  When a space is updated, it provides each of the systems added to it
  with a vector of entities that meet the system's registration
  requirements.
  The space then tells each system to update.
  \param The time that elapsed during the last frame update.

  */
  /**************************************************************************/
  void GameSession::Update(float dt) {
    if (TRACE_UPDATE)
      DCTrace << ObjectName << "::Update \n";

    // DEPRECATED: Spaces are not updated by gamesession
    // Update all active spaces
    //for (auto space : ActiveSpaces)
    //  UpdateSpace(space.second, dt);
    
    if (TRACE_UPDATE)
      DCTrace << ObjectName << "::Update - All spaces updated. \n";
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
      ActiveSpaces.emplace(name, SpaceStrongPtr(new DCEngine::Space(name, *this)));

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
    SpaceMap::iterator it = ActiveSpaces.find(name);

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

  Space* GameSession::getDefaultSpace()
  {
    return DefaultSpace;
  }

  /**************************************************************************/
  /*!
  \brief Updates the space, updating all its systems.
  */
  /**************************************************************************/
  void GameSession::UpdateSpace(SpacePtr space, float dt) {
    space->Update(dt);
  }

  void GameSession::RemoveSpace(SpacePtr space)
  {
    SpaceMap::iterator it = ActiveSpaces.find(space->getObjectName());
    ActiveSpaces.erase(it);
    return;

    DCTrace << "GameSession::RemoveSpace - Removing: " << space->getObjectName() << "\n";
    DCTrace << "Remaining spaces: \n";
    for (auto& space : ActiveSpaces) {
      DCTrace << "- " << space.second->getObjectName() << "\n";
    }
  }


}