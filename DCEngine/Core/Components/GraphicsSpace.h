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
#include "../Objects/DebugDraw.h"

namespace DCEngine {
  
  class Camera;
  class CameraViewport;
  class Graphics;
  class GraphicsSpace : public Component {
    friend class Graphics; 

  public:

    GraphicsSpace(Entity& owner) : Component(std::string("GraphicsSpace"), owner) {}
    void Initialize();

    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);

    void RegisterSprite(GameObject* sprite);
    void OnSpriteRegistrationEvent(Events::SpriteRegistration* eventObj);
    void OnSpriteUnregistrationEvent(Event* eventObj);

    /* Debug Drawing functions*/
    void DrawCircle(Real3& pos, Real& radius, Real4& color);
    void DrawRectangle(Real3& pos, Real& width, Real& height, Real4& color);
    void DrawLineSegment(Real3& startPos, Real3& endPos, Real& length, Real4& color);

    GameObjectRawVec getSprites(); // Return the vector of sprites to be drawn

    void OnLogicUpdate(Events::LogicUpdate* updateEvent);

  private:
    Graphics* GraphicsSystem;
    CameraViewport* CameraViewportComponent;
    GameObjectRawVec sprites_; //!< Container of sprites that need to be drawn    
    GameObjectRawVec models_;  //!< Container of models that need to be rendered
    DebugDrawObjVec debugDrawObjs_; //!< Container of debug draw objects to be drawn this frame.

  };

}