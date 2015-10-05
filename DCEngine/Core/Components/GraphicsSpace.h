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

<<<<<<< .merge_file_a13324
#include "../Components/Sprite.h"
=======
//#include "../Components/Sprite.h"
>>>>>>> .merge_file_a09184
#include "../Components/SpriteText.h"

namespace DCEngine {
  
  // Forward Declarations
  class Graphics;
  class Camera;
  class CameraViewport;
<<<<<<< .merge_file_a13324
  
  // Containers
  using SpriteTextContainer = std::vector<SpriteText*>;
  using SpriteContainer = std::vector<Sprite*>;
=======
  class Sprite;
  //class SpriteText;
    
  using SpriteTextContainer = std::vector<SpriteText*>;
>>>>>>> .merge_file_a09184

  class GraphicsSpace : public Component {
    friend class Graphics; 

  public:

    GraphicsSpace(Entity& owner) : Component(std::string("GraphicsSpace"), owner) {}
    void Initialize();
<<<<<<< .merge_file_a13324
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root); 
            
    void AddSprite(Sprite& sprite);
    void RemoveSprite(Sprite& sprite);
    void AddSpriteText(SpriteText& spriteText);
    void RemoveSpriteText(SpriteText& spriteText);
=======

    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root); 

    void RegisterSprite(GameObject* sprite);
    void OnSpriteRegistrationEvent(Events::SpriteRegistration* eventObj);
    void OnSpriteUnregistrationEvent(Event* eventObj);
    
    void AddSpriteText(SpriteText& spriteText);

>>>>>>> .merge_file_a09184

    /* Debug Drawing functions*/
    void DrawCircle(Real3& pos, Real& radius, Real4& color);
    void DrawRectangle(Real3& pos, Real& width, Real& height, Real4& color);
    void DrawLineSegment(Real3& startPos, Real3& endPos, Real& length, Real4& color);

<<<<<<< .merge_file_a13324
    /* Getters */
    SpriteContainer getSprites(); // Return the vector of sprites to be drawn
    SpriteTextContainer getSpriteTextContainer(); // Returns the container of SpriteText to be drawn
    
    void OnLogicUpdate(Events::LogicUpdate* updateEvent);

  private:

    Graphics* GraphicsSystem;
    CameraViewport* CameraViewportComponent;    
    GameObjectRawVec models_;  //!< Container of models that need to be rendered
    SpriteContainer SpriteContainer; //!< Container of sprites that need to be drawn    
=======
    GameObjectRawVec getSprites(); // Return the vector of sprites to be drawn
    SpriteTextContainer getSpriteTextContainer(); // Returns the container of SpriteText to be drawn


    void OnLogicUpdate(Events::LogicUpdate* updateEvent);

  private:
    Graphics* GraphicsSystem;
    CameraViewport* CameraViewportComponent;
    GameObjectRawVec sprites_; //!< Container of sprites that need to be drawn    
    GameObjectRawVec models_;  //!< Container of models that need to be rendered
>>>>>>> .merge_file_a09184
    SpriteTextContainer SpriteTextContainer;  //!< Container of models that need to be rendered
    DebugDrawObjVec DebugDrawObjContainer; //!< Container of debug draw objects to be drawn this frame.

  };

}