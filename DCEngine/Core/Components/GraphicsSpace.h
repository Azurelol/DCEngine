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

#include "../Components/Sprite.h"
#include "../Components/SpriteText.h"

namespace DCEngine {

  // Forward Declarations
  class Graphics;
  class Camera;
  class CameraViewport;

  // Containers
  using SpriteTextContainer = std::vector<SpriteText*>;
  using SpriteContainer = std::vector<Sprite*>;

  class GraphicsSpace : public Component {
    friend class Graphics;

  public:

    GraphicsSpace(Entity& owner) : Component(std::string("GraphicsSpace"), owner) {}
    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);

    void AddSprite(Sprite& sprite);
    void RemoveSprite(Sprite& sprite);
    void AddSpriteText(SpriteText& spriteText);
    void RemoveSpriteText(SpriteText& spriteText);

    /* Debug Drawing functions*/
    void DrawCircle(Vec3& pos, Real& radius, Vec4& color);
    void DrawRectangle(Vec3& pos, Real& width, Real& height, Vec4& color);
    void DrawLineSegment(Vec3& startPos, Vec3& endPos, Real& length, Vec4& color);

    /* Getters */
    SpriteContainer getSprites(); // Return the vector of sprites to be drawn
    SpriteTextContainer getSpriteTextContainer(); // Returns the container of SpriteText to be drawn

    void OnLogicUpdate(Events::LogicUpdate* updateEvent);

  private:

    Graphics* GraphicsSystem;
    CameraViewport* CameraViewportComponent;
    GameObjectRawVec models_;  //!< Container of models that need to be rendered
    SpriteContainer SpriteContainer; //!< Container of sprites that need to be drawn    
    SpriteTextContainer SpriteTextContainer;  //!< Container of models that need to be rendered
    DebugDrawObjVec DebugDrawObjContainer; //!< Container of debug draw objects to be drawn this frame.

  };

}