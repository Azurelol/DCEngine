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

    /**************************************************************************/
    /*!
    @brief  Attempts to select any objects within the boundary drawn
            by the mouse cursor.
    @param  event The current mouse position.
    */
    /**************************************************************************/
    void Editor::SelectMultiple(Vec2 & mousePosition)
    {
      if (!Settings.MultiSelectDragging)
        return;

      DCTrace << "Multi-selecting !!! \n";

      // Get the current mouse position.
      auto endPos = Vec3(CurrentSpace->getComponent<Components::CameraViewport>()->ScreenToViewport(mousePosition), 0);
      auto& startPos = Settings.MultiSelectStartPos;
      // Calculate the bounding box created between the endpoints (where the selection started, 
      // and the currnent mouse position)
      auto bounding = endPos - startPos;
      auto midpoint = Vec3( (endPos.x + startPos.x) / 2,
                            (endPos.y + startPos.y) / 2,
                            (endPos.y + startPos.y) / 2);
      // Draw the bounding rectangle
      CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawRectangle(midpoint,
                                                               bounding.x, bounding.y, Settings.MultiSelectColor);
      
      SelectedObjects.clear();
      // Check for objects within the selected area. Perform a bounding box check.
      for (auto& gameObject : CurrentSpace->GameObjectContainer) {
        // If the object lies within the bounding area..
        if (Daisy->getSystem<Physics>()->IsObjectWithinBoundingArea(midpoint, bounding.x, bounding.y, gameObject)) {
          SelectedObjects.push_back(gameObject);
        }
      }

    }


  }
}