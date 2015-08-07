#include "Space.h"
#include "..\Debug\Debug.h"

namespace DCEngine {

  extern std::unique_ptr<Engine> ENGINE;
  
  /**************************************************************************/
  /*!
  \brief  Constructor for the Space class.
  */
  /**************************************************************************/
  Space::Space(std::string & name) : Entity(name) {
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
  \brief  Updates every system in the space.
  */
  /**************************************************************************/
  void Space::Update(float dt) {
    for (auto &system : _systems) {
      // Checks if the system has all the entities in this space which
      // meet the requirements
      PopulateEntities(system);

      // Update the system now that it has been filled with the correct entities.
      system->Update(dt);
    }
  }

  /**************************************************************************/
  /*!
  \brief  Adds the system to the space.
  */
  /**************************************************************************/
  void Space::AddSystem(SystemPtr system) {
    for (auto systems : _systems) {
      if (systems == system)
        throw std::exception("Attempted to add two copies of the same system to one space!");
    }

    _systems.push_back(system);
    std::cout << "Space::AddSystem; " << _name << " has added " << system->_name << std::endl;
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