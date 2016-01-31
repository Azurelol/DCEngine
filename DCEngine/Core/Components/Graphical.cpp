/******************************************************************************/
/*!
@file   Graphical.cpp
@author William Mao, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   1/22/2016
@brief  The Graphical component is the base class from which all graphical
components derive frm.

@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Graphical.h"

namespace DCEngine {
  namespace Components {

    /**************************************************************************/
    /*!
    @brief Provides the definition of this class to Zilch.
    @note This can only go in the translational unit (.cpp)
    */
    /**************************************************************************/
    #if(DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDefineType(Graphical, "Graphical", DCEngineCore, builder, type) {
      //DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(Collider);
      DCE_BINDING_DEFINE_ATTRIBUTE(Hidden);
      // Constructor / Destructor
      ZilchBindConstructor(builder, type, Graphical, "graphicalComponent, owner", std::string, Entity&);
      ZilchBindDestructor(builder, type, Graphical);
      //DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(Graphical);
      // Properties
			DCE_BINDING_DEFINE_PROPERTY(Graphical, DrawLayer);
    }
    #endif

    /**************************************************************************/
    /*!
    @brief Graphical constructor.
    @param owner A reference to the Entity that owns this component.
    @param graphicalComponent The name of the derived graphical component.
    */
    /**************************************************************************/
		Graphical::Graphical(std::string graphicalComponent, Entity & owner) : Component(graphicalComponent, owner),
			DrawLayer(0)
    {
    }

    /**************************************************************************/
    /*!
    @brief Graphical destructor.
    */
    /**************************************************************************/
    Graphical::~Graphical()
    {
    }

    /**************************************************************************/
    /*!
    @brief Graphical initializer
    */
    /**************************************************************************/
    void Graphical::Initialize()
    {

    }

  }
}