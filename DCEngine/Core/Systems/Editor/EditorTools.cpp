/******************************************************************************/
/*!
@file   EditorTools.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/23/2015
@brief
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Editor.h"

#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief  Calculates the bounding area of the currently selected objects.
    */
    /**************************************************************************/
    void Editor::CalculateSelectionBounding()
    {
      if (!IsSelectableGameObject(SelectedObject()))
        return;

      // Find the values for the boundary box composed of all selected objects
      float xMin = 0.0f;
      float xMax = 0.0f;
      float yMin = 0.0f;
      float yMax = 0.0f;

      // First time: EWWWWWWWWWWWWW
      auto transform = dynamic_cast<GameObjectPtr>(SelectedObject())->getComponent<Components::Transform>();
      auto& translation = transform->getTranslation();
      auto& scale = transform->getScale();
      // Calculate the four boundaries of the object
      xMin = (translation.x - (scale.x / 2));
      xMax = (translation.x + (scale.x / 2));
      yMax = (translation.y + (scale.y / 2));
      yMin = (translation.y - (scale.y / 2));

      for (auto& object : SelectedObjects) {

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
      Selection.SelectedBoundingWidth = std::abs(xMax - xMin);
      Selection.SelectedBoundingHeight = std::abs(yMax - yMin);
      Selection.SelectedBoundingCenter = Vec3( (xMin + xMax) / 2.0f,
                                               (yMin + yMax) / 2.0f,
                                               0.0f);

      // Calculate the endpoints of the 3 axes boundaries
      TransformData.XAxisBoundaryEnd = Selection.SelectedBoundingCenter + Vec3(TransformData.Radius, 0, 0);
      TransformData.YAxisBoundaryEnd = Selection.SelectedBoundingCenter + Vec3(0, TransformData.Radius, 0);
      TransformData.ZAxisBoundaryEnd = Selection.SelectedBoundingCenter + Vec3(0, 0, TransformData.Radius);
      // Calculate the midpoints of the 3 axes boundaries
      TransformData.XAxisMidpoint = Vec3((Selection.SelectedBoundingCenter.x + TransformData.XAxisBoundaryEnd.x) / 2,
                                         (Selection.SelectedBoundingCenter.y + TransformData.XAxisBoundaryEnd.y) / 2,
                                         (Selection.SelectedBoundingCenter.z + TransformData.XAxisBoundaryEnd.z) / 2);
      TransformData.YAxisMidpoint = Vec3((Selection.SelectedBoundingCenter.x + TransformData.YAxisBoundaryEnd.x) / 2,
                                         (Selection.SelectedBoundingCenter.y + TransformData.YAxisBoundaryEnd.y) / 2,
                                         (Selection.SelectedBoundingCenter.z + TransformData.YAxisBoundaryEnd.z) / 2);

    }




  }
}