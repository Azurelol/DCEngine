#include "GraphicsSpace.h"
#include "EngineReference.h"
#include "../Systems/Graphics/Graphics.h" // Access to the graphics system

namespace DCEngine {


  /**************************************************************************/
  /*!
  \brief Initializes the GraphicsSpace component.
  */
  /**************************************************************************/
  void GraphicsSpace::Initialize() {
    // Store a reference to the space's 'CameraViewport' component
    CameraViewportComponent = Owner()->getComponent<CameraViewport>();
    // Register this space to the graphics system
    Daisy->getSystem<Systems::Graphics>(EnumeratedSystem::Graphics)->Register(*this);
  }

  /**************************************************************************/
  /*!
  \brief Serialize/Deserialize.
  */
  /**************************************************************************/
  void GraphicsSpace::Serialize(Json::Value & root) {

  }

  void GraphicsSpace::Deserialize(Json::Value & root) {

  }

  /**************************************************************************/
  /*!
  \brief Registers a SpriteText into the GraphicsSpace
  */
  /**************************************************************************/
  void GraphicsSpace::AddSpriteText(SpriteText& spriteText)
  {
    SpriteTextContainer.push_back(&spriteText);
    trace << "GraphicsSpace::AddSpriteText - "
      << spriteText.Owner()->Name() << "\n";
  }

  /**************************************************************************/
  /*!
  @brief  Registers a SpriteText into the GraphicsSpace
  @param  A reference to an instance of the 'Sprite' component.
  */
  /**************************************************************************/
  void GraphicsSpace::AddSprite(Sprite & sprite)
  {
    SpriteContainer.push_back(&sprite);
    trace << "GraphicsSpace::AddSprite - "
      << sprite.Owner()->Name() << "\n";
  }

  /**************************************************************************/
  /*!
  \brief Passes a call requesting a DebugDraw call on to the Graphics System.
  */
  /**************************************************************************/
  void GraphicsSpace::DrawCircle(Real3& pos, Real& radius, Real4& color)
  {
    auto cam = *CameraViewportComponent->getCamera();
    Daisy->getSystem<Systems::Graphics>(EnumeratedSystem::Graphics)->DrawCircle(pos, radius, color, cam);
  }

  void GraphicsSpace::DrawRectangle(Real3& pos, Real& width, Real& height, Real4& color)
  {
    auto cam = *CameraViewportComponent->getCamera();
    Daisy->getSystem<Systems::Graphics>(EnumeratedSystem::Graphics)->DrawRectangle(pos, width, height, color, cam);
  }

  void GraphicsSpace::DrawLineSegment(Real3& startPos, Real3& endPos, Real& length, Real4& color)
  {
    auto cam = *CameraViewportComponent->getCamera();
    Daisy->getSystem<Systems::Graphics>(EnumeratedSystem::Graphics)->DrawLineSegment(startPos, endPos, length, color, cam);
  }

  /**************************************************************************/
  /*!
  \brief  Returns the container of Sprite pointers.
  \return A container of Sprite pointers.
  */
  /**************************************************************************/
  std::vector<Sprite*> GraphicsSpace::getSprites() {
    return SpriteContainer;
  }

  SpriteTextContainer GraphicsSpace::getSpriteTextContainer()
  {
    return SpriteTextContainer;
  }

}