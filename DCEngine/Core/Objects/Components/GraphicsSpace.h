/*****************************************************************************/
/*!
\file   GraphicsSpace.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   9/16/2015
\brief  This component receives drawing requests from entities with Sprite,
        SpriteText and Model components, and passes the requests to the 
        engine's underlying graphics system.
*/
/******************************************************************************/
#pragma once
#include "..\Component.h"

namespace DCEngine {

  class GraphicsSpace : public Component {
  public:
    GraphicsSpace(Entity& owner) : Component(std::string("GraphicsSpace"),
                                   EnumeratedComponent::GraphicsSpace,
                                   BitfieldComponent::GraphicsSpace, owner) {}
    void Initialize();
    void OnLogicUpdate(Event* updateEvent);
    
    void Draw();
    void Render(); //!< Renders a sprite a draw



  };

}