/******************************************************************************/
/*!
\file   Entity.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   7/30/2015
\brief  The x

*/
/******************************************************************************/
#include "Entity.h"

#include "Components\ComponentsInclude.h"

namespace DCEngine {

  /**************************************************************************/
  /*!
  \brief  Constructor.
  */
  /**************************************************************************/
  Entity::Entity(std::string name) {
    _name = name;
    trace << _name << "::Entity - Constructor \n";
  }

  /**************************************************************************/
  /*!
  \brief BAND-AID: Pass the update to every entity, who propagate it to their
         components. In the desired implementation, components will be updated
         through an update event... which requires an event system.
  */
  /**************************************************************************/
  void Entity::Update(float dt) {
    for (auto component : _components)
      component->Update();
  }


  /**************************************************************************/
  /*!
  \brief  Checks if the entity has all of a set of components by OR-ing
          together multiple MaskComponente values.
  \return True if the component has every specified component.
  */
  /**************************************************************************/
  bool Entity::CheckMask(mask m) {
    return ((_mask & m) == m);
  }



  /**************************************************************************/
  /*!
  \brief  Adds a component to the entity by pointer.
  \param  A component pointer to the component being added to the entity.
  */
  /**************************************************************************/
  void Entity::AddComponent(std::shared_ptr<Component> component) {
    
    // If this evaluates to 0, there is already a component of the specified type.
    //assert(_components[component->_type].get() == nullptr);

    //_components[component->_type] = component;

    

    // Adds the component to the entity
    _components.push_back(component);
    trace << _name << "::AddComponent - " << component->Name() << "\n";
    
  }

  /**************************************************************************/
  /*!
  \brief  Initialize all of the entity's components.
  */
  /**************************************************************************/
  void Entity::Initialize() {
    trace << _name << "::Initialize \n";
    for (auto component : _components)
      component->Initialize();
  }

}