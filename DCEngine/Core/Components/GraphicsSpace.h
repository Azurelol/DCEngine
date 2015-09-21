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
#include "ComponentReference.h"
#include "../Objects/Entities/GameObject.h"

namespace DCEngine {

  enum class ProjectionType {
    Orthographic,
    Perspective,
  };

  class Graphics;
  class GraphicsSpace : public Component {
    friend class Graphics; 

  public:

    GraphicsSpace(Entity& owner) : Component(std::string("GraphicsSpace"), owner) {}
    void Initialize();

    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);
    void OnSpriteRegistrationEvent(Events::SpriteRegistration* eventObj);
    void OnSpriteUnregistrationEvent(Event* eventObj);
    std::vector<GameObject*> getSprites(); // Return the vector of sprites to be drawn

    void OnLogicUpdate(Events::LogicUpdate* updateEvent);

  private:
    
    std::vector<GameObject*> sprites_; // Container of sprites that need to be drawn    
    std::vector<GameObject*> models_;  // Container of models that need to be rendered

  };

}