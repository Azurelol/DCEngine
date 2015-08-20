#include "GameSession.h"

#include "../../Debug/Debug.h"


namespace DCEngine {

  extern std::unique_ptr<Engine> ENGINE;

  GameSession::GameSession(std::string& name) : Entity(name) {
    trace << "GameSession::GameSession constructed of name: " << name << " created \n";
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
    trace << "GameSession::Initialize \n";

    // Systems are added to the GameSession
    _systems.push_back(SystemPtr(new Systems::GraphicsGL));

    // Creates the default space
    SpacePtr space = CreateSpace(_defaultSpaceName);

    // Initialize all internal systems
    for (auto sys : _systems) {
      sys->Initialize();
    }
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
    trace << "GameSession::Update \n";

    // Update all the systems in the space
    for (auto space : _spaces)
      UpdateSpace(space.second, dt);

    trace << "GameSession::Update - All systems updated. \n";
  }

  /**************************************************************************/
  /*!
  \brief  Creates a space with the given name.
  \param  The name of the space.
  \return A pointer to the space.
  */
  /**************************************************************************/
  SpacePtr GameSession::CreateSpace(std::string name) {
    trace << "GameSession::CreateSpace - " << name << " has been constructed. \n";

    // Check if the space already exists.
    auto space = _spaces.find(name);
    if (space != _spaces.end())
      return (*space).second;

    // If the space doesn't exist, create it
    else
      _spaces.emplace(name, SpacePtr(new Space(name)));

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
    SpaceMap::iterator it = _spaces.find(name);

    // Check if the space was found
    if (it != _spaces.end())
      return it->second;

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
  \brief Updates the space, updating all its systems.
  */
  /**************************************************************************/
  void GameSession::UpdateSpace(SpacePtr space, float dt) {
    space->Update(dt);
  }
}