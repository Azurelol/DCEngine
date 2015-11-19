#include "GraphicsSpace.h"
#include "EngineReference.h"
#include "../Systems/Graphics/Graphics.h" // Access to the graphics system

namespace DCEngine {


  /**************************************************************************/
  /*!
  \brief Default GraphicsSpace constructor.
  */
  /**************************************************************************/
  GraphicsSpace::GraphicsSpace(Entity & owner) : Component(std::string("GraphicsSpace"), owner)
  {
  }

  /**************************************************************************/
  /*!
  \brief Default GraphicsSpace destructor.
  */
  /**************************************************************************/
  GraphicsSpace::~GraphicsSpace() {
    // Deregister from the Graphics System

  }

  /**************************************************************************/
  /*!
  \brief Initializes the GraphicsSpace component.
  */
  /**************************************************************************/
  void GraphicsSpace::Initialize() {
    // Store a reference to the space's 'CameraViewport' component
    CameraViewportComponent = Owner()->getComponent<CameraViewport>();
    // Register this space to the graphics system
    Daisy->getSystem<Systems::Graphics>()->Register(*this);
  }

  /**************************************************************************/
  /*!
  \brief Registers a SpriteText into the GraphicsSpace
  */
  /**************************************************************************/
  void GraphicsSpace::AddSpriteText(SpriteText& spriteText)
  {
    SpriteTextContainer.push_back(&spriteText);
    DCTrace << "GraphicsSpace::AddSpriteText - "
      << spriteText.Owner()->Name() << "\n";
  }

  /**************************************************************************/
  /*!
  @brief  Registers a Sprite into the GraphicsSpace
  @param  A reference to an instance of the 'Sprite' component.
  */
  /**************************************************************************/
  void GraphicsSpace::AddSprite(Sprite & sprite)
  {
    SpriteContainer.push_back(&sprite);
    DCTrace << "GraphicsSpace::AddSprite - "
            << sprite.Owner()->Name() << "\n";
  }

  /**************************************************************************/
  /*!
  @brief  Deregisters a Sprite from the GraphicsSpace
  @param  A reference to an instance of the 'Sprite' component.
  */
  /**************************************************************************/
  void GraphicsSpace::RemoveSprite(Sprite & sprite)
  {
    DCTrace << "GraphicsSpace::RemoveSprite - "
            << sprite.Owner()->Name() << "\n";
    Sprite* spritePtr = &sprite;
    SpriteContainer.erase(std::remove(SpriteContainer.begin(),
      SpriteContainer.end(), spritePtr),
      SpriteContainer.end());
  }

  /**************************************************************************/
  /*!
  \brief Passes a call requesting a DebugDraw call on to the Graphics System.
  */
  /**************************************************************************/
  void GraphicsSpace::DrawCircle(Vec3& pos, Real& radius, Vec4& color)
  {
    auto cam = CameraViewportComponent->getCamera();
    Daisy->getSystem<Systems::Graphics>()->DrawCircle(pos, radius, color, *cam);
  }

  void GraphicsSpace::DrawRectangle(Vec3& pos, Real& width, Real& height, Vec4& color)
  {
    auto cam = CameraViewportComponent->getCamera();
    Daisy->getSystem<Systems::Graphics>()->DrawRectangle(pos, width, height, color, *cam);
  }

  void GraphicsSpace::DrawLineSegment(Vec3& startPos, Vec3& endPos, Vec4& color)
  {
    auto cam = CameraViewportComponent->getCamera();
    Daisy->getSystem<Systems::Graphics>()->DrawLineSegment(startPos, endPos, color, *cam);
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