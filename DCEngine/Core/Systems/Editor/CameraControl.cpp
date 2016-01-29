#include "Editor.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief  Pans the camera when the specified mouse-button is held.
    @param  event The current mouse position.
    */
    /**************************************************************************/
    void Editor::PanCamera(Vec2 mousePosition)
    {
      if (Settings.Panning) {
        // Get the mouse position
        auto mousePos = Vec3(CurrentSpace->getComponent<Components::CameraViewport>()->ScreenToViewport(mousePosition), 0);
        auto camPos = EditorCamera->getComponent<Components::Transform>()->getTranslation();
        //auto offset = Vec3(mousePos.x - camPos.x, mousePos.y - camPos.y, camPos.z);

        Vec3 cameraDiff = camPos - Settings.CamStartPos;
        Vec3 mouseDiff = mousePos - Settings.MouseStartPos;

        //float width = 5;
        //CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawRectangle(mousePos, width, width, Vec4(1, 0, 0, 1));

        // Set the camera's position
        EditorCamera->getComponent<Components::Transform>()->setTranslation(Settings.CamStartPos + cameraDiff - mouseDiff);

      }


    }


  }
}