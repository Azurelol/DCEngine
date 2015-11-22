/******************************************************************************/
/*!
@file   ComponentFactoryMap.cpp
@author Christian Sagel
@par    email: blaine.reiner\@digipen.edu
@par    DigiPen login: blaine.reiner
@date   11/20/2015
@brief  Constructs a component factory for every bound component class in
        the engine. This allows them to be created at runtime through
        reflection.
*/
/******************************************************************************/
#include "Factory.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief  Constructs a component factory for every bound component type.
    */
    /**************************************************************************/
    void Factory::ConstructComponentFactoryMap()
    {
      DCTrace << "Factory::ConstructComponentFactoryMap - Constructing all component factories \n";
      AddComponentFactory(Transform::ZilchGetStaticType(), std::make_unique<ComponentFactory<Transform>>());
      AddComponentFactory(Sprite::ZilchGetStaticType(), std::make_unique<ComponentFactory<Sprite>>());



    }

  }
}