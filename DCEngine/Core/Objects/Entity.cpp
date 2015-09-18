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

// Headers
#include "Components\ComponentsInclude.h"
//#include "Component.h"

#include "Entities\Space.h"
#include "Entities\GameSession.h"

namespace DCEngine {

  /**************************************************************************/
  /*!
  \brief  Constructor.
  */
  /**************************************************************************/
  Entity::Entity(std::string name) : Object("Entity") {
    _name = name;
    //if (TRACE_ON && TRACE_CONSTRUCTOR)
    //  trace << _name << "::Entity - Constructor \n";
  }

  ///**************************************************************************/
  ///*!
  //\brief BAND-AID: Pass the update to every entity, who propagate it to their
  //       components. In the desired implementation, components will be updated
  //       through an update event... which requires an event system.
  //*/
  ///**************************************************************************/
  //void Entity::Update(float dt) {

  //  for (auto component : _components)
  //    component->Update();
  //}

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
    if (TRACE_ON)
      trace << _name << "::AddComponent - " << component->Name() << "\n";
    // If this evaluates to 0, there is already a component of the specified type.
    //assert(_components[component->_type].get() == nullptr);

    //_components[component->_type] = component;
     


    // Adds the component to the entity
    _components.push_back(component);
    
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

  /**************************************************************************/
  /*!
  \brief  Returns a pointer to the given component.
  \param  The name, in a string, of the component.
  \return A pointer to the component.
  */
  /**************************************************************************/
  Component* Entity::GetComponent(std::string & name) {

    for (auto component : _components) {
      if (component->Name() == name)
        return component.get();
    }

    return NULL;

  }


}