#include "Editor.h"

#include "..\..\Engine\Engine.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief  Pans the camera when the specified mouse-button is held.
    @param  event The current mouse position.
    */
    /**************************************************************************/
    void Editor::PanCamera(Vec2& mousePosition)
    {
      if (!Settings.Panning)
        return;

      // Get the mouse position
      auto mousePos = Vec3(CurrentSpace->getComponent<Components::CameraViewport>()->ScreenToViewport(mousePosition), 0);
      auto camPos = EditorCamera->getComponent<Components::Transform>()->getTranslation();

      Vec3 cameraDiff = camPos - Settings.CamStartPos;
      Vec3 mouseDiff = mousePos - Settings.MouseStartPos;

      // Set the camera's position
      EditorCamera->getComponent<Components::Transform>()->setTranslation(Settings.CamStartPos + cameraDiff - mouseDiff);
    }



  }
}