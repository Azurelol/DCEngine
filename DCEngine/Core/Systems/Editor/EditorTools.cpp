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
      Transformation.XAxisBoundaryEnd = Selection.SelectedBoundingCenter + Vec3(Transformation.Radius, 0, 0);
      Transformation.YAxisBoundaryEnd = Selection.SelectedBoundingCenter + Vec3(0, Transformation.Radius, 0);
      Transformation.ZAxisBoundaryEnd = Selection.SelectedBoundingCenter + Vec3(0, 0, Transformation.Radius);
      // Calculate the midpoints of the 3 axes boundaries
      Transformation.XAxisMidpoint = Vec3((Selection.SelectedBoundingCenter.x + Transformation.XAxisBoundaryEnd.x) / 2,
                                         (Selection.SelectedBoundingCenter.y + Transformation.XAxisBoundaryEnd.y) / 2,
                                         (Selection.SelectedBoundingCenter.z + Transformation.XAxisBoundaryEnd.z) / 2);
      Transformation.YAxisMidpoint = Vec3((Selection.SelectedBoundingCenter.x + Transformation.YAxisBoundaryEnd.x) / 2,
                                         (Selection.SelectedBoundingCenter.y + Transformation.YAxisBoundaryEnd.y) / 2,
                                         (Selection.SelectedBoundingCenter.z + Transformation.YAxisBoundaryEnd.z) / 2);

    }

    /**************************************************************************/
    /*!
    @brief The translate tool allows the user to move an object on screen.
    @param The GameObject to which to apply the tool on.
    */
    /**************************************************************************/
    CommandObjectTransform* TransCommand = NULL;
    void Editor::UseTool(GameObjectPtr gameObject, Vec2& position)
    {
      if (TransCommand != NULL)
      {
        delete TransCommand;
        TransCommand = NULL;
      }

      // If the 'Translate' tool is active...
      if (ActiveTool == EditorTools::Translate) {

        // Start dragging
        TransformStartDragging();

        // And a valid GameObject was selected, start dragging it
        //if (SelectedObject() && gameObject && gameObject->getObjectName() != std::string("EditorCamera")) {
        //  Transformation.Dragging = true;
        //  auto transform = gameObject->getComponent<Components::Transform>();
        //  TransCommand = new CommandObjectTransform(transform);
        //  DCTrace << "EditorRef::OnMouseDownEvent - Dragging: '" << gameObject->getObjectName() << "'\n";
        //  return;
        //}

        // CODE FOR DRAGGING X/Y ??
        //if we have a valid Object selected still
        if (SelectedObject() && SelectedObject()->getObjectName() != std::string("EditorCamera")) {
          auto transform = dynamic_cast<GameObject*>(SelectedObject())->getComponent<Components::Transform>();
          //and it has a transform
          if (transform != NULL)
          {
            auto mousePos = CurrentSpace->getComponent<Components::CameraViewport>()->ScreenToViewport(position);
            auto xPos = transform->getTranslation().x;
            auto yPos = transform->getTranslation().y;
            //data for translate editor tools
            Real radius = 8;
            Real arrowTip = 1;
            //if within horizontal tool, drag along x axis
            if (mousePos.x > xPos && mousePos.x < xPos + radius && mousePos.y < yPos + arrowTip && mousePos.y > yPos - arrowTip)
            {
              Transformation.DraggingX = true;
              Transformation.DragOffset = mousePos.x - xPos;

              TransCommand = new CommandObjectTransform(transform);
              DCTrace << "EditorRef::OnMouseDownEvent - DraggingX: '" << SelectedObject()->getObjectName() << "'\n";
              return;
            }
            //if within vertical tool, drag along y axis
            if (mousePos.x > xPos - arrowTip && mousePos.x < xPos + arrowTip && mousePos.y < yPos + radius && mousePos.y > yPos)
            {
              Transformation.DraggingY = true;
              Transformation.DragOffset = mousePos.y - yPos;
              TransCommand = new CommandObjectTransform(transform);
              DCTrace << "EditorRef::OnMouseDownEvent - DraggingY: '" << SelectedObject()->getObjectName() << "'\n";
              return;
            }
          }
        }
      }
    }

    /**************************************************************************/
    /*!
    @brief  Start dragging all currently selected GameObjects.
    */
    /**************************************************************************/
    void Editor::TransformStartDragging()
    {
      // Clear the currently saved positions
      Transformation.InitialGameObjectPositions.clear();
      // For every selected GameObject, record its initial position      
      for (auto& object : SelectedObjects) {
        auto gameObject = dynamic_cast<GameObjectPtr>(object);
        auto& translation = gameObject->getComponent<Components::Transform>()->getTranslation();

        Transformation.InitialGameObjectPositions.push_back(TransformToolData::GameObjectInitialPosition(gameObject, translation));
        DCTrace << "EditorRef::TransformStartDragging - Dragging: '" << gameObject->getObjectName() << "'\n";
      }

      Transformation.Dragging = true;
    }


    /**************************************************************************/
    /*!
    @brief  Drags the object
    @param  The mouse's current position.
    */
    /**************************************************************************/
    void Editor::TransformDrag(Vec2& pos)
    {
      if (!Transformation.Dragging)
        return;

      // Only drag GameObjects in the Space while in translate mode
      if (ActiveTool != EditorTools::Translate)
        return;

      // If the selected object is a GameObject on the space
      if (!IsSelectableGameObject(SelectedObject()))
        return;

     // Calculate the current mouse position
     auto mousePos = Vec3(CurrentSpace->getComponent<Components::CameraViewport>()->ScreenToViewport(pos), 0);     
     auto mouseDiff = mousePos - Settings.MouseStartPos;

     // Drag all selected objects
     for (auto& gameObject : Transformation.InitialGameObjectPositions) {
       auto& objectCurrentPos = gameObject.first->getComponent<Components::Transform>()->getTranslation();
       auto& objectStartPos = gameObject.second;
       auto objectDiff = objectCurrentPos - objectStartPos;

       gameObject.first->getComponent<Components::Transform>()->setTranslation(objectStartPos - objectDiff + mouseDiff);
     }      
      
      //if (Transformation.DraggingX) {
      //  //DCTrace << "Dragging! \n ";
      //  // If the selected object is a GameObject on the space
      //  if (auto gameObject = dynamic_cast<GameObject*>(SelectedObject())) {
      //    // Calculate the current mouse positionDragObjects
      //    auto mousePos = CurrentSpace->getComponent<Components::CameraViewport>()->ScreenToViewport(pos);
      //    //auto mvtScale = EditorCamera->getComponent<Components::Camera>()->TransformComponent->getTranslation().z;
      //    // Move the object
      //    gameObject->getComponent<Components::Transform>()->setTranslation(Vec3(mousePos.x - Transformation.DragOffset,
      //      gameObject->getComponent<Components::Transform>()->getTranslation().y,
      //      gameObject->getComponent<Components::Transform>()->getTranslation().z));
      //  }
      //}
      //if (Transformation.DraggingY) {
      //  //DCTrace << "Dragging! \n ";
      //  // If the selected object is a GameObject on the space
      //  if (auto gameObject = dynamic_cast<GameObject*>(SelectedObject())) {
      //    // Calculate the current mouse position
      //    auto mousePos = CurrentSpace->getComponent<Components::CameraViewport>()->ScreenToViewport(pos);
      //    //auto mvtScale = EditorCamera->getComponent<Components::Camera>()->TransformComponent->getTranslation().z;
      //    // Move the object
      //    gameObject->getComponent<Components::Transform>()->setTranslation(Vec3(gameObject->getComponent<Components::Transform>()->getTranslation().x,
      //      mousePos.y - Transformation.DragOffset,
      //      gameObject->getComponent<Components::Transform>()->getTranslation().z));
      //  }
      
    }


    /**************************************************************************/
    /*!
    @brief  Moves an object by translation by the specified direction vector.
    @param  direction A vector.
    */
    /**************************************************************************/
    void Editor::MoveObject(const Vec3& direction)
    {
      if (!SelectedObject())
        return;

      // If the first entry is at least a valid GameObject, the rest will be....
      if (IsSelectableGameObject(SelectedObject())) {
        // For every currently selected GameObject
        for (auto& object : SelectedObjects) {
          auto gameObject = IsSelectableGameObject(object);

          DCTrace << "Editor::MoveObject - Moving '" << SelectedObject()->Name() << "' \n";
          // Get the object's transform data
          auto transform = gameObject->getComponent<Components::Transform>();
          Vec3 pos = transform->getTranslation();
          // Translate the object
          auto TransCommand = new CommandObjectTransform(transform);
          transform->setTranslation(pos + direction);
          TransCommand->SaveNew(transform);
          auto command = CommandPtr(TransCommand);
          Add(command);

        }
      }
    }


    /**************************************************************************/
    /*!
    @brief  Releases the selected object at the current dragged position.
    */
    /**************************************************************************/
    void Editor::ReleaseObject()
    {
      if (!SelectedObject())
        return;

      if (Transformation.Dragging || Transformation.DraggingX || Transformation.DraggingY) {

        // Snap the object to the nearest (x,y) snapDistance      
        if (Settings.Snapping) {
          auto& translation = dynamic_cast<GameObjectPtr>(SelectedObject())->getComponent<Components::Transform>()->getTranslation();
          auto snappedPos = Math::Snap(Vec2(translation.x, translation.y));
          dynamic_cast<GameObjectPtr>(SelectedObject())->getComponent<Components::Transform>()->setTranslation(Vec3(snappedPos.x, snappedPos.y, translation.z));

          DCTrace << "EditorRef::ReleaseObject - Releasing '" << SelectedObject()->getObjectName() << "' at: \n"
            << "x: " << snappedPos.x << ", y: " << snappedPos.y << "\n";
        }
      }
    }






  }
}