/******************************************************************************/
/*!
@file   MultiSelector.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   2/5/2015
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "Selector.h"

// Engine
#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief  Calculates the bounding area of the currently selected objects.
    */
    /**************************************************************************/
    void Selector::CalculateSelectionBounding(ObjectPtr selectedObject, ObjectContainer selectedObjects)
    {
      if (!IsSelectableGameObject(selectedObject))
        return;

      // Find the values for the boundary box composed of all selected objects
      float xMin = 0.0f;
      float xMax = 0.0f;
      float yMin = 0.0f;
      float yMax = 0.0f;

      // First time: EWWWWWWWWWWWWW
      auto transform = dynamic_cast<GameObjectPtr>(selectedObject)->getComponent<Components::Transform>();
      auto& translation = transform->getTranslation();
      auto& scale = transform->getScale();
      // Calculate the four boundaries of the object
      xMin = (translation.x - (scale.x / 2));
      xMax = (translation.x + (scale.x / 2));
      yMax = (translation.y + (scale.y / 2));
      yMin = (translation.y - (scale.y / 2));

      for (auto& object : selectedObjects) {

        auto gameObject = dynamic_cast<GameObjectPtr>(object);
        // Do nothing if the GameObject does not have a transform.
        if (!gameObject->HasComponent(std::string("Transform")))
          continue;

        // Get the object's position
        auto transform = gameObject->getComponent<Components::Transform>();
        auto& translation = transform->getTranslation();
        auto& scale = transform->getScale();

        // Calculate the four boundaries of the object
        auto objLeftBoundary = (translation.x - (scale.x / 2));
        auto objRightBoundary = (translation.x + (scale.x / 2));
        auto objTopBoundary = (translation.y + (scale.y / 2));
        auto objBottomBoundary = (translation.y - (scale.y / 2));

        // Left
        if (objLeftBoundary < xMin)
          xMin = objLeftBoundary;
        if (objRightBoundary > xMax)
          xMax = objRightBoundary;
        if (objBottomBoundary < yMin)
          yMin = objBottomBoundary;
        if (objTopBoundary > yMax)
          yMax = objTopBoundary;
      }

      // Calculate the midpoint between 2 opposite corners to find the center
      // of the rectangle        
      SelectedBoundingWidth = std::abs(xMax - xMin);
      SelectedBoundingHeight = std::abs(yMax - yMin);
      SelectedBoundingCenter = Vec3((xMin + xMax) / 2.0f,
        (yMin + yMax) / 2.0f,
        0.0f);
    }

    /**************************************************************************/
    /*!
    @brief  Selector constructor.
    */
    /**************************************************************************/
    Selector::Selector(Systems::Editor& editor) : Editor(editor)
    {
    }

    /**************************************************************************/
    /*!
    @brief  Checks if the object is a valid selectable GameObject. That is,
    one with an active transform component.
    @param  object An Object*
    */
    /**************************************************************************/
    GameObjectPtr Selector::IsSelectableGameObject(ObjectPtr object)
    {
      if (auto gameObject = dynamic_cast<GameObjectPtr>(object)) {
        if (gameObject->HasComponent(std::string("Transform"))) {
          return gameObject;
        }
      }
      return nullptr;
    }

    /**************************************************************************/
    /*!
    @brief  Attempts to select any objects within the boundary drawn
            by the mouse cursor.
    @param  event The current mouse position.
    */
    /**************************************************************************/
    void Selector::SelectMultiple(Vec2 & mousePosition, ObjectContainer selectedObjects, SpacePtr currentSpace)
    {
      if (!MultiSelectDragging)
        return;

      // Get the current mouse position.
      auto endPos = Vec3(currentSpace->getComponent<Components::CameraViewport>()->ScreenToViewport(mousePosition), 0);
      auto& startPos = MultiSelectStartPos;
      // Calculate the bounding box created between the endpoints (where the selection started, 
      // and the currnent mouse position)
      MultiSelectArea = endPos - startPos;
      MultiSelectMidpoint = Vec3((endPos.x + startPos.x) / 2,
        (endPos.y + startPos.y) / 2,
        (endPos.y + startPos.y) / 2);


      // Check for objects within the selected area. Perform a bounding box check.
      selectedObjects.clear();
      for (auto& gameObject : *currentSpace->AllObjects()) {

        // Do nothing if it has a camera component
        if (gameObject->HasComponent(std::string("Camera")))
          continue;

        // If the object lies within the bounding area..
        if (Daisy->getSystem<Physics>()->IsObjectWithinBoundingArea(MultiSelectMidpoint, MultiSelectArea.x, 
              MultiSelectArea.y, gameObject)) {
          selectedObjects.push_back(gameObject);
        }
      }
    }

    /**************************************************************************/
    /*!
    @brief  Draws the multi-selection bounding area.
    */
    /**************************************************************************/
    void Selector::DrawMultiSelect(SpacePtr currentSpace)
    {
      if (!MultiSelectDragging)
        return;

      // Draw the bounding rectangle
      currentSpace->getComponent<Components::GraphicsSpace>()->DrawRectangle(MultiSelectMidpoint,
                                          MultiSelectArea.x, MultiSelectArea.y, MultiSelectColor);
    }



  }
}