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
#include "..\Entities\GameObject.h"

namespace DCEngine {

  class Graphics;

  class GraphicsSpace : public Component {
    friend class Graphics; 

  public:
    GraphicsSpace(Entity& owner) : Component(std::string("GraphicsSpace"),
                                   EnumeratedComponent::GraphicsSpace,
                                   BitfieldComponent::GraphicsSpace, owner) {}
    void Initialize();

    void OnSpriteRegistrationEvent(Events::SpriteRegistration* eventObj);
    void OnSpriteUnregistrationEvent(Event* eventObj);
    std::vector<GameObject*> getSprites(); // Return the vector of sprites to be drawn

    void OnLogicUpdate(Events::LogicUpdate* updateEvent);

  private:
    // Container of sprites that need to be drawn
    std::vector<GameObject*> sprites_;
    // Container of models that need to be rendered
    std::vector<GameObject*> models_;

  };

}